/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/widget/text.hpp
 * Last Modified: Saturday, 14th November 2020 5:21:15 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <string_view>

#include <Clarity/core/util/common.hpp>

#include <WebApp/gui/widget/link.hpp>
#include <WebApp/gui/manager.hpp>

#include <imgui.h>

namespace webapp {

using wrapped_text_link_t =
    std::function<bool /* hovered */ (const char* /* name */, ImVec2 /* pos */, bool /* rerender_hovered */)>;

// 'wrapped_text()' / 'bullet_wrapped_text()'
//
// We don't hide text after ## in this end-user function.
// Valid text is any standard character, spaces and newlines (no tabs etc).
// For a link, use '{..}': 'some text {link here} some more text'.
// Link name must only contain standard characters and spaces (no newlines, tabs, etc).

namespace detail {
  void wrapped_text(const char* txt, const wrapped_text_link_t* links);
  void bullet_wrapped_text(const char* txt, const wrapped_text_link_t* links);
} // namespace detail

template <typename T>
void wrapped_text(T&& txt, std::vector<wrapped_text_link_t> const& links = {}) {
  if constexpr (!std::is_convertible_v<T, const char*>) {
    wrapped_text(clr::convert_to<std::string>(std::forward<T>(txt)).c_str(), links);
  } else {
    static_assert(!std::is_same_v<std::nullptr_t, std::decay_t<T>>);
    detail::wrapped_text(txt, links.data());
  }
}

template <typename T>
void bullet_wrapped_text(T&& txt, std::vector<wrapped_text_link_t> const& links = {}) {
  if constexpr (!std::is_convertible_v<T, const char*>) {
    bullet_wrapped_text(clr::convert_to<std::string>(std::forward<T>(txt)).c_str(), links);
  } else {
    static_assert(!std::is_same_v<std::nullptr_t, std::decay_t<T>>);
    detail::bullet_wrapped_text(txt, links.data());
  }
}

[[nodiscard]] wrapped_text_link_t wrapped_text_external_link(std::string link,
                                                             std::string_view tooltip_desc = "View website");

template <typename Page>
[[nodiscard]] wrapped_text_link_t wrapped_text_page_link() {
  return [](const char* const name, const ImVec2 pos, const bool hovered) {
    if (hovered) [[unlikely]] {
      link::underline(pos, ImGui::CalcTextSize(name), hovered);
      return hovered;
    } else {
      auto info = link::add(pos, name, "View page");
      if (info.pressed) {
        gui_manager::change_page_next_frame_<Page>();
      }
      return info.hovered;
    }
  };
}

} // namespace webapp