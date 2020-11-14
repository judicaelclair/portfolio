/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/widget/radio.cpp
 * Last Modified: Saturday, 14th November 2020 12:19:09 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/widget/radio.hpp>
#include <WebApp/gui/util/imgui.hpp>

namespace webapp {

bool radio_button(const char* label, const bool active, const ImGuiButtonFlags flags) {
  ImGuiWindow* window = ImGui::GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext& ctx       = *ImGui::GetCurrentContext();
  const ImGuiStyle& style = ctx.Style;
  const ImGuiID id        = window->GetID(label);
  const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

  const float square_sz = ImGui::GetFrameHeight();
  const ImVec2 pos      = window->DC.CursorPos;
  const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
  const ImRect total_bb(pos,
                        pos
                            + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f),
                                     label_size.y + style.FramePadding.y * 2.0f));
  ImGui::ItemSize(total_bb, style.FramePadding.y);
  if (!ImGui::ItemAdd(total_bb, id)) {
    return false;
  }

  ImVec2 center      = check_bb.GetCenter();
  center.x           = IM_ROUND(center.x);
  center.y           = IM_ROUND(center.y);
  const float radius = (square_sz - 1.0f) * 0.5f;

  bool hovered, held;
  bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held, flags);
  if (pressed) {
    ImGui::MarkItemEdited(id);
  }

  ImGui::RenderNavHighlight(total_bb, id);
  window->DrawList->AddCircleFilled(center, radius,
                                    ImGui::GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive
                                                       : hovered         ? ImGuiCol_FrameBgHovered
                                                                         : ImGuiCol_FrameBg),
                                    16);
  if (active) {
    const float pad = ImMax(1.0f, IM_FLOOR(square_sz / 6.0f));
    window->DrawList->AddCircleFilled(center, radius - pad, ImGui::GetColorU32(ImGuiCol_CheckMark), 16);
  }

  if (style.FrameBorderSize > 0.0f) {
    window->DrawList->AddCircle(center + ImVec2(1, 1), radius, ImGui::GetColorU32(ImGuiCol_BorderShadow), 16,
                                style.FrameBorderSize);
    window->DrawList->AddCircle(center, radius, ImGui::GetColorU32(ImGuiCol_Border), 16, style.FrameBorderSize);
  }

  if (ctx.LogEnabled)
    ImGui::LogRenderedText(&total_bb.Min, active ? "(x)" : "( )");
  if (label_size.x > 0.0f) {
    if (flags & ImGuiButtonFlags_Disabled) {
      ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(ImGuiCol_TextDisabled));
    }
    ImGui::RenderText(ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y), label);
    if (flags & ImGuiButtonFlags_Disabled) {
      ImGui::PopStyleColor();
    }
  }

  IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags);
  return pressed;
}

} // namespace webapp