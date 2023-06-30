#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void signal_handler(int sig)
{
    std::cout << __FUNCTION__ << " : sig = " << sig << "\n";
}

int r_read(char *data,const int len)
{
    int ret {-1};

    while ( data && 
        ( -1 == ( ret = read( STDIN_FILENO,data,len - 1 ) ) ) &&
        (EINTR == errno) ){
        std::cout << "restart syscall manually...\n";
    }

    if (-1 != ret){
        data[len] = 0;
    }

    return ret;
}

int main(int argc,char* argv[])
{
    // signal(SIGINT, signal_handler);
    // signal(40, signal_handler);
    sysv_signal(SIGINT, signal_handler);
    //bsd_signal(SIGINT, signal_handler);/*在cpp文件编译不通过,暂时不知道原因,编译器提示换成ssignal(...)*/

    char buf[32]{};
    r_read(buf,sizeof(buf));
    std::cout << "input: " << buf << "\n";

    return 0;
}

