#include <iostream>
#include <thread>
#include <exception>
#include "custom_rand.hpp"
#include "queue.hpp"
#include "conint.hpp"

using namespace std;
using namespace chrono;
using namespace this_thread;

static constexpr auto N{5};

typedef struct
{
    QueueNode head{};
    void(*task)(void* arg){};
    void* arg{};
} CusTask;

static Queue g_queue {};
static ConInt g_count {};

static void customer_task(void* arg)
{
    const auto v {reinterpret_cast<long>(arg)};
    
    cout << "begin task: " << v << "\n" << flush;
    
    for(long i{}; i < v; i++){
        sleep_for(chrono::microseconds(rand_() % N * 50000));
    }

    cout << "end task: " << v << "\n" << flush;
}

int main(int argc, char const *argv[])
{
    rand_();

    g_queue = Queue_Create();
    g_count = ConInt_Create(0);

    pthread_t t[N]{};

    for (int i{}; i < N; i++){
        pthread_create(t + i,nullptr,nullptr,reinterpret_cast<void*>(i));
    }

    for(auto &i : t){
        pthread_join(i,nullptr);
    }

    ConInt_Destroy(g_count);
    Queue_Destroy(g_queue);

    return 0;
}
