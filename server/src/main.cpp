/**
 * @file main.cpp
 * @brief Entry point for the image-search server.
 * @author Paolo D'Apice
 */

#include "server.hpp"
#include <iostream>
#include <cstdlib>

void usage() {
    std::cerr << "Usage: server <port>\n";
}

int main(int argc, char** argv) {
    try {
        if (argc != 2) {
            usage();
            return 1;
        }

        vis::Server server(std::atoi(argv[1]));
        server.start();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

