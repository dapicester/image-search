/**
 * @file connection.hpp
 * @brief Image search connection.
 * @author Paolo D'Apice
 */

#ifndef VIS_CONNECTION_HPP
#define VIS_CONNECTION_HPP

#include "protocol.hpp"

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <memory>

namespace vis {
namespace server {

class ConnectionManager;
class RequestHandler;

/// @brief A single TCP connection from a client.
class Connection
        : public std::enable_shared_from_this<Connection>,
          private boost::noncopyable {
public:
    /// Create a new connection on the \c socket.
    Connection(boost::asio::ip::tcp::socket socket,
            ConnectionManager& manager, RequestHandler& handler);

    /// Start processing the connection.
    void start();

    /// Stop operations associated with the connection.
    void stop();

private:
    /// Read request from socket.
    void doRead();

    /// Write response to socket.
    void doWrite();

private:
    boost::asio::ip::tcp::socket socket;
    ConnectionManager& manager;
    RequestHandler& handler;

    size_t header;
    boost::asio::streambuf buf;
    vis::Response response;
};

typedef std::shared_ptr<Connection> ConnectionPtr;

} // namespace server
} // namespace vis

#endif // VIS_CONNECTION_HPP

