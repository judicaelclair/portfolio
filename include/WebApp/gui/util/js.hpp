/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/util/js.hpp
 * Last Modified: Saturday, 14th November 2020 2:36:06 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <string_view>
#include <string>

namespace webapp {

namespace detail {

  extern "C" void open_email(const char* s, int len);
  extern "C" void open_link(const char* s, int len);
  extern "C" void open_link_popup(const char* s, int len);
  extern "C" const char* current_url();

} // namespace detail

void open_email(std::string_view email);
void open_link(std::string_view link);
void open_link_popup(std::string_view link);
[[nodiscard]] std::string current_url();

} // namespace webapp