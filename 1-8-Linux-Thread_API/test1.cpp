//#define _GNU_SOURCE     /* To get pthread_getattr_np() declaration */
#include <thread>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>

using namespace std;

static void print_attr(const char* prefix,const pthread_attr_t* attr)
{
    int i {};
    auto r { pthread_attr_getdetachstate(attr, &i)};
    cout << (prefix ? prefix : "") << " - Detach state(" << r << ")        = " << ((i == PTHREAD_CREATE_DETACHED) ? "PTHREAD_CREATE_DETACHED" :
                                                                            (i == PTHREAD_CREATE_JOINABLE) ? "PTHREAD_CREATE_JOINABLE" :"???") << "\n";

    r = pthread_attr_getscope(attr, &i);
    cout << (prefix ? prefix : "") << " - Scope(" << r << ")                = " << ((i == PTHREAD_SCOPE_SYSTEM)  ? "PTHREAD_SCOPE_SYSTEM" :
                                                                                (i == PTHREAD_SCOPE_PROCESS) ? "PTHREAD_SCOPE_PROCESS" :"???") << "\n";

    r = pthread_attr_getinheritsched(attr, &i);
    cout << (prefix ? prefix : "") << " - Inherit scheduler(" << r << ")    = " << ((i == PTHREAD_INHERIT_SCHED)? "PTHREAD_INHERIT_SCHED" :
                                                                                (i == PTHREAD_EXPLICIT_SCHED)? "PTHREAD_EXPLICIT_SCHED" :"???") << "\n";

    r = pthread_attr_getschedpolicy(attr, &i);
    cout << (prefix ? prefix : "") << " - Scheduling policy(" << r << ")    = " << ((i == SCHED_OTHER) ? "SCHED_OTHER" :
                                                                                (i == SCHED_FIFO)  ? "SCHED_FIFO" : 
                                                                                (i == SCHED_RR)? "SCHED_RR" :"???") << "\n";

    sched_param sp {};
    r = pthread_attr_getschedparam(attr, &sp);
    cout << (prefix ? prefix : "") << " - Scheduling priority(" << r << ")  = " << sp.sched_priority << "\n";

    size_t v {};
    r = pthread_attr_getguardsize(attr, &v);
    cout << (prefix ? prefix : "") << " - Guard size(" << r << ")          = 0x" << hex << v << " bytes\n";

    void* stkaddr {};
    r = pthread_attr_getstack(attr, &stkaddr, &v);
    cout << (prefix ? prefix : "") << " - Stack address(" << r << ")       = " << stkaddr << "\n";
    cout << (prefix ? prefix : "") << " - Stack size(" << r << ")          = 0x" << hex << v << " bytes\n\n";

}

static void* thread_entry(void*)
{   
    cout << "thread: " << pthread_self() << "\n";

    pthread_attr_t attr{};
    pthread_getattr_np(pthread_self(), &attr);
    
    print_attr("thread", &attr);
    return {};
}

int main(int argc, char const *argv[])
{
    pthread_attr_t attr{};

    pthread_attr_init(&attr);

    print_attr("init", &attr);
    
    //auto r {pthread_attr_setstacksize(&attr, 4 * 1024)}; /*不建议手动设置栈大小*/
    /*设置4K会出错*/

    auto r {pthread_attr_setstacksize(&attr, 1024 * 1024)};

    pthread_t t{};
    pthread_create(&t, &attr, thread_entry, nullptr);
    pthread_join(t, nullptr);

    pthread_getattr_np(pthread_self(), &attr);
    
    print_attr("main", &attr);
    
    pthread_attr_destroy(&attr);

    return {};
}
