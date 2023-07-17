#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

struct SigInfo
{
    int sig,index;
};

static int g_index{};
static SigInfo g_sig_arr[128] {};

static void signal_handler(const int,siginfo_t* info,void*)
{
    g_sig_arr[g_index].sig = info->si_signo;
    g_sig_arr[g_index].index = info->si_value.sival_int;
    ++g_index;
}

int main(int argc,const char* argv[])
{
    std::cout << "current pid (" << getpid() << ") ...\n";

    struct sigaction act {};
    act.sa_sigaction = signal_handler;
    act.sa_flags = SA_SIGINFO | SA_RESTART;

    for (int i {1}; i <= 64; i++){  //屏蔽信号，不让别的信号中断当前信号的处理
        sigaddset(&act.sa_mask,i);
    }

    for (int i {1}; i <= 64; i++){
        sigaction(i,&act,nullptr);
    }

    sigset_t set {};

    sigfillset(&set);
    sigprocmask(SIG_SETMASK,&set,nullptr);/*屏蔽所有信号，让所有信号处于未决
                                            信号无法触发任何处理函数，包括默认行为*/

    for (int i {}; i < 15; i++){
        sleep(1);
        std::cout << "i = " << i << "\n";
    }

    std::cout << "\n";

    sigemptyset(&set);
    sigprocmask(SIG_SETMASK,&set,nullptr);/*解除所有被屏蔽的信号，让信号递达进程，
                                            按信号优先级处理*/

    for (int i {}; i < g_index; i++){

        std::cout << "send signum = " << g_sig_arr[i].sig << 
            ",send index = " << g_sig_arr[i].index << "\n";
    }

    exit(0);
}

#if 0
//sigfillset(&act.sa_mask);   //屏蔽信号，不让别的信号中断当前信号的处理
void print_sigset(const sigset_t *set)
{
    for(int i {1}; i < NSIG; ++i){
        sigismember(set,i) ? (std::cout << "1") : (std::cout << "0");
    }
    std::cout << "\n";
}
#endif
