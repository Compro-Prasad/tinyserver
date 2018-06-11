TARGET = libinotify.so

SOURCE = inotify.c

$(TARGET): inotify.o
	gcc -shared -Wl,-soname,$@ -o $@ $<

inotify.o: inotify.c
	gcc -c -fPIC $< -o $@
