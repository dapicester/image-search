/**
 * @file main.cpp
 * @brief Entry point for the image-search server.
 * @author Paolo D'Apice
 */

#include "server.hpp"
#include "logging.hpp"

#include <iostream>
#include <cstdlib>

_INITIALIZE_EASYLOGGINGPP

void usage() {
    std::cerr << "Usage: server <port>\n";
}

void init(int argc, char** argv) {
    _START_EASYLOGGINGPP(argc, argv);
    vis::registerLoggers({"server", "connection"});
}

int main(int argc, char** argv) {
    try {
        if (argc != 2) {
            usage();
            return 1;
        }

        init(argc, argv);

        vis::Server server(std::atoi(argv[1]));
        server.start();

        // TODO wait for completion
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

