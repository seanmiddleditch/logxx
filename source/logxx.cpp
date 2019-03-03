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
#include "logxx/logger_stdio.h"
#include "reader_writer_lock.h"

#include <mutex>
#include <cstdio>
#include <vector>

namespace {
    logxx::logger_stdio default_logger(stdout);

    logxx::aligned_atomic_uint32_t global_lock = 0;
    logxx::writer_lock global_writer_lock(global_lock);
    logxx::reader_lock global_reader_lock(global_lock);
    std::vector<logxx::logger_base*> global_loggers{ &default_logger };

    thread_local std::vector<logxx::logger_base*> thread_loggers;
}

char const* LOGXX_API logxx::level_string(log_level level) {
    switch (level) {
    case log_level::fatal: return "fatal";
    case log_level::error: return "error";
    case log_level::warning: return "warning";
    case log_level::info: return "info";
    case log_level::debug: return "debug";
    case log_level::trace: return "trace";
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

auto LOGXX_API logxx::dispatch_message(log_message const& message) -> log_result_code {
    for (logger_base* logger : thread_loggers) {
        log_operation op = logger->handle(message);
        if (op == log_operation::op_break) {
            return log_result_code::success;
        }
    }

    std::unique_lock<reader_lock> _(global_reader_lock);

    for (logger_base* logger : global_loggers) {
        log_operation op = logger->handle(message);
        if (op == log_operation::op_break) {
            return log_result_code::success;
        }
    }

    return log_result_code::success;
}
