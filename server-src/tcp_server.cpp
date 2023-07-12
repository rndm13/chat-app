#include "tcp_server.hpp"

void tcp_server::start_accept() {
    waiting_accept_connection =
        tcp_connection::create(thread_pool, mutex, new_messages, to_process);

    acceptor.async_accept(waiting_accept_connection->get_socket(),
                          boost::bind(&tcp_server::handle_accept, this,
                                      boost::asio::placeholders::error));
}

void tcp_server::handle_accept(const boost::system::error_code &error) {
    if (!error) {
        fmt::print("[INFO {}] Received a connection.\n",
                   boost::this_thread::get_id());

        active_connections.push_back(waiting_accept_connection);

        boost::asio::dispatch(
            thread_pool,
            boost::bind(&tcp_connection::start, waiting_accept_connection));
    }
    start_accept();
}

void tcp_server::send_to_all(const Message &msg) {
    for (auto &conn : active_connections) {
        if (conn.expired()) {
            std::swap(conn, active_connections.back());
            active_connections.pop_back();
        }
    }

    for (auto &conn : active_connections) {
        boost::shared_ptr(conn)->send_message(msg);
    }
}

void tcp_server::handle_messages() {
    for (;;) {
        boost::unique_lock lock(mutex);
        new_messages.wait(lock);

        while (!to_process.empty()) {
            send_to_all(to_process.front());
            to_process.pop();
        }
    }
}
