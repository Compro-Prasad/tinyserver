#!/usr/bin/env python

import os
from ctypes import cdll

# Run make to update libnotify.so
inotify = cdll.LoadLibrary("./libinotify.so")

watch_file = "inotify.c"
if inotify.watch(watch_file):
    print("File changed")
else:
    print("Error")
