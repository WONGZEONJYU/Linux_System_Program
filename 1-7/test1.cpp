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
    const auto pid {getpid()};
    const auto tid {gettid()};
    const auto self {pthread_self()};
    
    cout << "thread:pid = " << pid << "\n";
    cout << "thread:tid = " << tid << "\n";
    cout << "thread:self = " << self << "\n";
    return nullptr;
}

int main(int argc, char const *argv[])
{
    const auto self {pthread_self()};
    const auto pid {getpid()};
    const auto tid {gettid()};
    
    cout << "main:pid = " << pid << "\n";
    cout << "main:tid = " << tid << "\n";
    cout << "main:self = " << self << "\n";

    pthread_t t {};
    pthread_create(&t, nullptr, thread_entry, nullptr);
    
    cout << "main:t = " << t << "\n";

    pthread_join(t, nullptr);
    
    return 0;
}
