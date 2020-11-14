/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/icon.cpp
 * Last Modified: Saturday, 14th November 2020 2:38:22 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/icon.hpp>

#define FONT_ICON_FILE_NAME "icons.ttf"

#define ICON_MIN 0xf002
#define ICON_MAX 0xf1f9

namespace webapp {

clr::gfx::imgui_font_component icons_font_component(const float size) {
  // static is required to persist its lifetime
  static const ImWchar ICONS_FONT_RANGES[] = {ICON_MIN, ICON_MAX, 0};

  ImFontConfig icons_config;
  icons_config.PixelSnapH = true;

  return {clr::gfx::font_manager::add_font_with_name(FONT_ICON_FILE_NAME), size, std::move(icons_config),
          ICONS_FONT_RANGES};
}

} // namespace webapp