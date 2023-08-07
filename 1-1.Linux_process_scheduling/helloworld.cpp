#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/types.h>
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
            ", pgid = " << getpgrp() << '\n';

    cout << "hello world\n";

    while (true){
        sleep(1);
    }

    return 0;
}


