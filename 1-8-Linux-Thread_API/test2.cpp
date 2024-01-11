//#define _GNU_SOURCE     /* To get pthread_getattr_np() declaration */
#include <thread>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <memory.h>
#include <iostream>
#include <type_traits>

using namespace std;

static void* thread_entry_1(void* )
{   
    char ret[] = "hello world";
    
    cout << "thread : " << pthread_self() << "\n";

    return ret; // OOPS!
}

static void* thread_entry_2(void*)
{   
    double pi {3.1415926};
    void* ret {};

    memcpy(&ret, &pi, sizeof(ret));

    return ret;
}

static void* thread_entry_3(void* arg)
{   
    const auto n {*static_cast<int*>(arg)};

    auto ret {static_cast<int*>(malloc(n * sizeof(int)))};

    for(int i {}; i < n; i++) {
        ret[i] = n - i;
    }

    return ret;
}

static void child_func(const int n)
{
    auto ret {static_cast<int*>(malloc(n * sizeof(int)))};

    for(int i {}; i < n; i++) {
        ret[i] = n - i;
    }
    pthread_exit(ret);
}

static void* thread_entry_3_1(void* arg)
{   
    child_func(*static_cast<int*>(arg));
    cout << "end " << __FUNCTION__ << "\n\n";
    return{};
}

static void* thread_entry_4(void* arg)
{   
    double* v {static_cast<double*>(arg)};
    auto r {*v};
    *v = 3.1415 * r * r;
    return {};
}

static void wait_thread_entry_2()
{
    cout << "begin " << __FUNCTION__ << "\n";

    pthread_t t {};
    double pi{};
    void* ret {};
    
    pthread_create(&t, nullptr, thread_entry_2, nullptr);
    
    pthread_join(t, &ret);
    
    memcpy(&pi, &ret, sizeof(pi));
    
    cout << "pi = " << pi << "\n";

    cout << "end " << __FUNCTION__ << "\n\n";
}

static void wait_thread_entry_3()
{
    cout << "begin " << __FUNCTION__ << "\n";

    pthread_t t {};
    void* ret {};
    int n{10};

    pthread_create(&t, nullptr, thread_entry_3, &n);
    pthread_join(t, &ret);

    for (decltype(n) i {}; i < n; i++){
        cout << static_cast<int*>(ret)[i] << " ";
    }

    cout << "\n";
    free(ret);

    cout << "end " << __FUNCTION__ << "\n\n";
}

static void wait_thread_entry_3_1()
{
    cout << "begin " << __FUNCTION__ << "\n";

    pthread_t t {};
    void* ret {};
    int n{20};

    pthread_create(&t, nullptr, thread_entry_3_1, &n);
    pthread_join(t, &ret);

    for (decltype(n) i {}; i < n; i++){
        cout << static_cast<int*>(ret)[i] << " ";
    }

    cout << "\n";
    free(ret);

    cout << "end " << __FUNCTION__ << "\n\n";
}

static void wait_thread_entry_4()
{
    cout << "begin " << __FUNCTION__ << "\n";

    pthread_t t {};
    double ret{2};
    pthread_create(&t, nullptr, thread_entry_4, &ret);
    pthread_join(t, nullptr);
    cout << "ret = " << ret << "\n";

    cout << "end " << __FUNCTION__ << "\n\n";
}

int main(int argc, char const *argv[])
{
    wait_thread_entry_2();
    wait_thread_entry_3();
    wait_thread_entry_3_1();
    wait_thread_entry_4();
    return 0;
}
