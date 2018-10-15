#include <stdio.h>
#include "minos.h"

void test(thread_t *ctx) {
  while (1) {
    minos_thread_await(3);
    printf("%s %d\n", ctx->udat, ctx->next_time);
  }
}

int main(int argc, char const *argv[]) {
  thread_t a, b, c;

  minos_init();
  minos_thread_init(50000, &a, test, "thread a");
  minos_thread_init(150000, &b, test, "thread b");
  minos_thread_init(250000, &c, test, "thread c");

  for (int i = 0; i < 30; i++) {
    minos_start(i);
  }

  return 0;
}
