/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/window.hpp
 * Last Modified: Saturday, 14th November 2020 1:36:36 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <functional>

namespace webapp {

extern ImFont* default_font;
extern ImFont* semi_large_font;
extern ImFont* semi_large_bold_font;
extern ImFont* subtitle_font;
extern ImFont* title_font_small;
extern ImFont* title_font_large;
extern ImFont* icons_font;

class window {
  friend class application;

  static GLFWwindow* glfw_window_;
  static ImGuiContext* imgui_ctx_;
  static std::function<void(void)> imgui_render_callback_;
  static bool loading_;

  [[nodiscard]] static bool init_();
  static void update_();
  static void destroy_();

public:
  template <typename F>
  static void imgui_render_callback(F&& f) {
    imgui_render_callback_ = std::forward<F>(f);
  }
  [[nodiscard]] static constexpr auto imgui_render_callback() noexcept -> std::function<void(void)> const& {
    return imgui_render_callback_;
  }

  static void loading(const bool value);
  [[nodiscard]] static bool loading() noexcept { return loading_; }
  [[nodiscard]] static GLFWwindow* glfw_window() noexcept { return glfw_window_; }
  [[nodiscard]] static ImGuiContext* imgui_ctx() noexcept { return imgui_ctx_; }

  static inline constexpr uint8_t CLEAR_COLOUR_R = 51;
  static inline constexpr uint8_t CLEAR_COLOUR_G = 51;
  static inline constexpr uint8_t CLEAR_COLOUR_B = 51;
  static inline constexpr uint8_t CLEAR_COLOUR_A = 255;
  static inline constexpr ImU32 CLEAR_COLOUR = IM_COL32(CLEAR_COLOUR_R, CLEAR_COLOUR_G, CLEAR_COLOUR_B, CLEAR_COLOUR_A);
};

} // namespace webapp
