#include "tcp_server.hpp"

i32 main() {
    boost::asio::thread_pool thread_pool;
    tcp_server server(thread_pool);
    thread_pool.join();
}
