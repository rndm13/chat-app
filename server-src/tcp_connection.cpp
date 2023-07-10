#include "tcp_connection.hpp"

void tcp_connection::handle_write(const boost::system::error_code &error,
                                  u64 bytes_transferred) {
    if (error) {
        fmt::print("[ERROR {}] Failed to sent a write: \"{}\" \n",
                   boost::this_thread::get_id(), error.what());
    }
}

void tcp_connection::handle_read(const boost::system::error_code &error,
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
    fmt::print("[INFO {}] Received a message: \"{}\"\n",
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

void tcp_connection::start() {
    boost::array<char, 128> message = {"[SERVER]\nReceived new connection.\n"};

    fmt::print("[INFO {}] Sent this message: \"{}\"\n",
               boost::this_thread::get_id(),
               std::string_view(message.data(), message.size()));
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
