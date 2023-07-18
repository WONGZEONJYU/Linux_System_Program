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

    constexpr int special[] {SIGSTOP,SIGKILL,SIGSEGV,SIGBUS,SIGILL,
                            SIGTRAP,SIGFPE,SIGSYS,32,33}; /*用于排除特殊信号*/
    constexpr auto slen {sizeof(special) / sizeof(*special)};

    constexpr int hight[] {SIGSEGV,SIGBUS,SIGILL,SIGTRAP,SIGFPE,SIGSYS}; /*优先级最高的信号*/
    constexpr auto hlen {sizeof(hight)/sizeof(*hight)};

    srand(time(nullptr));   /*用时间（单位秒）去初始化随机种子*/

    for (int i {}; i < num; i++){   /*先发送随机产生的信号*/

        int sig{};
        do{
            sig = rand() % 64 + 1; //随机产生信号
        } while (find(special,slen,sig));

        std::cout << "send sig[" << (i + 1) << "](" << 
            sig << ") to process(" << pid << ")...\n";

        const sigval sv {i+1};
        sigqueue(pid,sig,sv);
    }

    std::cout << "\n";

    /*发送优先级最高的信号(硬件相关的不可靠信号)*/
    for (int i {}; i < hlen; i++){  
        
        const auto sig {hight[i]};
        std::cout << "sig[" << (-(i+1)) << "] = " << sig << "\n";
        const sigval sv {-(i+1)};/*用负数便于观察和区分*/
        sigqueue(pid,sig,sv);
    }

    return 0;
}


