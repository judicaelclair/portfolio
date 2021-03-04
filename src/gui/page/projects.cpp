/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/page/projects.cpp
 * Last Modified: Thursday, 4th March 2021 4:54:08 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/page/projects.hpp>
#include <WebApp/gui/util/imgui.hpp>
#include <WebApp/gui/util/js.hpp>
#include <WebApp/gui/widget/icon_btn.hpp>
#include <WebApp/gui/widget/network_icons.hpp>
#include <WebApp/gui/widget/link.hpp>
#include <WebApp/gui/widget/text.hpp>
#include <WebApp/gui/widget/radio.hpp>
#include <WebApp/gui/widget/search_bar.hpp>
#include <WebApp/gui/page/project/asteroids.hpp>
#include <WebApp/gui/page/project/breadboard_computer.hpp>
#include <WebApp/gui/page/project/clarity.hpp>
#include <WebApp/gui/page/project/hand.hpp>
#include <WebApp/gui/page/project/haptic_grasping.hpp>
#include <WebApp/gui/page/project/path_bench.hpp>
#include <WebApp/gui/page/project/portfolio.hpp>
#include <WebApp/gui/page/project/ruined_kingdoms.hpp>
#include <WebApp/gui/icon.hpp>
#include <WebApp/window.hpp>

#include <Clarity/core/util/str.hpp>
#include <Clarity/graphics/imgui/font/font_manager.hpp>

#include <imgui_stdlib.h>

#if __has_include(<Clarity/graphics/window/common.hpp>)
  #include <Clarity/graphics/window/common.hpp>
#else /** __has_include(<Clarity/graphics/window/common.hpp>) **/
  #include <GLFW/glfw3.h>

namespace clr::gfx {
struct key {
  using type = int;
  enum types : key::type {
    KEY_LEFT_CTRL  = GLFW_KEY_LEFT_CONTROL,
    KEY_RIGHT_CTRL = GLFW_KEY_RIGHT_CONTROL,
    KEY_Z          = GLFW_KEY_Z,
    KEY_X          = GLFW_KEY_X
  };
};
} // namespace clr::gfx
#endif /** __has_include(<Clarity/graphics/window/common.hpp>) **/

using clr::gfx::key;

