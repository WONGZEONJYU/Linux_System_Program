#include <cstdio>
#include <sys/types.h>
#include <unistd.h>

#define EXE "child.out"

int create_process(char* path, char* args[])
{
    int ret {fork()};
    
    if( 0 == ret ){
        execve(path, args, nullptr);
    }

    return ret;
}

void zero_str(char* s)
{
    while( s && *s ) *s++ = 0;
}

int main()
{
    char path[] {EXE};
    char arg1[] {"hello"};
    char arg2[] {"world"};
    char* args[]{path, arg1, arg2, nullptr};

    printf("%d begin\n", getpid()); 

    printf("%d child = %d\n", getpid(), create_process(const_cast<char*>(EXE), args));

    zero_str(path);
    zero_str(arg1);
    zero_str(arg2);

    printf("%d path = %s\n", getpid(), path);
    printf("%d arg1 = %s\n", getpid(), arg1);
    printf("%d arg2 = %s\n", getpid(), arg2);

    printf("%d end\n", getpid());

    return 0;
}
