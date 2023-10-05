#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sched.h>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    cout << "child = " << getpid() << 
            ", ppid = " << getppid() << 
            ", pgid = " << getpgrp() << 
            "\nhello world\n";

    for(;;){
    }

    return 0;
}


