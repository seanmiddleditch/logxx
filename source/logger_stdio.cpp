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

#include "logxx/logger_stdio.h"

auto logxx::logger_stdio::handle(message const& message) -> operation {
    std::fprintf(_file,
#if LOGXX_USE_SOURCE_LOCATION
        "%.*s(%i):%.*s "
#endif
        "(%s) %.*s\n",
#if LOGXX_USE_SOURCE_LOCATION
        static_cast<int>(message.location.file.size()),
        message.location.file.data(),
        message.location.line,
        static_cast<int>(message.location.function.size()),
        message.location.function.data(),
#endif
        level_string(message.level),
        static_cast<int>(message.message.size()),
        message.message.data()
    );

    if (_flush) {
        fflush(_file);
    }

    return operation::op_continue;
}
