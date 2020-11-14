/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/widget/search_bar.cpp
 * Last Modified: Saturday, 14th November 2020 1:38:29 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/widget/search_bar.hpp>
#include <WebApp/gui/util/imgui.hpp>
#include <WebApp/gui/icon.hpp>
#include <imgui_stdlib.h>

#include <Clarity/core/util/bit.hpp>

namespace webapp {

static constexpr auto BTN_OFFSET_ = 10;

ImVec2 search_bar::size(const float input_width) {
  auto& ctx   = *ImGui::GetCurrentContext();
  auto& style = ctx.Style;

  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {style.FramePadding.x, style.FramePadding.y + 3});
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, ImGui::GetStyle().FrameRounding);

  const auto btn_sz = get_button_size(icon::SEARCH);

  ImGui::PopStyleVar(2); // padding & rounding

  return {input_width + BTN_OFFSET_ + btn_sz.x, btn_sz.y};
}

search_bar::info search_bar::add(std::string& buf, const char* const id, const ImU32 border_col,
                                 const float input_width) {
  info return_data;
  auto const& sz = (return_data.size = size(input_width));

  auto& ctx    = *ImGui::GetCurrentContext();
  auto& window = *ImGui::GetCurrentWindow();
  auto& style  = ctx.Style;
  auto& dlist  = window.DrawList;

  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {style.FramePadding.x, style.FramePadding.y + 3});
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, ImGui::GetStyle().FrameRounding);

  const auto min = window.DC.CursorPos;

  dlist->AddRectFilled(min, {min.x + sz.x, min.y + sz.y}, ImGui::GetColorU32(style.Colors[ImGuiCol_FrameBg]),
                       ImGui::GetStyle().FrameRounding);

  ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0)); // transparent frame

  ImGui::SetNextItemWidth(input_width);
  return_data.pressed =
      ImGui::InputText(id, &buf, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll);
  if (!ImGui::IsItemActive() && buf.empty()) {
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 51));
    constexpr char txt[] = "Search...";
    ImGui::RenderText({min.x + style.ItemInnerSpacing.x, min.y + style.FramePadding.y}, txt, txt + sizeof(txt));
    ImGui::PopStyleColor();
  }
  ImGui::PopStyleColor(); // transparent frame

  const bool focused = ImGui::IsItemActive();

  ImGui::SameLine();
  window.DC.CursorPos.x = ImGui::GetItemRectMax().x + BTN_OFFSET_;
  return_data.pressed |= ImGui::ButtonEx(
      icon::SEARCH, {0, 0},
      static_cast<ImGuiButtonFlags>(clr::conditional_bit_field(buf.empty(), ImGuiButtonFlags_Disabled)));

  if (focused) {
    dlist->AddRect({min.x - 0.5f, min.y - 0.5f}, {min.x + sz.x + 0.5f, min.y + sz.y + 0.5f}, border_col,
                   ImGui::GetStyle().FrameRounding, ImDrawCornerFlags_All, 1.5f);
  }

  ImGui::PopStyleVar(2); // padding & rounding

  return return_data;
}

} // namespace webapp
