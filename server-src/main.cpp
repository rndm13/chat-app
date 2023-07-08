#include "fmt/ostream.h"
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind/bind.hpp>
#include <boost/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <fmt/format.h>
#include <fmt/std.h>
#include <iostream>
#include <string>
#include <thread>

using boost::asio::ip::tcp;

template <>
struct fmt::formatter<boost::thread::id> : fmt::ostream_formatter {};

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

std::string make_daytime_string() {
    std::time_t now = std::time(0);
    return std::ctime(&now);
}

class tcp_connection : public boost::enable_shared_from_this<tcp_connection> {
    tcp::socket socket;
    std::string message;

    void handle_write(const boost::system::error_code &error,
                      u64 bytes_transferred) {
        fmt::print("[INFO {}] Sent a write.\n", boost::this_thread::get_id());
    }

  public:
    typedef boost::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_context &io_context) {
        return pointer(new tcp_connection(io_context));
    }

    tcp::socket &get_socket() { return socket; }

    void start() {
        message = make_daytime_string();
        boost::asio::async_write(
            socket, boost::asio::buffer(message),
            boost::bind(&tcp_connection::handle_write, shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }

    tcp_connection(boost::asio::io_context &ctx) : socket(ctx) {}
};

class tcp_server {
    boost::asio::io_context &io_context;
    tcp::acceptor acceptor;

    void start_accept() {
        tcp_connection::pointer new_connection =
            tcp_connection::create(io_context);

        acceptor.async_accept(new_connection->get_socket(),
                              boost::bind(&tcp_server::handle_accept, this,
                                          new_connection,
                                          boost::asio::placeholders::error));
    }

    void handle_accept(tcp_connection::pointer new_connection,
                       const boost::system::error_code &error) {
        if (!error) {
            new_connection->start();
            fmt::print("[INFO {}] Received a connection.\n", boost::this_thread::get_id());
        }

        start_accept();
    }

  public:
    tcp_server(boost::asio::io_context &io)
        : io_context(io), acceptor(io, tcp::endpoint(tcp::v4(), 8000)) {
        start_accept();
        fmt::print("[INFO {}] Started the server.\n", boost::this_thread::get_id());
    }
};

i32 main() {
    try {
        boost::asio::io_context io_context;
        tcp_server server(io_context);
        io_context.run();
    } catch (boost::system::system_error &err) {
        fmt::print(stderr, "[ERROR] {}", err.what());
        exit(2);
    }
}
