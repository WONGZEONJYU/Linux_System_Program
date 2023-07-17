#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>

struct Job
{
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

static void signal_handler(const int sig, siginfo_t* info, void*)
{
    g_sig_arr[sig] = *info;
    g_sig_arr[0].si_signo++;
}

static void do_sig_process(siginfo_t* info)
{
    std::cout << __FUNCTION__ << ": " <<  
        info->si_signo << " --> " << 
        info->si_value.sival_int << "\n";
    /*do process for the obj signal*/
    switch (info->si_signo)
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

static void process_signal()
{
    if (g_sig_arr[0].si_signo){
        
        for (int i {1}; i < g_slen; i++){

            if (g_sig_arr[i].si_signo){

                do_sig_process(&g_sig_arr[i]);
                g_sig_arr[i].si_signo = 0;
                --g_sig_arr[0].si_signo;
            }
        }
    }
}

static void app_init()
{
    struct sigaction act {};
    act.sa_sigaction = signal_handler;
    act.sa_flags = SA_RESTART | SA_SIGINFO;

    for (int i {1}; i <= 64; i++) {
        sigaddset(&act.sa_mask, i);
    }

    for (int i {1}; i <= 64; i++) {
        sigaction(i, &act, NULL);
    }

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
