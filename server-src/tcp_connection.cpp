#include "tcp_connection.hpp"
#include "fmt/format.h"

void tcp_connection::handle_write(const boost::system::error_code &error,
                                  u64 bytes_transferred) {

    if (error) {
        fmt::print("[ERROR {}] Failed to send a write: \"{}\" \n",
                   boost::this_thread::get_id(), error.what());
        return;
    }

    fmt::print("[INFO {}] Sent a message to {}\n", boost::this_thread::get_id(),
               user_name);
}

void tcp_connection::handle_read(const boost::system::error_code &error,
                                 u64 bytes_transferred) {

    if (error == boost::asio::error::eof) {
        fmt::print("[INFO {}] Connection with {} ended.\n",
                   boost::this_thread::get_id(), user_name);

        push_message({fmt::format("{} disconnected.", fmt::to_string(user_name)), "SERVER"});

        return;
    } else if (error) {
        fmt::print("[ERROR {}] Error when receiving message: \"{}\"\n",
                   boost::this_thread::get_id(), error.what());
        return;
    }

    push_message({received_message, user_name});

    socket.async_read_some(
        boost::asio::buffer(received_message),
        boost::bind(&tcp_connection::handle_read, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void tcp_connection::handle_join(const boost::system::error_code &error,
                                 u64 bytes_transferred) {

    if (!error) {
        push_message({fmt::format("Connection from {}.", fmt::to_string(user_name)), "SERVER"});


        socket.async_read_some(
            boost::asio::buffer(received_message),
            boost::bind(&tcp_connection::handle_read, shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }
}

void tcp_connection::start() {
    socket.async_read_some(
        boost::asio::buffer(user_name),
        boost::bind(&tcp_connection::handle_join, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void tcp_connection::push_message(const Message &msg) {
    boost::unique_lock<boost::mutex> lock(mutex);

    to_process.push(msg);
    new_messages.notify_one();
}

void tcp_connection::send_message(const Message &msg) {
    socket.async_write_some(
        boost::asio::buffer(msg.to_string()),
        boost::bind(&tcp_connection::handle_write, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}


