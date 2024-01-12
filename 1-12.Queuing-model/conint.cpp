#include <thread>
#include <exception>
#include <atomic>
#include <iostream>
#include "conint.hpp"

struct TConInt
{
    //pthread_mutex_t mutex{};
    std::atomic<int> val{};
};

ConInt ConInt_Create(const int v)
{
    TConInt *ret{};

    try{
        ret = new TConInt;
        pthread_mutexattr_t attr {};
        // pthread_mutexattr_init(&attr);
        // pthread_mutex_init(&ret->mutex, &attr);
        ret->val.store(v);

    }catch(const std::bad_alloc& e){
        std::cerr << e.what() << '\n';
        ret = nullptr;
    }
    return ret;
}

void ConInt_Destroy(ConInt ci)
{
    auto c{static_cast<TConInt*>(ci)};
    //pthread_mutex_destroy(&c->mutex);
    delete c;
}

int ConInt_Value(ConInt ci)
{
    auto c{static_cast<TConInt*>(ci)};
    return c->val.load();
}

ConInt ConInt_Add(ConInt ci,const int v)
{
    auto c{static_cast<TConInt*>(ci)};
    c->val.fetch_add(v);
    return c;
}

ConInt ConInt_Dec(ConInt ci,const int v)
{   
    auto c{static_cast<TConInt*>(ci)};
    c->val.fetch_sub(v);
    return c;
}
