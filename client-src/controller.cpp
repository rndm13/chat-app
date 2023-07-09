#include "controller.hpp"
#include <boost/asio/detail/chrono.hpp>

void Controller::wait(const boost::asio::chrono::duration<i64> &dur) {
    boost::asio::steady_timer timer(thr_pool.get_executor(), dur);
    timer.wait();
    fmt::print("[INFO {}] Waited out 1 second.\n",
               boost::this_thread::get_id());
}
