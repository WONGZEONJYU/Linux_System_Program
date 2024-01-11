#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

static inline constexpr auto PATHNAME {"."};
static inline constexpr auto PROID {10};
static inline constexpr auto MSG_TYPE {100};
static inline constexpr auto MSG_TEXT_SIZE {128};

struct msgbuf_type {
    long mtype;       /* message type, must be > 0 */
    char mtext[MSG_TEXT_SIZE];    /* message data */
};

int main(int argc, char const *argv[])
{
    const auto key_ {ftok(PATHNAME,PROID)};
    if (key_ < 0){
        cerr << "[ERROR] ftok(...)\n";
        exit(EXIT_FAILURE);
    }
    
    const auto msg_id{msgget(key_,IPC_CREAT | S_IRWXU )};

    if (msg_id < 0){
        cerr << "[ERROR] msgget(...)\n";
        exit(EXIT_FAILURE);
    }

    for (;;){

        msgbuf_type buf{};

        const auto rbytes{msgrcv(msg_id,&buf,sizeof(buf),MSG_TYPE,0)};

        if (rbytes < 0){
            cerr << "[ERROR] msgrcv(...)\n";
            exit(EXIT_FAILURE);
        }

        cout << "type  = " << buf.mtype << " ,text : " << buf.mtext << "\n";
        
        if (!strcmp("quit",buf.mtext)){
            cout << "recv proc exit...\n";
            break;
        }
    }
    
    if (msgctl(msg_id,IPC_RMID,nullptr) < 0){ /*发送和接收随便一端调用,两端同时调用会出错*/
        cerr << "[ERROR] msgctl(...)\n";
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
