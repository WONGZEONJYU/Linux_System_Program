#include <iostream>
#include <thread>
#include <exception>
#include "custom_rand.hpp"
#include "queue.hpp"
#include "conint.hpp"

using namespace std;
using namespace chrono;
using namespace this_thread;

static constexpr auto N{20};

struct CusTask {
    QueueNode head{};
    void(*task)(void* arg){};
    void* arg{};
} ;

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

static void* service_thread(void* arg)
{  
    cout << "begin service: " << reinterpret_cast<long>(arg) << "\n" << flush;
    
    while( ConInt_Value(g_count) < N ){

        if( Queue_Length(g_queue) ){

            auto t {reinterpret_cast<CusTask*>(Queue_Remove(g_queue))};

            if(t){
                t->task(t->arg);
                delete t;
                ConInt_Add(g_count, 1);
            }
        }else{
            sleep_for(50ms);
        }
    }

    cout << "end service: " << reinterpret_cast<long>(arg) << "\n" << flush;

    return nullptr;
}

int main(int argc, char const *argv[])
{
    constexpr auto T{5};
    rand_();

    g_queue = Queue_Create();
    g_count = ConInt_Create(0);

    pthread_t t[T]{};

    for (long i{}; i < T; i++){
        pthread_create(t + i,nullptr,service_thread,reinterpret_cast<void*>(i));
    }

    for(long i{}; i < N; i++){
        try{
            auto ct {new CusTask};
            ct->task = customer_task;
            ct->arg = reinterpret_cast<void*>(i);
            Queue_Add(g_queue, reinterpret_cast<QueueNode*>(ct));
            sleep_for(1s);
        }catch(const std::bad_alloc& e){
            std::cerr << e.what() << '\n';
            return -1;
        }
    }

    for(auto &i : t){
        pthread_join(i,nullptr);
    }

    ConInt_Destroy(g_count);
    Queue_Destroy(g_queue);

    return 0;
}
