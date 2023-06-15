#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

constexpr char EXE[] {"helloworld.out"};

int create_process(const char* path,char* const args[],char* const env[],const int wait)
{
    int ret {fork()};

    if (0 == ret){

        if (-1 == execve(path,args,env)){
            exit(-1);
        }
    }

    if (wait && ret){   /*等待就返回子进程状态，不等待返回子进程PID*/
        waitpid(ret,&ret,0);
    }

    return ret;
}

int main(int argc, char *argv[])
{
    char* target {argv[1]};
    char* const ps_argv[] {target,nullptr};
    char* const ps_envp[] {const_cast<char*>("PATH=/bin:/usr/bin"),const_cast<char*>("TEST=WONG"),nullptr};

    if (argc < 2){
        exit(-1);
    }

    std::cout << "current pid: " << getpid() << "\n";

    int result {create_process(target,ps_argv,ps_envp,1)};

    std::cout << "result = " << result << "\n";

    return 0;
}
