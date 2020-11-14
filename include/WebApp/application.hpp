/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/application.hpp
 * Last Modified: Saturday, 14th November 2020 1:36:30 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <functional>
#include <vector>

namespace webapp {

// execute 'payload' after given number of 'frames'.
// useful when payload can cause undesired intermitent
// state if executed immediately (e.g. because of
// immediate loss of window focus).
struct delayed_execution {
  std::size_t frames;
  std::function<void()> payload;
};

class application {
  static void loop_();
  static std::vector<delayed_execution> delayed_executions_;

public:
  [[nodiscard]] static bool init();
  static void destroy();
  static void mainloop();

  static void add(delayed_execution exec) { delayed_executions_.emplace_back(exec); }
};

} // namespace webapp