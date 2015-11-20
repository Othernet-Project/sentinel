// Copyright 2015, Outernet Inc.
// Some rights reserved.
// This software is free software licensed under the terms of GPLv3. See COPYING
// file that comes with the source code, or http://www.gnu.org/licenses/gpl.txt.
#include <syslog.h>

#include <iostream>
#include <string>

#include "sentinel/logging.h"


namespace logging {

    Level current_level = ERROR;

    void set_level(Level chosen_level) {
        current_level = chosen_level;
    }

    void open(std::string app_name) {
        openlog(app_name.c_str(), LOG_PID, LOG_USER);
    }

    void close() {
        closelog();
    }

    void log(Level chosen_level, std::string message) {
        if (chosen_level <= current_level) {
            switch (chosen_level) {
                case CRITICAL:
                    std::cerr << "[CRITICAL] " << message << std::endl;
                    syslog(LOG_CRIT, message.c_str());
                    break;
                case ERROR:
                    std::cerr << "[ERROR] " << message << std::endl;
                    syslog(LOG_ERR, message.c_str());
                    break;
                case WARNING:
                    std::cerr << "[WARNING] " << message << std::endl;
                    syslog(LOG_WARNING, message.c_str());
                    break;
                case INFO:
                    std::clog << "[INFO] " << message << std::endl;
                    syslog(LOG_INFO, message.c_str());
                    break;
                case DEBUG:
                    std::clog << "[DEBUG] " << message << std::endl;
                    syslog(LOG_DEBUG, message.c_str());
                    break;
            }
        }
    }

    void critical(std::string message) {
        log(CRITICAL, message);
    }

    void error(std::string message) {
        log(ERROR, message);
    }

    void warning(std::string message) {
        log(WARNING, message);
    }

    void info(std::string message) {
        log(INFO, message);
    }

    void debug(std::string message) {
        log(DEBUG, message);
    }
}  // namespace logging
