/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/widget/icon_btn.hpp
 * Last Modified: Saturday, 14th November 2020 1:35:04 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <imgui.h>
#include <imgui_internal.h>

#include <WebApp/window.hpp>
#include <WebApp/gui/widget/link.hpp>

#include <Clarity/core/util/str.hpp>

namespace webapp {

struct icon_btn {
  icon_btn() = delete;

  [[nodiscard]] static float dimension();

  template <typename... Args>
  [[nodiscard]] static auto add(Args&&... args) {
    ImGui::PushFont(icons_font);
    auto info = link::add<link::NO_UNDERLINE | link::NO_BUTTON_COLOUR>(std::forward<Args>(args)..., default_font);
    ImGui::PopFont();
    return info;
  }
};

} // namespace webapp
