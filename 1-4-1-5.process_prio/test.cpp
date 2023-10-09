#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <iostream>
#include <sched.h>
#include <signal.h>

using namespace std;

static void heavy_work()
{
    int i {};
    for(;;){
        sin(i++);
    }
}

static auto set_nice(const int nice)
{
    return (!setpriority(PRIO_PROCESS,0,nice));
}

static auto get_nice()
{
    auto ret {getpriority(PRIO_PROCESS,0)};
    return ((-1 == ret) && (0 != errno)) ? 0xff : ret;
}

int main(int argc, char* argv[])
{
    sched_param sp{};

    constexpr int ts[] {5,3,2};

    cout << "child = " << getpid() << 
            ", ppid = " << getppid() << 
            ", pgid = " << getpgrp() << '\n';

    sp.sched_priority = 99;

    pid_t pids[3] {};

    if (-1 != sched_setscheduler(0,SCHED_FIFO,&sp) ){

        for (size_t i {}; i < 3; i++){

            const auto pid {fork()};

            if (pid > 0){

                pids[i] = pid;
                sp.sched_priority = 0;

                if (-1 == sched_setscheduler(0,SCHED_OTHER,&sp)){
                    cerr << "set policy error ===> "  << __LINE__ << "\n";
                    kill(pid,SIGINT);
                    pids[i] = 0;
                }

            }else if(!pid){

                cout << "child = " << getpid() << 
                        ", ppid = " << getppid() << 
                        ", pgid = " << getpgrp() << '\n';
                heavy_work();

            }else {
                cerr << "fork error ===> "  << __LINE__ << "\n";
                exit(-1);
            }
        }

    }else {
        cerr << "set parent policy error ===> "  << __LINE__ << "\n";
        exit(-1);
    }

    int i {};

    for (;;){

        sp.sched_priority = 0;
        if (-1 == sched_setscheduler(pids[i],SCHED_OTHER,&sp)){ /*把上一个进程设置为普通进程,随时保持着两个进程为普通进程*/
            cerr << "set process to normal policy error ===> "  << __LINE__ << "\n";
        }

        i = (i+1) % 3;

        sp.sched_priority = 30;
        if (-1 == sched_setscheduler(pids[i],SCHED_FIFO,&sp)){
            cerr << "schedule policy error ===> "  << __LINE__ << "\n";
        }

        sleep(ts[i]);
    }

    return 0;
}
