#include <cstdio>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    sleep(3);
    for(int i {}; i < argc; i++){
        printf("exec = %d, %s\n", getpid(), argv[i]);
    }

    return 0;
}
