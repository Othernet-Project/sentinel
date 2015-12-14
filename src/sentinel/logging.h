// Copyright 2015, Outernet Inc.
// Some rights reserved.
// This software is free software licensed under the terms of GPLv3. See COPYING
// file that comes with the source code, or http://www.gnu.org/licenses/gpl.txt.
#ifndef SENTINEL_LOGGING_H_
#define SENTINEL_LOGGING_H_
#include <string>

namespace logging {
    enum Level {
        CRITICAL = 0,
        ERROR = 1,
        WARNING = 2,
        INFO = 3,
        DEBUG = 4
    };

    extern Level current_level;

    void open(const std::string& app_name);
    void set_level(Level chosen_level);
    void close();
    void log(Level chosen_level, const std::string& message);
    void critical(const std::string& message);
    void error(const std::string& message);
    void warning(const std::string& message);
    void info(const std::string& message);
    void debug(const std::string& message);
}  // namespace logging
#endif  // SENTINEL_LOGGING_H_
