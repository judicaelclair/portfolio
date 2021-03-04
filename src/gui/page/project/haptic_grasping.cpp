/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/page/project/haptic_grasping.cpp
 * Last Modified: Thursday, 4th March 2021 3:06:17 am
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/page/project/haptic_grasping.hpp>
#include <WebApp/gui/widget/text.hpp>
#include <WebApp/gui/widget/link.hpp>
#include <WebApp/gui/util/js.hpp>
#include <WebApp/gui/icon.hpp>
#include <WebApp/window.hpp>

namespace webapp {

haptic_grasping_page::haptic_grasping_page()
    : base_project_page("Object Grasping with Haptic Feedback",
                        "2020 - Present  |  Individual  |  Academic  |  Robotics  |  Python") {
  imgs_.emplace_back(clr::gfx::imgui_image::load_from_file(PRELOAD_DATA_DIR "/full_size/gpis_bunny.png"));
  imgs_.emplace_back(clr::gfx::imgui_image::load_from_file(PRELOAD_DATA_DIR "/full_size/gpis_bowl.png"));
}

void haptic_grasping_page::render_content_() {
  const float old_work_rect_x               = ImGui::GetCurrentWindow()->WorkRect.Max.x;
  ImGui::GetCurrentWindow()->WorkRect.Max.x = std::min<float>(old_work_rect_x, imgs_.back()->width() + 150);
  base_project_page::render_content_();
  ImGui::GetCurrentWindow()->WorkRect.Max.x = old_work_rect_x;
}

void haptic_grasping_page::render_project_content_() {
  ImGui::PushFont(semi_large_bold_font);
  ImGui::Text("Description");
  ImGui::PopFont();

  wrapped_text("This is my final year BEng individual research project; noting that {Prof Andrew Davison} is the "
               "supervisor. The goal is, given no knowledge of the surroundings and relying exclusively on haptic "
               "feedback, to locate, identify, and accurately estimate an object's shape for subsequent manipulation.",
               {wrapped_text_external_link("https://www.doc.ic.ac.uk/~ajd/", "View his website")});

  ImGui::Dummy({0, 10});

  wrapped_text(
      "For shape estimation, I have experimented with Gaussian Process Implicit Surface (GPIS) potentials "
      "(see figures). However, since tactile data is sparse, strong priors must be used in order to perform "
      "accurate object reconstruction with few contact points. To this end, I am now using a class-conditioned "
      "variational autoencoder that outputs a reconstructed occupancy grid.");

  ImGui::Dummy({0, 10});

  wrapped_text("As of writing this summary (12th February 2021), the project was still in its infancy; this is just a taster.");
}

} // namespace webapp