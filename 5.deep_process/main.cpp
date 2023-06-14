#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <iomanip>

int main(int argc, char const *argv[])
{
    pid_t pid {};
    int a{1},b{},status{};
    
    std::cout << "parent pid = " << getpid() << "\n";

    if (0 == (pid = fork())){
        exit(-1);
    }

    std::cout << "child pid = " << pid << "\n";

    if(0 == (pid = fork())){
        abort();
    }
    
    std::cout << "child pid = " << pid << "\n";

    if(0 == (pid = fork())){
        a /= b;
        exit(1);
    }

    std::cout << "child pid = " << pid << "\n";

    sleep(3);

    while ((pid = wait(&status)) > 0){

        std::cout << "child pid: " << std::dec << pid << "," 
                    << "status = " << std::hex << status << "\n";
        //printf("child pid: %d, status = %x\n",pid,status);
    }

    return 0;
}


