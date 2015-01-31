#ifndef VIS_CLIENT_HPP
#define VIS_CLIENT_HPP

#include "protocol.hpp"

#include <boost/asio.hpp>
#include <glog/logging.h>
#include <iostream>

namespace vis {

static const int maxLength = 1024;

class Client {
public:
    Client(const std::string& h, const std::string& p)
        : host(h), port(p), socket(io_service) {}

    virtual ~Client() {}

    bool probe() {
        LOG(INFO) << "Probing " << host << ":" << port << " ...";
        try {
            boost::asio::ip::tcp::resolver resolver(io_service);
            boost::asio::connect(socket, resolver.resolve({host,port}));
            LOG(INFO) << "Probe OK";
            return true;
        } catch (std::exception& e) {
            LOG(WARNING) << "Probe ERROR: " << e.what();
            return false;
        }
    }

    Response sendRequest(const Request& request) {
        LOG(INFO) << "Sending request [" << request << "] ...";
        {
            put(buf, request);
            header = buf.size();

            std::array<boost::asio::const_buffer,2> buffers = {
                boost::asio::buffer(&header, sizeof(header)),
                buf.data()
            };

            const size_t n = boost::asio::write(socket, buffers);
            buf.consume(n);
            LOG(INFO) << "Request sent " << n << " bytes";
        }

        LOG(INFO) << "Waiting for response ...";
        {
            size_t n = boost::asio::read(socket, boost::asio::buffer(&header, sizeof(header)));

            boost::system::error_code ec;
            n += boost::asio::read(socket, buf.prepare(header), ec);
            buf.commit(header);
            LOG(INFO) << "Response received " << n << " bytes";

            Response response;
            get(buf, response);

            return response;
        }
    }

private:
    const std::string host;
    const std::string port;

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket;

    size_t header;
    boost::asio::streambuf buf;
};

} // namespace vis

#endif // VIS_CLIENT_HPP

