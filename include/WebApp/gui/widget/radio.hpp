/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/widget/radio.hpp
 * Last Modified: Saturday, 14th November 2020 1:35:39 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <imgui.h>
#include <imgui_internal.h>

namespace webapp {

[[nodiscard]] bool radio_button(const char* label, bool active, ImGuiButtonFlags flags = 0);

} // namespace webapp