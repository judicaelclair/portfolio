/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/page/about.hpp
 * Last Modified: Saturday, 14th November 2020 2:35:41 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <WebApp/gui/page/standard.hpp>

namespace webapp {

class about_page final : public standard_page {
  void render_content_() final;

public:
  [[nodiscard]] constexpr std::string_view name() const noexcept final { return page_name::ABOUT; }
};

} // namespace webapp