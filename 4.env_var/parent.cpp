#include <cstdio>
#include <sys/types.h>
#include <unistd.h>

#define EXE "child.out"

int create_process(char* path, char* args[], char* env[])
{
    int ret {fork()};
    
    if( 0 == ret ){
        execve(path, args, env);
    }

    return ret;
}

int main()
{
    char path[]{EXE},arg1[]{"hello"},arg2[]{"world"};

    char* args[] {path, arg1, arg2, nullptr};

    printf("%d begin\n", getpid()); 
    
    printf("%d child = %d\n", getpid(), create_process(const_cast<char*>(EXE), args,args));   

    printf("%d end\n", getpid());

    return 0;
}
