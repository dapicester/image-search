/**
 * @file connection_manager.hpp
 * @brief Image search connection manager/
 * @author Paolo D'Apice
 */

#ifndef VIS_CONNECTION_MANAGER_HPP
#define VIS_CONNECTION_MANAGER_HPP

#include "connection.hpp"
#include <boost/noncopyable.hpp>
#include <set>

namespace vis {
namespace server {

/**
 * Manages open connections so that they may be cleanly stopped
 * when the server needs to shut down.
 */
class ConnectionManager : private boost::noncopyable {
public:
    /// Construct a connection manager.
    ConnectionManager();

    /// Add the specified connection to the manager and start it.
    void start(ConnectionPtr con);

    /// Stop the specified connection.
    void stop(ConnectionPtr con);

    /// Stop all connections.
    void stopAll();

private:
    /// The managed connections.
    std::set<ConnectionPtr> connections;
};

} // namespace server
} // namespace vis

#endif // VIS_CONNECTION_MANAGER_HPP

