#include <iostream>
#include <thread>

using namespace std;
using namespace chrono;
using namespace this_thread;

static int g_count {};
static pthread_mutex_t g_mutex{};
static pthread_spinlock_t g_spin{};

void* thread_1(void*)
{
    for(;;){

        pthread_spin_lock(&g_spin);
        //pthread_mutex_lock(&g_mutex);

        cout << __FUNCTION__ << " : " << g_count++ << "\n" << flush;

        sleep_for(100ms);

        pthread_spin_unlock(&g_spin);
        //pthread_mutex_unlock(&g_mutex);
    }

    pthread_detach(pthread_self());
    return nullptr;
}

void* thread_2(void*)
{
    for(;;){

        pthread_spin_lock(&g_spin);
        //pthread_mutex_lock(&g_mutex);

        cout << __FUNCTION__ << " : " << g_count++ << "\n" << flush;

        sleep_for(100ms);

        pthread_spin_unlock(&g_spin);
        //pthread_mutex_unlock(&g_mutex);
    }
    pthread_detach(pthread_self());
    return nullptr;
}

int main(int argc, char const *argv[])
{
    pthread_mutexattr_t mattr{};
    pthread_mutexattr_init(&mattr);
    pthread_mutex_init(&g_mutex, &mattr);

    pthread_spin_init(&g_spin,PTHREAD_PROCESS_PRIVATE);
    pthread_t t{};
    pthread_create(&t,nullptr,thread_1,nullptr);
    pthread_create(&t,nullptr,thread_2,nullptr);
    cout << "Hello World\n";

    for(;;){
        sleep_for(1s);
    }

    return 0;
}

