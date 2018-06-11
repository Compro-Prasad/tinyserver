#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>

#define mallocate(type, length) ((type *)malloc(sizeof(type) * (length)))

#define PRINT(str) printf("%s:%d %s\n", __FILE__, __LINE__, (str))


void print(const char *s) {
  for (int i = 0; s[i]; i += 4) {
    putchar(s[i]);
  }
  putchar('\n');
}


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

int print_py_str(const char *py_str)
{
  char *c_str = py_str_to_c_str(py_str);
  if (c_str)
    {
      int return_value = printf("%s\n", c_str);
      free(c_str);
      return return_value;
    }
  return 0;
}

int fd = -1;

int init_watchers() {
  if (fd < 0)
    fd = inotify_init();
  return fd;
}

int watch_for_file(const char *py_str)
{
  if (fd < 0)
    init_watchers();
  return inotify_add_watch(fd, py_str, IN_MODIFY | IN_CLOSE_WRITE);
}

struct inotify_event* listen_for_writes()
{
  if (fd < 0)
    PRINT("Can't listen on None. Add a watchpoint for a file using\
watch_for_file(<filename>)");
  struct inotify_event *event = mallocate(struct inotify_event, 1);
  int l = read(fd, (void *)event, sizeof(*event));
  return event;
}

int main()
{
  int fd = inotify_init();
  const char *c = "inotify.c";
  int wd = inotify_add_watch(fd, c, IN_MODIFY | IN_CLOSE_WRITE);
  struct inotify_event watch_event;
  int len = read(fd, (void *)&watch_event, sizeof(watch_event) + 100);
  if (len == sizeof(watch_event))
    for (int i = 0; watch_event.name[i]; i++)
      printf("%d ", watch_event.name[i]);
  else
    printf("Not equal");
  printf("\n");
  close(fd);
  return 0;
}
