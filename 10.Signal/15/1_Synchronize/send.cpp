#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>

int main(int argc, char const *argv[])
{
    const auto pid {atoi(argv[1])};
    const auto sig {atoi(argv[2])};
    const auto num {atoi(argv[3])};

    std::cout << "current pid(" << getpid() << ") ...\n";
    std::cout << "send sig(" << sig << ") to process(" << pid << ")...\n";

    for (int i {}; i < num; i++){
        const sigval sv{.sival_int = i};
        sigqueue(pid,sig,sv);
    }

    return 0;
}


