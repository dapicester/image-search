/**
 * @file server.cpp
 * @brief TCP server service.
 * @author Paolo D'Apice
 */

#include "server.hpp"
#include "connection.hpp"
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <iostream>

namespace vis {

Server::Server(short port)
    : acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      socket(io_service), signals(io_service) {
    std::cerr << "New server on port " << port << std::endl;
    signals.add(SIGINT);
    signals.add(SIGTERM);
#if defined(SIGQUIT)
    signals.add(SIGQUIT);
#endif
    signals.async_wait([this](boost::system::error_code, int signal) {
        std::cerr << "Received signal " << signal << std::endl;
        stop();
    });
}

Server::~Server() {
    if (running) stop();
}

void
Server::start() {
    std::cerr << "Starting server ... ";
    doAccept();
    boost::thread(boost::bind(&boost::asio::io_service::run, &io_service));
    running = true;
    std::cerr << "OK\n";
}

void
Server::stop() {
    std::cerr << "Stopping server ... ";
    acceptor.close();
    io_service.stop();
    running = false;
    std::cerr << "OK\n";
}

void
Server::doAccept() {
    std::cerr << "Waiting for requests\n";
    acceptor.async_accept(socket, [this](boost::system::error_code ec) {
        if (not ec) {
            std::cerr << "Incoming request\n";
            std::make_shared<Connection>(std::move(socket))->start();
        }

        doAccept();
    });
}

} // namespace vis

