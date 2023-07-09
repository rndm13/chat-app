#pragma once

#include <cstdint>
#include <fmt/ostream.h>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

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

using boost::asio::ip::tcp;
