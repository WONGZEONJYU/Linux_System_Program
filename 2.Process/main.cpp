#include <cstdio>
#include <unistd.h>

static int g_global {};

int main(int argc, char const *argv[])
{
    int pid {};

    printf("Hello World!\n");
    printf("current = %d!\n",getpid());

    if ((pid = fork()) > 0){
        
        g_global = 1;
        usleep(100);
        printf("child = %d!\n",pid);
        printf("%d g_global = %d\n",getpid(),g_global);
    }else{
        
        g_global = 10;
        printf("parent = %d!\n",getppid());
        printf("%d g_global = %d\n",getpid(),g_global);
    }

    return 0;
}
