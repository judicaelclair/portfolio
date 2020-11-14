/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/widget/search_bar.hpp
 * Last Modified: Saturday, 14th November 2020 1:35:47 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <string>
#include <vector>

#include <imgui.h>
#include <imgui_internal.h>

namespace webapp {

struct search_bar {
  search_bar() = delete;

  struct info {
    bool pressed;
    ImVec2 size;

    constexpr operator bool() const noexcept { return pressed; }
  };

  static ImVec2 size(float input_width = 200.0f);
  static info add(std::string& buf, const char* id = "###search_bar", ImU32 border_col = IM_COL32_WHITE,
                  float input_width = 200.0f);
};

} // namespace webapp