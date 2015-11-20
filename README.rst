========
Sentinel
========

A very simplistic process guardian. It uses a configuration mapping of pid
file paths and commands, which can start the process to which the pid file
belongs. Performing periodic checks against all pid files in the configuration,
if it notices that a process is not alive, it will attempt to start the process
in question. Optionally, it can stop feeding the kernel's watchdog if the
failure count of restart attempts exceeds the allowed limit, which causes a
system reboot.
