/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/content/common.hpp
 * Last Modified: Friday, 13th November 2020 9:58:48 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */


#pragma once

#include <string_view>
using namespace std::literals::string_view_literals;

namespace webapp::content {

static inline constexpr auto people_call_me     = "Judi"sv;
static inline constexpr auto full_name          = "Judicaël CLAIR"sv;
static inline constexpr auto full_name_all_caps = "JUDICAËL CLAIR"sv;
static inline constexpr auto copyright_holders  = full_name;

static inline constexpr auto personal_email   = "clair.judicael@gmail.com"sv;
static inline constexpr auto university_email = "judicael.clair18@imperial.ac.uk"sv;
static inline constexpr auto mobile_number    = "+44 758 593 3800"sv;

static inline constexpr auto default_email = personal_email;
static inline constexpr auto github        = "https://github.com/judicaelclair"sv;
static inline constexpr auto linkedin      = "https://www.linkedin.com/in/judica%C3%ABl-clair-1b40b6194/"sv;

} // namespace webapp::content