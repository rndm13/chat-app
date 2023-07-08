#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/system/system_error.hpp>
#include <boost/thread/thread.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iostream>

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using boost::asio::ip::tcp;

i32 main(i32 argc, char **argv) {
    if (argc != 2) {
        fmt::print("Usage:\n"
                   "   client <host>\n");
        return 1;
    }
    boost::asio::io_context io_ctx;

    tcp::resolver resolver(io_ctx);

    tcp::socket socket(io_ctx);

    try {
        tcp::resolver::results_type endpoints =
            resolver.resolve(argv[1], "8000");

        boost::asio::connect(socket, endpoints);
    } catch (boost::system::system_error &err) {
        fmt::print(stderr, "Failed to set up connection: {}\n", err.what());
        exit(2);
    }

    for (;;) {
        boost::array<char, 128> buf;
        boost::system::error_code error;
        u64 len = 0;
        try {
            len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof) {
                break;
            } else if (error) {
                throw boost::system::system_error(error);
            }
        } catch (boost::system::system_error &err) {
            fmt::print(stderr, "Failed to read data: {}", err.what());
            exit(3);
        }

        std::cout.write(buf.data(), len);
    }

    return 0;
}
