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

    cout << "child = " << getpid() << 
            ", ppid = " << getppid() << 
            ", pgid = " << getpgrp() << '\n';

    sp.sched_priority = 2;

    if (-1 == sched_setscheduler(0,SCHED_RR,&sp) ){
        cerr << "set policy error ===> "  << __LINE__ << "\n";
    }

    int pid {};
    if ((pid = fork()) > 0){

        if (set_nice(2)){
            heavy_work();
        }else {
            cerr << "set nice_value failed ===> " << __LINE__ << "\n";
        }

    }else if(!pid){

        cout << "child = " << getpid() << 
            ", ppid = " << getppid() << 
            ", pgid = " << getpgrp() << '\n';

        sp.sched_priority = 5;

        if (-1 == sched_setscheduler(0,SCHED_RR,&sp) ){
            cerr << "set policy error ===> "  << __LINE__ << "\n";
        }

        if (set_nice(5)){
            heavy_work();
        }else {
            cerr << "set nice_value failed ===> " << __LINE__ << "\n";
        }

    }else {
        cerr << "child process create failed...\n";
    }

    return 0;
}
