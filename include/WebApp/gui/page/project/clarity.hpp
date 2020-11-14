/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/page/project/clarity.hpp
 * Last Modified: Saturday, 14th November 2020 5:10:51 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <WebApp/gui/page/project/base.hpp>

namespace webapp {

class clarity_page final : public base_project_page {
  void render_content_() final;
  void render_project_content_() final;

public:
  clarity_page();
  [[nodiscard]] constexpr std::string_view name() const noexcept final { return page_name::CLARITY; }
};

} // namespace webapp