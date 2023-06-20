#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <iostream>

int main(int argc,char* argv[])
{
    int pid{};

    if( (pid = fork()) > 0 ){
        std::cout << "parrent pid = " << getpid() << ", ppid = " << getppid() << 
                ", pgid = " << getpgrp() << ", sid = " << getsid(getpid()) << "\n";
        std::cout << "new: " << pid << "\n";
        exit(0);
    }else if( pid == 0 ){
        setsid();

        sleep(60);

        int i {open(argv[1],O_RDONLY)}; /* 0 ===> stdin 以只读方式打开设备，从设备读取数据 == 从设备输入数据*/
        i+= open(argv[1],O_WRONLY);     /*1 ===> stdout 以只写方式打开设备，向设备写入数据 == 向设备输出数据*/
        i+= open(argv[1],O_RDWR);       /*2 ===> stderr 以读写方式打开设备，意味着就是标准错误*/

        std::cout << "child = " << getpid() << ", ppid = " << getppid() << 
                ", pgid = " << getpgrp() << ", sid = " << getsid(getpid()) << "\n";
        
        std::cout << "i = " << i << "\n";
    }else{

        std::cout << "fork error...\n";
    }

    sleep(240);
    
    return 0;
}
