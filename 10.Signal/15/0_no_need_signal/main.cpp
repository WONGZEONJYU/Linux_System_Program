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
    {1,const_cast<char*>("D.T.Software"),do_job},
    {1,const_cast<char*>("Tang"),do_job},
};

static constexpr int g_jlen {sizeof(g_job) / sizeof(*g_job)};

// static siginfo_t g_sig_arr[65] {};
// static constexpr int g_slen {sizeof(g_sig_arr) / sizeof(*g_sig_arr)};

static void make_all_signal()
{
    sigset_t set {};
    sigfillset(&set);
    sigprocmask(SIG_SETMASK,&set,nullptr);
}

int main(int argc, char *argv[])
{
    make_all_signal();

    std::cout << "current pid(" << getpid() << ") ...\n";

    for (int i {}; i < g_jlen; i++){
        
        const int argc {g_job[i].argc};
        char* argv { static_cast<char*>(g_job[i].argv) };

        g_job[i].Job_func(argc,argv);

    }

    return 0;
}
