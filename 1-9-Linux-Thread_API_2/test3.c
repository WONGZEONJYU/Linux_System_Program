#include <stdio.h>
#include <pthread.h>

void clean_test(void* arg)
{
    printf("clean_test(...) : %lld\n",(long long ) arg);
}

void foo()
{
    printf("begin\n");
    printf("I'm foo()\n");
    printf("end\n");
}

void* thread_entry(void* arg)
{
    int i = 0;
    pthread_cleanup_push(clean_test,(void*)(111))
    foo();
    if (i)
        pthread_cleanup_pop(1);
    return 0;
}

int main(int argc, char const *argv[])
{
    pthread_t t = 0;
    pthread_create(&t,0,thread_entry,0);
    pthread_join(t,0);
    return 0;
}
