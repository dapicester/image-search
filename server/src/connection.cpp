/**
 * @file connection.cpp
 * @brief TCP connection.
 * @author Paolo D'Apice
 */

#include "connection.hpp"
#include "connection_manager.hpp"
#include "protocol_serialization.hpp"
#include "request_handler.hpp"

#include <glog/logging.h>
#include <iostream>

namespace vis {
namespace server {

Connection::Connection(boost::asio::ip::tcp::socket socket,
        ConnectionManager& mgr, RequestHandler& h)
    : socket(std::move(socket)),
      manager(mgr), handler(h) {}

void
Connection::start() {
    doRead();
}

void
Connection::stop() {
    socket.close();
    LOG(INFO) << "Connection closed";
}

void
Connection::doRead() {
    auto self(shared_from_this());
    LOG(INFO) << "Reading request data ...";

    boost::asio::async_read(socket, boost::asio::buffer(&header, sizeof(header)),
        [this, self](boost::system::error_code ec, size_t transferred) {
            LOG(INFO) << "transferred: " << transferred << " bytes";
            if (not ec) {
                boost::asio::async_read(socket, buf.prepare(header),
                    [this, self](boost::system::error_code, size_t len) {
                        buf.commit(header);
                        LOG(INFO) << "read: " << len + sizeof(header) << " bytes";

                        BaseRequest* request;
                        get(buf, request);

                        // TODO handle parse errors
                        handler.handle(*request, response);
                        delete request;

                        doWrite();
                    }
                );
            } else if (ec != boost::asio::error::operation_aborted) {
                manager.stop(shared_from_this());
            }
        }
    );
}

void
Connection::doWrite() {
    auto self(shared_from_this());

    put(buf, response);
    header = buf.size();

    std::array<boost::asio::const_buffer,2> buffers = {
        boost::asio::buffer(&header, sizeof(header)),
        buf.data()
    };

    LOG(INFO) << "Writing response ...";
    boost::asio::async_write(socket, buffers,
        [this, self](boost::system::error_code ec, size_t len) {
            if (not ec) {
                buf.consume(len);
                LOG(INFO) << "written: " << len << " bytes";

                boost::system::error_code ignored;
                socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored);
            }

            if (ec != boost::asio::error::operation_aborted) {
                manager.stop(shared_from_this());
            }
        }
    );
}

} //namespace server
} //namespace vis

