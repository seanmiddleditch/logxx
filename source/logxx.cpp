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

#include "logxx/logxx.h"
#include "reader_writer_lock.h"

#if defined(_WIN32)
#   include "logxx/logger_outputdebugstring.h"
#else
#   include "logxx/logger_stdio.h"
#endif

#include <mutex>
#include <cstdio>
#include <vector>

namespace {
#if defined(_WIN32)
    logxx::logger_outputdebugstring default_logger;
#else
    logxx::logger_stdio default_logger(stderr);
#endif

    logxx::aligned_atomic_uint32_t global_lock = 0;
    logxx::writer_lock global_writer_lock(global_lock);
    logxx::reader_lock global_reader_lock(global_lock);
    std::vector<logxx::logger_base*> global_loggers;

    logxx::logger_base* current_default_logger = &default_logger;

    thread_local std::vector<logxx::logger_base*> thread_loggers;
}

char const* LOGXX_API logxx::level_string(log_level level) noexcept {
    switch (level) {
    case log_level::fatal: return "FATAL";
    case log_level::error: return "ERROR";
    case log_level::warning: return "WARNING";
    case log_level::info: return "INFO";
    case log_level::debug: return "DEBUG";
    case log_level::trace: return "TRACE";
    default: return "$unknown$";
    }
}

logxx::scoped_logger::scoped_logger(logger_base& logger) : _logger(logger) {
    std::unique_lock<writer_lock> _(global_writer_lock);
    global_loggers.push_back(&logger);
}

logxx::scoped_logger::~scoped_logger() {
    std::unique_lock<writer_lock> _(global_writer_lock);
    global_loggers.pop_back();
}

logxx::scoped_logger_thread_local::scoped_logger_thread_local(logger_base& logger) : _logger(logger) {
    thread_loggers.push_back(&logger);
}

logxx::scoped_logger_thread_local::~scoped_logger_thread_local() {
    thread_loggers.pop_back();
}

auto LOGXX_API logxx::dispatch(log_level level, source_location location, string_view message) -> result_code {
    logxx::message msg{ level, message, location };

    for (auto index = thread_loggers.size(); index != 0; --index) {
        operation op = thread_loggers[index - 1]->handle(msg);
        if (op == operation::op_break) {
            return result_code::success;
        }
    }

    std::unique_lock<reader_lock> _(global_reader_lock);

    for (auto index = global_loggers.size(); index != 0; --index) {
        operation op = global_loggers[index - 1]->handle(msg);
        if (op == operation::op_break) {
            return result_code::success;
        }
    }

    current_default_logger->handle(msg);

    return result_code::success;
}

auto LOGXX_API logxx::set_default_logger(logger_base* new_logger, logger_base** old_logger) noexcept -> result_code {
    if (new_logger == nullptr) {
        new_logger = &default_logger;
    }

    if (old_logger != nullptr) {
        *old_logger = current_default_logger;
    }

    current_default_logger = new_logger;

    return result_code::success;
}
