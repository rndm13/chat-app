#pragma once

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <cstdint>
#include <fmt/ostream.h>
#include <thread>

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

template <>
struct fmt::formatter<boost::thread::id> : fmt::ostream_formatter {};

template <> struct fmt::formatter<std::thread::id> : fmt::ostream_formatter {};

using boost::asio::ip::tcp;

struct Message {
    boost::array<char, 256> contents{};
    boost::array<char, 64> user_name{};

    Message();
    Message(const boost::array<char, 256> &contents,
            const boost::array<char, 64> &user_name);
    Message(std::string_view, std::string_view);
    static Message from_message_string(std::string_view);
};

template <u64 size>
std::string arr_to_str(const boost::array<char, size> &buffer) {
    std::string result;
    result.reserve(size);
    for (char c : buffer) {
        if (c == 0x00) {
            break;
        }
        result.push_back(c);
    }
    return result;
}

template <u64 size> struct fmt::formatter<boost::array<char, size>> {
    constexpr auto parse(format_parse_context &ctx)
        -> format_parse_context::iterator {
        auto it = ctx.begin(), end = ctx.end();
        return it;
    }

    auto format(const boost::array<char, size> &msg, format_context &ctx) const
        -> format_context::iterator {
        // ctx.out() is an output iterator to write to.
        return fmt::format_to(ctx.out(), "{}", arr_to_str(msg));
    }
};

template <> struct fmt::formatter<Message> {
    constexpr auto parse(format_parse_context &ctx)
        -> format_parse_context::iterator {
        auto it = ctx.begin(), end = ctx.end();
        return it;
    }

    auto format(const Message &msg, format_context &ctx) const
        -> format_context::iterator {
        // ctx.out() is an output iterator to write to.
        return fmt::format_to(ctx.out(), "[{}]\n{}\n", msg.user_name,
                              msg.contents);
    }
};
