#include <thread>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <memory.h>
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

static void* thread_facc(void* arg)
{
    const auto n{reinterpret_cast<long long>(arg)};
    long long ret{};

    if (n > 1){
        
        pthread_t t1{},t2{};
        void* r1{},*r2{};
        pthread_create(&t1,nullptr,thread_facc,reinterpret_cast<void*>(n-1)),
        pthread_create(&t2,nullptr,thread_facc,reinterpret_cast<void*>(n-2));

        pthread_join(t1,&r1),
        pthread_join(t2,&r2);

        ret = reinterpret_cast<long long>(r1) + reinterpret_cast<long long>(r2);

    }else{
        ret = 1;
    }

    return reinterpret_cast<void*>(ret);
}

int main(int argc, char const *argv[])
{
    if (argc < 2){
        cerr << "[ERROR] parm miss\n";
        return -1;
    }
    
    pthread_t t {};
    void* ret {};
    const auto n{atoll(argv[1])};
    cout << "input number : " << n << "\n";

    const auto begin_time {high_resolution_clock::now()};
    
    pthread_create(&t, nullptr, thread_facc, reinterpret_cast<void*>(n));
    pthread_join(t, &ret);

    const auto end_time {high_resolution_clock::now()};

    const auto duration {end_time - begin_time};

    const auto time_ms {duration.count() * 1000.0};

    cout << "need time : " << time_ms << " ms\n";
    cout << "ret = " << reinterpret_cast<long long>(ret) <<"\n";
    return {};
}

