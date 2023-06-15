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

    sleep(120);

    while ((pid = wait(&status)) > 0){

        if (WIFEXITED(status)){
            std::cout << "Normal - child pid: " << pid << ", " << 
                "code: " << std::dec << static_cast<int>(static_cast<char>(WEXITSTATUS(status))) << "\n";
        }else if(WIFSIGNALED(status)){
            std::cout << "Signaled - child pid: " << pid << ", " << 
                "code: " << WTERMSIG(status) << "\n";
        }else{
            std::cout << "Paused - child pid: " << pid << ", " << 
                "code: " << WSTOPSIG(status) << "\n";
        }
    }

    return 0;
}

