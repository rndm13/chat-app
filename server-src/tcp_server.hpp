#pragma once
#include "tcp_connection.hpp"

class tcp_server {
    boost::asio::thread_pool &thread_pool;
    tcp::acceptor acceptor;

    tcp_connection::pointer waiting_accept_connection;
    std::vector<boost::weak_ptr<tcp_connection>> active_connections;

    boost::mutex mutex;
    boost::condition_variable new_messages;

    std::queue<Message> to_process;

    void start_accept();

    void handle_accept(const boost::system::error_code &error);

    void handle_messages();

  public:
    void send_to_all(const Message &);

    tcp_server(boost::asio::thread_pool &_thread_pool)
        : thread_pool(_thread_pool),
          acceptor(_thread_pool, tcp::endpoint(tcp::v4(), 8000)) {
        fmt::print("[INFO {}] Started the server.\n",
                   boost::this_thread::get_id());

        boost::asio::dispatch(thread_pool,
                              boost::bind(&tcp_server::start_accept, this));

        handle_messages();
    }
};
