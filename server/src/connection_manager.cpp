/**
 * @file connection_manager.cpp
 * @brief Image search connection manager/
 * @author Paolo D'Apice
 */

#include "connection_manager.hpp"
#include <glog/logging.h>

namespace vis {
namespace server {

ConnectionManager::ConnectionManager() {}

void
ConnectionManager::start(ConnectionPtr con) {
    LOG(INFO) << "starting connection";
    connections.insert(con);
    con->start();
}

void
ConnectionManager::stop(ConnectionPtr con) {
    LOG(INFO) << "stopping connection";
    connections.erase(con);
    con->stop();
}

void
ConnectionManager::stopAll() {
    LOG(INFO) << "stopping all connections";
    for (auto con : connections) {
        con->stop();
    }
    connections.clear();
}

} // namespace server
} // namespace vis

