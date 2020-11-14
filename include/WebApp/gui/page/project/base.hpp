/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/page/project/base.hpp
 * Last Modified: Saturday, 14th November 2020 2:35:13 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <WebApp/gui/page/standard.hpp>
#include <Clarity/graphics/imgui/image.hpp>

#include <string>
#include <vector>
#include <memory>

namespace webapp {

class base_project_page : public standard_page {
  std::string title_;
  std::string tags_;
  std::string git_;

protected:
  struct video_ {
    std::string name;
    std::string link;
    std::string desc;
  };

  [[nodiscard]] ImVec2 header_size_();
  constexpr virtual void render_project_content_() = 0;
  void render_content_();

  std::vector<std::unique_ptr<clr::gfx::imgui_image>> imgs_;
  std::vector<video_> videos_;

public:
  base_project_page(std::string title, std::string tags, std::string git = "");

  static inline constexpr float INDENTATION     = 15.0f;
  static inline constexpr float SECTION_SPACING = 5.0f;
};

} // namespace webapp