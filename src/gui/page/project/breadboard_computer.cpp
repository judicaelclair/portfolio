/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/page/project/breadboard_computer.cpp
 * Last Modified: Saturday, 16th January 2021 11:05:46 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/page/project/breadboard_computer.hpp>
#include <WebApp/gui/widget/text.hpp>
#include <WebApp/window.hpp>

namespace webapp {

breadboard_computer_page::breadboard_computer_page()
    : base_project_page("Breadboard Computer", "2015  |  Individual  |  Extracurricular  |  Hardware") {
  imgs_.emplace_back(clr::gfx::imgui_image::load_from_file(PRELOAD_DATA_DIR "/full_size/breadboard_computer.png"));
  imgs_.emplace_back(clr::gfx::imgui_image::load_from_file(PRELOAD_DATA_DIR "/full_size/ttl-comp-prototype.png"));
}

void breadboard_computer_page::render_content_() {
  const float old_work_rect_x               = ImGui::GetCurrentWindow()->WorkRect.Max.x;
  ImGui::GetCurrentWindow()->WorkRect.Max.x = std::min<float>(old_work_rect_x, imgs_.back()->width() + 150);
  base_project_page::render_content_();
  ImGui::GetCurrentWindow()->WorkRect.Max.x = old_work_rect_x;
}

void breadboard_computer_page::render_project_content_() {
  ImGui::PushFont(semi_large_bold_font);
  ImGui::Text("Description");
  ImGui::PopFont();

  wrapped_text(
      "Built an 8-bit TTL computer based on the SAP-1 (Simple-As-Possible) architecture, using discrete logic (74LS "
      "series chips) with the exception of two EEPROMS. The EEPROMS are used as lookup tables for determining the "
      "state of each control signal (whether to be high or low) based on the currently executing microcode. It can "
      "perform basic addition and subtraction with a seven-segment display as output (signed and unsigned integer "
      "format). Programs can be written into the RAM using dip switches with one of the most complicated programs it "
      "can run to be the Fibonacci sequence because of its limited amount of memory (16 bytes).");

  ImGui::PushFont(semi_large_bold_font);
  ImGui::Text("Recommended Reading");
  ImGui::PopFont();

  // TTL Cookbook
  bullet_wrapped_text(
      "{TTL Cookbook (Developer's Library)} by Donald Lancaster",
      {wrapped_text_external_link(
          "https://www.amazon.co.uk/Transistor-transistor-Logic-Cook-Developers-Library/dp/0672210355", "View book")});

  // Digital computer electronics: An introduction to microcomputers
  bullet_wrapped_text(
      "{Digital computer electronics: An introduction to microcomputers (second edition)} by Alberto Paul Malvino",
      {wrapped_text_external_link(
          "https://www.amazon.com/Digital-computer-electronics-introduction-microcomputers/dp/0070399018",
          "View book")});
}

} // namespace webapp