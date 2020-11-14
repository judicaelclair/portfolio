/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/widget/icon_btn.cpp
 * Last Modified: Saturday, 14th November 2020 1:38:16 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/widget/icon_btn.hpp>
#include <WebApp/gui/icon.hpp>

namespace webapp {

float icon_btn::dimension() {
  static float value = []() {
    ImGui::PushFont(semi_large_bold_font);
    const auto sz = ImGui::CalcTextSize(icon::ENVELOPE);
    ImGui::PopFont();
    return sz.x;
  }();
  return value;
}

} // namespace webapp
