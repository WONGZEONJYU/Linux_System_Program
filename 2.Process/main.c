#include <stdio.h>
#include <unistd.h>

static int g_global = 0;

int main(int argc, char const *argv[])
{
    
    int pid = 0;

    printf("Hello World!\n");

    if ((pid = fork()) > 0){
        
        g_global = 1;
        usleep(100);
        printf("g_global = %d\n",g_global);

    }else{
        
        g_global = 10;
        printf("g_global = %d\n",g_global);
    }

    return 0;
}

