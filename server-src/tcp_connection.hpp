#pragma once
#include "../commmon-src/common_lib.hpp"
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr/make_shared_array.hpp>
#include <boost/system/system_error.hpp>
#include <boost/thread.hpp>
#include <boost/thread/detail/thread.hpp>
#include <fmt/format.h>
#include <fmt/std.h>
#include <iostream>
#include <memory_resource>
#include <queue>
#include <string>
#include <string_view>
#include <thread>

class tcp_connection : public boost::enable_shared_from_this<tcp_connection> {
    tcp::socket socket;
    boost::array<char, 256> received_message;
    boost::array<char, 64> user_name;

    void handle_write(const boost::system::error_code &error,
                      u64 bytes_transferred);

    void handle_read(const boost::system::error_code &error,
                     u64 bytes_transferred);

    void handle_join(const boost::system::error_code &error,
                     u64 bytes_transferred);

    // references to members of tcp_server
    boost::mutex &mutex;
    boost::condition_variable &new_messages;

    std::queue<Message> &to_process;

  public:
    typedef boost::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::thread_pool &_thread_pool,
                          boost::mutex &_mutex,
                          boost::condition_variable &_cond,
                          std::queue<Message> &_to_process) {
        return boost::make_shared<tcp_connection>(_thread_pool, _mutex, _cond,
                                                  _to_process);
    }

    tcp::socket &get_socket() { return socket; }

    void start();

    ~tcp_connection() {
        fmt::print("[INFO {}] Destroyed connection.\n",
                   boost::this_thread::get_id());
    }

    void push_message(const Message &msg);
    void send_message(const Message &msg);

    tcp_connection(boost::asio::thread_pool &_thread_pool, boost::mutex &_mutex,
                   boost::condition_variable &_cond,
                   std::queue<Message> &_to_process)
        : socket(_thread_pool.get_executor()), mutex(_mutex),
          new_messages(_cond), to_process(_to_process) {
        fmt::print("[INFO {}] Created a new connection.\n",
                   boost::this_thread::get_id());
    }
};
