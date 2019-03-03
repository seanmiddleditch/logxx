# logxx

Liteweight C++ logging library.

## Authors

Sean Middleditch <sean.middleditch+logxx@gmail.com>

## About

logxx is intended to be a very lite-weight logging library. It doesn't aim to be the fastest
logging library, nor the most configurable, nor the most fully-featured. It instead aims to be
small and quick-to-compile, with reasonably good performance, reasonably idiomatic C++ where
possible, and ease of integration into an existing project.

While it aims to include features like multi-level loggers, log filtering, multiple log sinks,
formatted log messages, logger categories, and run-time configuration, it does so in as minimal
a way possible. Where necessary, core features are split into separate headers so you only pay
for what you use.

## Usage

logxx has only a few basic concepts. The first is `logxx::logger_base`. A custom logger (sink)
should derive from this class and implement `handle(log_message const&)`.

Loggers can be registered globally via the free function
`logxx::set_global_logger(logger_base* logger, logger_base** old)`, and for the curren thread
via `logxx::set_thread_local_logger(logger_base* logger, logger_base** old)`.

The second parameters are optional but if non-nullptr, they will be set to the previous logger
if any.

Log messages can be written with `logxx::dispatch_message(log_message const&)`.

Scoped helpers for registering loggers, named `logxx::scoped_logger` for global loggers and
`logxx::scoped_thread_local_logger` for thread-local loggers can be used.

Macros such as `LOGXX_LOG(level, message)`, `LOGXX_LOG_DEBUG(message)`, and so on can be
used for convenient dispatch of static strings to `dispatch_message`.

## History and Design Notes

The author originally used [spdlog](https://github.com/gabime/spdlog) for logging. However,
spdlog uses [fmtlib](https://github.com/fmtlib/fmt) for its formatted logging, among other
libraries (including the standard library), which makes it particularly heavy-weight on
compile times.

This same author created [formatxx](https://github.com/seanmiddleditch/formatxx) to optimize
the build overhead of fmtlib, and logxx is a similar response to spdlog.

Of great importance to note is that with C++20 around the corner, the introduction of C++
Modules may so greatly reduce the compile-time overhead of libraries like spdlog and fmtlib
(which might itself also be part of C++ in an upcoming release) that this library will be
obsolete. It may be that template instantiation overhead is still a serious problem with
those other libraries. Time will tell.

## To Do

- Optimization and profling
- Multi-logger implementation
- Structured logging?
- Filtering
- Error contexts
- More built-in loggers
  - Rotating file logger
  - Network logger?
  - *nix syslog logger
- Formatted logging via formatxx
  - Maybe also ostream and fmtlib (or stdlib) formatting
- Real tests

## Copying

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
