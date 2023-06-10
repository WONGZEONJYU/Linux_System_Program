#include <cstdio>
#include <sys/types.h>
#include <unistd.h>

constexpr char EXE[] {"helloworld.out"};

int main(int argc, char const *argv[])
{
    char* args[] {const_cast<char*>(EXE),nullptr};

    printf("begin\n");

    printf("pid = %d\n",getpid());

    execve(EXE,args,nullptr);

    printf("end\n");

    return 0;
}
