/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/page/project/ruined_kingdoms.cpp
 * Last Modified: Saturday, 14th November 2020 5:01:51 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/page/project/ruined_kingdoms.hpp>
#include <WebApp/gui/widget/text.hpp>
#include <WebApp/window.hpp>

namespace webapp {

ruined_kingdoms_page::ruined_kingdoms_page()
    : base_project_page("Ruined Kingdoms", "2012 - 2016  |  Individual  |  Extracurricular  |  Game  |  Python") {
  imgs_.emplace_back(clr::gfx::imgui_image::load_from_file(PRELOAD_DATA_DIR "/full_size/rk.png"));
  videos_.emplace_back(
      video_{"Ruined Kingdoms #1 - GUI", "https://youtu.be/fASTAdaQMYA",
             ": footage is from one of the later versions of the game, and demos the main parts of the GUI."});
  videos_.emplace_back(video_{
      "Ruined Kingdoms #2 - 2D Gameplay", "https://youtu.be/53y45UHClSY",
      ": footage is from midway through development, hence, GUI in this demo isn't polished (visual artefacts)."});
}

void ruined_kingdoms_page::render_content_() {
  const float old_work_rect_x               = ImGui::GetCurrentWindow()->WorkRect.Max.x;
  ImGui::GetCurrentWindow()->WorkRect.Max.x = std::min<float>(old_work_rect_x, imgs_.back()->width() + 150);
  base_project_page::render_content_();
  ImGui::GetCurrentWindow()->WorkRect.Max.x = old_work_rect_x;
}

void ruined_kingdoms_page::render_project_content_() {
  ImGui::PushFont(semi_large_bold_font);
  ImGui::Text("Description");
  ImGui::PopFont();

  wrapped_text(
      "Ruined Kingdoms is a 2D fantasy multiplayer online role playing game written in Python using {PyGame} "
      "for graphics (attempts at 3D graphics used OpenGL instead) and {Twisted} for networking.",
      {wrapped_text_external_link("https://www.pygame.org"), wrapped_text_external_link("https://twistedmatrix.com")});

  ImGui::Dummy({0, 4});
  bullet_wrapped_text("Initial prototypes were scrapped in favour of using {PyORPG} as a foundation for the game.",
                      {wrapped_text_external_link("http://www.pygame.org/project-PyORPG-2831-4640.html")});

  bullet_wrapped_text("Developed a custom GUI library from scratch for customisability, notably:");
  ImGui::Indent();
  bullet_wrapped_text(
      "fully featured input box with the ability to, for instance, select text and move cursor position.");
  bullet_wrapped_text("scrollabe text box with transparent background.");
  bullet_wrapped_text("widgets (e.g. button) can use images.");
  bullet_wrapped_text("window system.");
  ImGui::Unindent();
  bullet_wrapped_text("Reworked all aspects of the game - added a plethora of new features & fixed flaws (a minuscule "
                      "portion of these are listed below):");
  ImGui::Indent();
  bullet_wrapped_text("Entire GUI of the game was written from scratch using the aforementioned custom GUI library.");
  bullet_wrapped_text("Item, inventory and equipment systems rework:");
  ImGui::Indent();
  bullet_wrapped_text("New types of equipment - can now have necklaces and capes to name a few.");
  bullet_wrapped_text(
      "Items now have a plethora of stats and methods of upgrading the item via enchantment or forging magic stones.");
  bullet_wrapped_text(
      "Fully featured inventory with capability of storing items in specific slots, moving them around, "
      "dropping them, etc.");
  ImGui::Unindent();
  bullet_wrapped_text("Blacksmith for forging, enchanting, rerolling and repairing items.");
  bullet_wrapped_text(
      "Item mall & shops for buying items with currencies being points (i.e. real money) and gold respectively.");
  bullet_wrapped_text("Warehouse for storing and retrieving items (characters of the same account have access to the "
                      "same warehouse - item transfer possible).");
  bullet_wrapped_text("Many more features and fixed flaws/bugs not listed here.");
  ImGui::Unindent();
  bullet_wrapped_text("Attempted to port the game 3D using raw OpenGL but quickly encountered performances issues "
                      "inherently caused by Python.");
}

} // namespace webapp