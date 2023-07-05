#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <cstring>
#include <time.h>

static bool find(const int* arr,const int len,const int v)
{
    bool ret {};

    for (int i {}; i < len; i++){

        ret = ret || (v == arr[i]);
    }

    return ret;
}

int main(int argc, const char* argv[])
{
    std::cout << "current pid(" << getpid() << ")...\n";

    const auto pid {atoi(argv[1])},     //process pid
    num {atoi(argv[2])};                //define the number of signal

    constexpr int special[] {SIGSTOP,SIGKILL,32,33};

    constexpr auto slen {sizeof(special) / sizeof(*special)};
    
    srand(time(nullptr));

    for (int i {}; i < num; i++){

        int sig{};
        do{
            sig = rand() % 33 + 32; //产生可靠信号
        } while (find(special,slen,sig));

        std::cout << "send sig[" << (i + 1) << "](" << sig << ") to process(" << pid << ")...\n";

        const sigval sv {i+1};
        sigqueue(pid,sig,sv);
    }

    return 0;
}


