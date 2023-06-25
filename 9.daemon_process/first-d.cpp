#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <iostream>

int main(int argc, char* argv[])
{

    int pid{};

    if( (pid = fork()) > 0 ){
        std::cout << "parrent pid = " << getpid() << ", ppid = " << getppid() << 
                ", pgid = " << getpgrp() << ", sid = " << getsid(getpid()) << "\n";
        std::cout << "new: " << pid << "\n";
        exit(0);
    }else if( 0 == pid ){

        setsid();


        if ((pid = fork()) > 0)
        {
            
        }else if (0 == pid){

            unmask(0);
            chdir("/");

            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);

            int i {open("/dev/null",O_RDONLY)}; /* 0 ===> stdin */
            i+= open("/dev/null",O_WRONLY);     /*1 ===> stdout */
            i+= open("/dev/null",O_RDWR);       /*2 ===> stderr */

        }else{
            std::cout << "grandson fork error...\n";
        }

        std::cout << "child = " << getpid() << ", ppid = " << getppid() << 
                ", pgid = " << getpgrp() << ", sid = " << getsid(getpid()) << "\n";
        
    }else{

        std::cout << "fork error...\n";
    }

    sleep(240);

    
    return 0;
}

