/**
 * @file server.cpp
 * @brief TCP server service.
 * @author Paolo D'Apice
 */

#include "server.hpp"
#include "connection.hpp"
#include "logging.hpp"

#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>

namespace vis {

#define _LOG(X) CLOG(X, "server")

Server::Server(const std::string& address, const std::string& port)
    : io_service(),
      signals(io_service),
      acceptor(io_service),
      socket(io_service)
{
    _LOG(INFO) << "New server on port " << port << " ...";
    signals.add(SIGINT);
    signals.add(SIGTERM);
#if defined(SIGQUIT)
    signals.add(SIGQUIT);
#endif

    doAwaitStop();

    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({address, port});
    acceptor.open(endpoint.protocol());
    acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor.bind(endpoint);
    acceptor.listen();

    doAccept();
}

Server::~Server() {}

void
Server::doAwaitStop() {
    signals.async_wait([this](boost::system::error_code, int signal) {
        _LOG(INFO) << "Received signal " << signal;
        stop();
    });
}

void
Server::start() {
    _LOG(INFO) << "Server started";
    io_service.run();
}

void
Server::startAsync() {
    _LOG(INFO) << "Server started (async)";
    service_thread.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service)));
}

void
Server::stop() {
    _LOG(INFO) << "Stopping server ...";
    acceptor.close();
    io_service.stop();
    if (service_thread.get() != 0) service_thread->join();
    _LOG(INFO) << "Server stopped";
}

void
Server::doAccept() {
    _LOG(INFO) << "Waiting for requests";
    acceptor.async_accept(socket, [this](boost::system::error_code ec) {
        if (not acceptor.is_open()) {
            // acceptors has been stopped
            return;
        }

        if (not ec) {
            _LOG(INFO) << "Incoming request from " << socket.remote_endpoint();
            // TODO connection manager
            std::make_shared<Connection>(std::move(socket))->start();
        }

        doAccept();
    });
}

} // namespace vis

