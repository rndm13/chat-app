#pragma once

#include "../commmon-src/common_lib.hpp"
#include <boost/array.hpp>
#include <fmt/format.h>
#include <stdexcept>

struct Message {
    boost::array<char, 256> contents{};
    boost::array<char, 64> user_name{};

    Message();
    Message(const boost::array<char, 256>& contents, const boost::array<char, 64>& user_name);
    Message(std::string_view, std::string_view);
    static Message from_message_string(std::string_view);
};

template <> struct fmt::formatter<Message> {
    constexpr auto parse(format_parse_context &ctx)
        -> format_parse_context::iterator {
        auto it = ctx.begin(), end = ctx.end();
        return it;
    }

    // Formats the point p using the parsed format specification (presentation)
    // stored in this formatter.
    auto format(const Message &msg, format_context &ctx) const
        -> format_context::iterator {
        // ctx.out() is an output iterator to write to.
        return fmt::format_to(ctx.out(), "[{}]\n{}\n",
                              std::string(msg.user_name.data()),
                              std::string(msg.contents.data()));
    }
};

struct Model {
    std::vector<Message> messages;
};
