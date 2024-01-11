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
        cout << "thread(" << pt << ") : i = " << i << " , &i = " << &i << "\n" << flush;
        sleep_for(1s);
    }

    pthread_detach(pthread_self());

    return reinterpret_cast<void*>(pt);
}

int main(int argc, char const *argv[])
{
    cout << "main(" << getpid() << ")\n";

    pthread_t t{};
    // pthread_attr_t attr{};
    // pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    // pthread_create(&t,&attr,thread_entry,reinterpret_cast<void*>(5));

    pthread_create(&t,nullptr,thread_entry,reinterpret_cast<void*>(5));

    cout << "pthread_t = " << t << "\n";
    sleep_for(10s);
    
    void* tr{};
    auto r{pthread_join(t,&tr)};
    cout << "r = " << r << " , tr = " << reinterpret_cast<int64_t>(tr) << "\n";

    pthread_create(&t,nullptr,thread_entry,reinterpret_cast<void*>(5));
    cout << "pthread_t = " << t << "\n";

    sleep_for(10s);

    r = pthread_join(t,&tr);
    cout << "r = " << r << " , tr = " << reinterpret_cast<int64_t>(tr) << "\n";

    return 0;
}
