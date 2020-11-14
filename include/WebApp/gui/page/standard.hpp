/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/page/standard.hpp
 * Last Modified: Saturday, 14th November 2020 5:20:28 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <WebApp/gui/manager.hpp>
#include <WebApp/gui/page/base.hpp>

#include <imgui_internal.h>

#include <WebApp/application.hpp>

namespace webapp {

class standard_page : public base_page {
  ImVec2 footer_rect_min_;
  ImGuiWindow* base_imgui_window_{nullptr};

  template <typename T, typename... Args>
  void open_(Args&&... args);

  void render_main_menu_bar_();
  void render_footer_();
  void render_page_();

protected:
  virtual void render_content_() = 0;

  [[nodiscard]] virtual float placeholder_header_(); /** returns height **/
  virtual void render_header_(const float height);
  virtual void render_header_content_();

  void reset_scroll_y_();

public:
  static inline constexpr float MAIN_MENU_BAR_HEIGHT = 36;

  standard_page();
  virtual ~standard_page() = default;
  void set_as_current_page() final;
  void reset() override;

  [[nodiscard]] float footer_height() const;
  [[nodiscard]] auto footer_rect_min() const noexcept -> ImVec2 { return footer_rect_min_; }

  [[nodiscard]] constexpr auto base_imgui_window() const noexcept -> ImGuiWindow* { return base_imgui_window_; }
};

} // namespace webapp