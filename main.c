#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./include/queue.h"
#include "./include/queue_thread.h"
#include "./include/timer_thread.h"

void func(void *data)
{
    // while(1) {
        printf("================\n");
        if (data != NULL) {
            printf("%s\n", (const char*)data);
            // free(data);
        }
        // usleep(1000);
        // sleep(1);
    // }
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char* argv[])
{
    queue_thread_t * thrd = qthread_start(&func);
    timer_thread_t * timer = tthread_start(10000);

    char *t = "timer timer";
    unsigned int timerId = tthread_add(timer, thrd, t, 100000, 100000);

    char *ch = "hello";

    int loop = 0;
    while(1)
    {
        if (++ loop > 4) {
            qthread_end(&thrd);
            sleep(2);
            break;
        }
        printf("main\n");
        sleep(2);
        tthread_remove(timer, timerId);
        tthread_end(&timer);
        qthread_push(thrd, ch);
        // qthread_end(thrd);
        // break;
    }

    tthread_end(&timer);

    return 0;
}


void test_queue()
{
    queue* que = queue_create(10);

    const char *c = "hello";
    queue_push(que, (void*)c);

    printf("%d\n", queue_count(que));
    printf("%d\n", queue_is_empty(que));

    const char *r = queue_pop(que);
    printf("%d\n", queue_count(que));
    const char *m = queue_pop(que);

    printf("%d\n", queue_count(que));
    printf("%d\n", queue_is_empty(que));
    printf("%s\n", r);
    printf("%s\n", m);
    queue_clear(que);
    printf("%d\n", queue_count(que));
    printf("%d\n", queue_is_empty(que));
    queue_free(&que);
}