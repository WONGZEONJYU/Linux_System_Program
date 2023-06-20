#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

#define EXE "helloworld.out"

int main(int argc, char const *argv[])
{
    char* args[] = {const_cast<char*>(EXE),NULL};

    std::cout << "begin\n";

    std::cout << "pid = " << getpid() << "\n";

    execve(EXE,args,NULL);

    printf("end\n");

    return 0;
}
