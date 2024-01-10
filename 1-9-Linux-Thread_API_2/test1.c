#include <stdio.h>
#include <pthread.h>

void clean_test(void* arg)
{
    printf("clean_test(...) : %lld\n",(long long)arg);
}

void foo()
{
    printf("begin\n");
    printf("I'm foo()\n");
    //pthread_exit(NULL);
    printf("end\n");
}

void* thread_entry(void* arg)
{
    pthread_cleanup_push(clean_test,(void*)111);
    foo();
    return 0;
    pthread_cleanup_pop(1);
}

int main(int argc, char const *argv[])
{
    pthread_t t = 0;
    pthread_create(&t,NULL,thread_entry,NULL);
    pthread_join(t,NULL);
    return 0;
}
