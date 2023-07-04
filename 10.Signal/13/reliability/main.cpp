#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

static int g_count{},g_obj_sig{};

void signal_handler(const int sig,siginfo_t* info,void*)
{
    if (sig == g_obj_sig){
        g_count++;
    }
}

int main(int argc,char* argv[])
{
    std::cout << "current pid (" << getpid() << ") ...\n";

    struct sigaction act {};
    act.sa_sigaction = signal_handler;
    act.sa_flags = SA_SIGINFO;

    g_obj_sig = atoi(argv[1]);
    sigaddset(&act.sa_mask,g_obj_sig);
    sigaction(g_obj_sig,&act,nullptr);

    sigset_t set {};

    sigfillset(&set);
    sigprocmask(SIG_SETMASK,&set,nullptr);

    for (int i {}; i < 15; i++){
        sleep(1);
        std::cout << "i = " << i << "\n";
    }

    sigemptyset(&set);
    sigprocmask(SIG_SETMASK,&set,nullptr);

    std::cout << "g_count = " << g_count << "\n";

    return 0;
}

