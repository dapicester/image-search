/**
 * @file main.cpp
 * @brief Entry point for the image-search server.
 * @author Paolo D'Apice
 */

#include "server.hpp"
#include <vis/configuration.hpp>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <glog/logging.h>
#include <iostream>

namespace fs = boost::filesystem;
namespace po = boost::program_options;

static const std::string DEFAULT_ADDRESS = "0.0.0.0";
static const std::string DEFAULT_PORT    = "4567";

/// @brief Logging initialization structure.
struct InitGlog {
    InitGlog() {
        google::InitGoogleLogging("vis_server");
    }
    ~InitGlog() {
        google::ShutdownGoogleLogging();
    }
};

int main(int argc, char** argv) {
    try {
        //InitGlog logging;

        // TODO move to function

        po::options_description genericOptions("Generic options");
        genericOptions.add_options()
            ("help,h", "Print help message")
            ("file,f",
                po::value<std::string>(),
                "Configuration file")
            ;

        std::string address, port, dataDir;

        po::options_description serverOptions("Server options");
        serverOptions.add_options()
            ("address,a",
                 po::value<std::string>(&address)->default_value(DEFAULT_ADDRESS),
                "Bind address\n"
                "Supports both IPv4 (e.g.: 0.0.0.0)\n"
                "and IPv6 (e.g.: 0::0)")
            ("port,p",
                 po::value<std::string>(&port)->default_value(DEFAULT_PORT),
                "Bind port")
            ;

        po::options_description cmdlineOptions;
        cmdlineOptions.add(genericOptions).add(serverOptions);

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, cmdlineOptions), vm);
        po::notify(vm);

        if (vm.empty() or vm.count("help")) {
            std::cout << cmdlineOptions << std::endl;
            return 1;
        }

        if (!vm.count("file")) {
            std::cerr << "Missing required options" << std::endl;
            std::cout << cmdlineOptions << std::endl;
            return 1;
        }

        std::string file = vm["file"].as<std::string>();
        if (not fs::is_regular_file(file)) {
            std::cerr << "cannot read file: " << file << std::endl;
            return 1;
        }

        vis::config::Configuration conf = vis::config::loadConfiguration(file);

        vis::server::Server server(address, port, conf);
        server.start();

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

