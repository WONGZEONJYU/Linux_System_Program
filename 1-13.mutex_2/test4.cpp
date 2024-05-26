#include <iostream>
#include <thread>

using namespace std;
using namespace chrono;
using namespace this_thread;

static pthread_mutex_t m1 (PTHREAD_MUTEX_INITIALIZER);
static pthread_mutex_t m2 (PTHREAD_MUTEX_INITIALIZER);

static int g_count {};

void* thread_1(void* )
{  
    for(;;){
        pthread_mutex_lock(&m1);
        pthread_mutex_lock(&m2);

        cout << __FUNCTION__ << " : " << g_count++ << "\n";

        sleep_for(100us);
            
        pthread_mutex_unlock(&m1);
        pthread_mutex_unlock(&m2);
    }
    
    pthread_detach(pthread_self());
    
    return nullptr;
}

void* thread_2(void* )
{  
    for(;;){
        pthread_mutex_lock(&m2);
        pthread_mutex_lock(&m1);

        cout << __FUNCTION__ << " : " << g_count++ << "\n";

        sleep_for(100us);

        pthread_mutex_unlock(&m1);
        pthread_mutex_unlock(&m2);
    }
    
    pthread_detach(pthread_self());

    return nullptr;
}

int main(int argc, char const *argv[])
{
    pthread_t t{};
    
    pthread_create(&t, nullptr, thread_1, nullptr);
    pthread_create(&t, nullptr, thread_2, nullptr);

    cout << "Hello World!\n";
    
    for(;;){
        sleep_for(1s);
    }
    
    return 0;
}
