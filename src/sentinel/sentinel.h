// Copyright 2015, Outernet Inc.
// Some rights reserved.
// This software is free software licensed under the terms of GPLv3. See COPYING
// file that comes with the source code, or http://www.gnu.org/licenses/gpl.txt.
#ifndef SENTINEL_SENTINEL_H_
#define SENTINEL_SENTINEL_H_
#include <map>
#include <string>


namespace sentinel {

typedef std::map<std::string, std::string> conf_map;


static const int SIGNAL_CHECK = 0;
static const int ALIVE = 0;
static const int DEFAULT_CHECK_INTERVAL = 5;
static const int DEFAULT_MAX_RETRIES = -1;
static const std::string DEFAULT_CONFIG_PATH("/etc/sentinel.conf");
static const std::string RM("rm");


class Sentinel {
 private:
    std::string config_path_;
    std::string watchdog_path_;
    int check_interval_;
    int max_retries_;

    conf_map config_;
    std::map<std::string, int> retry_attempts_;
    int watchdog_fd_;

    bool load_config();
    bool init_watchdog();
    int watch();
    void keep_system_alive();
    bool is_process_dead(const std::string& pid_path);
    void start_process(const std::string& cmd, const std::string& pid_path);
    int stop();

 public:
    explicit Sentinel(const std::string& config_path,
                      const std::string& watchdog_path,
                      int check_interval,
                      int max_retries);
    int start();
};

}  // namespace sentinel
#endif  // SENTINEL_SENTINEL_H_
