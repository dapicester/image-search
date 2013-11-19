#ifndef VIS_CLIENT_HPP
#define VIS_CLIENT_HPP

#include "logging.hpp"

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

    std::string sendRequest(const std::string& request) {
        _LOG(INFO) << "Sending request [" << request << "] ...";
        boost::asio::write(socket,
                boost::asio::buffer(request.data(), request.length()));
        _LOG(INFO) << "Request sent";

        _LOG(INFO) << "Receiving response ...";
        char response[1024];
        boost::system::error_code ec;
        size_t length = boost::asio::read(socket,
                boost::asio::buffer(response), boost::asio::transfer_all(), ec);
        /*
         *if (ec) {
         *    _LOG(ERROR) << ec.message();
         *}
         */

        _LOG(INFO) << "Response: " << length << " bytes received";
        return std::string(response, length);
    }

private:
    const std::string host;
    const short port;

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket;
};

} // namespace vis

#endif // VIS_CLIENT_HPP

