/**
 * @file connection.cpp
 * @brief TCP connection.
 * @author Paolo D'Apice
 */

#include "connection.hpp"
#include "logging.hpp"

#include <iostream>

namespace vis {

#define _LOG(X) CLOG(X, "connection")

Connection::Connection(boost::asio::ip::tcp::socket socket)
    : socket(std::move(socket)) {}

void
Connection::start() {
    doRead();
}

void
Connection::doRead() {
    auto self(shared_from_this());
    _LOG(INFO) << "Reading request data ...";
    socket.async_read_some(boost::asio::buffer(buffer),
        [this, self](boost::system::error_code ec, std::size_t length) {
            if (not ec) {
                _LOG(INFO) << "Read " << length;
                doWrite(length);
            }
        });
}

void
Connection::doWrite(std::size_t length) {
    auto self(shared_from_this());
    _LOG(INFO) << "Writing echo back ...";
    boost::asio::async_write(socket, boost::asio::buffer(buffer, length),
        [this, self](boost::system::error_code ec, std::size_t length) {
            if(not ec) {
                _LOG(INFO) << "Written " << length;
                //doRead();
            }
        });
}

} //namespace vis

