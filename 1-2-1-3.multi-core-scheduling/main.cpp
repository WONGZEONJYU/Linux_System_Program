#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sched.h>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

#if 0
#define NLOOP_FOR_ESTIMATION    1000000000UL
#define NSECS_PER_MSEC  1000000UL
#define NSECS_PER_SEC   1000000000UL
#else
static constexpr unsigned long NLOOP_FOR_ESTIMATION {1000000000UL};
static constexpr unsigned long NSECS_PER_MSEC {1000000UL};
static constexpr unsigned long NSECS_PER_SEC {1000000000UL};
#endif

#define DiffNS(begin,end)   ((end.tv_sec - begin.tv_sec) * NSECS_PER_SEC \
                            + (end.tv_nsec - begin.tv_nsec))

static unsigned long g_load_per_slice {};
static timespec g_time_begin {};

static unsigned long estimate_loops_per_msec() /*1ms有多少次循环*/
{
    timespec begin{},end{};
    clock_gettime(CLOCK_MONOTONIC,&begin);
    for(unsigned long i {};i < NLOOP_FOR_ESTIMATION;++i);
    clock_gettime(CLOCK_MONOTONIC,&end);
    return NLOOP_FOR_ESTIMATION * NSECS_PER_MSEC / DiffNS(begin,end);
    /*以下书写可能更容易理解*/
    /* NLOOP_FOR_ESTIMATION / DiffNS(begin,end) * NSECS_PER_MSEC */
}

static inline void work()
{
    /* g_load_per_slice 每个时间片的循环次数
    	经过 1 个时间片的循环次数后返回,模拟工作 1 个时间片*/
    for(unsigned long i {};i < g_load_per_slice;++i);
}

static void test(const int ep, const int id, const int nrecord)
{
    timespec ts {};
    char buf[128]{};
    for (int i {}; i < nrecord; i++){
        work();
    }

    clock_gettime(CLOCK_MONOTONIC,&ts);

    sprintf(buf,"./%d-%d-proc.log",ep,id);

    cout << buf << (DiffNS(g_time_begin,ts) / NSECS_PER_MSEC) << '\n';

}

static void epoch(const int nproc,int total,const int slice)
{
    const auto nrecord {total / slice};
    pid_t* pids {reinterpret_cast<pid_t*>(malloc(nproc * sizeof(*pids)))};
    total = total / slice * slice;

    if (pids){
        
        cout << "nproc = " << nproc << '\n';
        cout << "total = " << total << '\n';
        cout << "slice = " << slice << '\n';
        int n{};
        timespec ts {};
        /*记录起始时间*/
        clock_gettime(CLOCK_MONOTONIC,&g_time_begin);

        for (int i {}; i < nproc; i++){
            
            pids[i] = fork();

            if (pids[i] < 0){
                cout << "process create error...\n";
                break;
            }else if (0 == pids[i]){
                test(nproc,i,nrecord);
            }else{
                ++n;
            }
        }

        for (int i {}; i < n; i++){
            wait(nullptr);
        }

        clock_gettime(CLOCK_MONOTONIC,&ts);

        cout << "nproc = " << nproc << " end -->" << (DiffNS(g_time_begin,ts) / NSECS_PER_MSEC);
    }

    free(pids);
}

int main(int argc, char* argv[])
{
    const auto nproc {atoi(argv[1])};  /*需要创建多少个进程*/
    const auto total {atoi(argv[2])};  /*每个创建出来的进程需要执行的总时间*/
    const auto slice {atoi(argv[3])};  /*时间片*/

    cout << "SCHED_OTHER = " << SCHED_OTHER << '\n';
    cout << "SCHED_FIFO = " << SCHED_FIFO << '\n';
    cout << "SCHED_RR = " << SCHED_RR << '\n';

    /*每个时间片的循环次数*/
    cout << "Begin ==> estimate work load per slice...\n";
    g_load_per_slice = estimate_loops_per_msec() * slice;
    cout << "End ==> g_load_per_slice = " << g_load_per_slice << '\n';

    for (int i {1}; i <= nproc; i++){
        epoch(i,total,slice);
    }

    return 0;
}
