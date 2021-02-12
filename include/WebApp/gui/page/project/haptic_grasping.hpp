/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/page/project/haptic_grasping.hpp
 * Last Modified: Thursday, 11th February 2021 11:57:50 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <WebApp/gui/page/project/base.hpp>

namespace webapp {

class haptic_grasping_page final : public base_project_page {
  void render_content_() final;
  void render_project_content_() final;

public:
  haptic_grasping_page();
  [[nodiscard]] constexpr std::string_view name() const noexcept final { return page_name::HAPTIC_GRASING; }
};

} // namespace webapp