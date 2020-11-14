/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/widget/filterer.hpp
 * Last Modified: Saturday, 14th November 2020 1:35:09 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <regex>
#include <string>
#include <vector>

#include <imgui.h>
#include <imgui_internal.h>

namespace webapp {

class filter {
  std::string str_;
  std::regex re_;
  bool exclusive_;

public:
  filter(std::string regex, bool exclusive) : str_(std::move(regex)), re_(str_), exclusive_(exclusive) {}

  [[nodiscard]] constexpr auto string() const noexcept -> std::string const& { return str_; }
  [[nodiscard]] constexpr auto regex() const noexcept -> std::regex const& { return re_; }
  [[nodiscard]] constexpr bool exclusive() const noexcept { return exclusive_; }
};

template <typename Item, std::string const& (*Stringiser)(Item&)>
class filterer {
  std::vector<filter> filters_;
  std::vector<Item*>* view_;
  std::function<void()> refresh_view_;
  std::function<void()> apply_callback_;

public:
  // apply all filters to current view
  void apply();

  // apply filter to current view
  void apply(filter const& f);

  // reset view & apply all filters
  void refresh();

  // remove all filters & reset view
  void reset();

  // remove filter, reset view then apply remaining filters
  void remove(ptrdiff_t idx);

  void add(filter f);

  // draw a horizontal list of all the filters
  void draw(const float offset = 0.0f, const float gap = 24.0f);

  // draw height
  [[nodiscard]] float height();

  [[nodiscard]] constexpr auto filters() const noexcept -> std::vector<filter> const& { return filters_; }

  template <typename F>
  constexpr void refresh_view(F&& f) {
    refresh_view_ = std::forward<F>(f);
  }
  [[nodiscard]] constexpr auto refresh_view() const noexcept -> std::function<void()> const& { return refresh_view_; }

  template <typename F>
  constexpr void apply_callback(F&& f) {
    apply_callback_ = std::forward<F>(f);
  }
  [[nodiscard]] constexpr auto apply_callback() const noexcept -> std::function<void()> const& {
    return apply_callback_;
  }

  template <typename RefreshView, typename ApplyCallback>
  filterer(std::vector<Item*>& view, RefreshView&& rv, ApplyCallback&& ac)
      : view_(&view), refresh_view_(std::forward<RefreshView>(rv)), apply_callback_(std::forward<ApplyCallback>(ac)) {}
};

template <typename Item, std::string const& (*Stringiser)(Item&)>
inline void filterer<Item, Stringiser>::apply(filter const& f) {
  view_->erase(std::remove_if(view_->begin(), view_->end(),
                              [&f](Item* i) { return std::regex_search(Stringiser(*i), f.regex()) == f.exclusive(); }),
               view_->end());
  apply_callback_();
}

template <typename Item, std::string const& (*Stringiser)(Item&)>
inline void filterer<Item, Stringiser>::apply() {
  for (size_t j = 0; j < filters_.size(); ++j) {
    apply(filters_[j]);
  }
}

template <typename Item, std::string const& (*Stringiser)(Item&)>
inline void filterer<Item, Stringiser>::refresh() {
  refresh_view_();
  apply();
}

template <typename Item, std::string const& (*Stringiser)(Item&)>
inline void filterer<Item, Stringiser>::reset() {
  filters_.clear();
  refresh();
}

template <typename Item, std::string const& (*Stringiser)(Item&)>
inline void filterer<Item, Stringiser>::remove(const ptrdiff_t idx) {
  filters_.erase(filters_.begin() + idx);
  refresh();
}

template <typename Item, std::string const& (*Stringiser)(Item&)>
inline void filterer<Item, Stringiser>::add(filter f) {
  // if regex already exists, ignore (don't want duplicated)
  for (auto it = filters_.cbegin(); it != filters_.cend(); ++it) {
    if ((*it).string() == f.string()) {
      return;
    }
  }
  apply(filters_.emplace_back(std::move(f)));
}

template <typename Item, std::string const& (*Stringiser)(Item&)>
inline void filterer<Item, Stringiser>::draw(const float offset, const float gap) {
  if (filters_.empty()) {
    return;
  }
  auto& ctx   = *ImGui::GetCurrentContext();
  auto& style = ctx.Style;

  const auto old_y_spacing = style.ItemSpacing.y;
  style.ItemSpacing.y      = 0;

  ImGui::Dummy({0, style.FramePadding.y});
  ImGui::Dummy({offset, 0});
  ImGui::SameLine();
  for (size_t i = 0; i < filters_.size(); ++i) {
    auto const& s = filters_[i].string();

    const ImU32 colour = filters_[i].exclusive() ? IM_COL32(255, 191, 0, 255) : IM_COL32(255, 255, 255, 255);

    ImGui::PushStyleColor(ImGuiCol_Text, colour);
    ImGui::Text("%s", s.c_str());
    ImGui::PopStyleColor();
    ImGui::SameLine();
    auto min = ImGui::GetItemRectMin();
    auto max = ImGui::GetItemRectMax();

    max.x += 2;
    ImVec2 close_btn_size =
        ImVec2(ctx.FontSize + (2.0f * style.FramePadding.x), ctx.FontSize + (2.0f * style.FramePadding.y));
    if (ImGui::CloseButton(ImGui::GetCurrentWindow()->GetID(("###\\\\filter close btn\\\\" + s).c_str()),
                           ImVec2(max.x, min.y - style.FramePadding.y))) {
      remove(static_cast<ptrdiff_t>(i--));
    }
    max.x += close_btn_size.x;
    ImGui::Dummy({gap, 0});
    if (i + 1 != filters_.size()) {
      ImGui::SameLine();
    }

    min.x -= style.FramePadding.x;
    min.y -= style.FramePadding.y;
    max.y += style.FramePadding.y;

    ImGui::GetWindowDrawList()->AddRect(min, max, colour);
  }
  ImGui::Dummy({0, style.FramePadding.y});

  style.ItemSpacing.y = old_y_spacing;
}

template <typename Item, std::string const& (*Stringiser)(Item&)>
inline float filterer<Item, Stringiser>::height() {
  auto& style = ImGui::GetStyle();
  return 2.0f * style.ItemSpacing.y + ImGui::CalcTextSize("Filter").y;
}

} // namespace webapp