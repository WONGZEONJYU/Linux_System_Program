#include <cstdio>
#include <sys/types.h>
#include <unistd.h>

constexpr char EXE[] {"helloworld.out"};

int create_process(const char* path,char* args[])
{
    int ret {fork()};

    if (0 == ret){
        execve(path,args,nullptr);
    }

    return ret;
}

int main(int argc, char const *argv[])
{
    char* args[] {const_cast<char*>(EXE),nullptr};

    printf("begin\n");

    printf("pid = %d\n",getpid());

    // execve(EXE,args,nullptr);

    printf("child pid = %d\n",create_process(EXE,args));

    printf("end\n");

    return 0;
}
