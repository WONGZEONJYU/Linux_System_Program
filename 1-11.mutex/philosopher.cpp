#include <iostream>
#include <thread>

using namespace std;
using namespace chrono;
using namespace this_thread;

static constexpr auto N {5};

static pthread_mutex_t g_chopsticks[N]{};

static void pick_up(const int i)
{
    pthread_mutex_lock(&g_chopsticks[i]);
}

static void put_down(const int i)
{
    pthread_mutex_unlock(&g_chopsticks[i]);
}

static void pick_up_left(const int i)
{
    pick_up(i);
}

static void pick_up_right(const int i)
{
    pick_up((i+1) % N);
}

static void put_down_left(const int i)
{
    put_down(i);
}

static void put_down_right(const int i)
{
    put_down((i+1)%N);
}

static void think(int i)
{
    cout << "philosopher " << i << ": thinking...\n" << flush;
    sleep_for(200ms);
}

static void eat(int i)
{
    cout << "philosopher " << i << ": eating...\n" << flush;
    sleep_for(500ms);
}

static void* philosopher(void* arg)
{  
    auto i {reinterpret_cast<long>(arg)};

    while( true ){
        think(i);
        pick_up_left(i);
        pick_up_right(i);
        eat(i);
        put_down_right(i);
        put_down_left(i);
    }

    return nullptr;
}

int main(int argc, char const *argv[])
{
    pthread_mutexattr_t mattr{};
    pthread_mutexattr_init(&mattr);

    for (auto & i : g_chopsticks){
        pthread_mutex_init(&i,&mattr);
    }

    for(long r{}; r<N; r++){
        pthread_t t{};
        pthread_create(&t, nullptr, philosopher, reinterpret_cast<void*>(r));
    }

    for(;;){
        sleep_for(1s);
    }

    return 0;
}
