/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/window.cpp
 * Last Modified: Saturday, 14th November 2020 1:39:44 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/window.hpp>
#include <WebApp/gui/manager.hpp>

#include <Clarity/core/diagnostics/manager.hpp>

#include <emscripten.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

#include <WebApp/gui/icon.hpp>

namespace webapp {

GLFWwindow* window::glfw_window_{nullptr};
ImGuiContext* window::imgui_ctx_{nullptr};
std::function<void(void)> window::imgui_render_callback_ = []() { ImGui::ShowDemoWindow(nullptr); };
bool window::loading_{true};

ImFont* default_font;
ImFont* semi_large_font;
ImFont* semi_large_bold_font;
ImFont* subtitle_font;
ImFont* title_font_small;
ImFont* title_font_large;
ImFont* icons_font;

namespace detail {

  EM_JS(void, show_loading_page, (), {
    document.getElementById("loading_page").style.display = "table";
    document.getElementById("canvas").style.display       = "none";
  });
  EM_JS(void, hide_loading_page, (), {
    document.getElementById("loading_page").style.display = "none";
    document.getElementById("canvas").style.display       = "inline";
  });
  EM_JS(int, canvas_get_width, (), { return Module.canvas.width; });
  EM_JS(int, canvas_get_height, (), { return Module.canvas.height; });
  EM_JS(void, resize_canvas, (), { js_resizeCanvas(); });

} // namespace detail

void window::loading(const bool value) {
  loading_ = value;
  if (value) {
    detail::show_loading_page();
  } else {
    update_(); // pre-render buffer so no visual stutters
    detail::hide_loading_page();
  }
}

void window::update_() {
  gui_manager::update();
  if (loading()) {
    imgui_render_callback([]() {});
    return;
  }

  glfwSetWindowSize(glfw_window_, detail::canvas_get_width(), detail::canvas_get_height());
  ImGui::SetCurrentContext(imgui_ctx_);
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  imgui_render_callback_();

  ImGui::Render();

  int display_w, display_h;
  glfwMakeContextCurrent(glfw_window_);
  glfwGetFramebufferSize(glfw_window_, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  {
    constexpr auto R = static_cast<float>(CLEAR_COLOUR_R) / std::numeric_limits<decltype(CLEAR_COLOUR_R)>::max();
    constexpr auto G = static_cast<float>(CLEAR_COLOUR_G) / std::numeric_limits<decltype(CLEAR_COLOUR_G)>::max();
    constexpr auto B = static_cast<float>(CLEAR_COLOUR_B) / std::numeric_limits<decltype(CLEAR_COLOUR_B)>::max();
    constexpr auto A = static_cast<float>(CLEAR_COLOUR_A) / std::numeric_limits<decltype(CLEAR_COLOUR_A)>::max();
    glClearColor(R, G, B, A);
  }
  glClear(GL_COLOR_BUFFER_BIT);

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwMakeContextCurrent(glfw_window_);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  ImGui_ImplGlfw_ScrollCallback(window, xoffset, -yoffset); // reverse vertical scrolling
}

bool window::init_() {
  if (!glfwInit()) {
    CLR_ERR("Failed to initialize GLFW");
    return false;
  }

  // Initialise Window
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfw_window_ = glfwCreateWindow(400, 400, "Web App", nullptr, nullptr);
  if (!glfw_window_) {
    CLR_ERR("Failed to create GLFW window");
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(glfw_window_);

  // Setup Dear ImGui
  IMGUI_CHECKVERSION();
  clr::discard(clr::gfx::font_manager::create_imgui_context());
  imgui_ctx_  = ImGui::GetCurrentContext();
  ImGuiIO& io = ImGui::GetIO();

  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigDockingWithShift = false;

  ImGui_ImplGlfw_InitForOpenGL(glfw_window_, false);
  ImGui_ImplOpenGL3_Init();

  // Setup style
  ImGui::StyleColorsDark();
  ImGui::GetStyle().FrameRounding = 7.0f;

  { // Load fonts
    constexpr auto font = [](auto&&... args) {
      return clr::gfx::font_manager::add_font_with_name(std::forward<decltype(args)>(args)...);
    };
    constexpr auto build = [](std::vector<clr::gfx::imgui_font_component> components, auto&&... args) {
      return clr::gfx::font_manager::build_imgui_font(std::move(components), std::forward<decltype(args)>(args)...);
    };
    constexpr auto icons = [](auto&&... args) { return icons_font_component(std::forward<decltype(args)>(args)...); };

    clr::gfx::font_manager::finder().add_path(PRELOAD_DATA_DIR);
    clr::gfx::font_manager::finder().add_path(EXT_PRELOAD_DATA_DIR);

    ImFontConfig config;
    config.OversampleH         = 2;
    config.OversampleV         = 1;
    config.GlyphExtraSpacing.x = 0.8f;

    constexpr float DEFAULT_SIZE    = 18.0f;
    constexpr float SEMI_LARGE_SIZE = 22.5f;
    constexpr float LARGE_SIZE      = 26.0f;
    constexpr const char REGULAR[]  = "barlow.regular.ttf";
    constexpr const char BOLD[]     = "barlow.extrabold.ttf";

    default_font = build({{font(REGULAR), DEFAULT_SIZE, config}, icons(DEFAULT_SIZE)})->compiled_font();

    title_font_small = build({{font(BOLD), 30.f}, icons(30.f)})->compiled_font();
    title_font_large = build({{font(BOLD), 70.f}, icons(70.f)})->compiled_font();

    subtitle_font = build({{font(BOLD), LARGE_SIZE}, icons(LARGE_SIZE)})->compiled_font();

    semi_large_font      = build({{font(REGULAR), SEMI_LARGE_SIZE}, icons(SEMI_LARGE_SIZE)})->compiled_font();
    semi_large_bold_font = build({{font(BOLD), SEMI_LARGE_SIZE}, icons(SEMI_LARGE_SIZE)})->compiled_font();

    icons_font = build({icons(SEMI_LARGE_SIZE)})->compiled_font();

    // todo: should be doing this from projects_page (by scheduling a build for subsequent frame & rebuilding atlas)
    clr::discard(build({{font(BOLD), 19.f}})); /** projects_page::tag_section_title_font_ **/
  }

  // Attach callbacks
  glfwSetMouseButtonCallback(glfw_window_, ImGui_ImplGlfw_MouseButtonCallback);
  glfwSetKeyCallback(glfw_window_, ImGui_ImplGlfw_KeyCallback);
  glfwSetCharCallback(glfw_window_, ImGui_ImplGlfw_CharCallback);
  glfwSetScrollCallback(glfw_window_, scroll_callback);

  detail::resize_canvas();
  return true;
}

void window::destroy_() {
  if (!glfw_window_) {
    return;
  }
  clr::gfx::font_manager::destroy_imgui_context(imgui_ctx_);
  ImGui_ImplOpenGL3_Shutdown();
  glfwTerminate();
  glfw_window_ = nullptr;
  imgui_ctx_   = nullptr;
}

} // namespace webapp