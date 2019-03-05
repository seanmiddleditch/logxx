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

#include <atomic>

#if !defined(_guard_LOGXX_READER_WRITER_LOCK_H)
#define _guard_LOGXX_READER_WRITER_LOCK_H 1
#pragma once

#include <cstdint>
#include <atomic>

namespace logxx {
    namespace reader_writer {
        static constexpr std::uint32_t write_value = std::uint32_t(-1);
        static constexpr std::uint32_t read_mask = write_value >> 1;
        static constexpr int alignment = 128;
    }

    struct alignas(reader_writer::alignment) aligned_atomic_uint32_t : std::atomic_uint32_t {
        constexpr aligned_atomic_uint32_t(std::uint32_t value) noexcept : std::atomic_uint32_t(value) {}

    private:
        char _padding[reader_writer::alignment - sizeof(std::atomic_uint32_t)] = {};
    };
    static_assert(alignof(aligned_atomic_uint32_t) == reader_writer::alignment);
    static_assert(sizeof(aligned_atomic_uint32_t) == reader_writer::alignment);

    class reader_lock {
    public:
        reader_lock(aligned_atomic_uint32_t& lock) noexcept : _lock(lock) {}

        void lock() noexcept {
            while (true) {
                std::uint32_t current = _lock.load(std::memory_order_relaxed);
                std::uint32_t next = (current & reader_writer::read_mask) + 1;
                if (_lock.compare_exchange_weak(current, next, std::memory_order_acquire)) {
                    break;
                }
            }
        }

        void unlock() noexcept {
            _lock.fetch_sub(1, std::memory_order_release);
        }

    private:
        std::atomic_uint32_t& _lock;
    };

    class writer_lock {
    public:
        writer_lock(aligned_atomic_uint32_t& lock) noexcept : _lock(lock) {}

        void lock() noexcept {
            while (true) {
                std::uint32_t current = 0;
                if (_lock.compare_exchange_weak(current, reader_writer::write_value, std::memory_order_acquire)) {
                    break;
                }
            }
        }

        void unlock() noexcept {
            _lock.store(0, std::memory_order_release);
        }

    private:
        std::atomic_uint32_t& _lock;
    };

} // namespace logxx

#endif // !defined(_guard_LOGXX_READER_WRITER_LOCK_H)
