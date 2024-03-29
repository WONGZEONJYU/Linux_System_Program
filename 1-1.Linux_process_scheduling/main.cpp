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

static void test(const int id,timespec* tss,const int nrecord)
{
	/*nrecord 记录的时间片数,也就是进程要执行的总时间*/
    for (int i {}; i < nrecord; i++){
        work();
        clock_gettime(CLOCK_MONOTONIC,tss + i);
    }

    char buf[128]{};
    sprintf(buf,"./%d-proc.log",id);
    cout << buf << '\n';

    int fd { open(buf,O_WRONLY | O_CREAT | O_TRUNC) };

    if (-1 != fd){
        
        for (int i {}; i < nrecord; i++){
            sprintf(buf,"%d\t%ld\t%d\n",
                        id,
                        DiffNS(g_time_begin,tss[i]) / NSECS_PER_MSEC,
                        (i+1) * 100 / nrecord);
            write(fd,buf,strlen(buf));
        }
    }
    
    close(fd);
}

int main(int argc, char const *argv[])
{
    int nproc {atoi(argv[1])};  /*需要创建多少个进程*/
    int total {atoi(argv[2])};  /*每个创建出来的进程需要执行的总时间*/
    int slice {atoi(argv[3])};  /*时间片*/

    int nrecord {total / slice};/*需要记录多少次系统时间*/

    timespec* logbuf {reinterpret_cast<timespec*>(malloc(nrecord * sizeof(*logbuf)))};
    pid_t* pids {reinterpret_cast<pid_t*>(malloc(nproc * sizeof(*pids)))};
    total = total / slice * slice;

    if (logbuf && pids){
        
        cout << "nproc = " << nproc << '\n';
        cout << "total = " << total << '\n';
        cout << "slice = " << slice << '\n';

        cout << "SCHED_OTHER = " << SCHED_OTHER << '\n';
        cout << "SCHED_FIFO = " << SCHED_FIFO << '\n';
        cout << "SCHED_RR = " << SCHED_RR << '\n';

		/*每个时间片的循环次数*/
        cout << "Begin estimate work load per slice...\n";
        g_load_per_slice = estimate_loops_per_msec() * slice;
        cout << "End ==> g_load_per_slice = " << g_load_per_slice << '\n';

        int n {};
        
        /*记录起始时间*/
        clock_gettime(CLOCK_MONOTONIC,&g_time_begin);
        
        for (int i {}; i < nproc; i++){

            pids[i] = fork();
			
            if (pids[i] < 0){
                /*某个进程创建失败,杀死所有已经创建的子进程*/
                int j{};
                while (j < n){
                    kill(pids[j++],SIGKILL);
                }
                cout << "Process create error...\n";
                break;
            }else if (0 == pids[i]){

                int sched {sched_getscheduler(0)};
                int pri {getpriority(PRIO_PROCESS,0)};

                cout << "task " << i << " ===> schedule policy: " << sched << '\n';
                cout << "task " << i << " ===> schedule priority: " << pri << '\n';

                test(i,logbuf,nrecord);

                exit(0);
            }else{
                n++;
            }
        }
    }

    free(logbuf);
    free(pids);

    return 0;
}
