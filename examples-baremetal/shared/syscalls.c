#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "hw_registers.h"

#define UART_TX_DATA_REG   (*(volatile unsigned int *)UART_TX_DATA)
#define UART_TX_CTRL_REG   (*(volatile unsigned int *)UART_TX_CTRL)
#define UART_RX_DATA_REG   (*(volatile unsigned int *)UART_RX_DATA)
#define UART_RX_CTRL_REG   (*(volatile unsigned int *)UART_RX_CTRL)

ssize_t uart_putc(char c) {
  UART_TX_DATA_REG = (unsigned char)c;
  UART_TX_CTRL_REG = UART_TX_CTRL_START;
  while (UART_TX_CTRL_REG & UART_TX_CTRL_BUSY) {
  }
  return 1;
}

static int uart_getc(char *c) {
  UART_RX_CTRL_REG = UART_RX_CTRL_START;
  while (!(UART_RX_CTRL_REG & UART_RX_CTRL_READY)) {
  }
  *c = (char)(UART_RX_DATA_REG & 0xFFu);
  return 1;
}

ssize_t _write(int fd, const void *buf, size_t count) {
  (void)fd;
  const char *data = (const char *)buf;
  for (size_t i = 0; i < count; i++) {
    uart_putc(data[i]);
  }
  return (ssize_t)count;
}

int _open(const char *path, int flags, ...) {
  (void)path;
  (void)flags;
  errno = ENOENT;
  return -1;
}

int _close(int fd) {
  (void)fd;
  return 0;
}

off_t _lseek(int fd, off_t offset, int whence) {
  (void)fd;
  (void)offset;
  (void)whence;
  errno = ESPIPE;  // not seekable
  return (off_t)-1;
}

int _read(int fd, void *buf, size_t count) {
  (void)fd;
  if (buf == NULL || count == 0) {
    return 0;
  }
  char *p = (char *)buf;
  for (size_t n = 0; n < count; n++) {
    uart_getc(&p[n]);
  }
  return (int)count;
}

int _fstat(int fd, struct stat *st) {
  (void)fd;
  st->st_mode = S_IFCHR;
  return 0;
}

int _stat(const char *path, struct stat *st) {
  (void)path;
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int fd) {
  (void)fd;
  return 1; /* All fds are UART/console */
}

int _getpid(void) { return 1; }

int _kill(int pid, int sig) {
  (void)pid;
  (void)sig;
  errno = EINVAL;
  return -1;
}

void _exit(int status) {
  (void)status;
  for (;;) {
  }
}

extern char _end;
extern char __heap_limit;
static char *heap_end;

void *_sbrk(int incr) {
  if (heap_end == 0) {
    heap_end = &_end;
  }
  if (incr <= 0) {
    errno = EINVAL;
    return (void *)-1;
  }
  char *prev = heap_end;
  if (heap_end + incr > &__heap_limit) {
    errno = ENOMEM;
    return (void *)-1;
  }
  heap_end += incr;
  return (void *)prev;
}