namespace webapp {

project::project(projects_page& page, std::string name, std::string description, std::initializer_list<size_t> date,
                 std::function<void()> open_callback, std::unique_ptr<clr::gfx::imgui_image> thumbnail, std::string git,
                 size_t relevance)
    : project(
        page, std::move(name), std::move(description),
        [&date]() -> std::pair<size_t, size_t> {
          if (date.size() == 0) {
            return {std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max()};
          } else if (date.size() == 1) {
            return {*date.begin(), *date.begin()};
          } else if (date.size() == 2) {
            return {*date.begin(), *(date.begin() + 1)};
          } else {
            throw std::runtime_error("invalid number of arguments for project date");
          }
        }(),
        std::move(open_callback), std::move(thumbnail), std::move(git), relevance) {}

project::project(projects_page& page, std::string name, std::string description, std::pair<size_t, size_t> date,
                 std::function<void()> open_callback, std::unique_ptr<clr::gfx::imgui_image> thumbnail, std::string git,
                 size_t relevance)
    : git_(std::move(git)), name_(std::move(name)), description_(std::move(description)), date_(std::move(date)),
      open_callback_(std::move(open_callback)), thumbnail_(std::move(thumbnail)), tag_groups_(page.tag_groups_),
      relevance_(relevance) {}

void project::add_tag(project_tag* tag) {
  for (auto t : tags_) {
    if (t == tag) {
      return;
    }
  }
  tags_.emplace_back(tag);
}

void project::add_tag(std::string_view tag) {
  for (auto const& tg : tag_groups_.get()) {
    for (auto const& t : tg->tags) {
      if (t->name == tag) {
        add_tag(t.get());
        return;
      }
    }
  }
  throw std::runtime_error(clr::fmt("%.*s tag not found", static_cast<int>(tag.size()), tag.data()));
}

// todo:
// Should be building 'tag_section_title_font_' here by scheduling a build for subsequent frame & rebuilding atlas.
// Atm it is being done in 'window::init_()'.
projects_page::projects_page()
    : listing_page(), filterer_(
                          items_view_,
                          [this]() {
                            update_tag_ui_state_();
                            base::refresh_();
                          },
                          [this]() {
                            update_tag_ui_state_(true);
                            page_index_(0);
                          }),
      tag_section_title_font_(clr::gfx::font_manager::find_imgui_font(
                                  {{clr::gfx::font_manager::find_font_with_name("barlow.extrabold.ttf"), 19.f}})
                                  ->compiled_font()) {
  using D = std::initializer_list<size_t>;

  visible_checker_([](project& p) { return p.visible(); });

  /** TAGS **/

  { // Work Context Tags
    auto& tg = tag_groups_.emplace_back(std::make_unique<project_tag_group>("Context"));
    // tg->tags.emplace_back(std::make_unique<project_tag>("Professional"));
    tg->tags.emplace_back(std::make_unique<project_tag>("Academic"));
    tg->tags.emplace_back(std::make_unique<project_tag>("Extracurricular"));
    tg->tags.emplace_back(std::make_unique<project_tag>("Individual"));
    tg->tags.emplace_back(std::make_unique<project_tag>("Group"));
  }

  { // General Category Tags
    auto& tg = tag_groups_.emplace_back(std::make_unique<project_tag_group>("General"));
    tg->tags.emplace_back(std::make_unique<project_tag>("Robotics"));
    tg->tags.emplace_back(std::make_unique<project_tag>("Hardware"));
    tg->tags.emplace_back(std::make_unique<project_tag>("Embedded"));
    tg->tags.emplace_back(std::make_unique<project_tag>("Web App"));
    tg->tags.emplace_back(std::make_unique<project_tag>("Game"));
  }

  { // Language Tags
    auto& tg = tag_groups_.emplace_back(std::make_unique<project_tag_group>("Language"));
    tg->tags.emplace_back(std::make_unique<project_tag>("C"));
    tg->tags.emplace_back(std::make_unique<project_tag>("C++"));
    tg->tags.emplace_back(std::make_unique<project_tag>("Python"));
    tg->tags.emplace_back(std::make_unique<project_tag>("MATLAB"));
    tg->tags.emplace_back(std::make_unique<project_tag>("VHDL"));
  }

  {
      // Technology Tags
      // auto& tg = tag_groups_.emplace_back(std::make_unique<project_tag_group>("Technology"));
      // tg->tags.emplace_back(std::make_unique<project_tag>("OpenCL"));
      // tg->tags.emplace_back(std::make_unique<project_tag>("SYCL"));
  }

  {
      // Miscellaneous
      // auto& tg = tag_groups_.emplace_back(std::make_unique<project_tag_group>("Miscellaneous"));
  }

  /** PROJECTS **/

  { // Final year BEng research project
    {
      constexpr const char NAME[] = "CORE: Haptic Object Reconstruction";
      constexpr const char DESC[] = "Final year BEng research project.";

      auto DATE = D{2020, PRESENT_DATE};
      auto tb   = clr::gfx::imgui_image::load_from_file(PRELOAD_DATA_DIR "/thumbnail/haptic_grasping.png");

      items_.emplace_back(*this, NAME, DESC, DATE, []() { gui_manager::change_page_next_frame_<haptic_grasping_page>(); }, std::move(tb), "", 8);
    }
    auto& p = items_.back();

    // work category
    p.add_tag("Individual");
    p.add_tag("Academic");

    // general category
    p.add_tag("Robotics");

    // languages
    p.add_tag("Python");

    // technologies
    // ---

    // miscellaneous
    // ---
  }

  { // Clarity
    {
      constexpr const char NAME[] = "Clarity";
      constexpr const char DESC[] =
          "Clarity is a framework that enables rapid production of multi-threaded software with advanced task "
          "scheduling, as well as a plethora of utilities. Clarity Studio is built on top of this library and offers "
          "an interactive and extensible development environment.";
      constexpr const char GIT[] = "https://github.com/judicaelclair/clarity";

      auto DATE = D{2017, PRESENT_DATE};
      auto tb   = clr::gfx::imgui_image::load_from_file(PRELOAD_DATA_DIR "/thumbnail/clarity.png");

      items_.emplace_back(
          *this, NAME, DESC, DATE, []() { gui_manager::change_page_next_frame_<clarity_page>(); }, std::move(tb), GIT,
          5);
    }
    auto& p = items_.back();

    // work category
    p.add_tag("Individual");
    p.add_tag("Extracurricular");

    // general category
    // ---

    // languages
    p.add_tag("C++");

    // technologies
    // ---

    // miscellaneous
    // ---
  }

  { // Humanoid Robot Hand
    {
      constexpr const char NAME[] = "Humanoid Robot Hand";
      constexpr const char DESC[] = "Devised human-sized fingers with precise joint position feedback "
                                    "using potentiometers and Hall effect sensors. This involved designing "
                                    "custom, compact hardware for high bandwidth sensing.";

      auto DATE = D{2019, PRESENT_DATE};
      auto tb   = clr::gfx::imgui_image::load_from_file(PRELOAD_DATA_DIR "/thumbnail/hand.png");

      items_.emplace_back(
          *this, NAME, DESC, DATE, []() { gui_manager::change_page_next_frame_<hand_page>(); }, std::move(tb), "", 4);
    }
    auto& p = items_.back();

    // work category
    p.add_tag("Individual");
    p.add_tag("Extracurricular");

    // general category
    p.add_tag("Robotics");
    p.add_tag("Embedded");
    p.add_tag("Hardware");

    // languages
    p.add_tag("C++");
    p.add_tag("VHDL");
    p.add_tag("MATLAB");

    // technologies
    // ---

    // miscellaneous
    // ---
  }

  { // PathBench
    {
      constexpr const char NAME[] = "PathBench";
      constexpr const char DESC[] = "Motion Planning Platform for classic and machine learning-based algorithms.";
      constexpr const char GIT[]  = "https://github.com/perfectly-balanced/PathBench";

      auto DATE = D{2020, 2021};
      auto tb   = clr::gfx::imgui_image::load_from_file(PRELOAD_DATA_DIR "/thumbnail/path_bench.png");

      items_.emplace_back(
          *this, NAME, DESC, DATE, []() { gui_manager::change_page_next_frame_<path_bench_page>(); }, std::move(tb),
          GIT, 6);
    }
    auto& p = items_.back();

    // work category
    p.add_tag("Group");
    p.add_tag("Academic");

    // general category
    p.add_tag("Robotics");

    // languages
    p.add_tag("Python");

    // technologies
    // ---

    // miscellaneous
    // ---
  }

  { // Breadboard Computer
    {
      constexpr const char NAME[] = "Breadboard Computer";
      constexpr const char DESC[] = "8-bit TTL Computer built on a breadboard using discrete logic.";

      auto DATE = D{2015};
      auto tb   = clr::gfx::imgui_image::load_from_file(PRELOAD_DATA_DIR "/thumbnail/breadboard_computer.png");

      items_.emplace_back(
          *this, NAME, DESC, DATE, []() { gui_manager::change_page_next_frame_<breadboard_computer_page>(); },
          std::move(tb), "", 3);
    }
    auto& p = items_.back();

    // work category
    p.add_tag("Individual");
    p.add_tag("Extracurricular");

    // general category
    p.add_tag("Hardware");

    // languages
    // ---

    // technologies
    // ---

    // miscellaneous
    // ---
  }

  { // Ruined Kingdoms
    {
      constexpr const char NAME[] = "Ruined Kingdoms";
      constexpr const char DESC[] = "2D fantasy multiplayer online role playing game.";

      auto DATE = D{2012, 2016};
      auto tb   = clr::gfx::imgui_image::load_from_file(PRELOAD_DATA_DIR "/thumbnail/rk.png");

      items_.emplace_back(
          *this, NAME, DESC, DATE, []() { gui_manager::change_page_next_frame_<ruined_kingdoms_page>(); },
          std::move(tb), "", 2);
    }
    auto& p = items_.back();

    // work category
    p.add_tag("Individual");
    p.add_tag("Extracurricular");

    // general category
    p.add_tag("Game");

    // languages
    p.add_tag("Python");

    // technologies
    // ---

    // miscellaneous
    // ---
  }

  { // Asteroids on Steroids
    {
      constexpr const char NAME[] = "Asteroids on Steroids";
      constexpr const char DESC[] = "Bare Metal game similar to the arcade game Asteroids but on steroids, running on "
                                    "a Raspberry Pi. No standard or third-party libraries were used.";

      auto DATE = D{2019};
      auto tb   = clr::gfx::imgui_image::load_from_file(PRELOAD_DATA_DIR "/thumbnail/asteroids.png");

      items_.emplace_back(
          *this, NAME, DESC, DATE, []() { gui_manager::change_page_next_frame_<asteroids_page>(); }, std::move(tb), "",
          1);
    }
    auto& p = items_.back();

    // work category
    p.add_tag("Group");
    p.add_tag("Academic");

    // general category
    p.add_tag("Game");
    p.add_tag("Embedded");
    p.add_tag("Hardware");

    // languages
    p.add_tag("C");

    // technologies
    // ---

    // miscellaneous
    // ---
  }

  { // Portfolio
    {
      constexpr const char NAME[] = "Portfolio";
      constexpr const char DESC[] = "My personal portfolio website (this site).";
      constexpr const char GIT[]  = "https://github.com/judicaelclair/portfolio";

      auto DATE = D{2020, PRESENT_DATE};
      auto tb   = clr::gfx::imgui_image::load_from_file(PRELOAD_DATA_DIR "/thumbnail/portfolio.png");

      items_.emplace_back(
          *this, NAME, DESC, DATE, []() { gui_manager::change_page_next_frame_<portfolio_page>(); }, std::move(tb),
          GIT);
    }
    auto& p = items_.back();

    // work category
    p.add_tag("Individual");
    p.add_tag("Extracurricular");

    // general category
    p.add_tag("Web App");

    // languages
    p.add_tag("C++");

    // technologies
    // ---

    // miscellaneous
    // ---
  }

  update_sort_comparator_();
  refresh_();
}

void projects_page::update_tag_ui_state_(const bool use_item_view) {
  num_tags_selected_ = 0;
  for (auto const& tg : tag_groups_) {
    for (auto const& t : tg->tags) {
      num_tags_selected_ += t->selected;
      t->visible_cnt = 0;
    }
  }

  if (use_item_view) {
    for (auto p : items_view_) {
      // visible only if selected tags are
      // related to this project.
      size_t tags_selected = 0;
      for (auto const& t : p->tags()) {
        tags_selected += t->selected;
      }
      p->visible(tags_selected == num_tags_selected_);

      if (p->visible()) {
        for (auto t : p->tags()) {
          ++t->visible_cnt;
        }
      }
    }
  } else {
    for (auto& p : items_) {
      // visible only if selected tags are
      // related to this project.
      size_t tags_selected = 0;
      for (auto const& t : p.tags()) {
        tags_selected += t->selected;
      }
      p.visible(tags_selected == num_tags_selected_);

      if (p.visible()) {
        for (auto t : p.tags()) {
          ++t->visible_cnt;
        }
      }
    }
  }

  // reset selection that is no longer valid.
  // This occurs when search based filtering.
  for (auto const& tg : tag_groups_) {
    for (auto const& t : tg->tags) {
      if (!t->visible_cnt) {
        t->selected = false;
      }
    }
  }
}

void projects_page::refresh_() {
  // calls 'update_tag_ui_state_()' & 'base::refresh_()'
  filterer_.refresh();
  sort_();
}

void projects_page::render_item_(project const& p) {
  constexpr auto TAG_COLOUR         = IM_COL32(255, 255, 255, 88);
  constexpr auto THUMBNAIL_WIDTH    = 200.0f;
  constexpr auto INDENT             = 1.0f;
  constexpr char TAG_SEPARATOR[]    = "  |  ";
  constexpr auto PROJECT_DESC_WIDTH = 550.0f;

  ImGui::Dummy({0, 6});

  auto base_indent = [has_thumbnail = !!p.thumbnail()]() {
    ImGui::Dummy({INDENT + (has_thumbnail ? (THUMBNAIL_WIDTH + 5.0f) : 0.0f),
                  0}); // item spacing does the rest of the offsetting
    ImGui::SameLine();
  };

  float thumbnail_max_y = 0;
  if (auto const& tb = p.thumbnail()) {
    auto& window       = *ImGui::GetCurrentWindow();
    const float shrink = THUMBNAIL_WIDTH / tb->width();
    const ImVec2 size{tb->width() * shrink, tb->height() * shrink};
    const ImVec2 min{window.DC.CursorPos.x + INDENT, window.DC.CursorPos.y};
    const ImVec2 max{min.x + size.x, min.y + size.y};
    window.DrawList->AddImage(tb->texture(), min, max);
    thumbnail_max_y = max.y + ImGui::GetStyle().ItemSpacing.y;

    bool hovered;
    ImRect interact_bb;
    interact_bb.Min = min;
    interact_bb.Max = max;
    const ImGuiID id{window.GetID((p.name() + "##thumbnail").c_str())};
    if (ImGui::ButtonBehavior(interact_bb, id, &hovered, nullptr)) {
      p.open_callback()();
    } else if (hovered) {
      ImGui::SetTooltip("%s Open project", icon::LINK);
    }
  }

  // TITLE
  base_indent();
  ImGui::PushFont(subtitle_font);
  if (link::add<link::NO_BUTTON_COLOUR | link::NO_NEUTRAL_UNDERLINE>(
          p.name().c_str(), clr::fmt("%s Open project", icon::LINK), default_font)) {
    p.open_callback()();
  }
  ImGui::PopFont();

  if (!p.git().empty()) {
    ImGui::SameLine();
    ImGui::Dummy({0, 0});
    ImGui::SameLine();
    ImGui::GetCurrentWindow()->DC.CursorPos.y += 2.0f;
    ImGui::PushID(int(ImGui::GetCurrentWindow()->GetID(p.name().c_str())));
    github_icon::add(p.git(), "Open GitHub Repository");
    ImGui::PopID();
    ImGui::GetCurrentWindow()->DC.CursorPos.y -= 2.0f;
  }

  std::string tag_txt;

  // DATE
  const bool has_date =
      !(p.start_date() == std::numeric_limits<size_t>::max() && p.end_date() == std::numeric_limits<size_t>::max());
  if (has_date) {
    const auto s = p.start_date();
    const auto e = p.end_date();

    if (s == e) {
      tag_txt = std::to_string(s);
    } else if (e != PRESENT_DATE) {
      tag_txt = clr::fmt("%zu - %zu", s, e);
    } else {
      tag_txt = clr::fmt("%zu - Present", s);
    }
  }

  { // TAGS
    auto& ts = p.tags();

    auto tag_visible = [&](const size_t min_idx = 0) {
      for (size_t i = min_idx; i < ts.size(); ++i) {
        if (ts[i]->visible) {
          return true;
        }
      }
      return false;
    };

    if (tag_visible()) {
      if (has_date) {
        tag_txt += TAG_SEPARATOR;
      }

      for (size_t i = 0; i < ts.size(); ++i) {
        auto t = ts[i];
        if (!t->visible) {
          continue;
        }

        tag_txt += t->name;

        if (tag_visible(i + 1)) {
          tag_txt += TAG_SEPARATOR;
        }
      }
    }
  }

  if (!tag_txt.empty()) {
    base_indent();
    ImGui::PushStyleColor(ImGuiCol_Text, TAG_COLOUR);
    wrapped_text(tag_txt.c_str());
    ImGui::PopStyleColor();
  }

  { // DESCRIPTION
    base_indent();
    auto& window          = *ImGui::GetCurrentWindow();
    const float old_x     = window.WorkRect.Max.x;
    window.WorkRect.Max.x = std::min<float>(old_x, PROJECT_DESC_WIDTH + window.DC.CursorPos.x);
    wrapped_text(p.description().c_str());
    window.WorkRect.Max.x = old_x;
  }

  ImGui::Dummy({0, 5 + std::max(0.0f, thumbnail_max_y - ImGui::GetCurrentWindow()->DC.CursorPos.y)});
}

void projects_page::update_sort_comparator_() {
  std::function<bool(project*, project*)> comp;
  switch (sort_type_) {
    case sort_by_::NAME: comp = [](project* lhs, project* rhs) { return lhs->name() < rhs->name(); }; break;
    case sort_by_::RELEVANCE:
      comp = [](project* lhs, project* rhs) { return lhs->relevance() > rhs->relevance(); };
      break;
    case sort_by_::OLDEST_TO_NEWEST_END_DATE:
      comp = [](project* lhs, project* rhs) { return lhs->end_date() < rhs->end_date(); };
      break;
    case sort_by_::NEWEST_TO_OLDEST_END_DATE:
      comp = [](project* lhs, project* rhs) { return lhs->end_date() > rhs->end_date(); };
      break;
    case sort_by_::OLDEST_TO_NEWEST_START_DATE:
      comp = [](project* lhs, project* rhs) { return lhs->start_date() < rhs->start_date(); };
      break;
    case sort_by_::NEWEST_TO_OLDEST_START_DATE:
      comp = [](project* lhs, project* rhs) { return lhs->start_date() > rhs->start_date(); };
      break;
    default: return;
  }
  sort_comparator_(std::move(comp));
}

void projects_page::render_content_() {
  ImGui::BeginColumns("content", 2);
  if (list_column_offset_ < 0.0f) {
    ImGui::SetColumnWidth(-1, 200.0f);
  }
  render_tags_();
  ImGui::NextColumn();
  ImGui::Dummy(search_header_size_());
  listing_page::render_content_();
  list_column_offset_ = ImGui::GetColumnOffset(1) + 1.0f;
  ImGui::EndColumns();
  render_search_header_();
}

void projects_page::render_tags_() {
  ImGui::GetCurrentWindow()->DC.CursorPos.y += 3.0f;
  {
    const ImVec2 pos{ImGui::GetColumnOffset() + ImGui::GetColumnWidth() - icon_btn::dimension() - 5.0f,
                     ImGui::GetCurrentWindow()->DC.CursorPos.y};
    const auto flags =
        static_cast<ImGuiButtonFlags>(clr::conditional_bit_field(!num_tags_selected_, ImGuiButtonFlags_Disabled));
    if (icon_btn::add(pos, icon::MINUS_SQUARE, "Reset selection", flags)) {
      for (auto const& tg : tag_groups_) {
        for (auto const& t : tg->tags) {
          t->selected = false;
        }
      }
      refresh_();
    }
  }

  for (auto const& tg : tag_groups_) {
    ImGui::PushFont(tag_section_title_font_);
    ImGui::Text("%s", tg->name.c_str());
    ImGui::PopFont();
    ImGui::Dummy({0, 0.5f});
    ImGui::Indent(5.0f);
    for (auto const& t : tg->tags) {
      auto f = static_cast<ImGuiButtonFlags>(clr::conditional_bit_field(!t->visible_cnt, ImGuiButtonFlags_Disabled));
      if (radio_button(t->name.c_str(), t->selected, f)) {
        t->selected = !t->selected;
        refresh_();
      }
    }
    ImGui::Unindent(5.0f);
    ImGui::Dummy({0, 5.0f});
  }
}

ImVec2 projects_page::search_header_size_() {
  auto& window = *ImGui::GetCurrentWindow();
  auto& style  = ImGui::GetStyle();

  const float spacing = style.ItemSpacing.y;

  ImVec2 sz;
  sz.x = window.InnerRect.Max.x - list_column_offset_ + 1.0f;
  sz.y = search_bar::size().y + 5.0f + spacing;
  if (filterer_.filters().empty()) {
    sz.y += spacing;
  } else {
    sz.y += filterer_.height() + style.ScrollbarSize;
    if (filter_list_scroll_) {
      sz.y += spacing;
    }
  }

  return sz;
}

void projects_page::render_search_bar_() {
  if ((ImGui::IsKeyDown(key::KEY_LEFT_CTRL) || ImGui::IsKeyDown(key::KEY_RIGHT_CTRL))
      && (ImGui::IsKeyDown(key::KEY_X) || ImGui::IsKeyDown(key::KEY_Z))) {
    search_exclusive = ImGui::IsKeyDown(key::KEY_X);
    ImGui::SetKeyboardFocusHere();
  }

  ImGui::Dummy({1, 0}); // item spacing does the rest of the offsetting
  ImGui::SameLine();

  if (search_bar::add(search_buf, "###project_search_bar",
                      search_exclusive ? IM_COL32(255, 191, 0, 255) : IM_COL32_WHITE)) {
    if (search_buf.empty()) {
      return;
    }

    try {
      filterer_.add({search_buf, search_exclusive});
    } catch (std::regex_error const& e) {
      search_buf.clear();
      return;
    }

    search_buf.clear();
  }

  ImGui::SameLine();
  help_marker("Project Search regex matches title (CASE SENSITIVE):\n(Ctrl+X) Exclusive Filtering hides projects that "
              "match the regex filter.\n(Ctrl+Z) Inclusive Filtering shows projects that match the regex filter.\n");
}

void projects_page::render_sort_by_() {
  ImGui::SameLine();
  ImGui::Dummy({5, 0});
  ImGui::SameLine();
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {ImGui::GetCurrentContext()->Style.FramePadding.x,
                                                   ImGui::GetCurrentContext()->Style.FramePadding.y + 3});
  static const float size = []() {
    float s = 0;
    for (auto str : SORT_TYPES_) {
      if (auto ns = ImGui::CalcTextSize(str).x; ns > s) {
        s = ns;
      }
    }
    return s + 40;
  }();
  ImGui::SetNextItemWidth(size);

