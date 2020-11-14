/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/page/projects.hpp
 * Last Modified: Saturday, 14th November 2020 2:35:00 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <WebApp/gui/page/listing.hpp>
#include <WebApp/gui/widget/filterer.hpp>

#include <Clarity/graphics/imgui/image.hpp>

#include <regex>
#include <string>
#include <vector>
#include <optional>

#include <imgui.h>
#include <imgui_internal.h>

namespace webapp {

class project;
struct project_tag;
struct project_tag_group;
class projects_page;

static inline constexpr size_t PRESENT_DATE = std::numeric_limits<size_t>::max();

class project {
  std::string git_;
  std::string name_;
  std::string description_;
  std::pair<size_t, size_t> date_;
  std::vector<project_tag*> tags_;
  std::function<void()> open_callback_;
  std::unique_ptr<clr::gfx::imgui_image> thumbnail_;
  std::reference_wrapper<std::vector<std::unique_ptr<project_tag_group>>> tag_groups_;
  size_t relevance_;
  bool visible_;

public:
  project(projects_page& page, std::string name, std::string description, std::pair<size_t, size_t> date,
          std::function<void()> open_callback, std::unique_ptr<clr::gfx::imgui_image> thumbnail = nullptr,
          std::string git = "", size_t relevance = 0);
  project(projects_page& page, std::string name, std::string description, std::initializer_list<size_t> date,
          std::function<void()> open_callback, std::unique_ptr<clr::gfx::imgui_image> thumbnail = nullptr,
          std::string git = "", size_t relevance = 0);

  [[nodiscard]] constexpr auto git() const noexcept -> std::string const& { return git_; }
  [[nodiscard]] constexpr auto name() const noexcept -> std::string const& { return name_; }
  [[nodiscard]] constexpr auto description() const noexcept -> std::string const& { return description_; }
  [[nodiscard]] constexpr auto date() const noexcept -> std::pair<size_t, size_t> const& { return date_; }
  [[nodiscard]] constexpr auto start_date() const noexcept -> size_t { return date().first; }
  [[nodiscard]] constexpr auto end_date() const noexcept -> size_t { return date().second; }
  [[nodiscard]] constexpr auto relevance() const noexcept -> size_t { return relevance_; }
  [[nodiscard]] constexpr auto visible() const noexcept -> bool { return visible_; }
  [[nodiscard]] constexpr auto open_callback() const noexcept -> std::function<void()> const& { return open_callback_; }
  [[nodiscard]] constexpr auto tags() const noexcept -> std::vector<project_tag*> const& { return tags_; }
  [[nodiscard]] constexpr auto thumbnail() const noexcept -> std::unique_ptr<clr::gfx::imgui_image> const& {
    return thumbnail_;
  }

  constexpr void visible(const bool v) noexcept { visible_ = v; }

  void add_tag(project_tag* tag);
  void add_tag(std::string_view tag);
};

struct project_tag {
  std::string name;
  size_t visible_cnt{0}; // how many visible projects have this tag
  bool visible{true};    // visible in project listing (underneath each project title)
  bool selected{false};  // check box selected
  bool selectable{true};

  project_tag(std::string name_, bool visible_ = true) : name(std::move(name_)), visible(visible_) {}
};

struct project_tag_group {
  std::string name;
  std::vector<std::unique_ptr<project_tag>> tags;

  project_tag_group(std::string name_, std::vector<std::unique_ptr<project_tag>> tags_ = {})
      : name(std::move(name_)), tags(std::move(tags_)) {}
};

class projects_page final : public listing_page<project> {
  friend class project;
  using base = listing_page<project>;

  [[nodiscard]] static constexpr std::string const& filterer_stringiser_(project& i) noexcept { return i.name(); }

  static inline constexpr const char* SORT_TYPES_[] = {"Name",
                                                       "Relevance",
                                                       "Oldest to Newest End Date",
                                                       "Newest to Oldest End Date",
                                                       "Oldest to Newest Start Date",
                                                       "Newest to Oldest Start Date"};

  enum sort_by_ : int {
    NAME,
    RELEVANCE,
    OLDEST_TO_NEWEST_END_DATE,
    NEWEST_TO_OLDEST_END_DATE,
    OLDEST_TO_NEWEST_START_DATE,
    NEWEST_TO_OLDEST_START_DATE
  };

  filterer<project, &filterer_stringiser_> filterer_;
  std::vector<std::unique_ptr<project_tag_group>> tag_groups_;
  std::underlying_type_t<sort_by_> sort_type_{sort_by_::RELEVANCE};
  ImFont* tag_section_title_font_;
  size_t num_tags_selected_{0};
  float list_column_offset_{-1.0f};
  bool filter_list_scroll_{false};
  bool search_exclusive{false};
  std::string search_buf{""};

  void update_sort_comparator_();
  void update_tag_ui_state_(bool use_item_view = false);

  void refresh_() final;
  void render_content_() final;
  void render_item_(project const& p) final;

  void render_tags_();
  void render_filters_();
  void render_sort_by_();
  void render_search_bar_();
  void render_search_header_();
  [[nodiscard]] ImVec2 search_header_size_();

public:
  projects_page();
  [[nodiscard]] constexpr std::string_view name() const noexcept final { return page_name::PROJECTS; }
  void reset() final;
};

} // namespace webapp