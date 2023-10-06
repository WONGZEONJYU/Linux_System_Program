#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <iostream>

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
    const auto pid {fork()};

    sched_param sp{};

    if (pid > 0){

        cout << "child = " << getpid() << 
            ", ppid = " << getppid() << 
            ", pgid = " << getpgrp() << '\n';

        if (set_nice(0)){
            heavy_work();
        }else {
            cerr << "set nice_value failed...\n";
        }

    }else if(!pid){
        
        cout << "child = " << getpid() << 
            ", ppid = " << getppid() << 
            ", pgid = " << getpgrp() << '\n';

        if (set_nice(5)){
            heavy_work();
        }else {
            cerr << "set nice_value failed...\n";
        }

    }else {
        cerr << "child process create failed...\n";
    }
    
    return 0;
}
