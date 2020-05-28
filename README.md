## analog clock for text console displays and terminals and terminal emulators 

Aclock is an analog clock program for text mode console displays, terminals, or terminal emulators. This program is obviously absolutely useless, except for turning your old, expensive mainframe or supercomputer into a wall clock.

![Aclock Hero Shots](https://raw.githubusercontent.com/tenox7/aclock/master/screenshots/aclock-hero-shots.gif "Aclock Hero Shots")

Aclock binaries have been compiled for nearly 250 computer system platforms. Most of them are [contributed](https://github.com/tenox7/aclock/blob/master/contrib.txt).

Some aclock builds are available as a bootable disk image under [bootimages](https://github.com/tenox7/aclock/tree/master/bootimages).

You need to have ncurses5 installed on unix based os's

Ubuntu/Debain:
```
sudo apt-get install libncurses5
```
Fedora/centos/FHEL:
```
sudo yum install ncurses-devel
```
Arch based:
```
sudo pacman -S ncurses
```

For source code, available are Curses, Termcap and AAlib for more modern systems, ANSI and K&R style. Provided are also specific, non-unix ports. There is also a vector graphics version for Tektronix 4014/4010 or XTerm/Kermit Tek emulator. 

Finally there is a [wanted list](https://github.com/tenox7/aclock/blob/master/wanted.txt) of platforms that I'm seeking to get a port or build. Some of them have a monetary reward. Please contact me for details.
