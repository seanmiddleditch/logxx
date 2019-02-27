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

#if !defined(_guard_LOGXX_H)
#define _guard_LOGXX_H 1
#pragma once

#include <cstdlib>
#include <cstring>

#if !defined(LOGXX_API)
#	if defined(_WIN32)
#		define LOGXX_API __stdcall
#	else
#		define LOGXX_API
#	endif
#endif

#if defined(_WIN32) && !defined(LOGXX_PUBLIC)
#	if defined(LOGXX_EXPORT)
#		define LOGXX_PUBLIC __declspec(dllexport)
#	else
#		define LOGXX_PUBLIC
#	endif
#elif __GNUC__ >= 4 && !defined(LOGXX_PUBLIC)
#	if defined(LOGXX_EXPORT)
#		define LOGXX_PUBLIC __attribute__((visibility("default")))
#	else
#		define LOGXX_PUBLIC
#	endif
#endif

#if !defined(LOGXX_SOURCE_LOCATION)
#   if defined(NDEBUG)
#      define LOGXX_SOURCE_LOCATION 0
#   else
#      define LOGXX_SOURCE_LOCATION 1
#   endif
#endif

namespace logxx {
    enum class log_level {
        fatal,
        error,
        warning,
        info,
        debug,
        trace,

        _max_
    };

    enum class log_result {
        success = 0,
        no_loggers,
        invalid_argument,
        duplicate_logger,
        unknown
    };

    class string_view final {
    public:
        using iterator = char const*;
        using size_type = std::size_t;

        constexpr string_view() noexcept = default;
        constexpr string_view(iterator begin, iterator end) noexcept : _begin(begin), _end(end) {}
        constexpr string_view(iterator begin, size_type count) noexcept : _begin(begin), _end(_begin + count) {}
        string_view(char const* str) noexcept : _begin(str), _end(str != nullptr ? str + std::strlen(str) : nullptr) {}

        constexpr bool empty() const noexcept { return _begin == _end; }
        constexpr char const* data() const noexcept { return _begin; }
        constexpr size_type size() const noexcept { return _end - _begin; }

        constexpr iterator begin() const noexcept { return _begin; }
        constexpr iterator end() const noexcept { return _begin; }

        explicit operator bool() const noexcept { return _begin != _end; }

    private:
        char const* _begin = nullptr;
        char const* _end = nullptr;
    };

    struct log_message final {
        log_level level = log_level::info;
        string_view message;
#if LOGXX_SOURCE_LOCATION
        string_view location_file;
        string_view location_symbol;
        int location_line;
#endif
    };

    class logger_base {
    public:
        virtual ~logger_base() = default;

        virtual void LOGXX_API handle(log_message const& message) = 0;
    };

    LOGXX_PUBLIC char const* LOGXX_API level_string(log_level level);

    LOGXX_PUBLIC log_result LOGXX_API set_global_logger(logger_base* logger, logger_base** old_logger = nullptr);
    LOGXX_PUBLIC log_result LOGXX_API set_thread_local_logger(logger_base* logger, logger_base** old_logger = nullptr);

    LOGXX_PUBLIC log_result LOGXX_API dispatch_message(log_message const& message);

    class scoped_logger {
    public:
        scoped_logger(logger_base& logger) : _logger(logger) {
            set_global_logger(&_logger, &_old);
        }

        ~scoped_logger() {
            set_global_logger(_old);
        }

    private:
        logger_base& _logger;
        logger_base* _old = nullptr;
    };

    class scoped_logger_thread_local {
    public:
        scoped_logger_thread_local(logger_base& logger) : _logger(logger) {
            set_thread_local_logger(&_logger, &_old);
        }

        ~scoped_logger_thread_local() {
            set_thread_local_logger(_old);
        }

    private:
        logger_base& _logger;
        logger_base* _old = nullptr;
    };

} // namespace logxx

#if LOGXX_SOURCE_LOCATION
#   define LOXX_LOG(level, message) (::logxx::dispatch_message({(level), (message), __FILE__, __FUNCTION__, __LINE__}))
#else
#   define LOXX_LOG(level, message) (::logxx::dispatch_message({(level), (message)}))
#endif

#define LOXX_LOG_ERROR(message) LOXX_LOG(::logxx::log_level::error, (message))
#define LOXX_LOG_INFO(message) LOXX_LOG(::logxx::log_level::info, (message))
#define LOXX_LOG_DEBUG(message) LOXX_LOG(::logxx::log_level::debug, (message))

#endif // !defined(_guard_LOGXX_H)
