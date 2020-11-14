macro(use_host_compiler)
  if (${CURRENT_COMPILER} STREQUAL "NATIVE")
    # Save current native flags
    set(NATIVE_C_FLAGS ${CMAKE_C_FLAGS} CACHE STRING "flags for the native compiler." FORCE)
    set(NATIVE_CXX_FLAGS ${CMAKE_CXX_FLAGS} CACHE STRING "flags for the native compiler." FORCE)

    # Change compiler
    set(CMAKE_C_COMPILER ${HOST_C_COMPILER})
    set(CMAKE_C_FLAGS ${HOST_C_FLAGS})
    set(CMAKE_CXX_COMPILER ${HOST_CXX_COMPILER})
    set(CMAKE_CXX_FLAGS ${HOST_CXX_FLAGS})
    set(CURRENT_COMPILER "HOST" CACHE STRING "Which compiler we are using." FORCE)
  endif()
endmacro()


macro(use_native_compiler)
  if (CMAKE_CROSSCOMPILING AND ${CURRENT_COMPILER} STREQUAL "HOST")
    # Save current host flags
    set(HOST_C_FLAGS ${CMAKE_C_FLAGS} CACHE STRING "flags for the host compiler." FORCE)
    set(HOST_CXX_FLAGS ${CMAKE_CXX_FLAGS} CACHE STRING "flags for the host compiler." FORCE)

    # Change compiler
    set(CMAKE_C_COMPILER ${NATIVE_C_COMPILER})
    set(CMAKE_C_FLAGS ${NATIVE_C_FLAGS})
    set(CMAKE_CXX_COMPILER ${NATIVE_CXX_COMPILER})
    set(CMAKE_CXX_FLAGS ${NATIVE_CXX_FLAGS})
    set(CURRENT_COMPILER "NATIVE" CACHE STRING "Which compiler we are using." FORCE)
  endif()
endmacro()

set(CURRENT_COMPILER "NATIVE")
set(NATIVE_C_COMPILER ${CMAKE_C_COMPILER})
set(NATIVE_CXX_COMPILER ${CMAKE_CXX_COMPILER})