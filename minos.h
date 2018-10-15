#ifndef MINOS_H
#define MINOS_H

#include <setjmp.h>
#include <stdint.h>

typedef struct thread_s {
  jmp_buf regs;
  struct thread_s *next;
  uint64_t next_time;
  void *udat;
} thread_t;


void minos_init(void);

void minos_thread_init(int offset, thread_t *ctx, void (*thread)(thread_t *),
                       void *udat);
int minos_thread_await(uint64_t tick);

uint64_t minos_start(uint64_t curr);

#endif