#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <cstring>

struct Message
{
    short type,length;
    char data[];
};

int main(int argc, const char* argv[])
{
    const auto pid {atoi(argv[1])},sig {atoi(argv[2])};

    auto data {argv[3]};

    const auto len {sizeof(Message) + strlen(data) + 1};

    const auto size {((len / 4) + !!(len % 4)) * 4}; // 4字节对齐

    auto msg {reinterpret_cast<Message*>(malloc(size))};

    if (msg){

        std::cout << "current pid (" << getpid() << ") ...\n";
        std::cout << "send sig(" << sig << ") to process(" << pid << ")...\n";
        
        msg->type = 0;
        msg->length = len - sizeof(Message);

        strcpy(msg->data,data);

        auto pi {reinterpret_cast<const int*>(msg)};

        for (int i {}; i < size; i+=4){
            sigval sv {*pi++};
            sigqueue(pid,sig,sv);
        }
    }

    free(msg);

    return 0;
}

