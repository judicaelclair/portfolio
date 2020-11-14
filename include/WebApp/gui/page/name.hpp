/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/page/name.hpp
 * Last Modified: Saturday, 14th November 2020 2:35:53 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <string_view>
using namespace std::literals::string_view_literals;

namespace webapp {

struct page_name {
  static inline constexpr std::string_view ASTEROIDS           = "asteroids"sv;
  static inline constexpr std::string_view BREADBOARD_COMPUTER = "breadboard_computer"sv;
  static inline constexpr std::string_view CLARITY             = "clarity"sv;
  static inline constexpr std::string_view HAND                = "hand"sv;
  static inline constexpr std::string_view PATH_BENCH          = "path_bench"sv;
  static inline constexpr std::string_view PORTFOLIO           = "portfolio"sv;
  static inline constexpr std::string_view RUINED_KINGDOMS     = "ruined_kingdoms"sv;
  static inline constexpr std::string_view ABOUT               = "about"sv;
  static inline constexpr std::string_view HOME                = "home"sv;
  static inline constexpr std::string_view PROJECTS            = "projects"sv;
};

} // namespace webapp