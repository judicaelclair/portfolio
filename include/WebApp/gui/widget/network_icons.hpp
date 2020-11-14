/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/widget/network_icons.hpp
 * Last Modified: Saturday, 14th November 2020 1:35:24 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <WebApp/content/common.hpp>

namespace webapp {

void add_all_network_icons(const float spacing = 10.0f);

struct email_icon {
  email_icon() = delete;
  static void add(std::string_view email = content::default_email, std::string_view tooltip_desc = "Send email");
};

struct github_icon {
  github_icon() = delete;
  static void add(std::string_view link = content::github, std::string_view tooltip_desc = "Open GitHub profile");
};

struct linkedin_icon {
  linkedin_icon() = delete;
  static void add(std::string_view link = content::linkedin, std::string_view tooltip_desc = "Open LinkedIn profile");
};

} // namespace webapp