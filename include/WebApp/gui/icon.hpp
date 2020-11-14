/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/icon.hpp
 * Last Modified: Saturday, 14th November 2020 1:36:15 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <Clarity/graphics/imgui/font/font_manager.hpp>

namespace webapp {

struct icon {
  static inline constexpr char ARROW_LEFT[]   = "\uf060";
  static inline constexpr char ARROW_RIGHT[]  = "\uf061";
  static inline constexpr char COPYRIGHT[]    = "\uf1f9";
  static inline constexpr char ENVELOPE[]     = "\uf0e0";
  static inline constexpr char PDF_FILE[]     = "\uf1c1";
  static inline constexpr char GITHUB[]       = "\uf09b";
  static inline constexpr char LINK[]         = "\uf0c1";
  static inline constexpr char LINKEDIN[]     = "\uf08c";
  static inline constexpr char MINUS_SQUARE[] = "\uf146";
  static inline constexpr char SEARCH[]       = "\uf002";
};

[[nodiscard]] clr::gfx::imgui_font_component icons_font_component(float size);

} // namespace webapp