@echo off
cl -c -Gsw -Zdp -FPa -Os aclock.c
link4 /align:16 /map aclock,,,slibw,aclock.def
