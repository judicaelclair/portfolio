/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/util/imgui.cpp
 * Last Modified: Saturday, 14th November 2020 12:15:22 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/util/imgui.hpp>
#include <imgui_internal.h>

namespace webapp {

ImVec2 get_button_size(const char* lbl) {
  const ImGuiStyle& style = ImGui::GetCurrentContext()->Style;
  const ImVec2 lbl_size   = ImGui::CalcTextSize(lbl, nullptr, true);
  return ImGui::CalcItemSize({0, 0}, lbl_size.x + style.FramePadding.x * 2.0f,
                             lbl_size.y + style.FramePadding.y * 2.0f);
}

void help_marker(const char* desc) {
  ImGui::TextDisabled("(?)");
  if (ImGui::IsItemHovered()) {
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(desc);
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }
}

} // namespace webapp