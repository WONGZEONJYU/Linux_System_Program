#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

static int g_current {-1};

static void add_func(const int n)
{
    g_current = 0;

    for (int i {}; i <= n; i++){
        g_current += i;
        sleep(1);
    }
}

static void signal_handler(const int,siginfo_t* info,void*)
{
    write(STDOUT_FILENO,"handler begin\n",14);
    add_func(5);
    write(STDOUT_FILENO,"handler end\n",12);
}

int main(int argc,const char* argv[])
{
    std::cout << "current pid (" << getpid() << ") ...\n";

    struct sigaction act {};
    act.sa_sigaction = signal_handler;
    act.sa_flags = SA_SIGINFO | SA_NODEFER | SA_RESTART;

    const auto obj_sig {atoi(argv[1])};

    sigaction(obj_sig,&act,nullptr);

    add_func(10);

    std::cout << "g_current = " << g_current << "\n";

    exit(0);
}
