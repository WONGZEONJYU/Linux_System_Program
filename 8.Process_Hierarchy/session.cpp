#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <iostream>

int main(void)
{
    int pid{};

    if( (pid = fork()) > 0 ){
        std::cout << "parrent pid = " << getpid() << ", ppid = " << getppid() << 
                ", pgid = " << getpgrp() << ", sid = " << getsid(getpid()) << "\n";
        std::cout << "new: " << pid << "\n";
    }else if( pid == 0 ){
        setsid();
        //sleep(180);
        sleep(3);
        std::cout << "child = " << getpid() << ", ppid = " << getppid() << 
                ", pgid = " << getpgrp() << ", sid = " << getsid(getpid()) << "\n";
    }else{

        std::cout << "fork error...\n";
    }

    sleep(240);
    
    return 0;
}
