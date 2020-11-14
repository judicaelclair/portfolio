/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/manager.cpp
 * Last Modified: Friday, 13th November 2020 10:19:03 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/manager.hpp>
#include <WebApp/gui/page/project/asteroids.hpp>
#include <WebApp/gui/page/project/breadboard_computer.hpp>
#include <WebApp/gui/page/project/clarity.hpp>
#include <WebApp/gui/page/project/hand.hpp>
#include <WebApp/gui/page/project/path_bench.hpp>
#include <WebApp/gui/page/project/portfolio.hpp>
#include <WebApp/gui/page/project/ruined_kingdoms.hpp>
#include <WebApp/gui/page/about.hpp>
#include <WebApp/gui/page/home.hpp>
#include <WebApp/gui/page/projects.hpp>

#include <emscripten.h>

#include <limits>
#include <utility>

namespace webapp {

std::vector<std::unique_ptr<base_page>> gui_manager::pages_;
size_t gui_manager::max_pages_ = std::numeric_limits<size_t>::max();

namespace detail {

  // Each call to _malloc() must be paired with free(), or heap memory will leak!
  EM_JS(const char*, get_hash, (), {
    var hash             = window.location.hash;
    var nbytes           = lengthBytesUTF8(hash) + 1;
    var str_on_wasm_heap = _malloc(nbytes);
    stringToUTF8(hash, str_on_wasm_heap, nbytes);
    return str_on_wasm_heap;
  });

  EM_JS(void, set_hash, (const char* s, int len), {
    var hash             = UTF8ToString(s, len);
    window.location.hash = hash;
  });

  static std::pair<std::string, bool> try_get_page_from_hash() {
    const char* hash = get_hash();
    void* data       = static_cast<void*>(const_cast<char*>(hash));
    if (strlen(hash) == 0) {
      free(data);
      return {"", false};
    } else if (*hash != '#') {
      free(data);
      return {hash, false};
    } else {
      std::string page = hash + 1;
      free(data);
      return {page, true};
    }
  }

  static void set_hash_from_page(std::string_view page) {
    std::string hash = "#";
    hash += page;
    set_hash(hash.c_str(), static_cast<int>(hash.size()));
  }

} // namespace detail

bool gui_manager::init_() {
  add_page(std::make_unique<home_page>())->set_as_current_page();
  return true;
}

void gui_manager::destroy_() {
  pages_.clear();
}

void gui_manager::max_pages(const size_t value) {
  max_pages_ = value;
  if (pages_.size() > max_pages()) {
    pages_.resize(max_pages());
  }
}

base_page* gui_manager::add_page(std::unique_ptr<base_page> page) {
  if (pages_.size() == max_pages()) {
    pages_.resize(max_pages() - 1);
  }
  return pages_.emplace(pages_.begin(), std::move(page))->get();
}

void gui_manager::remove_page(base_page* page) {
  for (auto it = pages_.begin(); it != pages_.end(); ++it) {
    if ((*it).get() == page) {
      pages_.erase(it);
      return;
    }
  }
}

bool gui_manager::set_page_with_name(const std::string_view n) {
  if (n == page_name::ASTEROIDS) {
    set_page_with_reset<asteroids_page>();
  } else if (n == page_name::BREADBOARD_COMPUTER) {
    set_page_with_reset<breadboard_computer_page>();
  } else if (n == page_name::CLARITY) {
    set_page_with_reset<clarity_page>();
  } else if (n == page_name::HAND) {
    set_page_with_reset<hand_page>();
  } else if (n == page_name::PATH_BENCH) {
    set_page_with_reset<path_bench_page>();
  } else if (n == page_name::PORTFOLIO) {
    set_page_with_reset<portfolio_page>();
  } else if (n == page_name::RUINED_KINGDOMS) {
    set_page_with_reset<ruined_kingdoms_page>();
  } else if (n == page_name::ABOUT) {
    set_page_with_reset<about_page>();
  } else if (n == page_name::HOME) {
    set_page_with_reset<home_page>();
  } else if (n == page_name::PROJECTS) {
    set_page_with_reset<projects_page>();
  } else {
    return false;
  }
  return true;
}

void gui_manager::update() {
  auto const& [hash, is_page]  = detail::try_get_page_from_hash();
  std::string_view actual_page = pages().front()->name();
  if (is_page && hash != actual_page) {
    if (!set_page_with_name(hash)) {
      detail::set_hash_from_page(actual_page);
    }
  } else if (hash.empty()) {
    set_page_with_reset<home_page>();
  }
}

void gui_manager::update_hash_() {
  detail::set_hash_from_page(pages().front()->name());
}

} // namespace webapp