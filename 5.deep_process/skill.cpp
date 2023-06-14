
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

static void worker(pid_t pid)
{
    std::cout << "grand-child: " << pid << "\n";
    sleep(150);
}

int main(int argc, char* argv[])
{   
    std::cout << "parent pid = " << getpid() << "\n";

    pid_t pid {fork()};

    if( pid < 0 ){
        std::cout << "fork error\n"; 
    }else if( 0 == pid ){
 
        for(int i {}; i < 5; ++i){
            if( 0 == (pid = fork()) ) {
                worker(getpid());
                break;
            }
        }

        sleep(60);

        std::cout << "child pid (" << getpid() << ") is over...\n";

    }else{

        std::cout << "wait child pid = " << pid << "\n";

        sleep(120);

        int status {};

        while( waitpid(pid, &status, 0) == pid ){

            std::cout << "Parent is over - child pid: " << pid << ", status = " << std::hex << status << "\n";
        }
    }

    return 0;
}
