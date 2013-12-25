/**
 * @file connection.hpp
 * @brief TCP connection.
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

/// @brief TCP connection.
class Connection
        : public std::enable_shared_from_this<Connection>,
          private boost::noncopyable {
public:
    /// Create a new connection on the \c socket.
    Connection(boost::asio::ip::tcp::socket socket);

    /// Start processing the connection.
    void start();

private:
    /// Read request from socket.
    void doRead();

    /// Process request.
    void doProcess(const BaseRequest*); // TODO request should be attribute

    /// Write response to socket.
    void doWrite(const Response&); // TODO response should be attribute

private:
    boost::asio::ip::tcp::socket socket;
    size_t header;
    boost::asio::streambuf buf;
};

} // namespace server
} // namespace vis

#endif // VIS_CONNECTION_HPP

