#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <sys/select.h>
#include <errno.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>

struct Job{
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

static constexpr Job g_job[] {
    {1,const_cast<char*>("Delphi"),do_job},
    {2,const_cast<char*>("D.T.Software"),do_job},
    {3,const_cast<char*>("Tang"),do_job},
};

static constexpr int g_jlen {sizeof(g_job) / sizeof(*g_job)};
static siginfo_t g_sig_arr[65] {};
static constexpr int g_slen {sizeof(g_sig_arr) / sizeof(*g_sig_arr)};
static int g_sig_fd {-1};

static int mask_all_signal()
{
    sigset_t set {};
    sigfillset(&set);
    sigprocmask(SIG_SETMASK,&set,nullptr);
    return signalfd(-1,&set,0); /*create signal fd*/
}

static void do_sig_process(signalfd_siginfo* info)
{
    std::cout << __FUNCTION__ << ": " <<  
        info->ssi_signo << " --> " << 
        info->ssi_int << "\n";
    /*do process for the obj signal*/
    switch (info->ssi_signo)
    {
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

static int select_handler(fd_set *const rest,fd_set *const reads,const int max)
{
    int ret {max};

    for (int i {}; i <= max; i++){
        
        if (FD_ISSET(i,rest)){
            
            if (i == g_sig_fd){

                signalfd_siginfo si {};

                if (read(g_sig_fd,&si,sizeof(si)) > 0){
                    do_sig_process(&si);
                } 
            }else {
                /*
                    handle other fd
                    the return value should be the max fd value
                    max = (max < fd) ? fd : max;
                */
            }

            FD_CLR(i,reads);
            ret = max;
        }
    }

    return ret;
}

static void process_signal()
{
    static int max {};
    timeval timeout {.tv_sec = 0,.tv_usec = 5000};
    fd_set reads{};

    FD_ZERO(&reads);
    FD_SET(g_sig_fd,&reads);
    max = g_sig_fd;
    fd_set rset {reads};

    while (select(max + 1,&rset,nullptr,nullptr,&timeout) > 0){
        max = select_handler(&rset,&reads,max);
    }
}

static void app_init()
{
    struct sigaction act {
        .sa_flags = SA_RESTART | SA_SIGINFO
    };

    for (int i {1}; i <= 64; i++) {
        /*信号处理函数正在处理的时候，不会被相同的信号打断，不同的信号不会影响*/
        sigaddset(&act.sa_mask, i);
    }

    for (int i {1}; i <= 64; i++) {
        sigaction(i, &act, NULL);
    }

    g_sig_fd = mask_all_signal();
}

int main(int argc, char *argv[])
{
    std::cout << "current pid(" << getpid() << ") ...\n";

    app_init();

    for (int i {}; i < g_jlen; i++){
        
        const int argc {g_job[i].argc};
        char* argv { static_cast<char*>(g_job[i].argv) };

        g_job[i].Job_func(argc,argv);

        process_signal();
    }

    return 0;
}
