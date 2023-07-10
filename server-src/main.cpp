#include "fmt/ostream.h"
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
#include "../commmon-src/common_lib.hpp"

class tcp_connection : public boost::enable_shared_from_this<tcp_connection> {
    tcp::socket socket;
    boost::array<char, 256+64+4> received_message;

    void handle_write(const boost::system::error_code &error,
                      u64 bytes_transferred) {
        if (error) {
            fmt::print("[ERROR {}] Failed to sent a write: \"{}\" \n", boost::this_thread::get_id(), error.what());
        }
    }

    void handle_read(const boost::system::error_code &error,
                     u64 bytes_transferred) {
        if (error == boost::asio::error::eof) {
            fmt::print("[INFO {}] Connection ended.\n",
                       boost::this_thread::get_id());
            return;
        } else if (error) {
            fmt::print("[ERROR {}] Error when receiving message: \"{}\"\n",
                       boost::this_thread::get_id(), error.what());
            return;
        }
        
        fmt::print(
            "[INFO {}] Received a message: \"{}\"\n",
            boost::this_thread::get_id(),
            std::string_view(received_message.data(), bytes_transferred));

        socket.async_write_some(
            boost::asio::buffer(received_message),
            boost::bind(&tcp_connection::handle_write, shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
        
        socket.async_read_some(
            boost::asio::buffer(received_message),
            boost::bind(&tcp_connection::handle_read, shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }

  public:
    typedef boost::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_context &io_context) {
        return pointer(new tcp_connection(io_context));
    }

    tcp::socket &get_socket() { return socket; }

    void start() {
        boost::array<char, 128> message = {"[SERVER]\nReceived new connection.\n"};
        
        fmt::print("[INFO {}] Sent this message: \"{}\"\n", boost::this_thread::get_id(), std::string_view(message.data(), message.size()));
        socket.async_write_some(
            boost::asio::buffer(message),
            boost::bind(&tcp_connection::handle_write, shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));

        socket.async_read_some(
            boost::asio::buffer(received_message),
            boost::bind(&tcp_connection::handle_read, shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }

    ~tcp_connection() { 
        fmt::print("[INFO] Disconnected.\n"); 
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
            fmt::print("[INFO {}] Received a connection.\n",
                       boost::this_thread::get_id());
        }
        start_accept();
    }

  public:
    tcp_server(boost::asio::io_context &io)
        : io_context(io), acceptor(io, tcp::endpoint(tcp::v4(), 8000)) {
        start_accept();
        fmt::print("[INFO {}] Started the server.\n",
                   boost::this_thread::get_id());
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
