#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>

static int g_global{};

int main(int argc, char *argv[])
{
    std::cout << "Hello World!\n";
    std::cout << "current = " << getpid() << "!\n";
    int pid {};

    if ((pid = fork()) > 0){
        
        g_global = 1;
        //sleep(60);
        usleep(100);
        std::cout << "child = " << pid << "!\n";
        std::cout << "child pid = " << getpid() << " g_global = " << g_global << "\n";
    }else{
        g_global = 10;
        std::cout << "parent = " << getppid() << "!\n";
        std::cout << "current pid = " << getpid() << " g_global = " << g_global << "!\n";
    }
    return 0;
}
