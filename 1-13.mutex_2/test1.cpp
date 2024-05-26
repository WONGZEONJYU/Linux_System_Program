#include <iostream>
#include <thread>

using namespace std;

int main(int argc, char const *argv[])
{
    cout << "PTHREAD_MUTEX_NORMAL = " << PTHREAD_MUTEX_NORMAL << "\n" << 
            "PTHREAD_MUTEX_TIMED_NP = " << PTHREAD_MUTEX_TIMED_NP << "\n" << 
            "PTHREAD_MUTEX_RECURSIVE = " << PTHREAD_MUTEX_RECURSIVE << "\n" <<
            "PTHREAD_MUTEX_RECURSIVE_NP = " << PTHREAD_MUTEX_RECURSIVE_NP << "\n" << 
            "PTHREAD_MUTEX_ERRORCHECK = " << PTHREAD_MUTEX_ERRORCHECK << "\n" << 
            "PTHREAD_MUTEX_ERRORCHECK_NP = " << PTHREAD_MUTEX_ERRORCHECK_NP << "\n" <<
            "PTHREAD_MUTEX_DEFAULT = " << PTHREAD_MUTEX_DEFAULT << "\n";

    return 0;
}
