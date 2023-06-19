#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

int main(int argc,char* argv[])
{
    std::cout << "parent pid = " << getpid() << ", ppid = " << getppid() << ", pgid = " << getpgrp() << "\n";

    int pid{},i{};

    while( i < 5 ) {
        if( (pid = fork()) > 0 ){
            std::cout << "new pid: " << pid << "\n";
        }else if( 0 == pid ){
            sleep(1);
            std::cout << "child pid = " << getpid() << ", ppid = " << getppid() << ", pgid = " << getpgrp() << "\n";
            sleep(60);
            std::cout << "last -- pgid = " << getpgrp() << "\n";
            break;
        }else{
            std::cout << "fork error...\n";
        }
        i++;
    }

    if( pid ){
        sleep(60);
    }

    return 0;
}
