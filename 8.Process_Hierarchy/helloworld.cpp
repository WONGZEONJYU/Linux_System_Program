#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

int main(int argc, char *argv[])
{
    sleep(5);
    std::cout << "child pid = " << getpid() << 
        " ppid = " << getppid() << " pgid = " << getpgrp() << "\n";

    std::cout << "Hello World\n";
    sleep(30);
    return 0;
}
