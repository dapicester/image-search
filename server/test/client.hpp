#ifndef VIS_CLIENT_HPP
#define VIS_CLIENT_HPP

#include "logging.hpp"
#include "protocol_serialization.hpp"

#include <boost/asio.hpp>
#include <iostream>

namespace vis {

static const int maxLength = 1024;

#define _LOG(X) CLOG(X, "client")

class Client {
public:
    Client(const std::string& h, short p)
        : host(h), port(p), socket(io_service) {}

    virtual ~Client() {}

    bool probe() {
        _LOG(INFO) << "Probing " << host << ":" << port << " ...";
        try {
            boost::asio::ip::tcp::resolver resolver(io_service);
            boost::asio::connect(socket, resolver.resolve({host,std::to_string(port)}));
            _LOG(INFO) << "Probe OK";
            return true;
        } catch (std::exception& e) {
            _LOG(WARNING) << "Probe ERROR: " << e.what();
            return false;
        }
    }

    Response sendRequest(const BaseRequest* request) {
        _LOG(INFO) << "Sending request [" << *request << "] ...";
        {
            put(buf, request);
            header = buf.size();

            std::array<boost::asio::const_buffer,2> buffers = {
                boost::asio::buffer(&header, sizeof(header)),
                buf.data()
            };

            const size_t n = boost::asio::write(socket, buffers);
            buf.consume(n);
            _LOG(INFO) << "Request sent " << n << " bytes";
        }

        _LOG(INFO) << "Receiving response ...";
        {
            size_t n = boost::asio::read(socket, boost::asio::buffer(&header, sizeof(header)));

            boost::system::error_code ec;
            n += boost::asio::read(socket, buf.prepare(header), ec);
            buf.commit(header);
            _LOG(INFO) << "Response received " << n << " bytes";

            Response response;
            get(buf, response);

            return response;
        }
    }

private:
    const std::string host;
    const short port;

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket;

    size_t header;
    boost::asio::streambuf buf;
};

} // namespace vis

#endif // VIS_CLIENT_HPP

