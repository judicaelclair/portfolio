/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/page/base.cpp
 * Last Modified: Saturday, 14th November 2020 1:37:40 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/page/base.hpp>

#include <emscripten.h>

namespace webapp {

void base_page::render_windows_() {
  while (windows_idx_ < static_cast<ptrdiff_t>(windows_.size())) {
    windows_[static_cast<size_t>(windows_idx_)]->draw();
    ++windows_idx_;
  }
  windows_idx_ = 0;
}

} // namespace webapp
