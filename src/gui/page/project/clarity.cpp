/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/page/project/clarity.cpp
 * Last Modified: Saturday, 14th November 2020 5:52:00 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/page/project/clarity.hpp>
#include <WebApp/gui/page/project/portfolio.hpp>
#include <WebApp/gui/page/project/hand.hpp>
#include <WebApp/gui/widget/text.hpp>
#include <WebApp/window.hpp>

namespace webapp {

clarity_page::clarity_page()
    : base_project_page("Clarity", "2017 - Present  |  Individual  |  Extracurricular  |  C++  |  Multi-threading",
                        "https://github.com/judicaelclair/clarity") {
  imgs_.emplace_back(clr::gfx::imgui_image::load_from_file(PRELOAD_DATA_DIR "/full_size/clarity_studio.png"));
}

void clarity_page::render_content_() {
  const float old_work_rect_x               = ImGui::GetCurrentWindow()->WorkRect.Max.x;
  ImGui::GetCurrentWindow()->WorkRect.Max.x = std::min<float>(old_work_rect_x, imgs_.back()->width() + 150);
  base_project_page::render_content_();
  ImGui::GetCurrentWindow()->WorkRect.Max.x = old_work_rect_x;
}

void clarity_page::render_project_content_() {
  ImGui::PushFont(semi_large_bold_font);
  ImGui::Text("Description");
  ImGui::PopFont();

  wrapped_text("Clarity is a framework that enables rapid production of multi-threaded software with advanced task "
               "scheduling, as well as a plethora of utilities. Clarity Studio is built on top of this library and "
               "offers an interactive and extensible development environment. Source code is available on {GitHub}.",
               {wrapped_text_external_link("https://github.com/judicaelclair/clarity", "View Clarity Repository")});
  ImGui::Dummy({0, SECTION_SPACING});
  wrapped_text("Currently, my {portfolio} and {humanoid robot hand} are using Clarity (Studio).",
               {wrapped_text_page_link<portfolio_page>(), wrapped_text_page_link<hand_page>()});
  ImGui::Dummy({0, SECTION_SPACING});
  ImGui::PushFont(semi_large_bold_font);
  ImGui::Text("Features");
  ImGui::PopFont();
  bullet_wrapped_text("Built from scratch with a continuing emphasis on the application architecture and scalability. "
                      "For example, invoking application::destroy() will lead to a well-defined clean destruction of "
                      "the entire application regardless of the complexity of the code base and calling context.");
  bullet_wrapped_text("Feature-rich work stealing thread pool task scheduler:");
  ImGui::Indent();
  bullet_wrapped_text("Static and dynamic task dependencies.");
  bullet_wrapped_text("Recurring tasks (keeps rescheduling itself until aborted).");
  bullet_wrapped_text("Task cancellation from any context based on a task handle, tag or regex.");
  bullet_wrapped_text(
      "Robust wait system with notifications, timeouts and forced wake-ups due to state changes (e.g. task aborted).");
  bullet_wrapped_text(
      "Many edge cases had to be considered with one of the most challenging being: finding the most natural way of "
      "aborting a task group (set of inter-dependent tasks - a static dependency construct) midway through its "
      "execution without causing catastrophic unexpected behaviour or stalling.");
  ImGui::Unindent();
  bullet_wrapped_text("Utility library contains various bits and pieces (e.g. plugin system) with a significant part "
                      "of it being over-engineered template code - leads to an extremely elegant code base since there "
                      "is significantly less code bloat.");
  bullet_wrapped_text("Graphics utilities, which leverage open-source graphics engines (e.g. google's filament).");
  bullet_wrapped_text("Clarity Studio is built on top of this library and offers an interactive and extensible "
                      "development environment.");
}

} // namespace webapp