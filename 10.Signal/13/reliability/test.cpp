#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>

int main(int argc, const char* argv[])
{
    const int pid {atoi(argv[1])},sig {atoi(argv[2])};

    std::cout << "current pid (" << getpid() << ") ...\n";
    std::cout << "send sig(" << sig << ") to process(" << pid << ")...\n";

    constexpr sigval sv {1234567};

    for (int i {}; i < 20000; i++){
        sigqueue(pid,sig,sv);
    }

    return 0;
}

