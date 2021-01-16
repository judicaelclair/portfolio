/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/page/project/path_bench.cpp
 * Last Modified: Saturday, 16th January 2021 11:15:03 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/page/project/path_bench.hpp>
#include <WebApp/gui/widget/text.hpp>
#include <WebApp/gui/widget/link.hpp>
#include <WebApp/gui/util/js.hpp>
#include <WebApp/gui/icon.hpp>
#include <WebApp/window.hpp>

namespace webapp {

path_bench_page::path_bench_page()
    : base_project_page("PathBench", "2020 - 2021  |  Group  |  Academic  |  Robotics  |  Python",
                        "https://github.com/perfectly-balanced/PathBench") {
  imgs_.emplace_back(clr::gfx::imgui_image::load_from_file(PRELOAD_DATA_DIR "/full_size/path_bench.png"));
}

void path_bench_page::render_content_() {
  const float old_work_rect_x               = ImGui::GetCurrentWindow()->WorkRect.Max.x;
  ImGui::GetCurrentWindow()->WorkRect.Max.x = std::min<float>(old_work_rect_x, imgs_.back()->width() + 150);
  base_project_page::render_content_();
  ImGui::GetCurrentWindow()->WorkRect.Max.x = old_work_rect_x;
}

void path_bench_page::render_project_content_() {
  ImGui::PushFont(semi_large_bold_font);
  ImGui::Text("Description");
  ImGui::PopFont();

  wrapped_text(
      "PathBench is an open-source motion planning platform used to develop, assess, compare and visualise the "
      "performance and behaviour of both classic and machine learning-based robot path planners in a two or "
      "three-dimensional space. Source code is available on {GitHub}.",
      {wrapped_text_external_link("https://github.com/perfectly-balanced/PathBench", "View PathBench Repository")});

  auto& window = *ImGui::GetCurrentWindow();
  ImGui::PushFont(semi_large_bold_font);
  { // report link
    constexpr const char TOOLTIP[]   = "View file";
    static const std::string CV_LINK = []() {
      auto url = current_url();
      if (url.back() != '/') {
        url += '/';
      }
      url += "path_bench_report.pdf";
      return url;
    }();

    ImGui::Dummy({0,10});

    const char* s_txt        = "Report";
    const std::string s_icon = clr::fmt("  %s", icon::PDF_FILE);

    link::info i_txt;
    const ImVec2 txt_pos = window.DC.CursorPos;
    if ((i_txt = link::add<link::NO_BUTTON_COLOUR | link::NO_NEUTRAL_UNDERLINE>(s_txt, TOOLTIP, default_font))) {
      open_link(CV_LINK);
    }
    ImGui::SameLine();
    if (auto i_icon = link::add<link::NO_BUTTON_COLOUR | link::NO_UNDERLINE>(s_icon, TOOLTIP); i_icon) {
      open_link(CV_LINK);
    } else if (i_icon.hovered)
      [[unlikely]] { link::underline<link::NO_BUTTON_COLOUR>(txt_pos, i_txt.size, i_icon.hovered); }
  }
  ImGui::PopFont();
}

} // namespace webapp