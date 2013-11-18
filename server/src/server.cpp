/**
 * @file server.cpp
 * @brief TCP server service.
 * @author Paolo D'Apice
 */

#include "server.hpp"
#include "connection.hpp"
#include <boost/bind.hpp>
#include <iostream>

namespace vis {

Server::Server(short port)
    : acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      socket(io_service), signals(io_service) {
    std::cout << "New server on port " << port << std::endl;
    signals.add(SIGINT);
    signals.add(SIGTERM);
#if defined(SIGQUIT)
    signals.add(SIGQUIT);
#endif
    signals.async_wait([this](boost::system::error_code, int signal) {
        std::cout << "Received signal " << signal << std::endl;
        stop();
    });
}

Server::~Server() {
    if (running) stop();
}

void
Server::start() {
    std::cout << "Starting server ...\n";
    doAccept();
    io.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service)));
    running = true;
    std::cout << "Server started\n";
}

void
Server::stop() {
    std::cout << "Stopping server ...\n";
    acceptor.close();
    io_service.stop();
    io->join();
    running = false;
    std::cout << "Server stopped\n";
}

void
Server::doAccept() {
    std::cout << "Waiting for requests\n";
    acceptor.async_accept(socket, [this](boost::system::error_code ec) {
        if (not ec) {
            std::cout << "Incoming request\n";
            std::make_shared<Connection>(std::move(socket))->start();
        }

        doAccept();
    });
}

} // namespace vis

