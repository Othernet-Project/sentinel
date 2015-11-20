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

    void open(std::string);
    void set_level(Level);
    void close();
    void log(Level, std::string);
    void critical(std::string);
    void error(std::string);
    void warning(std::string);
    void info(std::string);
    void debug(std::string);
}  // namespace logging
#endif  // SENTINEL_LOGGING_H_
