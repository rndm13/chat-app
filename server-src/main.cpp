#include "tcp_server.hpp"

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
