#include <unistd.h>
#include <cstdio>
#include <sys/wait.h>
#include <iostream>
#include "sem_.hpp"
#include "mysignal.hpp"
#include <thread>
#include <chrono>

using namespace std;
using namespace chrono;
using namespace this_thread;

static int test1();
static int test2();

int main(int argc, char const *argv[])
{

    return test2();
}

static int test1()
{
    bool _exit{};

    auto m{wtd::MySignal::Create(SIGINT,0,[&](){
        _exit = true;
    })};

    uint16_t semarr[]{1,1};
    semun _semn{.array = semarr};
    Sem s;

    try{
        s = {_semn,2};
    }catch(const std::string& e){
        std::cerr << e << '\n';
        return -1;
    }

    const auto cpid{fork()};

    if (cpid < 0){
        cerr << "[ERROR] fork(...)\n";
        exit(EXIT_FAILURE);
    }else if(!cpid){
        
        while(!_exit){
            s.sem_p(0);
            cout << "----------------\n";
            cout << "C Start.\n";
            sleep(1);
            cout << "C End.\n";
            cout << "----------------\n";
            s.sem_v(0);
        }
        exit(EXIT_SUCCESS);
    }else{
        usleep(100);
        while(!_exit){
            s.sem_p(0);
            cout << "----------------\n";
            cout << "P Start.\n";
            sleep(1);
            cout << "P End.\n";
            cout << "----------------\n";
            s.sem_v(0);
        }
        wait(nullptr);
    }
    return 0;
}

static int test2()
{
    constexpr auto SEM_CONTROL_P{0};
    constexpr auto SEM_CONTROL_C{1};

    uint16_t semarr[]{1,0};
    semun _semn{.array = semarr};
    Sem s;

    try{
        s = {_semn,2};
    }catch(const string& e){
        cerr << e << '\n';
        return -1;
    }

    bool _exit{};

    auto m{wtd::MySignal::Create(SIGINT,0,[&](){
        _exit = true;
    })};

    const auto cpid{fork()};

    if (cpid < 0){
        cerr << "[ERROR] fork(...)\n";
        exit(EXIT_FAILURE);
    }else if(!cpid){
        
        while(!_exit){
            s.sem_p(SEM_CONTROL_C);
            cout << "B" << flush;
            s.sem_v(SEM_CONTROL_P);
        }
        exit(EXIT_SUCCESS);
    }else{

        while(!_exit){
            s.sem_p(SEM_CONTROL_P);
            cout << "A" << flush;
            s.sem_v(SEM_CONTROL_C);

            s.sem_p(SEM_CONTROL_P);
            cout << "A" << flush;
            s.sem_v(SEM_CONTROL_P);

            sleep(1);
            cout << '\n';
        }

        int exit_sata{};
        waitpid(cpid,&exit_sata,0);
        cout << "\n" << WEXITSTATUS(exit_sata) << "\n";
    }
    return 0;
}