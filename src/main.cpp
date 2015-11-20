// Copyright 2015, Outernet Inc.
// Some rights reserved.
// This software is free software licensed under the terms of GPLv3. See COPYING
// file that comes with the source code, or http://www.gnu.org/licenses/gpl.txt.
#include <iostream>
#include <string>
#include <vector>

#include "sentinel/logging.h"
#include "sentinel/sentinel.h"


static const std::string APP_NAME = "sentinel";
static const int OPTION_COUNT = 4;
static const std::array<std::string, OPTION_COUNT> OPTIONS = {
    "conf",
    "watchdog",
    "checkinterval",
    "maxretry"
};


void print_usage() {
    std::cerr << "Usage: sentinel "
              << "[--conf CONFIG_PATH] "
              << "[--maxretry NUMBER] "
              << "[--checkinterval SECONDS] "
              << "[--watchdog WATCHDOG_DEVICE_PATH] "
              << std::endl;
}


bool parse_args(int argc, char* argv[], sentinel::conf_map* into) {
    for (int i = 1; i < argc; i += 2) {
        bool matched = false;
        for (auto it = OPTIONS.begin(); it != OPTIONS.end(); ++it) {
            if (std::string(argv[i]) == "--" + *it) {
                if (i + 1 < argc) {
                    (*into)[*it] = std::string(argv[i + 1]);
                    matched = true;
                } else {
                    std::cerr << "--" + *it + " option requires one argument."
                              << std::endl;
                    return false;
                }
            }
        }
        if (!matched) {
            print_usage();
            return false;
        }
    }
    return true;
}


int main(int argc, char* argv[]) {
    // prepare default options
    std::string conf_path = sentinel::DEFAULT_CONFIG_PATH;
    std::string watchdog_path;
    int check_interval = sentinel::DEFAULT_CHECK_INTERVAL;
    int max_retries = sentinel::DEFAULT_MAX_RETRIES;
    // parse command line args
    sentinel::conf_map args;
    if (!parse_args(argc, argv, &args))
        return 1;
    // override default options with defined command line arguments
    if (args.find("conf") != args.end())
        conf_path = args["conf"];
    if (args.find("watchdog") != args.end())
        watchdog_path = args["watchdog"];
    if (args.find("checkinterval") != args.end())
        check_interval = std::stoi(args["checkinterval"]);
    if (args.find("maxretry") != args.end())
        max_retries = std::stoi(args["maxretry"]);
    // set up logging and start application
    logging::open(APP_NAME);
    logging::set_level(logging::DEBUG);
    sentinel::Sentinel guard(conf_path,
                             watchdog_path,
                             check_interval,
                             max_retries);
    int ret = guard.start();
    logging::close();
    return ret;
}
