#include <ctime>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <iostream>

using namespace std;

void* thread_entry(void* arg)
{
    const auto id {pthread_self()};
    const auto n{reinterpret_cast<long long>(arg)};
    for (size_t i {}; i < n; i++){
        
        cout << "id = " << id << " i = " << i << "\n" << flush;
        this_thread::sleep_for(1s);
    }

    return nullptr;
}

int main(int argc, char const *argv[])
{
    pthread_t t1{},t2{};
    long long arg1{10},arg2{5};
    pthread_create(&t1,nullptr,thread_entry,reinterpret_cast<void*>(arg1));
    pthread_create(&t2,nullptr,thread_entry,reinterpret_cast<void*>(arg2));
    pthread_join(t1,nullptr);
    pthread_join(t2,nullptr);

    cout << "t1 = " << t1 << "\nt2 = " << t2 << "\n";

    return 0;
}

