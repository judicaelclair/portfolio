/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/widget/text.cpp
 * Last Modified: Saturday, 14th November 2020 2:38:08 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/widget/text.hpp>
#include <WebApp/gui/widget/link.hpp>
#include <WebApp/gui/util/imgui.hpp>
#include <WebApp/gui/util/js.hpp>
#include <WebApp/gui/icon.hpp>

#include <Clarity/core/util/str.hpp>

#include <imgui.h>
#include <imgui_internal.h>

namespace webapp {

static ImVec2 calc_text_size_no_rounding(const char* text, const char* text_end = nullptr,
                                         const bool hide_text_after_double_hash = false,
                                         const float wrap_width                 = -1.0f) {
  ImGuiContext& ctx = *ImGui::GetCurrentContext();

  const char* text_display_end;
  if (hide_text_after_double_hash)
    text_display_end = ImGui::FindRenderedTextEnd(text, text_end); // Hide anything after a '##' string
  else
    text_display_end = text_end;

  ImFont* const font    = ctx.Font;
  const float font_size = ctx.FontSize;
  if (text == text_display_end) {
    return {0.0f, font_size};
  }
  return font->CalcTextSizeA(font_size, FLT_MAX, wrap_width, text, text_display_end, NULL);
}

wrapped_text_link_t wrapped_text_external_link(std::string lk, std::string_view desc) {
  std::string tp = clr::fmt("%s %.*s\n%s", icon::LINK, int(desc.size()), desc.data(), lk.c_str());
  return [tp{std::move(tp)}, lk{std::move(lk)}](const char* name, ImVec2 pos, bool hovered) {
    if (hovered) [[unlikely]] {
      link::underline(pos, ImGui::CalcTextSize(name), hovered);
      return hovered;
    } else {
      auto info = link::add(pos, name, tp);
      if (info.pressed) {
        open_link(lk);
      }
      return info.hovered;
    }
  };
}

} // namespace webapp

