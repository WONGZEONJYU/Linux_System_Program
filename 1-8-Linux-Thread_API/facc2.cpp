#include <thread>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <memory.h>
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;
using namespace this_thread;

struct Array{
    unsigned int i,* arr;
} ;

static void thread_facc(Array* pa)
{
    auto a{pa};
    const auto k{a->i};

    if (!a->arr[k]){
        Array a1{k-1,a->arr},a2{k-2,a->arr};

        thread th2(thread_facc,&a2);

        sleep_for(k * 200us);

        thread th1(thread_facc,&a1);

        th2.join();
        th1.join();

        a->arr[k] = a->arr[k-1] + a->arr[k-2];
    }
}

long long facc(unsigned int n)
{
    auto arr {new unsigned int[n]};
    long long ret {};
    
    if( arr ){
        Array a { n - 1, arr }; 

        a.arr[0] = 1;
        a.arr[1] = 1;

        thread th(thread_facc,&a);

        th.join();

        ret = a.arr[n-1] + a.arr[n-2];
    }
    
    delete[] arr;
    
    return ret;
}

int main(int argc, char const *argv[])
{
    if (argc < 2){
        cerr << "[ERROR] parm miss\n";
        return -1;
    }
    
    const auto n{atoll(argv[1])};
    cout << "input number : " << n << "\n";

    cout << "ret = " << facc(n) << "\n";

    return {};
}

