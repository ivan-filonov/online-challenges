#include <sys/inotify.h>
#include <limits.h>
#include <stdio.h>
int main() {
  int fd = inotify_init();
  int wd = inotify_add_watch(fd, ".", IN_ALL_EVENTS);
  const int BUF_LEN = (10 * (sizeof(struct inotify_event) + 255 + 1));
  char buf[BUF_LEN];
  for(;;) {
    int n = read(fd, buf, BUF_LEN);
    char * p;
    for(p = buf; p < buf + n; ) {
      struct inotify_event * ev = (struct inotify_event *)p;

      printf("mask: %x\tname: %s\n", ev->mask, ev->len > 0 ? ev->name : "");

      p += sizeof(struct inotify_event) + ev->len;
    }
  }
  close(fd);
  return 0;
}
