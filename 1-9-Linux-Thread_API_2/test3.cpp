#include <iostream>
#include <thread>

using namespace std;

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
    int i {};
    pthread_cleanup_push(clean_test,reinterpret_cast<void*>(111));
    foo();
    if (i)
        pthread_cleanup_pop(0);/*为什么清理函数还会被执行?*/
    return nullptr;
}

int main(int argc, char const *argv[])
{
    pthread_t t{};
    pthread_create(&t,nullptr,thread_entry,nullptr);
    pthread_join(t,nullptr);
    return 0;
}
