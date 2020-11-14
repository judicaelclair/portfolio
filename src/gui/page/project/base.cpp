/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/page/project/base.cpp
 * Last Modified: Saturday, 14th November 2020 5:07:20 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/page/project/ruined_kingdoms.hpp>
#include <WebApp/gui/widget/text.hpp>
#include <WebApp/window.hpp>
#include <WebApp/gui/widget/icon_btn.hpp>
#include <WebApp/gui/widget/network_icons.hpp>
#include <WebApp/gui/widget/link.hpp>
#include <WebApp/gui/page/projects.hpp>
#include <WebApp/gui/util/js.hpp>

namespace webapp {

base_project_page::base_project_page(std::string title, std::string tags, std::string git)
    : title_(std::move(title)), tags_(std::move(tags)), git_(std::move(git)) {}

ImVec2 base_project_page::header_size_() {
  auto& window = *ImGui::GetCurrentWindow();

  ImVec2 sz;
  sz.x = window.InnerRect.Max.x;
  sz.y = 5.0f + ImGui::GetStyle().ItemSpacing.y;
  ImGui::PushFont(subtitle_font);
  sz.y -= 2.0f;
  sz.y += ImGui::CalcTextSize("PROJECT TITLE").y;
  ImGui::PopFont();
  sz.y += ImGui::GetStyle().ItemSpacing.y;
  sz.y += ImGui::CalcTextSize("Some | Tag").y;
  sz.y += ImGui::GetStyle().ItemSpacing.y;
  return sz;
}

void base_project_page::render_content_() {
  const auto hsz = header_size_();
  ImGui::Dummy({0, SECTION_SPACING + hsz.y});
  ImGui::Indent(INDENTATION);

  for (auto& i : imgs_) {
    const auto w       = ImGui::GetCurrentWindow();
    const float max_sz = w->InnerRect.Max.x - 2 * w->DC.CursorPos.x;
    const float shrink = (i->width() < max_sz) ? 1.0f : (max_sz / i->width());
    const ImVec2 size{i->width() * shrink, i->height() * shrink};
    ImGui::Image(i->texture(), size);
  }
  if (!imgs_.empty()) {
    ImGui::Dummy({0, SECTION_SPACING});
  }

  if (!videos_.empty()) {
    ImGui::PushFont(semi_large_bold_font);
    ImGui::Text("Videos");
    ImGui::PopFont();
    for (auto const& v : videos_) {
      bullet_wrapped_text(clr::fmt("{%s} %s", v.name.c_str(), v.desc.c_str()),
                          {wrapped_text_external_link(v.link, "Open video")});
    }
    ImGui::Dummy({0, SECTION_SPACING});
  }

  render_project_content_();

  ImGui::Unindent(INDENTATION);

  {
    ImGuiWindow* const window   = ImGui::GetCurrentWindow();
    ImDrawList* const draw_list = window->DrawList;

    // latches onto the top of the footer so that it doens't hover over the footer
    const ImVec2 pos{0, std::min<float>(footer_rect_min().y - hsz.y, MAIN_MENU_BAR_HEIGHT)};

    draw_list->AddRectFilled({pos.x, 0}, {pos.x + hsz.x, pos.y + hsz.y}, window::CLEAR_COLOUR);
    ImGui::SetNextWindowPos(pos);
    ImGui::SetNextWindowSize(hsz);
  }

  ImGui::SetNextWindowBgAlpha(0.0f);

  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration
                                  | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings
                                  | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  if (ImGui::Begin("project_header", nullptr, window_flags)) {
    ImGui::PopStyleVar(3);
    ImGui::Dummy({0, 5});
    ImGui::Indent(INDENTATION);

    ImGui::GetCurrentWindow()->DC.CursorPos.y -= 2.0f;
    const float title_x = ImGui::GetCurrentWindow()->DC.CursorPos.x;
    ImGui::PushFont(subtitle_font);
    ImGui::Text("%s", title_.c_str());
    ImGui::PopFont();
    ImGui::GetCurrentWindow()->DC.CursorPos.y += 2.0f;

    if (!git_.empty()) {
      ImGui::SameLine();
      ImGui::Dummy({0, 0});
      ImGui::SameLine();
      ImGui::GetCurrentWindow()->DC.CursorPos.y += 3.0f;
      github_icon::add(git_, "Open GitHub Repository");
      ImGui::GetCurrentWindow()->DC.CursorPos.y -= 3.0f;
    }

    constexpr auto TAG_COLOUR                 = IM_COL32(255, 255, 255, 88);
    ImGui::GetCurrentWindow()->DC.CursorPos.x = title_x;
    ImGui::PushStyleColor(ImGuiCol_Text, TAG_COLOUR);
    ImGui::Text("%s", tags_.c_str());
    ImGui::PopStyleColor();

    ImGui::Unindent(INDENTATION);
  } else {
    ImGui::PopStyleVar(3);
  }
  ImGui::End();
}

} // namespace webapp