  int prev = sort_type_;
  ImGui::Combo("###//sort by combo//", &sort_type_, SORT_TYPES_, IM_ARRAYSIZE(SORT_TYPES_));
  if (prev != sort_type_) {
    update_sort_comparator_();
    sort_();
  }
  ImGui::PopStyleVar();
}

void projects_page::render_filters_() {
  if (!filterer_.filters().empty()) {
    if (ImGui::BeginChild("filters",
                          {ImGui::GetCurrentWindow()->Size.x, filterer_.height() + ImGui::GetStyle().ScrollbarSize},
                          false, ImGuiWindowFlags_HorizontalScrollbar)) {
      filterer_.draw(5.0f);
    }
    filter_list_scroll_ = ImGui::GetCurrentWindow()->ScrollbarX;
    ImGui::EndChild();
  } else {
    filter_list_scroll_ = false;
  }
}

void projects_page::render_search_header_() {
  {
    ImGuiWindow* const window   = ImGui::GetCurrentWindow();
    ImDrawList* const draw_list = window->DrawList;
    const auto sz               = search_header_size_();

    // latches onto the top of the footer so that it doens't hover over the footer
    const ImVec2 pos{list_column_offset_, std::min<float>(footer_rect_min().y - sz.y, MAIN_MENU_BAR_HEIGHT)};

    draw_list->AddRectFilled({pos.x, 0}, {pos.x + sz.x, pos.y + sz.y}, window::CLEAR_COLOUR);

    ImGui::SetNextWindowPos(pos);
    ImGui::SetNextWindowSize(sz);
  }

  ImGui::SetNextWindowBgAlpha(0.0f);

  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration
                                  | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings
                                  | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  if (!ImGui::Begin("search_header", nullptr, window_flags)) {
    ImGui::PopStyleVar(3);
    ImGui::End();
    return;
  }
  ImGui::PopStyleVar(3);

  ImGui::Dummy({0, 5});

  render_search_bar_();
  render_sort_by_();
  render_filters_();

  ImGui::End();
}

void projects_page::reset() {
  search_exclusive = false;
  search_buf       = "";
  for (auto const& tg : tag_groups_) {
    for (auto const& t : tg->tags) {
      t->selected = false;
    }
  }
  filterer_.reset();
  refresh_();
  base::reset();
}

} // namespace webapp