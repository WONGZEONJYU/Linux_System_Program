#include <fcntl.h>
#include <iostream>
#include <thread>
#include <ctime>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


using namespace std;

static constexpr auto NSECS_PER_MSEC {1000000UL};
static constexpr auto NSECS_PER_SEC {1000000000UL};
static constexpr auto TEST_LOOPS {10000};

#define DiffNS(begin,end) ((end.tv_sec - begin.tv_sec) * NSECS_PER_SEC + (end.tv_nsec - begin.tv_nsec))

static void* thread_entry(void*){
    return {};
}

static void thread_test(const int t) {

    cout << "thread test : \n";
    timespec begin{},end{};
    
    clock_gettime(CLOCK_MONOTONIC,&begin);
    
    for (int i {}; i < t; i++){
        pthread_t tid{};
        pthread_create(&tid,nullptr,thread_entry,nullptr);
        pthread_join(tid,nullptr);
    }

    clock_gettime(CLOCK_MONOTONIC,&end);

    auto diff{DiffNS(begin,end)/NSECS_PER_MSEC};

    cout << "result = " << diff << "ms\n";
    
}

static void process_test(const int t)
{
    cout << "process test : \n";
    timespec begin{},end{};

    clock_gettime(CLOCK_MONOTONIC,&begin);

    for (int i {}; i < t; i++) {
        const auto pid{fork()};
        if (pid){
            waitpid(pid,nullptr,0);
        }else{
            return;
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC,&end);

    auto diff{DiffNS(begin,end)/NSECS_PER_MSEC};

    cout << "result = " << diff << "ms\n";
}

int main(int argc, char const *argv[]) {
    
    if (argc < 2){
        cerr << "error\n";
        return -1;
    }

    auto t{atoi(argv[1])};
    
    thread_test(t);
    process_test(t);
    return 0;
}
