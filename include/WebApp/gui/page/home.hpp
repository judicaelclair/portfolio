/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/page/home.hpp
 * Last Modified: Saturday, 14th November 2020 2:35:47 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <WebApp/gui/page/standard.hpp>
#include <Clarity/graphics/imgui/image.hpp>

#include <memory>

namespace webapp {

class home_page final : public standard_page {
  std::unique_ptr<clr::gfx::imgui_image> profile_;

  void render_content_() final;

public:
  home_page();
  [[nodiscard]] constexpr std::string_view name() const noexcept final { return page_name::HOME; }
};

} // namespace webapp