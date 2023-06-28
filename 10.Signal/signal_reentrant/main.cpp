//#define _GNU_SOURCE
// #define _XOPEN_SOURCE   600
// #define _POSIX_C_SOURCE 200800L

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
        std::cout << "sleep " << i <<"...\n";
        sleep(1);
    }

    std::cout << "end " << __FUNCTION__ << "...\n";
}

int main(int argc,char* argv[])
{
    signal(SIGINT, delay_handler);
    // signal(40, signal_handler);
    //sysv_signal(SIGINT, signal_handler);
    //bsd_signal(SIGINT, signal_handler);/*在cpp文件编译不通过,暂时不知道原因,编译器提示换成ssignal(...)*/
    //ssignal(SIGINT, signal_handler); 

    while( true ){
        sleep(1);
    }

    return 0;
}

