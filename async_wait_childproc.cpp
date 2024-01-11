#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

#include "mysignal.hpp"

using namespace std;
using namespace wtd;

int main(int argc, char const *argv[])
{
    int exit_{};
    MySignal::_sp_MySignal_type m_sig{MySignal::Create(SIGCHLD,SA_RESTART,[&](){
        cout << "sig : " << m_sig->sig() << "\n";
        wait(nullptr);
        exit_ = 1;
    })};

    auto pid{fork()};
    if (pid < 0){
        cerr << "[ERROR] fork()\n";
        exit(EXIT_FAILURE);
    }else if (!pid){
        sleep(3);
        cout << "child prco exit...\n";
        exit(EXIT_SUCCESS);
    }else{

        for(;;){
            sleep(1);
            if (exit_){
                cout << "parent prco exit...\n";
                break;
            }
        }
    }
    
    return 0;
}
