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
        
        cout << "Please input : ";
        string str;
        cin >> str;
        if (str.size() >= MSG_TEXT_SIZE){
            cerr << "[ERROR] Input is out of range\n";
            continue;
        }
        
        msgbuf_type buf{.mtype = MSG_TYPE};
        strcpy(buf.mtext,str.c_str());

        if (msgsnd(msg_id,&buf,sizeof(buf),0) < 0){
            cerr << "[ERROR] msgsnd(...)\n";
            exit(EXIT_FAILURE);
        }

        if ("quit" == str){
            cout << "send proc exit...\n";
            break;
        }
    }
    
    return 0;
}
