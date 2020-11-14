/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/page/project/breadboard_computer.hpp
 * Last Modified: Saturday, 14th November 2020 2:35:16 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <WebApp/gui/page/project/base.hpp>
#include <Clarity/graphics/imgui/image.hpp>

namespace webapp {

class breadboard_computer_page final : public base_project_page {
  void render_project_content_() final;
  void render_content_() final;

public:
  breadboard_computer_page();
  [[nodiscard]] constexpr std::string_view name() const noexcept final { return page_name::BREADBOARD_COMPUTER; }
};

} // namespace webapp