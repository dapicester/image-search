/**
 * @file connection.cpp
 * @brief TCP connection.
 * @author Paolo D'Apice
 */

#include "connection.hpp"
#include <iostream>

namespace vis {

Connection::Connection(boost::asio::ip::tcp::socket socket)
    : socket(std::move(socket)) {}

void
Connection::start() {
    doRead();
}

void
Connection::doRead() {
    auto self(shared_from_this());
    //std::cerr << "* reading request data ...\n";
    socket.async_read_some(boost::asio::buffer(buffer),
        [this, self](boost::system::error_code ec, std::size_t length) {
            if (not ec) {
                //std::cerr << "* read " << length << "\n";
                doWrite(length);
            }
        });
}

void
Connection::doWrite(std::size_t length) {
    auto self(shared_from_this());
    //std::cerr << "* writing echo back ...\n";
    boost::asio::async_write(socket, boost::asio::buffer(buffer.data(), length),
        [this, self](boost::system::error_code ec, std::size_t length) {
            if(not ec) {
                //std::cerr << "* written " << length << "\n";
                //doRead();
            }
        });
}

} //namespace vis

