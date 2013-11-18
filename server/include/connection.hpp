/**
 * @file connection.hpp
 * @brief TCP connection.
 * @author Paolo D'Apice
 */

#ifndef VIS_CONNECTION_HPP
#define VIS_CONNECTION_HPP

#include <boost/asio.hpp>
#include <memory>

namespace vis {

/// @brief TCP connection.
class Connection : public std::enable_shared_from_this<Connection> {
public:
    /// Create a new connection on the \c socket.
    Connection(boost::asio::ip::tcp::socket socket);

    /// Start processing the connection.
    void start();

private:
    /// Read data from socket.
    void doRead();

    /// Write data to socket.
    void doWrite(std::size_t length);

private:
    boost::asio::ip::tcp::socket socket;
    char buffer[1024];
};

} //namespace vis

#endif // VIS_CONNECTION_HPP

