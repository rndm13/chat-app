#include "tcp_server.hpp"
void tcp_server::start_accept() {
    tcp_connection::pointer new_connection = tcp_connection::create(io_context);
    acceptor.async_accept(new_connection->get_socket(),
                          boost::bind(&tcp_server::handle_accept, this,
                                      new_connection,
                                      boost::asio::placeholders::error));
}

void tcp_server::handle_accept(tcp_connection::pointer new_connection,
                               const boost::system::error_code &error) {
    if (!error) {
        new_connection->start();
        fmt::print("[INFO {}] Received a connection.\n",
                   boost::this_thread::get_id());
    }
    start_accept();
}

void tcp_server::disconnect(tcp_connection *conn) {
    std::erase_if(active_connections, [&conn](const auto &connect) {
        return connect.get() == conn;
    });
}

void tcp_server::send_to_all(const Message &msg) {
    for (auto &conn : active_connections) {
        conn->send_message(msg);
    }
}
