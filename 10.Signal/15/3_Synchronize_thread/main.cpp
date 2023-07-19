#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <pthread.h>

struct Job {
    int argc;
    void* argv;
    void(*Job_func)(int ,void*);
};

static void do_job(const int argc,void* argv)
{
    std::cout << "do_job : " << argc << "--->" << 
        (argv ? reinterpret_cast<const char*>(argv) : "null") << '\n';

    for (int i {}; i < 5; i++){
        sleep(1);
    }
}

static constexpr Job g_job [] {
    {1,const_cast<char*>("Delphi"),do_job},
    {2,const_cast<char*>("D.T.Software"),do_job},
    {3,const_cast<char*>("Tang"),do_job},
};

static constexpr int g_jlen {sizeof(g_job) / sizeof(*g_job)};

static void mask_all_signal()
{
    sigset_t set {};
    sigfillset(&set);
    sigprocmask(SIG_SETMASK,&set,nullptr);
}

static void do_sig_process(siginfo_t* info)
{
    std::cout << __FUNCTION__ << ": " <<  
        info->si_signo << " --> " << 
        info->si_value.sival_int << "\n";
    /*do process for the obj signal*/
    switch (info->si_signo){
        case SIGINT:
            /* call process function for  SIGINT*/
            break;
        case 40:
            /*  call process function for 40*/
            break;
        default:
            break;
    }
}

static void signal_handler(int,siginfo_t * info,void*)
{
    do_sig_process(info);
}

static void app_init()
{
    struct sigaction act {
        .sa_flags = SA_RESTART | SA_SIGINFO
    };

    act.sa_sigaction = signal_handler;

    for (int i {1}; i <= 64; i++) {
        /*信号处理函数正在处理的时候，不会被相同的信号打断，不同的信号不会影响*/
        sigaddset(&act.sa_mask, i);
    }

    for (int i {1}; i <= 64; i++) {
        sigaction(i, &act, nullptr);
    }
}

static void* thread_enter(void*)
{
    mask_all_signal();/*子线程简单粗暴屏蔽所有信号，不处理信号*/

    for (int i {}; i < g_jlen; i++){

        const int argc {g_job[i].argc};

        char* argv {static_cast<char*>(g_job[i].argv)};

        g_job[i].Job_func(argc,argv);
    }

    return nullptr;
}

int main(int argc, char *argv[])
{
    std::cout << "current pid(" << getpid() << ") ...\n";

    app_init();

    pthread_t tid {};

    pthread_create(&tid,nullptr,thread_enter,nullptr);

    pthread_join(tid,nullptr);/* Wait for the child thread to end*/

    mask_all_signal();

    std::cout << "app end\n";

    return 0;
}


