#include <sys/wait.h>
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <fcntl.h>

#define NLOOP_FOR_ESTIMATION 1000000000UL
#define NSECS_PER_MSEC 1000000UL
#define NSECS_PER_SEC 1000000000UL

#define DiffNS(begin, end) ((end.tv_sec - begin.tv_sec) * NSECS_PER_SEC \
                            + (end.tv_nsec - begin.tv_nsec))

static unsigned long g_load_per_slice;
static struct timespec g_time_begin;

static unsigned long estimate_loops_per_msec() // 1ms有多少次循环
{
    struct timespec begin = {0};
    struct timespec end   = {0};
    unsigned long i = 0;

    clock_gettime(CLOCK_MONOTONIC, &begin);
    while (i < NLOOP_FOR_ESTIMATION) i++;
    clock_gettime(CLOCK_MONOTONIC, &end);

    return NLOOP_FOR_ESTIMATION / (DiffNS(begin, end) / NSECS_PER_MSEC);
}

static inline void work()
{
    unsigned int i = 0;

    // g_load_per_slice 每个时间片的循环次数
    // 经过 1 个时间片的循环次数后返回，模拟工作 1 个时间片
    while (i < g_load_per_slice) i++;
}

static void test(int id, struct timespec* tss, int nrecord)
{
    struct timespec ts = {0};
    char buf[128] = {0};
    int fd = -1;
    int i = 0;

    // nrecord 记录的时间片数，也就是进程要执行的总时间
    for (i = 0; i < nrecord; i++) {
        work();
        clock_gettime(CLOCK_MONOTONIC, tss + i);
    }

    sprintf(buf, "./%d-proc.log", id);
    printf("%s\n", buf);
    
    fd = open(buf, O_WRONLY | O_CREAT | O_TRUNC);
    if (fd != -1) {
        for (i = 0; i < nrecord; i++) {
            sprintf(buf, "%d\t%ld\t%d\n",
                            id,
                            DiffNS(g_time_begin, tss[i]) / NSECS_PER_MSEC,
                            (i + 1) * 100 / nrecord);
            write(fd, buf, strlen(buf));
        }
    }

    close(fd);
}

int main(int argc, char* argv[])
{
    int nproc = atoi(argv[1]); // 创建多少个进程
    int total = atoi(argv[2]); // 每个进程需要执行的时间
    int slice = atoi(argv[3]); // 时间片
    int nrecord = total / slice;
    // 申请记录 nrecord 条记录的 logbuf
    struct timespec* logbuf = malloc(nrecord * sizeof(*logbuf));
    pid_t* pids = malloc(nproc * sizeof(*pids));

    total = total / slice * slice;

    if (logbuf && pids) {
        int i = 0;
        int n = 0;

        printf("nproc = %d\n", nproc);
        printf("total = %d\n", total);
        printf("slice = %d\n", slice);

        printf("SCHED_OTHER = %d\n", SCHED_OTHER);
        printf("SCHED_FIFO  = %d\n", SCHED_FIFO);
        printf("SCHED_RR    = %d\n", SCHED_RR);

        printf("Begin estimate work load per slice...\n");
        g_load_per_slice = estimate_loops_per_msec() * slice; // 每个时间片的循环次数
        printf("End ==> g_load_per_slice = %ld\n", g_load_per_slice);

        clock_gettime(CLOCK_MONOTONIC, &g_time_begin); // 统计的起始时间

        for (i = 0; i < nproc; i++) {
            pids[i] = fork();

            if (pids[i] < 0) {
                int j = 0;
                while (j < n) {
                    kill(pids[j++], SIGKILL); // 杀死创建成功的子进程
                }
                printf("Process create error...\n");
                break;
            } else if (pids[i] == 0) {
                int sched = sched_getscheduler(0);
                int pri = getpriority(PRIO_PROCESS, 0);

                printf("task %d ==> schedule policy: %d\n", i, sched);
                printf("task %d ==> schedule priority: %d\n", i, pri);

                test(i, logbuf, nrecord);

                exit(0);
            } else {
                n++;
            }
        }
    }

    free(logbuf);
    free(pids);

    return 0;
}
