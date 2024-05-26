#include <iostream>
#include <thread>

using namespace std;
using namespace chrono;
using namespace this_thread;

static pthread_mutex_t m1 (PTHREAD_MUTEX_INITIALIZER);
static pthread_mutex_t m2 (PTHREAD_MUTEX_INITIALIZER);

static int g_count {};

void live_lock(pthread_mutex_t* pm[], const int n)
{
    int r = -1;
    
    while( r ){
        for(int i{}; i < n; i++){
            timespec tout {};
            clock_gettime(CLOCK_REALTIME, &tout);
            tout.tv_sec += 1; 
            r = pthread_mutex_timedlock(pm[i], &tout);

            if( r ){
                for(int j{}; j < i; j++) {
                    pthread_mutex_unlock(pm[j]);
                }
                sleep_for(100ms);
                break;
            }
        }
    }
}

void live_unlock(pthread_mutex_t* pm[], const int n)
{
    for (int i {}; i < n; i++){
        pthread_mutex_unlock(pm[i]);
    }
}

void* thread_1(void* )
{  
    pthread_mutex_t* pm[] {&m1, &m2};
    constexpr auto n {sizeof(pm)/sizeof(*pm)};

    for(;;){
        live_lock(pm, n);

        cout << __FUNCTION__ << " : " << g_count++ << "\n";
        
        sleep_for(100us);

        live_unlock(pm, n);
    }
    
    pthread_detach(pthread_self());
    
    return nullptr;
}

void* thread_2(void* )
{  
    pthread_mutex_t* pm[] {&m2, &m1};
    constexpr auto n {sizeof(pm)/sizeof(*pm)};
    
    for(;;){
        live_lock(pm, n);

        cout << __FUNCTION__ << " : " << g_count++ << "\n";
        
        sleep_for(100us);

        live_unlock(pm, n);
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
