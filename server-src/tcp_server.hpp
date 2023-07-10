#pragma once
#include "tcp_connection.hpp"

class tcp_server {
    boost::asio::io_context &io_context;
    tcp::acceptor acceptor;

    void start_accept();

    void handle_accept(tcp_connection::pointer new_connection,
                       const boost::system::error_code &error);

  public:
    tcp_server(boost::asio::io_context &io)
        : io_context(io), acceptor(io, tcp::endpoint(tcp::v4(), 8000)) {
        start_accept();
        fmt::print("[INFO {}] Started the server.\n",
                   boost::this_thread::get_id());
    }
};
