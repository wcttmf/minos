#include "minos.h"

static jmp_buf __minos_main;
static thread_t *__minos_head;
static uint64_t __minos_curr;

void minos_init(void) {
  __minos_head = (thread_t *)0;
  __minos_curr = 0;
}

static void __minos_thread_init(thread_t *ctx, void (*thread)(thread_t *),
                                void *udat) {
  ctx->next = __minos_head;
  __minos_head = ctx;
  ctx->udat = udat;
  ctx->next_time = 0;
  if (setjmp(__minos_head->regs)) {
    thread(ctx);
  }
}

void minos_thread_init(int offset, thread_t *ctx,
                              void (*thread)(thread_t *), void *udat) {
  volatile char space[offset];
  __minos_thread_init(ctx, thread, udat);
}

int minos_thread_await(uint64_t tick) {
  thread_t head, *t, *c = __minos_head;
  int ret;
  if (__minos_head) {
    head.next = c->next;
    c->next_time += tick;
    t = &head;
    while (t->next && t->next->next_time <= c->next_time)
      t = t->next;
    c->next = t->next;
    t->next = c;
    __minos_head = head.next;
    if (__minos_head->next_time <= __minos_curr) {
      if (!(ret = setjmp(c->regs))) longjmp(__minos_head->regs, -1);
    } else {
      if (!(ret = setjmp(c->regs))) longjmp(__minos_main, -1);
    }
    return ret;
  } else
    return 0;
}

uint64_t minos_start(uint64_t curr) {
  __minos_curr = curr;
  if (__minos_head) {
    if (__minos_head->next_time > __minos_curr) return __minos_head->next_time;
    if (!setjmp(__minos_main)) longjmp(__minos_head->regs, -1);
    return __minos_head->next_time;
  } else
    return 0;
}