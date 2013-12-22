/**
 * @file connection.cpp
 * @brief TCP connection.
 * @author Paolo D'Apice
 */

#include "connection.hpp"
#include "logging.hpp"
#include "protocol_serialization.hpp"

#include <iostream>

namespace vis {

#define _LOG(X) CLOG(X, "connection")

Connection::Connection(boost::asio::ip::tcp::socket socket)
    : socket(std::move(socket)) {}

void
Connection::start() {
    doRead();
}

void
Connection::doRead() {
    auto self(shared_from_this());
    _LOG(INFO) << "Reading request data ...";

    boost::asio::async_read(socket, boost::asio::buffer(&header, sizeof(header)),
        [this, self](boost::system::error_code, size_t) {
            boost::asio::async_read(socket, buf.prepare(header),
                [this, self](boost::system::error_code, size_t len) {
                    buf.commit(header);
                    _LOG(INFO) << "read: " << len + sizeof(header) << " bytes";

                    BaseRequest* request;
                    get(buf, request);

                    doProcess(request);
                }
            );
        }
    );
}

void
Connection::doProcess(const BaseRequest* request) {
    _LOG(INFO) << "request: " << *request;

    Response response;
    // TODO process request here
    _LOG(INFO) << "response: " << response;

    doWrite(response);
    delete request; // XXX
}

void
Connection::doWrite(const Response& resp) {
    auto self(shared_from_this());

    put(buf, resp);
    header = buf.size();

    std::array<boost::asio::const_buffer,2> buffers = {
        boost::asio::buffer(&header, sizeof(header)),
        buf.data()
    };

    _LOG(INFO) << "Writing response ...";
    boost::asio::async_write(socket, buffers,
        [this, self](boost::system::error_code, size_t len) {
            buf.consume(len);
            _LOG(INFO) << "written: " << len << " bytes";
        }
    );
}

} //namespace vis

