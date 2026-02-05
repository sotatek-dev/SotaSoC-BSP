/**
 * Stub syscalls for FreeRTOS when linking full newlib (e.g. ST7789 driver with float).
 * Provides __wrap_* symbols required by freertos-rv32.mk LDFLAGS.
 * Add to SOURCE_FILES in any example that needs libc (memset, strncpy, vprintf, float).
 */
#include <errno.h>
#include <sys/stat.h>
#include <stddef.h>
#include "hw_registers.h"

#define UART_TX_DATA_REG  (*(volatile unsigned int *)UART_TX_DATA)
#define UART_TX_CTRL_REG  (*(volatile unsigned int *)UART_TX_CTRL)
#define UART_RX_DATA_REG  (*(volatile unsigned int *)UART_RX_DATA)
#define UART_RX_CTRL_REG  (*(volatile unsigned int *)UART_RX_CTRL)

static int uart_putc(char c) {
  UART_TX_DATA_REG = (unsigned char)c;
  UART_TX_CTRL_REG = UART_TX_CTRL_START;
  while (UART_TX_CTRL_REG & UART_TX_CTRL_BUSY) { }
  return 1;
}

int __wrap__close(int fd) {
  (void)fd;
  return 0;
}

int __wrap__open(const char *path, int flags, ...) {
  (void)path;
  (void)flags;
  errno = ENOENT;
  return -1;
}

long __wrap__lseek(int fd, long offset, int whence) {
  (void)fd;
  (void)offset;
  (void)whence;
  errno = ESPIPE;
  return (long)-1;
}

int __wrap__read(int fd, void *buf, size_t count) {
  (void)fd;
  if (buf == NULL || count == 0) return 0;
  for (size_t i = 0; i < count; i++) {
    UART_RX_CTRL_REG = UART_RX_CTRL_START;
    while (!(UART_RX_CTRL_REG & UART_RX_CTRL_READY)) { }
    ((char *)buf)[i] = (char)(UART_RX_DATA_REG & 0xFFu);
  }
  return (int)count;
}

int __wrap__write(int fd, const void *buf, size_t count) {
  (void)fd;
  const char *p = (const char *)buf;
  for (size_t i = 0; i < count; i++) uart_putc(p[i]);
  return (int)count;
}

int __wrap__fstat(int fd, struct stat *st) {
  (void)fd;
  st->st_mode = S_IFCHR;
  return 0;
}

int __wrap__stat(const char *path, struct stat *st) {
  (void)path;
  st->st_mode = S_IFCHR;
  return 0;
}

int __wrap__isatty(int fd) {
  (void)fd;
  return 1;
}

int __wrap__getpid(void) { return 1; }

int __wrap__kill(int pid, int sig) {
  (void)pid;
  (void)sig;
  errno = EINVAL;
  return -1;
}

void __wrap___exit(int status) {
  (void)status;
  for (;;) { }
}

extern char _end;
extern char __heap_limit;
static char *heap_end;

void *__wrap__sbrk(int incr) {
  if (heap_end == 0) heap_end = &_end;
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
