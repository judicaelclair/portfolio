/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/page/about.cpp
 * Last Modified: Saturday, 14th November 2020 7:12:52 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/page/about.hpp>
#include <WebApp/gui/widget/text.hpp>
#include <WebApp/gui/widget/link.hpp>
#include <WebApp/gui/util/js.hpp>
#include <WebApp/gui/icon.hpp>
#include <WebApp/window.hpp>

#include <Clarity/core/util/str.hpp>

namespace webapp {

void about_page::render_content_() {
  constexpr auto BODY_WIDTH = 800.0f;

  auto& window = *ImGui::GetCurrentWindow();
  auto& style  = ImGui::GetStyle();

  auto old_x_spacing          = style.ItemSpacing.x;
  style.ItemSpacing.x         = 0;
  const float old_work_rect_x = window.WorkRect.Max.x;
  window.WorkRect.Max.x       = std::min<float>(old_work_rect_x, (window.InnerRect.Max.x + BODY_WIDTH) / 2.0f);
  const float indentation     = (window.InnerRect.Max.x - BODY_WIDTH) / 2.0f;
  ImGui::Dummy({0, 15});
  ImGui::Indent(indentation);

  wrapped_text("My long term goal is to develop an autonomous humanoid robot to assist mobility impaired people and "
               "provide disaster relief.");
  ImGui::Dummy({0, 5});
  wrapped_text("My current focus is on dexterous robot manipulation, as well as high-performance scientific computing. "
               "I also have a growing interest in distributed reinforcement learning and physics "
               "simulations; particularly in the context of robot motion and control.");
  ImGui::Dummy({0, 30});
  ImGui::PushFont(semi_large_bold_font);
  { // CV link
    constexpr const char TOOLTIP[]   = "View file";
    static const std::string CV_LINK = []() {
      auto url = current_url();
      if (url.back() != '/') {
        url += '/';
      }
      url += "CV.pdf";
      return url;
    }();

    const char* s_txt        = "Curriculum Vitae (CV)";
    const std::string s_icon = clr::fmt("  %s", icon::PDF_FILE);

    { // centering
      const float txt_sz_x  = ImGui::CalcTextSize(s_txt).x + ImGui::CalcTextSize(s_icon.c_str()).x;
      window.DC.CursorPos.x = (window.InnerRect.Max.x - txt_sz_x) / 2.0f;
    }

    link::info i_txt;
    const ImVec2 txt_pos = window.DC.CursorPos;
    if ((i_txt = link::add<link::NO_BUTTON_COLOUR | link::NO_NEUTRAL_UNDERLINE>(s_txt, TOOLTIP, default_font))) {
      open_link(CV_LINK);
    }
    ImGui::SameLine();
    if (auto i_icon = link::add<link::NO_BUTTON_COLOUR | link::NO_UNDERLINE>(s_icon, TOOLTIP); i_icon) {
      open_link(CV_LINK);
    } else if (i_icon.hovered) [[unlikely]] {
      link::underline<link::NO_BUTTON_COLOUR>(txt_pos, i_txt.size, i_icon.hovered);
    }
  }
  ImGui::PopFont();

  ImGui::Unindent(indentation);
  window.WorkRect.Max.x = old_work_rect_x;
  style.ItemSpacing.x   = old_x_spacing;
}

} // namespace webapp