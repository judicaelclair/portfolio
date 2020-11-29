set(WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
# ---------------------------------------------------------

cmake_minimum_required(VERSION 3.10)
set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)
project(WebAppProject CXX)

# Mac support
set(CMAKE_C_LINK_FLAGS "")
set(CMAKE_CXX_LINK_FLAGS "")

include(cmake/cmake_utils/StandardProjectSettings.cmake)
include(cmake/CrossCompiling.cmake)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)
 
# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# cache system
include(cmake/cmake_utils/Cache.cmake)

# compiler warnings
include(cmake/cmake_utils/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options
include(cmake/cmake_utils/Sanitizers.cmake)
enable_sanitizers(project_options)

# static analysis
include(cmake/cmake_utils/StaticAnalyzers.cmake)

function(add_public_targets)
    foreach(target IN LISTS ARGN)
        set_target_properties(${target} PROPERTIES
                              RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/public_html
                              LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/public_html
                              ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/static)
    endforeach()
    install(TARGETS ${ARGV}
            RUNTIME DESTINATION public_html
            LIBRARY DESTINATION public_html
            ARCHIVE DESTINATION lib/static)
endfunction()

if(NOT DEFINED EMCC)
  get_filename_component(EMCC "$ENV{HOME}/emsdk/upstream/emscripten/emcc" ABSOLUTE)
endif()

set(HOST_C_COMPILER ${EMCC})
set(HOST_CXX_COMPILER ${EMCC})
use_host_compiler()

# data folders
set(PRELOAD_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")

set(EXT_DATA_TO_PRELOAD "")
set(CLR_DATA_DIR "$ENV{HOME}/Dropbox/ClairIndustries/clarity_private/clarity/clarity_graphics/data")
set(EXT_DATA_TO_PRELOAD "${CLR_DATA_DIR}/barlow.regular.ttf" "${CLR_DATA_DIR}/barlow.extrabold.ttf")
set(EXT_PRELOAD_DATA_DIR "${CMAKE_CURRENT_BINARY_DIR}/clr_preload_data")

foreach(DATA_FILE ${EXT_DATA_TO_PRELOAD})
  get_filename_component(DATA_FILE_NAME ${DATA_FILE} NAME)
  configure_file("${DATA_FILE}" "${EXT_PRELOAD_DATA_DIR}/${DATA_FILE_NAME}" COPYONLY)
endforeach()

# create executable
set(CMAKE_EXECUTABLE_SUFFIX ".js")
add_executable(webapp)
add_public_targets(webapp)
if(NOT CMAKE_BUILD_TYPE MATCHES Rel)
  set(SAFE_FLAG_STATUS "0")
else()
  set(SAFE_FLAG_STATUS "1")
endif()
set_target_properties(webapp PROPERTIES COMPILE_FLAGS "-s DISABLE_EXCEPTION_CATCHING=0 -s SAFE_HEAP=${SAFE_FLAG_STATUS} -s ASSERTIONS=${SAFE_FLAG_STATUS} -s ALLOW_MEMORY_GROWTH=1")
set_target_properties(webapp PROPERTIES LINK_FLAGS "-s DISABLE_EXCEPTION_CATCHING=0 --preload-file ${PRELOAD_DATA_DIR} --preload-file ${EXT_PRELOAD_DATA_DIR} -s WASM=1 -s SAFE_HEAP=${SAFE_FLAG_STATUS} -s ASSERTIONS=${SAFE_FLAG_STATUS} -s ALLOW_MEMORY_GROWTH=1")
target_link_libraries(webapp PRIVATE project_options project_warnings)
target_compile_definitions(webapp PRIVATE "PRELOAD_DATA_DIR=\"${PRELOAD_DATA_DIR}\"")
target_compile_definitions(webapp PRIVATE "EXT_PRELOAD_DATA_DIR=\"${EXT_PRELOAD_DATA_DIR}\"")

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
target_sources(webapp PRIVATE ${SOURCES})
target_include_directories(webapp PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# dependencies
disable_source_warnings(${CMAKE_CURRENT_SOURCE_DIR}/dependencies)

# Clarity
set(CLR_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(CLR_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(CLR_ENABLE_GRAPHICS ON CACHE BOOL "" FORCE)
set(CLR_ENABLE_DILIGENT OFF CACHE BOOL "" FORCE)
set(CLR_ENABLE_FILAMENT OFF CACHE BOOL "" FORCE)
set(CLR_ENABLE_NATIVE_OPENGL ON CACHE BOOL "" FORCE)
set(CLR_IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui" CACHE STRING "" FORCE)
set(CLR_GLFW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw-3.2" CACHE STRING "" FORCE)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/clarity" "${CMAKE_CURRENT_BINARY_DIR}/.clarity")
target_link_libraries(webapp PRIVATE Clarity)
add_public_targets(Clarity)

# imgui <-> glfw
target_sources(webapp PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui_glfw_bindings/imgui_impl_glfw.cpp" # we have to use our custom version since monitor implementation has UB (disabled in our version).
    "${CLR_IMGUI_DIR}/examples/imgui_impl_opengl3.cpp")

# misc dependencies
target_include_directories(webapp SYSTEM PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/IconFontCppHeaders" "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/stb")

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/html/" DESTINATION "${PROJECT_BINARY_DIR}/public_html")
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/html_untracked")
  file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/html_untracked/" DESTINATION "${PROJECT_BINARY_DIR}/public_html")
endif()

use_native_compiler()
