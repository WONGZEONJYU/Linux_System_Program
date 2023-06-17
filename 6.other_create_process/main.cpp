#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[])
{   

    std::cout << "parent = " << getpid() << "\n";    
    
    pid_t pid {};
    int var {88};

    if( (pid = vfork()) < 0 ){
        //printf("vfork error\n");
        std::cout << "vfork error\n";
    }
    else if( 0 == pid ){
        std::cout << "pid = " <<  getpid() << ", var = " << var << "\n";
        var++;
        std::cout << "pid = " <<  getpid() << ", var = " << var << "\n";
        // return 0; /* destroy parent stack frame */
        _exit(0);
    }

    std::cout << "parent = " << getpid() << ", var = " << var << "\n";

    return 0;
}
