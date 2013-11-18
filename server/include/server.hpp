/**
 * @file server.hpp
 * @brief TCP server service.
 * @author Paolo D'Apice
 */

#ifndef VIS_SERVER_HPP
#define VIS_SERVER_HPP

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/scoped_ptr.hpp>

namespace vis {

/// @brief TCP asynchronous server.
class Server {
public:
    /// Create a new server binding port \c port.
    Server(short port);

    virtual ~Server();

    /// Start accepting requests.
    void start();

    /// Stop accepting requests.
    void stop();

    /// @return \c true if the server is running.
    bool isRunning() const { return running; }

private:
    void doAccept();

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::socket socket;
    boost::asio::signal_set signals;

    bool running = false;
    boost::scoped_ptr<boost::thread> io;
};

} // namespace vis

#endif // VIS_SERVER_HPP

