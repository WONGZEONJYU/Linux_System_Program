// #define _GNU_SOURCE 
#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200800L

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

void signal_handler(int sig)
{
    printf("%s : sig = %d\n",__FUNCTION__,sig);
}

int r_read(char *data,const int len)
{
    int ret = -1;

    while ( data && 
        ( -1 == ( ret = read( STDIN_FILENO,data,len - 1 ) ) ) &&
        (EINTR == errno) ){
        printf("restart syscall manually...\n");
    }

    if (-1 != ret){
        data[len] = 0;
    }
    
    return ret;
}

int main(void)
{
    //signal(SIGINT, signal_handler);
    //sysv_signal(SIGINT, signal_handler);
    bsd_signal(SIGINT, signal_handler);
    char buf[32] = {0};
    r_read(buf,sizeof(buf));
    printf("input: %s\n",buf);

    return 0;
}

