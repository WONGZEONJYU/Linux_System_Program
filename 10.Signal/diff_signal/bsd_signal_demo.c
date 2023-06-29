// #define _GNU_SOURCE 
#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200800L

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

void delay_handler(int sig)
{
    printf("begin %s...\n",__FUNCTION__);

    for (int i = 0; i < 5; i++){
        printf("sleep %d ...\n",i);
        sleep(1);
    }

    printf("end %s...\n",__FUNCTION__);
}

void signal_handler(int sig)
{
    printf("%s : sig = %d\n",__FUNCTION__,sig);
}

int main(void)
{
    bsd_signal(SIGINT, delay_handler);

    bsd_signal(40, signal_handler);

    for(;;){
        sleep(1);
    }

    return 0;
}

