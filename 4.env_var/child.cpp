#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[],char* env[])
{
    sleep(1);

    printf("process parameter\n");

    for(int i {};i < argc;++i){
        printf("exec = %d, %s\n", getpid(), argv[i]);
    }

    printf("environment list:\n");

    int i {};
    while (env[i]){
        printf("exec = %d, %s\n",getpid(),env[i++]);
    }
    

    return 0;
}
