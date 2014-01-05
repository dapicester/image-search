/**
 * @file main.cpp
 * @brief Entry point for the image-search server.
 * @author Paolo D'Apice
 */

#include "server.hpp"
#include "logging.hpp"

#include <iostream>

_INITIALIZE_EASYLOGGINGPP

void usage() {
    std::cerr << "Usage: server <address> <port> <datadir>\n";
    std::cerr << "  For IPv4, try:\n";
    std::cerr << "    server 0.0.0.0 12345 .\n";
    std::cerr << "  For IPv6, try:\n";
    std::cerr << "    server 0::0 12345 .\n";
}

void init(int argc, char** argv) {
    _START_EASYLOGGINGPP(argc, argv);
    vis::logging::registerLoggers({"server", "connection", "handler", "manager"});
}

int main(int argc, char** argv) {
    try {
        if (argc != 4) {
            usage();
            return 1;
        }

        init(argc, argv);

        vis::server::Server server(argv[1], argv[2], argv[3]);
        server.start();

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

