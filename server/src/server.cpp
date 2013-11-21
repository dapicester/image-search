/**
 * @file server.cpp
 * @brief TCP server service.
 * @author Paolo D'Apice
 */

#include "server.hpp"
#include "connection.hpp"
#include "logging.hpp"

#include <boost/bind.hpp>
#include <iostream>

namespace vis {

#define _LOG(X) CLOG(X, "server")

Server::Server(short port)
    : acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      socket(io_service), signals(io_service) {
    _LOG(INFO) << "New server on port " << port;
    signals.add(SIGINT);
    signals.add(SIGTERM);
#if defined(SIGQUIT)
    signals.add(SIGQUIT);
#endif
    signals.async_wait([this](boost::system::error_code, int signal) {
        _LOG(INFO) << "Received signal " << signal;
        stop();
    });
}

Server::~Server() {
    if (running) stop();
}

void
Server::start() {
    _LOG(INFO) << "Starting server ...";
    doAccept();
    io.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service)));
    running = true;
    _LOG(INFO) << "Server started";
}

void
Server::stop() {
    _LOG(INFO) << "Stopping server ...";
    acceptor.close();
    io_service.stop();
    io->join();
    running = false;
    _LOG(INFO) << "Server stopped";
}

void
Server::doAccept() {
    _LOG(INFO) << "Waiting for requests";
    acceptor.async_accept(socket, [this](boost::system::error_code ec) {
        if (not ec) {
            _LOG(INFO) << "Incoming request from " << socket.remote_endpoint();
            std::make_shared<Connection>(std::move(socket))->start();
        }

        doAccept();
    });
}

} // namespace vis

