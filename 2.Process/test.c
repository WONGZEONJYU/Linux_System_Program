#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define EXE "helloworld.out"

int main(int argc, char const *argv[])
{
    char* args[] = {EXE,NULL};

    printf("begin\n");

    printf("pid = %d\n",getpid());

    execve(EXE,args,NULL);

    printf("end\n");

    return 0;
}
