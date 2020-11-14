/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/page/listing.hpp
 * Last Modified: Saturday, 14th November 2020 2:35:50 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <WebApp/gui/page/standard.hpp>
#include <WebApp/gui/icon.hpp>
#include <WebApp/gui/widget/icon_btn.hpp>
#include <WebApp/gui/util/imgui.hpp>

#include <regex>
#include <string>
#include <vector>
#include <optional>

#include <imgui.h>
#include <imgui_internal.h>

namespace webapp {

template <typename Item>
class listing_page : public standard_page {
  unsigned m_page_index_{0};
  unsigned m_max_items_per_page_{8};

  std::function<bool(Item*, Item*)> m_sort_comparator_;
  std::function<bool(Item&)> m_visible_checker_;

  void draw_items_();
  void draw_list_nav_();

protected:
  std::vector<Item> items_;
  std::vector<Item*> items_view_;

  void sort_();
  virtual void refresh_();
  void render_content_() override;

  virtual void render_item_(Item const& i) = 0;

  void page_index_(const unsigned v) {
    m_page_index_ = v;
    reset_scroll_y_();
  }

  constexpr void max_items_per_page_(const unsigned v) {
    m_max_items_per_page_ = v;
    page_index_(0);
  }

  template <typename F>
  constexpr void sort_comparator_(F&& f) {
    m_sort_comparator_ = std::forward<F>(f);
  }

  template <typename F>
  constexpr void visible_checker_(F&& f) {
    m_visible_checker_ = std::forward<F>(f);
  }

  [[nodiscard]] constexpr auto page_index_() const noexcept -> unsigned { return m_page_index_; }
  [[nodiscard]] constexpr auto max_items_per_page_() const noexcept -> unsigned { return m_max_items_per_page_; }
  [[nodiscard]] constexpr auto sort_comparator_() const noexcept -> std::function<bool(Item*, Item*)> const& {
    return m_sort_comparator_;
  }
  [[nodiscard]] constexpr auto visible_checker_() const noexcept -> std::function<bool(Item&)> const& {
    return m_visible_checker_;
  }
};

template <typename Item>
void listing_page<Item>::refresh_() {
  items_view_.clear();
  items_view_.reserve(items_.size());
  auto const& check = visible_checker_();
  for (auto& i : items_) {
    if (!check || check(i)) {
      items_view_.emplace_back(&i);
    }
  }
  page_index_(0);
}

template <typename Item>
void listing_page<Item>::sort_() {
  if (m_sort_comparator_) {
    std::sort(items_view_.begin(), items_view_.end(), m_sort_comparator_);
  }
  page_index_(0);
}

template <typename Item>
void listing_page<Item>::draw_items_() {
  auto end = (((page_index_() + 1) * max_items_per_page_()) <= items_view_.size())
                 ? items_view_.begin() + static_cast<ptrdiff_t>((page_index_() + 1) * max_items_per_page_())
                 : items_view_.end();
  for (auto it = items_view_.begin() + static_cast<ptrdiff_t>(page_index_() * max_items_per_page_()); it != end; ++it) {
    render_item_(*(*it));

    { // Separator (with column support)
      constexpr float THICKNESS = 1.0f;
      ImGui::Dummy({0, THICKNESS});
      auto min = ImGui::GetItemRectMin();
      ImGui::GetWindowDrawList()->AddLine({min.x, min.y + THICKNESS / 2.0f},
                                          {ImGui::GetCurrentWindow()->Size.x - 5.0f, min.y + THICKNESS / 2.0f},
                                          ImGui::GetColorU32(ImGuiCol_Border));
    }
  }
  ImGui::Dummy({0, 10});
}

template <typename Item>
void listing_page<Item>::draw_list_nav_() {
  constexpr float SPACING = 16.f;

  // remove x-axis spacing for widget centering
  auto& style         = ImGui::GetStyle();
  auto old_x_spacing  = style.ItemSpacing.x;
  style.ItemSpacing.x = 0;

  auto& window = *ImGui::GetCurrentWindow();

  const auto btn_size = get_button_size(icon::ARROW_LEFT).x;

  ImGui::Dummy(
      {(window.Size.x - (window.DC.CurrentColumns ? ImGui::GetColumnOffset(-1) : 0.0f) - SPACING) / 2.0f - btn_size,
       0});
  ImGui::SameLine();
  {
    auto f = static_cast<ImGuiButtonFlags>(clr::conditional_bit_field(page_index_() == 0, ImGuiButtonFlags_Disabled));
    if (icon_btn::add(icon::ARROW_LEFT, "Previous Page", f)) {
      page_index_(page_index_() - 1); // resets scroll region as well
    }
  }
  ImGui::SameLine();
  ImGui::Dummy({SPACING, 0});
  ImGui::SameLine();
  {
    auto f = static_cast<ImGuiButtonFlags>(clr::conditional_bit_field(
        ((page_index_() + 1) * max_items_per_page_()) >= items_view_.size(), ImGuiButtonFlags_Disabled));
    if (icon_btn::add(icon::ARROW_RIGHT, "Next Page", f)) {
      page_index_(page_index_() + 1); // resets scroll region as well
    }
  }
  // restore x-axis spacing - was removed for widget centering
  style.ItemSpacing.x = old_x_spacing;
}

template <typename Item>
void listing_page<Item>::render_content_() {
  draw_items_();
  draw_list_nav_();
}

} // namespace webapp