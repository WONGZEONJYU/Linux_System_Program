#include <iostream>
#include <thread>

using namespace std;
using namespace chrono;
using namespace this_thread;

void clean_test(void* arg)
{
    cout << __FUNCTION__ << " : " << reinterpret_cast<long long>(arg) << "\n";
}

void foo()
{
    cout << __FUNCTION__ << " begin\n";
    cout << "I'm " << __FUNCTION__ << "\n" << flush;
    cout << __FUNCTION__ << " end\n";
}

void* thread_entry(void* arg)
{
    (void)arg;
    for (size_t i {}; i < 20; i++){
        cout << "thread => " << i << "\n" << flush;
        sleep_for(1s);
    }
    
    return nullptr;
}

int main(int argc, char const *argv[])
{
    pthread_t t{};
    pthread_create(&t,nullptr,thread_entry,nullptr);
    //pthread_exit(nullptr);
    exit(0);

    for (size_t i {}; i < 20; i++){
        cout << "main => " << i << "\n" << flush;
        sleep_for(1s);
    }
    return 0;
}
