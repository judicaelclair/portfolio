/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /include/WebApp/gui/widget/link.hpp
 * Last Modified: Saturday, 14th November 2020 1:40:14 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#pragma once

#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>
#include <string>

#include <Clarity/core/util/str.hpp>
#include <Clarity/core/util/common.hpp>

namespace webapp {

class link {
  using RA = clr::required_args<std::string>;
  using OA = clr::unordered_optional_args<std::string, ImFont*, ImGuiButtonFlags>;

public:
  link() = delete;

  using property_t = unsigned;
  enum property : property_t {
    NO_BUTTON_COLOUR     = clr::bit_flag(0),
    NO_UNDERLINE         = clr::bit_flag(1),
    NO_NEUTRAL_UNDERLINE = clr::bit_flag(2)
  };

  struct info {
    bool hovered;
    bool pressed;
    ImVec2 size;

    constexpr operator bool() const noexcept { return pressed; }
  };

  template <property_t PROPS = clr::BIT_FLAG_NONE<property_t>>
  static void underline(const ImVec2 pos, const ImVec2 size, const bool hovered) {
    auto render = [&](ImColor col) {
      const ImVec2 beg = {pos.x, pos.y + size.y};
      const ImVec2 end = {beg.x + size.x - ImGui::GetFont()->ConfigData->GlyphExtraSpacing.x, beg.y};
      ImGui::GetWindowDrawList()->AddLine(beg, end, col, 1.0f);
    };

    if (hovered) {
      if constexpr (!(PROPS & NO_UNDERLINE)) {
        const auto col = (PROPS & NO_BUTTON_COLOUR) ? ImGuiCol_Text : ImGuiCol_ButtonHovered;
        render(ImGui::GetStyle().Colors[col]);
      }
    } else {
      if constexpr (!(PROPS & (NO_NEUTRAL_UNDERLINE | NO_UNDERLINE))) {
        const auto col = (PROPS & NO_BUTTON_COLOUR) ? ImGuiCol_Text : ImGuiCol_Button;
        render(ImGui::GetStyle().Colors[col]);
      }
    }
  }

  template <property_t PROPS = clr::BIT_FLAG_NONE<property_t>, typename... Args,
            typename         = typename std::enable_if_t<clr::param_pack_valid_v<clr::given_args<Args...>, RA, OA>>>
  [[nodiscard]] static info add(const ImVec2 pos, Args&&... args) {
    auto&& text         = clr::get<0, RA, OA>(std::forward<Args>(args)...);
    auto&& tooltip      = clr::get_optional<1, RA, OA>(nullptr, std::forward<Args>(args)...);
    auto&& tooltip_font = clr::get_optional<2, RA, OA>(nullptr, std::forward<Args>(args)...);
    auto&& flags        = clr::get_optional<3, RA, OA>(0, std::forward<Args>(args)...);

    if constexpr (!std::is_convertible_v<decltype(text), const char*>) {
      return add<PROPS>(pos, clr::convert_to<std::string>(std::forward<decltype(text)>(text)).c_str(),
                        std::forward<decltype(tooltip)>(tooltip), std::forward<decltype(tooltip_font)>(tooltip_font),
                        std::forward<decltype(flags)>(flags));
    } else if constexpr (!std::is_convertible_v<decltype(tooltip), const char*>) {
      return add<PROPS>(pos, std::forward<decltype(text)>(text),
                        clr::convert_to<std::string>(std::forward<decltype(tooltip)>(tooltip)).c_str(),
                        std::forward<decltype(tooltip_font)>(tooltip_font), std::forward<decltype(flags)>(flags));
    } else {
      static_assert(!std::is_same_v<std::nullptr_t, std::decay_t<decltype(text)>>);

      ImGuiContext& ctx   = *ImGui::GetCurrentContext();
      ImGuiWindow& window = *ImGui::GetCurrentWindow();
      info return_data;

      // text colouring
      if constexpr (!(PROPS & NO_BUTTON_COLOUR)) {
        const auto col = (flags & ImGuiButtonFlags_Disabled) ? ImGuiCol_TextDisabled : ImGuiCol_ButtonHovered;
        ImGui::PushStyleColor(ImGuiCol_Text, ctx.Style.Colors[col]);
      } else {
        if (flags & ImGuiButtonFlags_Disabled) {
          ImGui::PushStyleColor(ImGuiCol_Text, ctx.Style.Colors[ImGuiCol_TextDisabled]);
        }
      }

      ImGui::RenderText(pos, text);
      return_data.size = ImGui::CalcTextSize(text);

      // pop text colouring
      if constexpr (!(PROPS & NO_BUTTON_COLOUR)) {
        ImGui::PopStyleColor();
      } else {
        if (flags & ImGuiButtonFlags_Disabled) {
          ImGui::PopStyleColor();
        }
      }

      bool held;
      {
        ImRect interact_bb;
        interact_bb.Min = pos;
        interact_bb.Max = pos;
        interact_bb.Max.x += return_data.size.x;
        interact_bb.Max.y += return_data.size.y;

        const ImGuiID id{window.GetID(text)};
        return_data.pressed = ImGui::ButtonBehavior(interact_bb, id, &return_data.hovered, &held, flags);
      }

      if (return_data.pressed) {
        return return_data;
      } else {
        if constexpr (!std::is_same_v<std::nullptr_t, std::decay_t<decltype(tooltip)>>) {
          if (return_data.hovered && strlen(tooltip) > 0) {
            constexpr bool MAYBE_FONT = !std::is_same_v<std::nullptr_t, std::decay_t<decltype(tooltip_font)>>;
            if constexpr (MAYBE_FONT) {
              if (tooltip_font) {
                ImGui::PushFont(tooltip_font);
              }
            }
            ImGui::SetTooltip("%s", tooltip);
            if constexpr (MAYBE_FONT) {
              if (tooltip_font) {
                ImGui::PopFont();
              }
            }
          }
        }
      }
      underline<PROPS>(pos, return_data.size, return_data.hovered);
      return return_data;
    }
  }

  template <property_t PROPS = clr::BIT_FLAG_NONE<property_t>, typename... Args,
            typename         = typename std::enable_if_t<clr::param_pack_valid_v<clr::given_args<Args...>, RA, OA>>>
  [[nodiscard]] static info add(Args&&... args) {
    auto&& text         = clr::get<0, RA, OA>(std::forward<Args>(args)...);
    auto&& tooltip      = clr::get_optional<1, RA, OA>(nullptr, std::forward<Args>(args)...);
    auto&& tooltip_font = clr::get_optional<2, RA, OA>(nullptr, std::forward<Args>(args)...);
    auto&& flags        = clr::get_optional<3, RA, OA>(0, std::forward<Args>(args)...);

    if constexpr (!std::is_convertible_v<decltype(text), const char*>) {
      std::string const& str = text; // force std::string conversion if necessary
      return add<PROPS>(str.c_str(), std::forward<decltype(tooltip)>(tooltip),
                        std::forward<decltype(tooltip_font)>(tooltip_font), std::forward<decltype(flags)>(flags));
    } else {
      static_assert(!std::is_same_v<std::nullptr_t, std::decay_t<decltype(text)>>);
      ImGui::Dummy(ImGui::CalcTextSize(text));
      return add<PROPS>(ImGui::GetItemRectMin(), text, std::forward<decltype(tooltip)>(tooltip),
                        std::forward<decltype(tooltip_font)>(tooltip_font), std::forward<decltype(flags)>(flags));
    }
  }
};

} // namespace webapp