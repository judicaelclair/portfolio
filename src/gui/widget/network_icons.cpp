/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/widget/network_icons.cpp
 * Last Modified: Friday, 13th November 2020 10:04:18 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/widget/network_icons.hpp>
#include <WebApp/gui/widget/icon_btn.hpp>
#include <WebApp/gui/util/js.hpp>
#include <WebApp/gui/icon.hpp>

#include <Clarity/core/util/str.hpp>

namespace webapp {

void add_all_network_icons(const float spacing) {
  const float dim = icon_btn::dimension();

  email_icon::add();

  ImGui::SameLine();
  ImGui::Dummy({spacing, dim});
  ImGui::SameLine();

  github_icon::add();

  ImGui::SameLine();
  ImGui::Dummy({spacing, dim});
  ImGui::SameLine();

  linkedin_icon::add();
}

namespace detail {
  [[nodiscard]] static bool add_network_icon(const char* icon, std::string_view lk, std::string_view desc) {
    const std::string tp = clr::fmt("%s %.*s\n%.*s", icon, int(desc.size()), desc.data(), int(lk.size()), lk.data());
    return icon_btn::add(icon, tp);
  }
} // namespace detail

void email_icon::add(std::string_view email, std::string_view tooltip_desc) {
  if (detail::add_network_icon(icon::ENVELOPE, email, tooltip_desc)) {
    open_email(email);
  }
}

void github_icon::add(std::string_view link, std::string_view tooltip_desc) {
  if (detail::add_network_icon(icon::GITHUB, link, tooltip_desc)) {
    open_link(link);
  }
}

void linkedin_icon::add(std::string_view link, std::string_view tooltip_desc) {
  if (detail::add_network_icon(icon::LINKEDIN, link, tooltip_desc)) {
    open_link(link);
  }
}

} // namespace webapp
