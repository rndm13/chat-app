#include "controller.hpp"
#include <boost/asio/detail/chrono.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/read_at.hpp>
#include <boost/thread/interruption.hpp>
#include <boost/thread/lock_types.hpp>
#include <stdexcept>
#include <string_view>

const Model &Controller::get_model() const { return model; }

Controller::~Controller() {
    socket.shutdown(socket.shutdown_both);
    thr_pool.stop();
}

void Controller::read_incoming_messages() {
    socket.async_receive(
        boost::asio::buffer(last_read_message),
        boost::bind(&Controller::handle_read, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void Controller::handle_read(const boost::system::error_code &error,
                             u64 bytes_transfered) {
    boost::unique_lock<boost::mutex> lock(model_mtx);
    auto message = std::string_view(last_read_message.data(), bytes_transfered);
    if (!error) {
        try {
            model.messages.push_back(Message::from_message_string(message));
        } catch (std::runtime_error &re) {
            model.messages.emplace_back(
                fmt::format("Failed to parse message: {}.", message), "CLIENT");
        }
        read_incoming_messages();
    } else if (error != boost::asio::error::eof) {
        model.messages.emplace_back(
            fmt::format("Failed to read: {}.", error.message()), "CLIENT");
    } else {
        model.messages.emplace_back("Connection with the server was closed "
                                    "unexpectedly.",
                                    "CLIENT");
    }
}

void Controller::send_message(const boost::array<char, 256> &msg) {
    socket.async_write_some(
        boost::asio::buffer(msg),
        boost::bind(&Controller::handle_write, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void Controller::handle_write(const boost::system::error_code &error,
                              u64 bytes_transfered) {}

void Controller::handle_after_login(const boost::system::error_code &error) {
    if (!error) {
        read_incoming_messages();
        return;
    }
    boost::unique_lock<boost::mutex> lock(model_mtx);

    model.messages.emplace_back(
        fmt::format("Failed to authenticate: {}.", error.message()), "CLIENT");
}

void Controller::handle_connection(const boost::system::error_code &error,
                                   std::string_view user_name) {
    if (!error) {
        socket.async_write_some(boost::asio::buffer(user_name),
                                boost::bind(&Controller::handle_after_login,
                                            shared_from_this(),
                                            boost::asio::placeholders::error));
        return;
    }
    boost::unique_lock<boost::mutex> lock(model_mtx);

    model.messages.emplace_back(
        fmt::format("Failed to connect: {}.", error.message()), "CLIENT");
}

void Controller::connect_to(std::string_view host_name,
                            std::string_view user_name) {
    tcp::resolver resolver(thr_pool.get_executor());
    auto result = resolver.resolve(host_name, port_number);

    boost::asio::async_connect(socket, result,
                               boost::bind(&Controller::handle_connection,
                                           shared_from_this(),
                                           boost::asio::placeholders::error(),
                                           user_name));
}
