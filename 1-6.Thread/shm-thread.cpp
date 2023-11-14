#include <iostream>
#include <thread>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

using namespace std;

void* thread_entry(void*arg)
{
    cout << "begin " <<__FUNCTION__ << "\n"
        << "pid = " << getpid() << ", ppid = " 
        << getppid() << ", pgid = " << getpgrp() << "\n";

    auto shmaddr {static_cast<char*>(arg)};

    strcpy(shmaddr,"Hello World\n");

    cout << "end " <<__FUNCTION__ << "\n";
    return {};
}

int main(int argc, char const *argv[])
{
    char* mem {};

    try{
        mem = new char[128];
    }catch(const std::bad_alloc& e){
        cerr << e.what() << '\n';
        exit(-1);
    }
    
    cout << "mem = 0x" << reinterpret_cast<long long>(mem) << "\n";

    pthread_t child{};
    
    const auto r{pthread_create(&child,nullptr,thread_entry,mem)};

    if (!r){
        cout << "pid = " << getpid() << ", ppid = " 
            << getppid() << ", pgid = " << getpgrp() << "\n";

        auto shmaddr {mem};
        pthread_join(child,nullptr);
        cout << mem << "\n";
    }else{
        cerr << "create thread error...\n";
    }

    delete []mem;
    mem = nullptr;
    return 0;
}
