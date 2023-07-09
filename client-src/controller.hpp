#pragma once

#include "../commmon-src/common_lib.hpp"
#include "model.hpp"
#include <boost/asio/dispatch.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

struct Controller : public boost::enable_shared_from_this<Controller> {
  private:
    boost::asio::thread_pool &thr_pool;

    tcp::socket socket;
    tcp::resolver::results_type endpoints;

    std::atomic<Model> model;

  public:
    typedef boost::shared_ptr<Controller> pointer;
    static pointer create(boost::asio::thread_pool &thr_pool) {
        return pointer(new Controller(thr_pool));
    }

    Controller(boost::asio::thread_pool &_thr_pool)
        : thr_pool(_thr_pool), socket(thr_pool.get_executor()) {}

    template <class MemberFunc, class... Args>
    void do_member(MemberFunc &&to_do, Args &&...args) {
        boost::asio::dispatch(thr_pool,
                              boost::bind(to_do, shared_from_this(), args...));
    }

    void wait(const boost::asio::chrono::duration<i64> &dur);
};
