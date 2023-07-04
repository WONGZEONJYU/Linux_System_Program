#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void delay_handler(int sig)
{
    std::cout << "begin " << __FUNCTION__ << "...\n";

    for (int i {}; i < 5; i++){

        std::cout << "sig(" << sig  << ") - sleep " << i << "...\n";
        sleep(1);
    }

    std::cout << "end " << __FUNCTION__ << "...\n";
}

void signal_handler(int sig, siginfo_t *info, void *)
{
    std::cout << __FUNCTION__ << " : sig = " << sig << "\n";
    std::cout << __FUNCTION__ << " : info->si_signo = " << info->si_signo << "\n";
    std::cout << __FUNCTION__ << " : info->si_code = " << info->si_code << "\n";
    std::cout << __FUNCTION__ << " : info->si_pid = " << info->si_pid << "\n";
    std::cout << __FUNCTION__ << " : info->si_value = " << info->si_value.sival_int << "\n";
}

int r_read(char* data,const int len)
{
    int ret{-1};

    while (data && 
        (-1 == (ret = read(STDIN_FILENO,data,len - 1))) && 
        (EINTR == errno)){

        std::cout << "restart syscall mannually...\n";
    }

    if (-1 != ret){
        data[len] = 0;
    }

    return ret;
}

int main(int argc,char* argv[])
{
    struct sigaction act{};
    act.sa_sigaction = signal_handler;
    
    act.sa_flags = SA_RESTART | SA_SIGINFO;

    sigaddset(&act.sa_mask,40);
    sigaddset(&act.sa_mask,SIGINT);

    sigaction(40,&act,nullptr);
    sigaction(SIGINT,&act,nullptr);

    char buf[32]{};

    r_read(buf,sizeof(buf));

    std::cout << "input: " << buf << "\n";

    return 0;
}

