// #define _GNU_SOURCE 
#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200800L

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void signal_handler(int sig)
{
    printf("handler : sig = %d\n", sig);
}

int main(void)
{
    //signal(SIGINT, signal_handler);
    //signal(40, signal_handler);
    //sysv_signal(SIGINT, signal_handler);
    bsd_signal(SIGINT, signal_handler);

    while( 1 )
    {
        sleep(1);
    }
    
    return 0;
}
