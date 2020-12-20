/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/page/project/path_bench.cpp
 * Last Modified: Sunday, 20th December 2020 12:59:08 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/page/project/path_bench.hpp>
#include <WebApp/gui/widget/text.hpp>
#include <WebApp/window.hpp>

namespace webapp {

path_bench_page::path_bench_page()
    : base_project_page("PathBench", "2020 - Present  |  Group  |  Academic  |  Robotics  |  Python",
                        "https://github.com/perfectly-balanced/PathBench") {
  imgs_.emplace_back(clr::gfx::imgui_image::load_from_file(PRELOAD_DATA_DIR "/full_size/path_bench_2d.png"));
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

  wrapped_text("PathBench is a 3D benchmarking framework for robot path planning - supports both classic (e.g. A*, "
               "RRT) and ML-based path planners (e.g. based on LSTMs). Source code is available on {GitHub}.",
               {wrapped_text_external_link("https://github.com/perfectly-balanced/PathBench", "View PathBench Repository")});

  ImGui::Dummy({0, SECTION_SPACING});
  ImGui::PushFont(semi_large_bold_font);
  ImGui::Text("Development");
  ImGui::PopFont();

  bullet_wrapped_text(
      "PathBench already existed before this project, however, pre-existing work only dealt with 2D algorithms.");
  bullet_wrapped_text("Added generic support for N-dimensional maps and algorithms.");
  bullet_wrapped_text(
      "Rich visualisation of 2D and 3D maps - replaced PyGame and Tkinter to exclusively use Panda3D for graphics.");
  bullet_wrapped_text("Many other things will be added later, project is still under active development.");
}

} // namespace webapp