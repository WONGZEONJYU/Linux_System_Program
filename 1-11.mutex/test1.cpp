#include <iostream>
#include <thread>

using namespace std;
using namespace chrono;
using namespace this_thread;

int g_count {};

void* thread_entry(void* arg)
{  
    (void)arg;

    for (size_t i {}; i < 10000; i++){
        ++g_count;
    }

    pthread_detach(pthread_self());

    return nullptr;
}

int main(int argc, char const *argv[])
{
    for(int r{}; r < 5; r++){
        pthread_t t {};
        pthread_create(&t, nullptr, thread_entry, nullptr);
    }

    sleep_for(5s);
    
    cout << "g_count = " << g_count << "\n";

    return 0;
}

