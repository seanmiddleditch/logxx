// logxx - C++ logging library.
//
// This is free and unencumbered software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non - commercial, and by any
// means.
// 
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// 
// For more information, please refer to <http://unlicense.org/>
//
// Authors:
//   Sean Middleditch <sean.middleditch+logxx@gmail.com>

#if !defined(_guard_LOGXX_LOG_FORMAT_H)
#define _guard_LOGXX_LOG_FORMAT_H 1
#pragma once

#include "logxx.h"
#include <formatxx/format.h>
#include <formatxx/fixed_writer.h>

namespace logxx {
    template <typename... T>
    void log_format(log_message const& message, T const& ... args) {
        formatxx::fixed_writer<4096> buffer;
        formatxx::format(buffer, formatxx::string_view{ message.message.data(), message.message.size() }, args...);
        log_message formatted_message = message;
        formatted_message.message = string_view{ buffer.c_str(), buffer.size() };
        dispatch_message(formatted_message);
    }
} // namespace logxx

#if LOGXX_SOURCE_LOCATION
#   define LOGXX_LOGF(level, format, ...) (::logxx::log_format({(level), (format), __FILE__, __FUNCTION__, __LINE__}, __VA_ARGS__))
#else
#   define LOGXX_LOGF(level, format, ...) (::logxx::log_format({(level), (format)}, __VA_ARGS__))
#endif

#endif // !defined(_guard_LOGXX_LOG_FORMAT_H)
