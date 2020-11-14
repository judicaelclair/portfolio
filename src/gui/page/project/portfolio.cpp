/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/page/project/portfolio.cpp
 * Last Modified: Friday, 13th November 2020 10:07:34 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/page/project/portfolio.hpp>
#include <WebApp/gui/widget/text.hpp>
#include <WebApp/window.hpp>

namespace webapp {

portfolio_page::portfolio_page()
    : base_project_page("Portfolio", "2020 - Present  |  Individual  |  Extracurricular  |  Web App  |  C++",
                        "https://github.com/judicaelclair/portfolio") {}

void portfolio_page::render_project_content_() {
  wrapped_text("My personal portfolio website (this site) is a C++ Web App compiled to WebAssembly using Emscripten. "
               "Source code is available on {GitHub}.",
               {wrapped_text_external_link("https://github.com/judicaelclair/portfolio", "View Portfolio Repository")});

  ImGui::Dummy({0, SECTION_SPACING});
  ImGui::PushFont(semi_large_bold_font);
  ImGui::Text("OSS");
  ImGui::PopFont();

  wrapped_text(
      "This software uses open source components, whose copyright and other rights belong to their respective owners:");

  bullet_wrapped_text("{Emscripten}", {wrapped_text_external_link("https://github.com/emscripten-core/emsdk")});
  bullet_wrapped_text("{GLFW}", {wrapped_text_external_link("https://github.com/glfw/glfw")});
  bullet_wrapped_text("{Dear ImGui}", {wrapped_text_external_link("https://github.com/ocornut/imgui")});
  bullet_wrapped_text("{IconFontCppHeaders}",
                      {wrapped_text_external_link("https://github.com/juliettef/IconFontCppHeaders")});
  bullet_wrapped_text("{ImGuiFontStudio}", {wrapped_text_external_link("https://github.com/aiekick/ImGuiFontStudio")});
  bullet_wrapped_text("{stb}", {wrapped_text_external_link("https://github.com/nothings/stb")});
}

} // namespace webapp