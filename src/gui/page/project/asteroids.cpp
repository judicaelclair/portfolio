/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/page/project/asteroids.cpp
 * Last Modified: Saturday, 14th November 2020 5:01:45 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/page/project/asteroids.hpp>
#include <WebApp/gui/widget/text.hpp>
#include <WebApp/window.hpp>

namespace webapp {

asteroids_page::asteroids_page()
    : base_project_page("Asteroids on Steroids",
                        "2019  |  Group  |  Academic  |  Game  |  Hardware  |  C  |  Bare Metal") {
  imgs_.emplace_back(clr::gfx::imgui_image::load_from_file(PRELOAD_DATA_DIR "/full_size/asteroids.png"));
  imgs_.emplace_back(clr::gfx::imgui_image::load_from_file(PRELOAD_DATA_DIR "/full_size/asteroids-hardware.png"));
  videos_.emplace_back(video_{"Demonstration", "https://youtu.be/H6RezIn-w94", ""});
}

void asteroids_page::render_content_() {
  const float old_work_rect_x               = ImGui::GetCurrentWindow()->WorkRect.Max.x;
  ImGui::GetCurrentWindow()->WorkRect.Max.x = std::min<float>(old_work_rect_x, imgs_.back()->width() + 150);
  base_project_page::render_content_();
  ImGui::GetCurrentWindow()->WorkRect.Max.x = old_work_rect_x;
}

void asteroids_page::render_project_content_() {
  ImGui::PushFont(semi_large_bold_font);
  ImGui::Text("Description");
  ImGui::PopFont();

  wrapped_text("Bare Metal game similar to the arcade game Asteroids but on steroids, running on a Raspberry Pi."
               " No standard or third-party libraries were used.");
  ImGui::Dummy({0, 3});
  bullet_wrapped_text(
      "Custom fixed-point library (e.g. sin, cos, arctan) - no floating-point arithmetic was used in the game.");
  bullet_wrapped_text("Interfaced with analog joystick using SPI (bit-banging).");
  bullet_wrapped_text("GPU interfacing for video output (double-buffered) through mailbox calls.");
  bullet_wrapped_text(
      "Initially game was running at ~2 frames per second (FPS), however, we eventually managed to achieve ~100 FPS!");
  bullet_wrapped_text("Aside from the traditional weapon (white), you have a limited supply of heat-seeking missiles "
                      "(red) and space mines (green); they replenish over time.");
}

} // namespace webapp