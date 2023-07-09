//#include <boost/array.hpp>
//#include <boost/asio.hpp>
//#include <boost/asio/connect.hpp>
//#include <boost/asio/io_context.hpp>
//#include <boost/asio/placeholders.hpp>
//#include <boost/bind/bind.hpp>
//#include <boost/system/system_error.hpp>
//#include <boost/thread/thread.hpp>
//#include <fmt/format.h>
//#include <fmt/ostream.h>
//#include <iostream>
//#include "../commmon-src/common_lib.hpp"
#include "view.hpp"

i32 main() {
    View view;
    view.loop();
}

// i32 main2(i32 argc, char **argv) {
//     if (argc != 2) {
//         fmt::print("Usage:\n"
//                    "   client <host>\n");
//         return 1;
//     }
//     boost::asio::io_context io_ctx;
// 
//     tcp::resolver resolver(io_ctx);
// 
//     tcp::socket socket(io_ctx);
// 
//     try {
//         tcp::resolver::results_type endpoints =
//             resolver.resolve(argv[1], "8000");
//         boost::asio::connect(socket, endpoints);
//     } catch (boost::system::system_error &err) {
//         fmt::print(stderr, "Failed to set up connection: {}\n", err.what());
//         exit(2);
//     }
// 
//     std::string str;
//     std::cin >> str;
//     socket.write_some(boost::asio::buffer(str));
// 
//     boost::array<char, 128> buf;
//     boost::system::error_code error;
//     u64 len = 0;
//     try {
//         len = socket.read_some(boost::asio::buffer(buf), error);
// 
//         if (error == boost::asio::error::eof) {
//         } else if (error) {
//             throw boost::system::system_error(error);
//         }
//     } catch (boost::system::system_error &err) {
//         fmt::print(stderr, "Failed to read data: {}", err.what());
//         exit(3);
//     }
//     fmt::print("Received this message: {}",
//                std::string_view(buf.data(), len));
// 
//     return 0;
// }
