#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>

int main(int argc, char* argv[])
{
    int pid {atoi(argv[1])},sig {atoi(argv[2])};

    std::cout << "send sig(" << sig << ") to process(" << pid << ")...\n";

    kill(pid, sig);

    raise(SIGINT);

    while( true ){
        std::cout << "while...\n";
        sleep(1);
    }

    return 0;
}

