@echo off
cl -c -Gsw -Zdp -FPa -Os aclkwin.c
link4 /noe /align:16 /map aclkwin,,,slibw,aclkwin.def
