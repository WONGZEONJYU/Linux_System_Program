#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <cstring>
#include "mysignal.hpp"

using namespace std;
using namespace wtd;

int main(int argc, char const *argv[])
{
    MySignal::_sp_MySignal_type m { MySignal::Create(SIGALRM,SA_RESTART,[&](){
        
        cout << m->sig() << "\n";
    })};

    alarm(5);
    pause();
    return 0;
}
