#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;
using namespace chrono;
using namespace this_thread;

void* thread_entry(void* arg)
{
    const auto pt{gettid()};

    const auto n{reinterpret_cast<int64_t>(arg)};

    for (int64_t i {}; i < n; i++){
        cout << "thread(" << pt << ") : i = " << i << "\n" << flush;
        sleep_for(1s);
    }

    return reinterpret_cast<void*>(pt);
}

void* wait_entry(void* arg)
{
    const auto pt{gettid()};
    
    const auto t{reinterpret_cast<pthread_t>(arg)};

    sleep_for(1s);

    cout << "begin(" << pt << ")\n" << flush;
    const auto r {pthread_join(t,nullptr)};
    cout << "end(" << pt << ")\n" << flush;
    
    cout << "thread(" << pt << ") r = " << r << "\n";
    return nullptr;
}

int main(int argc, char const *argv[])
{
    cout << "main(" << getpid() << ")\n";
    pthread_t t{},wt{};
    pthread_create(&t,nullptr,thread_entry,reinterpret_cast<void*>(5));
    pthread_create(&wt,nullptr,wait_entry,reinterpret_cast<void*>(t));
    pthread_create(&wt,nullptr,wait_entry,reinterpret_cast<void*>(t));

    for(;;){
        sleep_for(6s);
    }

    return 0;
}
