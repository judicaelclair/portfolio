/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/manager.hpp
 * Last Modified: Saturday, 14th November 2020 5:22:58 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <WebApp/gui/page/base.hpp>
#include <WebApp/application.hpp>

#include <vector>
#include <memory>
#include <limits>

namespace webapp {

class gui_manager {
  friend class application;
  static std::vector<std::unique_ptr<base_page>> pages_;
  static size_t max_pages_;

  [[nodiscard]] static bool init_();
  static void destroy_();
  static void update_hash_();

public:
  [[nodiscard]] static constexpr std::vector<std::unique_ptr<base_page>> const& pages() noexcept { return pages_; }

  [[nodiscard]] static size_t max_pages() noexcept { return max_pages_; }
  static void max_pages(const size_t value);

  [[nodiscard]] static base_page* add_page(std::unique_ptr<base_page> page);
  static void remove_page(base_page* page);

  template <typename P>
  struct page_elem {
    P* page;
    size_t pos;

    constexpr operator bool() const noexcept { return !!page; }
    constexpr operator P*() const noexcept { return page; }
  };

  template <typename P>
  [[nodiscard]] static page_elem<P> get() {
    for (size_t i = 0; i < pages_.size(); ++i) {
      if (auto ptr = dynamic_cast<P*>(pages_[i].get())) {
        return {ptr, i};
      }
    }
    return {nullptr, std::numeric_limits<size_t>::max()};
  }

  template <typename To, typename... Args>
  static void set_page(Args&&... args) {
    To* page;
    page_elem<To> pe;
    if ((pe = get<To>())) {
      page = pe;

      // move page to front
      auto p = std::move(pages_[pe.pos]);
      pages_.erase(pages_.begin() + static_cast<ptrdiff_t>(pe.pos));
      pages_.insert(pages_.begin(), std::move(p));
    } else {
      page = static_cast<To*>(add_page(std::make_unique<To>(std::forward<Args>(args)...)));
    }
    page->set_as_current_page();
    update_hash_();
  }

  template <typename To, typename... Args>
  static void set_page_with_reset(Args&&... args) {
    To* page;
    page_elem<To> pe;
    if ((pe = get<To>())) {
      page = pe;
      page->reset();

      // move page to front
      auto p = std::move(pages_[pe.pos]);
      pages_.erase(pages_.begin() + static_cast<ptrdiff_t>(pe.pos));
      pages_.insert(pages_.begin(), std::move(p));
    } else {
      page = static_cast<To*>(add_page(std::make_unique<To>(std::forward<Args>(args)...)));
    }
    page->set_as_current_page();
    update_hash_();
  }

  [[nodiscard]] static bool set_page_with_name(std::string_view name);
  static void update();

  template <typename T, typename... Args>
  static void change_page_next_frame_(Args&&... args) {
    application::add({0, [tas = std::make_tuple(std::forward<Args>(args)...)]() mutable {
                        std::apply([](auto&&... args) { gui_manager::set_page<T>(std::forward<Args>(args)...); },
                                   std::move(tas));
                      }});
  }

  template <typename T, typename... Args>
  static void change_page_with_reset_next_frame_(Args&&... args) {
    application::add({0, [tas = std::make_tuple(std::forward<Args>(args)...)]() mutable {
                        std::apply(
                            [](auto&&... args) { gui_manager::set_page_with_reset<T>(std::forward<Args>(args)...); },
                            std::move(tas));
                      }});
  }
};

} // namespace webapp