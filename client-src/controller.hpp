#pragma once

#include "../common-src/common_lib.hpp"
#include "model.hpp"
#include <boost/asio/dispatch.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

struct Controller : public boost::enable_shared_from_this<Controller> {
  private:
    static constexpr std::string_view port_number = "8000";
    boost::asio::thread_pool &thr_pool;

    tcp::socket socket;

    mutable boost::mutex model_mtx;
    Model model;

    boost::array<char, 256 + 64 + 4> last_read_message{};

  public:
    typedef boost::shared_ptr<Controller> pointer;
    static pointer create(boost::asio::thread_pool &thr_pool) {
        return pointer(new Controller(thr_pool));
    }

    Controller(boost::asio::thread_pool &_thr_pool)
        : thr_pool(_thr_pool), socket(thr_pool.get_executor()) {}

    ~Controller();

    template <class MemberFunc, class... Args>
    auto do_member(MemberFunc &&to_do, Args &&...args) {
        return boost::asio::dispatch(
            thr_pool, boost::bind(to_do, shared_from_this(), args...));
    }

    const Model &get_model() const;

    void read_incoming_messages();
    void handle_read(const boost::system::error_code &, u64);
    void send_message(const boost::array<char, 256>&);
    void handle_write(const boost::system::error_code &error,
                      u64 bytes_transfered);
    void connect_to(std::string_view host_name, std::string_view user_name);
    void handle_connection(const boost::system::error_code &error,
                           std::string_view user_name);
    void handle_after_login(const boost::system::error_code &error);
};
