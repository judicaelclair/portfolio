/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/page/home.cpp
 * Last Modified: Saturday, 14th November 2020 5:21:05 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/page/home.hpp>
#include <WebApp/gui/page/projects.hpp>
#include <WebApp/gui/page/about.hpp>
#include <WebApp/gui/manager.hpp>
#include <WebApp/gui/widget/icon_btn.hpp>
#include <WebApp/gui/widget/network_icons.hpp>
#include <WebApp/window.hpp>

namespace webapp {

home_page::home_page() : standard_page() {
  profile_ = clr::gfx::imgui_image::load_from_file(PRELOAD_DATA_DIR "/profile.png");
}

void home_page::render_content_() {
  // remove x-axis spacing for widget centering
  auto old_x_spacing              = ImGui::GetStyle().ItemSpacing.x;
  ImGui::GetStyle().ItemSpacing.x = 0;

  auto window = ImGui::GetCurrentWindow();

  auto centered_text = [window](std::string_view txt, const float offset = 0) {
    const ImVec2 txt_size = ImGui::CalcTextSize(txt.data(), txt.data() + txt.size() + 1, true);
    ImGui::Dummy({(window->Size.x - txt_size.x) / 2.0f + offset, 0});
    ImGui::SameLine();
    ImGui::Text("%.*s", static_cast<int>(txt.size()), txt.data());
  };

  // Profile Picture
  ImGui::Dummy({0, std::max<float>((window->Size.y - profile_->height()) * 1.0f / 4.0f, 20)});
  ImGui::Dummy({(window->Size.x - profile_->width() * (3.0f / 4.0f)) / 2.0f, 0});
  ImGui::SameLine();
  ImGui::Image(profile_->texture(), ImVec2(profile_->width() * (3.0f / 4.0f), profile_->height() * (3.0f / 4.0f)));
  ImGui::Dummy({0, 20});

  { // message
    static const std::string msg = "I'm " + std::string(content::people_call_me);
    ImGui::PushFont(semi_large_bold_font);
    centered_text(msg);
    ImGui::PopFont();
  }

  { // job / work description
    ImGui::PushFont(semi_large_font);
    centered_text("Software Developer  |  Aspiring Roboticist"sv, -4.0f); // offset is to line up the bar '|'
    ImGui::PopFont();
  }

  { // networking
    constexpr float NETWORK_ICON_SPACING = 10.0f;
    const float dim                      = icon_btn::dimension();

    ImGui::Dummy({0, 10});
    ImGui::Dummy({(window->Size.x - dim) / 2.0f - (dim + NETWORK_ICON_SPACING), dim});
    ImGui::SameLine();

    add_all_network_icons(NETWORK_ICON_SPACING);
  }

  { // See Projects
    ImGui::PushFont(semi_large_font);
    ImGui::Dummy({0, 5});
    centered_text("See Projects >"sv);
    ImGui::PopFont();

    const ImVec2 min = ImGui::GetItemRectMin();
    const ImVec2 max = ImGui::GetItemRectMax();

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior({min, max}, window->GetID("//btn//See Projects >"), &hovered, &held);

    if (pressed) {
      gui_manager::change_page_next_frame_<projects_page>();
    } else if (hovered) {
      // underline
      ImGui::GetWindowDrawList()->AddLine({min.x, max.y}, {max.x, max.y}, IM_COL32(255, 255, 255, 255), 1.0f);
    }
  }

  // restore x-axis spacing - was removed for widget centering
  ImGui::GetStyle().ItemSpacing.x = old_x_spacing;
}

} // namespace webapp