/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/util/imgui.hpp
 * Last Modified: Saturday, 14th November 2020 2:36:03 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <imgui.h>

namespace webapp {

ImVec2 get_button_size(const char* lbl);

// Helper to display a little (?) mark which shows a tooltip when hovered.
void help_marker(const char* desc);

static inline ImVec2 operator*(const ImVec2& lhs, const float rhs) {
  return ImVec2(lhs.x * rhs, lhs.y * rhs);
}

static inline ImVec2 operator/(const ImVec2& lhs, const float rhs) {
  return ImVec2(lhs.x / rhs, lhs.y / rhs);
}

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) {
  return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) {
  return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y);
}

static inline ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs) {
  return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y);
}

static inline ImVec2 operator/(const ImVec2& lhs, const ImVec2& rhs) {
  return ImVec2(lhs.x / rhs.x, lhs.y / rhs.y);
}

static inline ImVec2& operator*=(ImVec2& lhs, const float rhs) {
  lhs.x *= rhs;
  lhs.y *= rhs;
  return lhs;
}

static inline ImVec2& operator/=(ImVec2& lhs, const float rhs) {
  lhs.x /= rhs;
  lhs.y /= rhs;
  return lhs;
}

static inline ImVec2& operator+=(ImVec2& lhs, const ImVec2& rhs) {
  lhs.x += rhs.x;
  lhs.y += rhs.y;
  return lhs;
}

static inline ImVec2& operator-=(ImVec2& lhs, const ImVec2& rhs) {
  lhs.x -= rhs.x;
  lhs.y -= rhs.y;
  return lhs;
}

static inline ImVec2& operator*=(ImVec2& lhs, const ImVec2& rhs) {
  lhs.x *= rhs.x;
  lhs.y *= rhs.y;
  return lhs;
}

static inline ImVec2& operator/=(ImVec2& lhs, const ImVec2& rhs) {
  lhs.x /= rhs.x;
  lhs.y /= rhs.y;
  return lhs;
}

static inline ImVec4 operator+(const ImVec4& lhs, const ImVec4& rhs) {
  return ImVec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

static inline ImVec4 operator-(const ImVec4& lhs, const ImVec4& rhs) {
  return ImVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

static inline ImVec4 operator*(const ImVec4& lhs, const ImVec4& rhs) {
  return ImVec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}

} // namespace webapp
