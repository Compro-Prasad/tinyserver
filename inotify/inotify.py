#!/usr/bin/env python

import os
from ctypes import cdll

os.system("make")
inotify = cdll.LoadLibrary("./libinotify.so")
inotify.print("Test")
inotify.print_py_str("Hello, World!")
inotify.main()
print(inotify.__dir__())
