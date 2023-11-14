#include <cstring>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <math.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>

using namespace std;

#define PATH_NAME "."
#define PROJ_ID 88

int main(int argc, char const *argv[])
{
    const auto k {ftok(PATH_NAME,PROJ_ID)};

    const auto shmid {shmget(k,128,IPC_CREAT | S_IRWXU)};

    cout << "shmid = " << shmid << "\n";

    if (-1 == shmid){
        cerr << "shmget error\n";
        exit(-1);
    }

    const auto pid {fork()};

    if (pid > 0){

        cout << "pid = " << getpid() << ", ppid = " 
            << getppid() << ", pgid = " << getpgrp() << "\n";

        auto shmaddr{static_cast<char*>(shmat(shmid,nullptr,0))};

        waitpid(pid,nullptr,0);

        cout << shmaddr << "\n";

    }else if(!pid){

        cout << "pid = " << getpid() << ", ppid = " 
            << getppid() << ", pgid = " << getpgrp() << "\n";

        auto shmaddr{static_cast<char*>(shmat(shmid,nullptr,0))};

        strcpy(shmaddr,"Hello world");

        exit(0);
    }else{
        cerr << "child process create error...\n";
    }
    
    shmctl(shmid,IPC_RMID,nullptr);

    return 0;
}
