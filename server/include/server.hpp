/**
 * @file server.hpp
 * @brief TCP server service.
 * @author Paolo D'Apice
 */

#ifndef VIS_SERVER_HPP
#define VIS_SERVER_HPP

#include "connection_manager.hpp"
#include "request_handler.hpp"

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace boost {
    class thread;
}

namespace vis {
namespace server {

/// @brief TCP asynchronous server.
class Server : private boost::noncopyable {
public:
    /// Create a new server binding port \c port.
    Server(const std::string& address, const std::string& port,
            const std::string& datadir, const std::vector<std::string>& categories);

    /// Destructor.
    ~Server();

    /// Start accepting requests (blocking call).
    void start();

    /// Start accepting requests (non-blocking call).
    void startAsync();

    /// Stop accepting requests.
    void stop();

private:

    void doAccept();

    void doAwaitStop();

    boost::asio::io_service io_service;
    boost::asio::signal_set signals;
    boost::asio::ip::tcp::acceptor acceptor;
    ConnectionManager connectionManager;
    boost::asio::ip::tcp::socket socket;
    RequestHandler requestHandler;

    boost::scoped_ptr<boost::thread> service_thread;
};

} // namespace server
} // namespace vis

#endif // VIS_SERVER_HPP

