/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/application.cpp
 * Last Modified: Saturday, 14th November 2020 1:39:05 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/application.hpp>
#include <WebApp/window.hpp>
#include <WebApp/gui/manager.hpp>
#include <emscripten.h>

namespace webapp {

std::vector<delayed_execution> application::delayed_executions_;

void application::loop_() {
  window::update_();
  for (auto it = delayed_executions_.begin(); it != delayed_executions_.end();) {
    auto& exec = *it;
    if (exec.frames == 0) {
      exec.payload();
      it = delayed_executions_.erase(it);
    } else {
      --exec.frames;
      ++it;
    }
  }
}

bool application::init() {
  return window::init_() && gui_manager::init_();
}

void application::mainloop() {
  window::loading(false);
  emscripten_set_main_loop(&application::loop_, 0, 1);
}

void application::destroy() {
  window::destroy_();
  emscripten_cancel_main_loop();
}

} // namespace webapp