/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/page/project/hand.cpp
 * Last Modified: Saturday, 14th November 2020 5:21:05 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/page/project/hand.hpp>
#include <WebApp/gui/page/project/clarity.hpp>
#include <WebApp/gui/widget/text.hpp>
#include <WebApp/gui/widget/link.hpp>
#include <WebApp/window.hpp>

namespace webapp {

hand_page::hand_page()
    : base_project_page(
        "Humanoid Robot Hand",
        "2019 - Present  |  Individual  |  Extracurricular  |  Robotics  |  Hardware  |  C++  |  VHDL  |  MATLAB") {
  imgs_.emplace_back(clr::gfx::imgui_image::load_from_file(PRELOAD_DATA_DIR "/full_size/hand_breakout_board.png"));
  imgs_.emplace_back(clr::gfx::imgui_image::load_from_file(PRELOAD_DATA_DIR "/full_size/hand_fingers.png"));
}

void hand_page::render_content_() {
  const float old_work_rect_x               = ImGui::GetCurrentWindow()->WorkRect.Max.x;
  ImGui::GetCurrentWindow()->WorkRect.Max.x = std::min<float>(old_work_rect_x, imgs_.back()->width() + 150);
  base_project_page::render_content_();
  ImGui::GetCurrentWindow()->WorkRect.Max.x = old_work_rect_x;
}

void hand_page::render_project_content_() {
  ImGui::PushFont(semi_large_bold_font);
  ImGui::Text("Overview");
  ImGui::PopFont();

  bullet_wrapped_text("Devised custom, compact hardware for high bandwidth position sensing in order to support high "
                      "frequency sampling of dozens of sensors simultaneously.");
  bullet_wrapped_text("Designed human-sized fingers (limited space) with precise joint position feedback using "
                      "potentiometers and Hall effect sensors.");
  bullet_wrapped_text("Key Technologies");
  ImGui::Indent();
  bullet_wrapped_text("Custom software ({Clarity Studio}) for visualisation and control.",
                      {[](const char* n, ImVec2 p, bool /* hovered */) {
                        if (link::add(p, n, "View personal project")) {
                          gui_manager::change_page_next_frame_<clarity_page>();
                        }
                        return false; /* hovered */
                      }});

  bullet_wrapped_text("FPGA programmed in VHDL and MATLAB for interfacing with peripheral hardware (sensors).");
  bullet_wrapped_text("Fusion 360 for 3D modelling / CAD.");
  ImGui::Unindent();
  ImGui::Dummy({0, SECTION_SPACING});
  ImGui::PushFont(semi_large_bold_font);
  ImGui::Text("Details");
  ImGui::PopFont();

  wrapped_text("Initially, I used popular ADC breakout boards for sampling analog position sensors. Due to their "
               "popularity it was straightforward to find code to run on a Raspberry Pi or Arduino for hardware "
               "verification. It was then simple to port the code to VHDL in order to run it on an FPGA.");
  ImGui::Dummy({0, 1});
  wrapped_text(
      "However, I quickly realised there was a need for a custom solution since commonly used ADCs had very low sample "
      "rates, "
      "and as a consequence were inadequate for accurate position feedback. In my case I was using the ADS1115, which "
      "is a "
      "delta sigma ADC, with a sample rate of only 250 samples per second. This is nowhere near the amount required "
      "for sampling multiple sensors per ADC. I was also getting into problems with I2C communication because of "
      "occasional deadlocks when corrupted data was transmitted; hard reboot of the FPGA is necessary. This could be "
      "mitigated with a tedious error handling algorithm. However, due to I2C having many other flaws, such as low "
      "data "
      "rates, this idea was quickly abandoned.");
  ImGui::Dummy({0, 1});
  wrapped_text(
      "Therefore, recent prototypes instead use the ADS7951, which is a Successive Approximation ADC with a throughput "
      "rate of up to 1 mega samples per second (MSPS) and uses the SPI protocol for serial communication. SPI is "
      "immune to "
      "catastrophic errors, therefore, no tedious error handling was required; rare corruption of readings has a "
      "negligible effect on the overall system and can be easily dealt with by using a basic filter. Furthermore, SPI "
      "can work at much higher data rates, meaning data acquisition could be done much more frequently. In order to "
      "use the ADS7951, I had to design and build both the hardware (custom PCB) and software from scratch.");
  ImGui::Dummy({0, 1});
  wrapped_text(
      "Early prototypes of the finger used potentiometers since there is no trickery involved to deduce the joint "
      "angle from raw sample data. However, using a potentiometer made the design of the 3D printed parts "
      "too difficult. Therefore, later prototypes use a miniature Hall effect sensor with a diametrically magnetised "
      "ring magnet. The raw data from the Hall effect sensor is very noisy and difficult to interpret accurately, "
      "however, this is a software problem and so it is relatively easy to solve. Finally, unlike potentiometers, "
      "Hall effect sensors do not wear out over time.");
}

} // namespace webapp