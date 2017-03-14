This is aclock - analog / ansi clock for consoles and terminals - http://www.tenox.net/#aclock

This version is for Windows NT HAL and runs as a kernel driver at system boot.

To run at NT boot screen copy aclock.sys to WINNT\System32\Drivers and double click on aclock.reg to merge the driver registration file.

WARNING: The application WILL DESTROY your system by making it unbootable! The driver has an infinite loop, so your system will NEVER boot past it. Use snapshots in VM or F8 for last good known configuration to work around.
