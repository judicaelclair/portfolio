/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/page/standard.cpp
 * Last Modified: Monday, 7th December 2020 11:20:04 am
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/page/standard.hpp>
#include <WebApp/gui/page/home.hpp>
#include <WebApp/gui/page/projects.hpp>
#include <WebApp/gui/page/about.hpp>
#include <WebApp/gui/page/project/portfolio.hpp>
#include <WebApp/gui/widget/network_icons.hpp>
#include <WebApp/gui/widget/text.hpp>
#include <WebApp/gui/widget/link.hpp>
#include <WebApp/gui/widget/search_bar.hpp>
#include <WebApp/gui/window/base.hpp>
#include <WebApp/gui/util/imgui.hpp>
#include <WebApp/gui/util/js.hpp>
#include <WebApp/gui/icon.hpp>
#include <WebApp/window.hpp>

#include <Clarity/core/util/common.hpp>
#include <Clarity/core/util/str.hpp>

#include <imgui_internal.h>
#include <imgui_stdlib.h>

#include <algorithm>
#include <memory>

static constexpr auto DARK_BG_COLOUR = IM_COL32(0, 0, 0, 100);

namespace webapp {

standard_page::standard_page() : base_page() {}

template <typename T, typename... Args>
void standard_page::open_(Args&&... args) {
  if constexpr (std::is_base_of_v<base_window, T>) {
    static_assert(sizeof...(Args) == 0);

    auto w   = get<T>();
    w->open_ = true;
    ImGui::SetWindowFocus(w->title_.c_str());
  } else if constexpr (std::is_base_of_v<base_page, T>) {
    gui_manager::change_page_with_reset_next_frame_<T>(std::forward<Args>(args)...);
  } else {
    static_assert(clr::always_false_v<T>, "T must be either a window or page");
  }
}

void standard_page::render_main_menu_bar_() {
  ImGuiWindow* const window   = ImGui::GetCurrentWindow();
  ImDrawList* const draw_list = window->DrawList;

  draw_list->AddRectFilled({0, 0}, {window->Size.x, MAIN_MENU_BAR_HEIGHT}, DARK_BG_COLOUR);

  ImVec2 pos{10, 6};
  ImGui::PushFont(semi_large_bold_font);
  if (link::add<link::NO_BUTTON_COLOUR | link::NO_NEUTRAL_UNDERLINE>(
          pos,
          clr::fmt("%.*s", static_cast<int>(content::full_name_all_caps.size()), content::full_name_all_caps.data()))) {
    open_<home_page>();
  }
  ImGui::PopFont();

  /** non-home pages **/
  constexpr float SPACING = 10;
  pos.x                   = window->InnerRect.Max.x - pos.x + SPACING; /** InnerRect to account for scrollbar **/

  auto page = [&](const char* const name) {
    pos.x -= SPACING + ImGui::CalcTextSize(name).x;
    return link::add<link::NO_BUTTON_COLOUR | link::NO_NEUTRAL_UNDERLINE>(pos, name);
  };

  ImGui::PushFont(semi_large_font);
  if (page("ABOUT")) {
    open_<about_page>();
  }
  if (page("PROJECTS")) {
    open_<projects_page>();
  }
  ImGui::PopFont();

  /** TODO (graphics work but logic isn't implemented)
  { // search widget
    auto& ctx   = *ImGui::GetCurrentContext();
    auto& style = ctx.Style;

    const auto sz = search_bar::size();

    // spacing
    ImGui::Dummy({0, (MAIN_MENU_BAR_HEIGHT - sz.y) / 2.0f - style.ItemSpacing.y * 2.0f});
    pos.x -= SPACING + sz.x;
    ImGui::Dummy({pos.x - style.ItemSpacing.x, 0});
    ImGui::SameLine();

    static std::string buf = "";
    if (search_bar::add(buf)) {
      // todo
    }
  }

  // back button
  ImGui::PushFont(icons_font);
  pos.x -= SPACING + ImGui::CalcTextSize(icon::ARROW_LEFT).x;
  if (link::add<link::NO_BUTTON_COLOUR | link::NO_NEUTRAL_UNDERLINE>(pos, icon::ARROW_LEFT,
                                                                     ImGuiButtonFlags_Disabled)) {
    // todo: go to previous page (state saved) - typically used for project -> projects.
    // todo: obfuscate ID for header buttons (remove conflicts which can arise such as here using a bare button).
  }
  ImGui::PopFont();
  TODO (graphics work but logic isn't implemented) **/

  // placeholder for commented out search bar above
  ImGui::Dummy({0, MAIN_MENU_BAR_HEIGHT - ImGui::GetStyle().ItemSpacing.y * 2.0f});
}

float standard_page::footer_height() const {
  float v = 0;

  // normal text
  v += ImGui::GetCurrentWindow()->CalcFontSize() * 7;

  // Contact title
  ImGui::PushFont(semi_large_font);
  v += 10;
  v += ImGui::GetCurrentWindow()->CalcFontSize();
  ImGui::PopFont();

  // Network buttons
  ImGui::PushFont(semi_large_bold_font);
  v += 2;
  v += ImGui::GetCurrentWindow()->CalcFontSize();
  v += 20;
  ImGui::PopFont();

  v += ImGui::GetStyle().ItemSpacing.y * 14;
  return v + 10; // add some breathing room to prevent infinite scrolling
}

void standard_page::render_footer_() {
  // Stick footer to the bottom of the window.
  ImGui::Dummy(
      {0, std::max<float>(
              ImGui::GetCurrentWindow()->Size.y - ImGui::GetCurrentWindow()->DC.CursorPos.y - footer_height(), 20)});

  footer_rect_min_ = {0, ImGui::GetCurrentWindow()->DC.CursorPos.y};
  {
    ImGuiWindow* const window   = ImGui::GetCurrentWindow();
    ImDrawList* const draw_list = window->DrawList;
    draw_list->AddRectFilled(footer_rect_min(), window->Size, DARK_BG_COLOUR);
  }

  auto& style = ImGui::GetStyle();

  const auto old_x_spacing = style.ItemSpacing.x;
  style.ItemSpacing.x      = 0;

  constexpr float INDENTATION = 10.0f;
  ImGui::Indent(INDENTATION);
  ImGui::Dummy({0, 3});

  // Description
  wrapped_text("Portfolio website written in C++. View source code on {GitHub} or {learn more}.",
               {wrapped_text_external_link("https://github.com/judicaelclair/portfolio", "View Portfolio Repository"),
                wrapped_text_page_link<portfolio_page>()});

  // Contact details
  ImGui::Dummy({0, 10});
  ImGui::PushFont(semi_large_font);
  ImGui::Text("Contact");
  ImGui::PopFont();

  ImGui::Text("Personal Email: ");
  ImGui::SameLine();
  if (link::add(clr::fmt("%.*s", static_cast<int>(content::personal_email.size()), content::personal_email.data()),
                clr::fmt("%s Send email", icon::ENVELOPE))) {
    open_email(content::personal_email);
  }

  ImGui::Text("University Email: ");
  ImGui::SameLine();
  if (link::add(clr::fmt("%.*s", static_cast<int>(content::university_email.size()), content::university_email.data()),
                clr::fmt("%s Send email", icon::ENVELOPE))) {
    open_email(content::university_email);
  }
  ImGui::Text("Work Email: ");
  ImGui::SameLine();
  if (link::add(clr::fmt("%.*s", static_cast<int>(content::work_email.size()), content::work_email.data()),
                clr::fmt("%s Send email", icon::ENVELOPE))) {
    open_email(content::work_email);
  }
  ImGui::Text("Mobile: %.*s", static_cast<int>(content::mobile_number.size()), content::mobile_number.data());

  // networking
  ImGui::Dummy({0, 2});

  add_all_network_icons();

  // Copyright notice
  ImGui::Dummy({0, 20});
  ImGui::Text("Copyright %s 2020 %.*s. All rights reserved.", icon::COPYRIGHT,
              static_cast<int>(content::copyright_holders.size()), content::copyright_holders.data());

  ImGui::Unindent(INDENTATION);
  style.ItemSpacing.x = old_x_spacing;
}

float standard_page::placeholder_header_() {
  ImGui::Dummy({0, MAIN_MENU_BAR_HEIGHT});
  return MAIN_MENU_BAR_HEIGHT;
}

void standard_page::render_header_(const float height) {
  {
    ImGuiWindow* window = ImGui::GetCurrentWindow();

    ImGui::SetNextWindowPos(window->Pos);
    ImGui::SetNextWindowSize({window->InnerRect.Max.x, height});
    ImGui::SetNextWindowBgAlpha(0.0f);
  }

  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration
                                  | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings
                                  | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin(("Header" + std::to_string(reinterpret_cast<std::uintptr_t>(this))).c_str(), nullptr, window_flags);
  ImGui::PopStyleVar(3);

  render_header_content_();

  ImGui::End();
}

void standard_page::render_header_content_() {
  render_main_menu_bar_();
}

void standard_page::reset_scroll_y_() {
  if (auto w = base_imgui_window()) {
    w->ScrollTarget.y            = 0.0f;
    w->ScrollTargetCenterRatio.y = 0.0f;
  }
}

void standard_page::render_page_() {
  ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);
  ImGui::SetNextWindowViewport(viewport->ID);
  ImGui::SetNextWindowBgAlpha(0.0f);

  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
                                  | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                                  | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin(("Page" + std::to_string(reinterpret_cast<std::uintptr_t>(this))).c_str(), nullptr, window_flags);
  ImGui::PopStyleVar(3);
  base_imgui_window_ = ImGui::GetCurrentWindow();

  // dummy item for header since we want to render at the end to overlay all content of window when scrolling.
  const float height = placeholder_header_();

  /** Push ids to prevent conflict between menu bar, content & footer. **/

  ImGui::PushID(10);
  render_content_();
  ImGui::PopID();

  ImGui::PushID(20);
  render_footer_();
  ImGui::PopID();

  // add padding to the bottom of page to have some breathing room in scroll mode.
  ImGui::Dummy({0, 10});

  // render this at the end to overlay all content of window when scrolling.
  render_header_(height);

  render_windows_();

  ImGui::End();
}

void standard_page::set_as_current_page() {
  window::imgui_render_callback([this]() { render_page_(); });

  // 'reset_scroll_y_' requires 'base_imgui_window_'
  base_imgui_window_ =
      ImGui::FindWindowByName(("Page" + std::to_string(reinterpret_cast<std::uintptr_t>(this))).c_str());
  reset_scroll_y_();
}

void standard_page::reset() {
  // 'reset_scroll_y_' requires 'base_imgui_window_'
  base_imgui_window_ =
      ImGui::FindWindowByName(("Page" + std::to_string(reinterpret_cast<std::uintptr_t>(this))).c_str());
  reset_scroll_y_();
}

} // namespace webapp