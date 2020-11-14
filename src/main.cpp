/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/main.cpp
 * Last Modified: Saturday, 14th November 2020 2:38:39 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/application.hpp>
#include <WebApp/window.hpp>
#include <cstdlib>

extern "C" int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
  if (!webapp::application::init()) {
    return EXIT_FAILURE;
  }
  webapp::application::mainloop();
  webapp::application::destroy();
  return EXIT_SUCCESS;
}