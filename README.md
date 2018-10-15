### minos
一个非抢占式单片机操作系统

```c

// 全局初始化
void minos_init(void);

// 开始任务
uint64_t minos_start(uint64_t curr);


// 线程初始化
void minos_thread_init(int offset, thread_t *ctx, void (*thread)(thread_t *),
                       void *udat);

// 线程放弃内核时间
int minos_thread_await(uint64_t tick);

```