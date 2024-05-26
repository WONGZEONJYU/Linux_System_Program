#include <iostream>
#include <thread>

using namespace std;
using namespace chrono;
using namespace this_thread;

void* thread_entry(void* arg)
{  
    auto pm {static_cast<pthread_mutex_t*>(arg)};
    
    sleep_for(1s);
    
    pthread_mutex_lock(pm);

    cout << __FUNCTION__ << "\n";
        
    pthread_mutex_unlock(pm);
    
    pthread_detach(pthread_self());
    
    return nullptr;
}

int main(int argc, char const *argv[])
{
    pthread_mutex_t mutex{};
    pthread_mutexattr_t mattr{};

    pthread_mutexattr_init(&mattr);

    //pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_ERRORCHECK);

    pthread_mutex_init(&mutex,&mattr);

    pthread_t t{};
    pthread_create(&t, nullptr, thread_entry, &mutex);

    auto r {pthread_mutex_lock(&mutex)};
    cout << "r = " << r << "\n";

    r = pthread_mutex_lock(&mutex);
    cout << "r = " << r << "\n";
    
    cout << "hello world\n";
    
    pthread_mutex_unlock(&mutex);
    //pthread_mutex_unlock(&mutex);

    for(;;){
        sleep_for(1s);
    }

    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&mattr);

    return 0;
}
