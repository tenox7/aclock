Aclock for Windows NT HAL running as a kernel driver at system boot blue screen.

![Screenshot](aclock.png)

To build, adjust [Sources](Sources), then run `build -Z` from **NTDDK** command prompt.

**WARNING**: The following process will render your NT system unbootable! The driver has an infinite loop, so your system will NEVER boot past it. Use `F8 -> Last Good Known Configuration` or VM with snapshots to work around.

To run at NT boot screen copy `aclock.sys` to `WINNT\System32\Drivers` and double click on [aclock.reg](aclock.reg) to merge the driver registration file.
