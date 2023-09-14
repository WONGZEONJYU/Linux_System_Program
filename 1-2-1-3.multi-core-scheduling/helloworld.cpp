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

static void* thread_entry(void* args)
{
    for(;;){

    }
    return args;
}

static void make_thread()
{
    pthread_t tid {};
    pthread_create(&tid,nullptr,thread_entry,nullptr);
}

int main(int argc, char const *argv[])
{
    cout << "child = " << getpid() << 
            ", ppid = " << getppid() << 
            ", pgid = " << getpgrp() << 
            "\nhello world\n";

    make_thread();

    for(;;){
        //sleep(1);
    }

    return 0;
}


