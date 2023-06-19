#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <iostream>

int main()
{
    std::cout << "parrent pid = " << getpid() << 
        ", ppid = " << getppid() << ", pgid = " << getpgrp() << "\n";

    int pid{};

    if( (pid = fork()) > 0 ){
        sleep(1);
        int r {setpgid(pid, pid)};
        std::cout << "new pid: " << pid << " , r = " << r << "\n";
    }else if( 0 == pid ){

        auto out {const_cast<char*>("./helloworld.out")};
        char* const ps_argv[] {out,nullptr};
        char* const ps_envp[] {const_cast<char*>("PATH:/bin:/usr/bin"),nullptr};
        execve(out,ps_argv,ps_envp);
        // setpgid(pid, pid);
        // sleep(1);
        // std::cout << "child pid = " << getpid() << 
        //     ", ppid = " << getppid() << ", pgid = " << getpgrp() << "\n";
    }else{
        std::cout << "fork error...\n";
    }  

    sleep(60);
    return 0;
}
