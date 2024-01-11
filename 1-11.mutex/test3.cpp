#include <iostream>
#include <thread>

using namespace std;
using namespace chrono;
using namespace this_thread;

int g_count {};

void* thread_entry(void* arg)
{  
    auto pm {static_cast<pthread_mutex_t*>(arg)};
    int i{};

    while(i < 10000){

        if( pthread_mutex_trylock(pm)){
            continue;
        }

        ++g_count;

        pthread_mutex_unlock(pm);

        ++i;
    }

    pthread_detach(pthread_self());

    return nullptr;
}

int main(int argc, char const *argv[])
{
    pthread_mutex_t mutex{};
    pthread_mutexattr_t mattr{};

    auto r {pthread_mutexattr_init(&mattr)};
    r = pthread_mutex_init(&mutex, &mattr);

    for(int i{}; i < 5; i++){
        pthread_t t {};
        pthread_create(&t, nullptr, thread_entry, &mutex);
    }

    sleep_for(5s);
    
    cout << "g_count = " << g_count << "\n";

    return 0;
}
