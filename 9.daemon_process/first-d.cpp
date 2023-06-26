#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <iostream>

int main(int argc, char* argv[])
{
    int pid{};

    if( (pid = fork()) > 0 ){   /*1*/
        std::cout << "parrent pid = " << getpid() << ", ppid = " << getppid() << 
                ", pgid = " << getpgrp() << ", sid = " << getsid(getpid()) << "\n";
        std::cout << "new pid: " << pid << "\n";
        exit(0);
    }else if( 0 == pid ){

        setsid();   /*2*/

        if ((pid = fork()) > 0){    /*3*/

            std::cout << "child pid= " << getpid() << ", ppid = " << getppid() << 
                ", pgid = " << getpgrp() << ", sid = " << getsid(getpid()) << "\n";

            std::cout << "grandson pid = " << pid << "\n";

            exit(0);

        }else if (0 == pid){
            /*4*/
            umask(0);
            chdir("/");
            /*5*/
            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);
            /*6*/
            int i {open("/dev/null",O_RDONLY)}; /* 0 ===> stdin */
            i+= open("/home/wong/Linux_System_Program/9.daemon_process/d.log",O_WRONLY);     /*1 ===> stdout */
            /*不想要任何输出，就把标准输出重定向到 /dev/null */
            i+= open("/dev/null",O_RDWR);       /*2 ===> stderr */

            std::cout << "grandson pid= " << getpid() << ", ppid = " << getppid() << 
                ", pgid = " << getpgrp() << ", sid = " << getsid(getpid()) << "\n";

            while (true){
                std::cout << "i = " << i++ << "\n";
                sleep(1);
                //fflush(stdout);/*强制把缓冲区数据写入磁盘*/
            }

        }else{
            std::cout << "grandson fork error...\n";
        }

    }else{

        std::cout << "fork error...\n";
    }

    return 0;
}

