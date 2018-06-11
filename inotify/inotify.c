#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>

// Better malloc to write less
#define mallocate(type, length) ((type *)malloc(sizeof(type) * (length)))

// For printing errors
#define PRINT(str) printf("%s:%d %s\n", __FILE__, __LINE__, (str))


// Calculate Python string length
size_t py_str_length(const char *py_str) {
  size_t len = 0, l = 0, j;
  while (py_str[len])
    {
      for (j = 0; j < 4 && py_str[len + j] != 0; j++);
      l += j;
      len += 4;
    }
  return l;
}


// Convert Python string to ASCII C string
char *py_str_to_c_str(const char *py_str)
{
  size_t l = py_str_length(py_str);
  char *c_str = mallocate(char, l);
  size_t k = 0;
  if (!c_str)
    {
      PRINT("Memory full");
      return NULL;
    }
  for (size_t i = 0; py_str[i]; i += 4)
    {
      for (size_t j = 0; j < 4 && py_str[i + j]; j++)
        c_str[k++] = py_str[i + j];
    }
  c_str[k] = 0;
  return c_str;
}


// Watch for modifications(only) on a given file or directory
int watch(const char *py_str)
{
  // Start the watcher system
  int fd = inotify_init();

  // Unable to initialize sets fd to a negative value
  if (fd < 0)
    return 0;

  // Convert Python string
  char *c = py_str_to_c_str(py_str);

  // Init watcher
  int wd = inotify_add_watch(fd, c, IN_MODIFY | IN_CLOSE_WRITE);

  // File change events are fed into this structure
  struct inotify_event watch_event;

  // Start watching for changes
  int len = read(fd, (void *)&watch_event, sizeof(watch_event));

  // Reached here means watch is over and given file was modified
  // So, close fd
  close(fd);

  // Free memory
  free(c);

  // The read might have been corrupted so the check is necessary
  if (len == sizeof(watch_event))
    return 1;

  return 0;
}
