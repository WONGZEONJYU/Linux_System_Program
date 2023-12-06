#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

static constexpr auto SEM_PATHNAME{"."};
static constexpr auto SEM_PRO_ID {100};

union semun
{
    int val;	
    struct semid_ds *buf;	
    unsigned short int *array;
    struct seminfo *__buf;
};

int main(int argc, char const *argv[])
{
    const auto key{ftok(SEM_PATHNAME,SEM_PRO_ID)};
    if (key < 0){
       cerr << "[ERROR] ftok(...)\n";
       exit(EXIT_FAILURE);
    }

    //IPC_PRIVATE

    const auto sem_id{semget(key,2,IPC_CREAT | S_IRWXU)};
    if (sem_id < 0){
        cerr << "[ERROR] semget(...)\n";
        exit(EXIT_FAILURE);
    }

    uint16_t array[]{0,1};
    semun s{};
    s.array = array;

    const auto ret {semctl(sem_id,0,SETALL,s)};
    if (ret < 0){
        cerr << "[ERROR] semctl(...)\n";
        exit(EXIT_FAILURE);
    }
    
    uint16_t arr1[2]{};
    
    semun s1{};
    s1.array = arr1;
    semctl(sem_id,0,GETALL,s1);

    for (const auto& i:arr1){
        cout << i << " ";
    }
    cout << "\n";

    semctl(sem_id,0,IPC_RMID,nullptr);

    return 0;
}

