/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/page/base.hpp
 * Last Modified: Saturday, 14th November 2020 2:35:44 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <vector>
#include <memory>
#include <string_view>

#include <WebApp/gui/window/base.hpp>
#include <WebApp/gui/page/name.hpp>

namespace webapp {

class base_page {
  std::vector<std::unique_ptr<base_window>> windows_;
  ptrdiff_t windows_idx_{};

public:
  [[nodiscard]] virtual bool cachable() const noexcept { return true; }
  virtual void set_as_current_page()                           = 0;
  virtual void reset()                                         = 0;
  [[nodiscard]] virtual std::string_view name() const noexcept = 0;

  virtual ~base_page() = default;

  template <typename W>
  [[nodiscard]] W* get(const char* title = nullptr) {
    for (auto const& w : windows_) {
      if (auto ptr = dynamic_cast<W*>(w.get())) {
        if (title) {
          if (title == w->title_) {
            return ptr;
          }
        } else {
          return ptr;
        }
      }
    }
    return nullptr;
  }

  void add_window(std::unique_ptr<base_window> w) {
    for (size_t i = 0; i != windows_.size(); ++i) {
      if (windows_[i]->title_ == w->title_) {
        return;
      }
    }
    windows_.emplace_back(std::move(w));
  }

  void destroy_window(base_window* w) {
    for (size_t i = 0; i != windows_.size(); ++i) {
      if (windows_[i].get() == w) {
        windows_.erase(windows_.begin() + static_cast<ptrdiff_t>(i));
        if (static_cast<ptrdiff_t>(i) <= windows_idx_) {
          windows_idx_ -= 1;
        }
        return;
      }
    }
  }

protected:
  void render_windows_();
};

} // namespace webapp