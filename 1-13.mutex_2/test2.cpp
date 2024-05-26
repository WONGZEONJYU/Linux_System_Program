#include <iostream>
#include <thread>

using namespace std;

int main(int argc, char const *argv[])
{
    pthread_mutex_t mutex{};
    pthread_mutexattr_t mattr{};
    pthread_mutexattr_init(&mattr);
    pthread_mutex_init(&mutex,&mattr);

    auto r{pthread_mutex_lock(&mutex)};
    cout << "r = " << r << "\n";
    r = pthread_mutex_lock(&mutex);
    cout << "r = " << r << "\n";

    cout << "hello world\n";

    pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&mutex);

    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&mattr);

    return 0;

}
