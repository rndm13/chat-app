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
        fmt::print("[INFO {}] Connection with {} ended.\n",
                   boost::this_thread::get_id(), user_name);

        return;
    } else if (error) {
        fmt::print("[ERROR {}] Error when receiving message: \"{}\"\n",
                   boost::this_thread::get_id(), error.what());
        return;
    }

    Message msg(received_message, user_name);

    send_message(msg);

    socket.async_read_some(
        boost::asio::buffer(received_message),
        boost::bind(&tcp_connection::handle_read, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void tcp_connection::handle_join(const boost::system::error_code &error,
                                 u64 bytes_transferred) {

    if (!error) {
        Message msg(fmt::format("User {} has connected.", user_name), "SERVER");

        send_message(msg);

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

void tcp_connection::send_message(const Message &msg) {
    try {
        socket.async_write_some(
            boost::asio::buffer(msg.to_string()),
            boost::bind(&tcp_connection::handle_write, shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    } catch (std::exception &err) {
        fmt::print("[ERROR] Failed to send message.\n");
    }
}
