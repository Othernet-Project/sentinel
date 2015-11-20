// Copyright 2015, Outernet Inc.
// Some rights reserved.
// This software is free software licensed under the terms of GPLv3. See COPYING
// file that comes with the source code, or http://www.gnu.org/licenses/gpl.txt.
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/watchdog.h>

#include <csignal>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "sentinel/logging.h"
#include "sentinel/sentinel.h"


namespace sentinel {

Sentinel::Sentinel(const std::string& config_path,
                   const std::string& watchdog_path,
                   int check_interval,
                   int max_retries): config_path_(config_path),
                                     watchdog_path_(watchdog_path),
                                     check_interval_(check_interval),
                                     max_retries_(max_retries),
                                     watchdog_fd_(-1) {}


bool Sentinel::load_config() {
    std::ifstream config_file(config_path_.c_str());

    if (!config_file.is_open() || config_file.fail())
        return false;
    // split lines of the config file on the ``=`` character, left part becomes
    // the key, right part the value of the config map
    std::string line;
    while (std::getline(config_file, line)) {
        std::istringstream line_stream(line);
        std::string key;
        if (std::getline(line_stream, key, '=')) {
            std::string value;
            if (std::getline(line_stream, value)) {
                config_.insert(std::make_pair(key, value));
            }
        }
    }
    return true;
}


bool Sentinel::init_watchdog() {
    watchdog_fd_ = open(watchdog_path_.c_str(), O_WRONLY);
    return watchdog_fd_ != -1;
}


bool Sentinel::is_process_alive(const std::string& pid_path) {
    std::ifstream pid_file;
    std::string pid_str;
    pid_file.open(pid_path);
    if (!pid_file.is_open() || pid_file.fail())
        return false;

    std::getline(pid_file, pid_str);
    pid_file.close();
    int pid;
    try {
        pid = std::stoi(pid_str);
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
    // sending the ``0`` signal to any pid just checks if the process is alive
    return kill(pid, SIGNAL_CHECK) == ALIVE;
}


void Sentinel::start_process(const std::string& cmd,
                             const std::string& pid_path) {
    // remove pid file
    std::string rmpid = RM + " " + pid_path;
    std::system(rmpid.c_str());
    // execute command that should start the process
    std::system(cmd.c_str());
}


void Sentinel::keep_system_alive() {
    // must feed it only if watchdog is available
    if (watchdog_fd_ != -1)
        ioctl(watchdog_fd_, WDIOC_KEEPALIVE, 0);
}


int Sentinel::watch() {
    std::string pid_path,
                start_cmd;

    while (true) {
        for (auto it = config_.begin(); it != config_.end(); ++it) {
            pid_path = it->first;
            start_cmd = it->second;
            if (!is_process_alive(pid_path)) {
                logging::error("Process under: " + pid_path + " not alive.");

                if (max_retries_ > DEFAULT_MAX_RETRIES &&
                        retry_attempts_[pid_path] >= max_retries_) {
                    logging::critical("Maximum process revival retries "
                                      "exceeded, aborting...");
                    return stop();
                }

                logging::info("Invoking: " + start_cmd);
                start_process(start_cmd, pid_path);
                retry_attempts_[pid_path]++;
            } else {
                // if the process is alive, reset it's retry attempts counter
                retry_attempts_[pid_path] = 0;
            }
        }
        keep_system_alive();
        sleep(check_interval_);
    }
}


int Sentinel::stop() {
    if (watchdog_fd_ != -1)
        close(watchdog_fd_);
    return 0;
}


int Sentinel::start() {
    if (!load_config()) {
        logging::error("Error opening config file: " + config_path_);
        return -1;
    }

    if (watchdog_path_.length() == 0)
        logging::info("Watchdog disabled.");
    else if (!init_watchdog())
        // watchdog not available
        logging::info("Watchdog not available. Continuing without it.");

    return watch();
}

}  // namespace sentinel
