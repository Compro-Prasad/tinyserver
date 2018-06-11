#!/usr/bin/env python

import os
from ctypes import cdll

os.system("make")
inotify = cdll.LoadLibrary("./libinotify.so")
inotify.print("Test")
inotify.print_py_str("Hello, World!")
watch_file = "inotify.c"
if inotify.watch(watch_file):
    print("File changed")
else:
    print("Error")
