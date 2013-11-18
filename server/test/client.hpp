#ifndef VIS_CLIENT_HPP
#define VIS_CLIENT_HPP

#include <boost/asio.hpp>
#include <iostream>

namespace vis {

static const int maxLength = 1024;

class Client {
public:
    Client(const std::string& h, short p)
        : host(h), port(p), socket(io_service) {}

    virtual ~Client() {}

    bool probe() {
        std::cout << "Probing " << host << ":" << port << " ... ";
        try {
            boost::asio::ip::tcp::resolver resolver(io_service);
            boost::asio::connect(socket, resolver.resolve({host,std::to_string(port)}));
            std::cout << "OK\n";
            return true;
        } catch (std::exception& e) {
            std::cout << "ERROR: " << e.what() << std::endl;
            return false;
        }
    }

    std::string sendRequest(const std::string& request) {
        std::cout << "Sending request [" << request << "] ...";
        boost::asio::write(socket,
                boost::asio::buffer(request.data(), request.length()));
        std::cout << "done\n";

        std::cout << "Receiving response ... ";
        char response[1024];
        boost::system::error_code ec;
        size_t length = boost::asio::read(socket,
                boost::asio::buffer(response), boost::asio::transfer_all(), ec);
        /*
         *if (ec) {
         *    std::cout << "ERROR: " << ec.message() << "\n";
         *}
         */

        std::cout << length << " bytes received\n";
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