namespace webapp::detail {

void bullet_wrapped_text(const char* txt, const wrapped_text_link_t* links) {
  ImGui::Bullet();
  ImGui::SameLine();
  ImGui::GetCurrentWindow()->DC.CursorPos.x += ImGui::GetStyle().FramePadding.x;
  wrapped_text(txt, links);
}

void wrapped_text(const char* txt, const wrapped_text_link_t* links) {
  ImGuiContext& ctx   = *ImGui::GetCurrentContext();
  ImGuiWindow& window = *ImGui::GetCurrentWindow();

  const float wrap_width = ImGui::CalcWrapWidthForPos(window.DC.CursorPos, 0.0f);
  const ImVec2 text_pos(window.DC.CursorPos.x, window.DC.CursorPos.y + window.DC.CurrLineTextBaseOffset);

  const char* tail_txt        = txt;
  const char* word_txt        = txt;
  const char* head_txt        = txt;
  float txt_size              = 0.0f;
  const float space_char_size = calc_text_size_no_rounding(" ").x;
  const ImU32 txt_col         = ImGui::GetColorU32(ImGuiCol_Text);
  const float line_height     = ImGui::GetTextLineHeight();
  ImVec2 pos                  = text_pos;
  size_t n_lines              = 0;
  float max_x                 = 0.0f;
  size_t link_idx             = 0;
  bool link_is_hovered        = false;
  std::vector<std::pair<std::string, ImVec2>> link_widgets;
  link_widgets.reserve(2);

  auto remove_trailing_spaces = [&](const char* end) {
    while (tail_txt != end && *(end - 1) == ' ') {
      --end;
      txt_size -= space_char_size;
    }
    return (tail_txt == end) ? nullptr : end;
  };

  auto add_text = [&](const char* start, const char* end) {
    if (pos.x == text_pos.x) {
      ++n_lines;
    }
    max_x = std::max<float>(max_x, txt_size);
    window.DrawList->AddText(ctx.Font, ctx.FontSize, pos, txt_col, start, end);
    pos.x = text_pos.x + txt_size;
  };

  auto add_link = [&](const char* start, const char* end) {
    if (pos.x == text_pos.x) {
      ++n_lines;
    }
    end   = remove_trailing_spaces(end);
    max_x = std::max<float>(max_x, txt_size);
    if (!end) {
      return;
    }
    std::string str{start, end};
    auto const& lf = links[link_idx];
    if (lf(str.c_str(), pos, false)) {
      link_is_hovered = true;
      for (auto const& lw : link_widgets) {
        lf(lw.first.c_str(), lw.second, true);
      }
    } else if (link_is_hovered) {
      lf(str.c_str(), pos, true);
    }
    link_widgets.emplace_back(std::move(str), pos);
    pos.x = text_pos.x + txt_size;
  };

  auto newline = [&]() {
    if (txt_size == 0) {
      ++n_lines;
    }
    txt_size = 0;
    pos.y += line_height;
    pos.x = text_pos.x;
  };

  auto render_up_to_head = [&]() {
    if (tail_txt != head_txt) {
      if (word_txt) {
        const float size_to_add = calc_text_size_no_rounding(word_txt, head_txt).x;
        if ((txt_size + size_to_add) <= wrap_width || txt_size == 0) {
          txt_size += size_to_add;
          add_text(tail_txt, head_txt);
        } else {
          add_text(tail_txt, word_txt);

          txt_size = size_to_add;
          pos.y += line_height;
          pos.x = text_pos.x;
          add_text(word_txt, head_txt);
        }
      } else {
        add_text(tail_txt, head_txt);
      }
    }

    while (true) {
      const char c = *head_txt;
      if (c == ' ') {
        ++head_txt;
      } else if (c == '\n') {
        ++head_txt;
        newline();
        break;
      } else {
        break;
      }
    }

    word_txt = (tail_txt = head_txt);
  };

  while (true) {
    switch (*head_txt) {
      case ' ': {
        const char* old_word_txt = word_txt;
        float size_to_add        = space_char_size;
        if (word_txt && head_txt != word_txt) {
          size_to_add += calc_text_size_no_rounding(word_txt, head_txt).x;
          word_txt = nullptr;
        }
        if ((txt_size + size_to_add) >= wrap_width) {
          if (txt_size > 0 && old_word_txt) {
            head_txt = old_word_txt;
          } else {
            txt_size += size_to_add;
          }

          add_text(tail_txt, head_txt);
          newline();

          while (true) {
            const char c = *head_txt;
            if (c == ' ') {
              ++head_txt;
            } else if (c == '\n') {
              ++head_txt;
              break;
            } else {
              break;
            }
          }

          word_txt = (tail_txt = head_txt);
        } else {
          txt_size += size_to_add;
          ++head_txt;
        }
        continue;
      }
      case '{': {
        render_up_to_head();

        word_txt = (tail_txt = ++head_txt);
        while (true) {
          switch (*head_txt) {
            case '}': {
              if (tail_txt != head_txt) {
                if (word_txt) {
                  const float size_to_add = calc_text_size_no_rounding(word_txt, head_txt).x;
                  if ((txt_size + size_to_add) <= wrap_width || txt_size == 0) {
                    txt_size += size_to_add;
                    add_link(tail_txt, head_txt);
                  } else {
                    add_link(tail_txt, word_txt);

                    txt_size = size_to_add;
                    pos.y += line_height;
                    pos.x = text_pos.x;
                    add_link(word_txt, head_txt);
                  }
                } else {
                  add_link(tail_txt, head_txt);
                }
              }
              word_txt = nullptr;
              tail_txt = ++head_txt;
              break;
            }
            case ' ': {
              const char* old_word_txt = word_txt;
              float size_to_add        = space_char_size;
              if (word_txt && head_txt != word_txt) {
                size_to_add += calc_text_size_no_rounding(word_txt, head_txt).x;
                word_txt = nullptr;
              }
              if ((txt_size + size_to_add) >= wrap_width) {
                if (txt_size > 0 && old_word_txt) {
                  head_txt = old_word_txt;
                } else {
                  txt_size += size_to_add;
                }
                add_link(tail_txt, head_txt);
                newline();

                while (*head_txt == ' ') {
                  ++head_txt;
                }

                word_txt = nullptr;
                tail_txt = head_txt;
              } else {
                txt_size += size_to_add;
                ++head_txt;
              }
              continue;
            }
            default: {
              if (!word_txt) {
                word_txt = head_txt;
              }
              ++head_txt;
              continue;
            }
          };
          break;
        }
        ++link_idx;
        link_widgets.clear();
        link_is_hovered = false;
        continue;
      }
      case '\n': {
        render_up_to_head();
        continue;
      }
      case '\0': {
        render_up_to_head();
        break;
      }
      default: {
        if (!word_txt) {
          word_txt = head_txt;
        }
        ++head_txt;
        continue;
      }
    }
    break;
  }

  ImGui::ItemSize({max_x, static_cast<float>(n_lines) * line_height}, 0.0f);
}

} // namespace webapp::detail