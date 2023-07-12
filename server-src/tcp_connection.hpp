#pragma once
#include "../commmon-src/common_lib.hpp"
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/system/system_error.hpp>
#include <boost/thread.hpp>
#include <fmt/format.h>
#include <fmt/std.h>
#include <iostream>
#include <string>
#include <string_view>
#include <thread>

class tcp_connection
    : public boost::enable_shared_from_this<tcp_connection> {
    tcp::socket socket;
    boost::array<char, 256> received_message;
    boost::array<char, 64> user_name;

    void handle_write(const boost::system::error_code &error,
                      u64 bytes_transferred); 

    void handle_read(const boost::system::error_code &error,
                     u64 bytes_transferred);

    void handle_join(const boost::system::error_code &error,
                     u64 bytes_transferred);

  public:
    typedef boost::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_context &io_context) {
        return pointer(new tcp_connection(io_context));
    }

    tcp::socket &get_socket() { return socket; }

    void start();

    ~tcp_connection() {
        fmt::print("[INFO] Someone disconnected.\n"); 
    }

    void send_message(const Message &msg); 

    tcp_connection(boost::asio::io_context &ctx)
        : socket(ctx) {}
};
