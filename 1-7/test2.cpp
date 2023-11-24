#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <iostream>
#include <thread>

using namespace std;

#if 0
pid_t gettid()
{
    return syscall(SYS_gettid);
}
#endif

static void* thread_entry(void*)
{   
    cout << "thread: " << pthread_self() << "\n" << fflush;

    for(;;){
        const auto pid {getpid()},tid {gettid()};
        cout << "pid = " << pid << ", tid = " << tid << "\n";
        sleep(5);
    }
    
    return nullptr;
}

int main(int argc, char const *argv[])
{
    cout << "main:pid = " << getpid() << "\n" <<
        "main:tid = " << gettid() << "\n";

    for(int i {}; i<4; i++){
        pthread_t t{};
        pthread_create(&t, nullptr, thread_entry, nullptr);
    }

    for(;;){
        sleep(1);
    }

    return 0;
}

