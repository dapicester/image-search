/**
 * @file connection_manager.cpp
 * @brief Image search connection manager/
 * @author Paolo D'Apice
 */

#include "connection_manager.hpp"
#include "logging.hpp"

#define _LOG(X) CLOG(X, "manager")

namespace vis {
namespace server {

ConnectionManager::ConnectionManager() {}

void
ConnectionManager::start(ConnectionPtr con) {
    _LOG(INFO) << "starting connection";
    connections.insert(con);
    con->start();
}

void
ConnectionManager::stop(ConnectionPtr con) {
    _LOG(INFO) << "stopping connection";
    connections.erase(con);
    con->stop();
}

void
ConnectionManager::stopAll() {
    _LOG(INFO) << "stopping all connections";
    for (auto con : connections) {
        con->stop();
    }
    connections.clear();
}

} // namespace server
} // namespace vis

