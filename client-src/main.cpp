#include "view.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/thread_pool.hpp>

i32 main() {
    boost::asio::thread_pool thr_pool;
    View view(thr_pool);
    view.loop();
}

