/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/window/base.hpp
 * Last Modified: Saturday, 14th November 2020 1:47:45 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <imgui.h>
#include <string>

namespace webapp {

class base_window {
  friend class base_page;
  friend class standard_page;

protected:
  std::string title_;
  bool open_;

public:
  template <typename T>
  explicit base_window(T&& title, bool open = true) : title_(std::forward<T>(title)), open_(open) {}
  virtual ~base_window() = default;

  virtual void draw() = 0;
};

} // namespace webapp