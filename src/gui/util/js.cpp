/*
 * Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
 * You cannot use, modify or redistribute this code without explicit
 * permission from the copyright holder.
 * -----
 * File: /src/gui/util/js.cpp
 * Last Modified: Friday, 13th November 2020 10:26:17 pm
 * Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
 */

#include <WebApp/gui/util/js.hpp>
#include <emscripten.h>

namespace webapp {

namespace detail {

  EM_JS(void, open_link, (const char* s, int len), {
    var url = UTF8ToString(s, len);
    window.open(url);
  });

  EM_JS(void, open_link_popup, (const char* s, int len), {
    var url   = UTF8ToString(s, len);
    var width = window.innerWidth * 0.66;
    // define the height in
    var height = width * window.innerHeight / window.innerWidth;
    // Ratio the hight to the width as the user screen ratio
    window.open(url, '_blank',
                'width=' + width + ', height=' + height + ', top=' + ((window.innerHeight - height) / 2) + ', left='
                    + ((window.innerWidth - width) / 2));
  });

  EM_JS(void, open_email, (const char* s, int len), {
    var email            = UTF8ToString(s, len);
    window.location.href = "mailto:" + email;
  });

  // Each call to _malloc() must be paired with free(), or heap memory will leak!
  EM_JS(const char*, current_url, (), {
    var url              = window.location.href;
    var nbytes           = lengthBytesUTF8(url) + 1;
    var str_on_wasm_heap = _malloc(nbytes);
    stringToUTF8(url, str_on_wasm_heap, nbytes);
    return str_on_wasm_heap;
  });

} // namespace detail

void open_email(std::string_view email) {
  detail::open_email(email.data(), static_cast<int>(email.size()));
}

void open_link(std::string_view link) {
  detail::open_link(link.data(), static_cast<int>(link.size()));
}

void open_link_popup(std::string_view link) {
  detail::open_link_popup(link.data(), static_cast<int>(link.size()));
}

std::string current_url() {
  const char* raw = detail::current_url();
  std::string str = raw;
  free(static_cast<void*>(const_cast<char*>(raw)));
  size_t hash_pos = str.find('#');
  if (hash_pos != std::string::npos) {
    str.resize(hash_pos);
  }
  return str;
}

} // namespace webapp