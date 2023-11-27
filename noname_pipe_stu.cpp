#include <unistd.h>
#include <iostream>
#include <cstring>
#include <string>
#include <sys/wait.h>
#include <limits>
#include <fcntl.h>

using namespace std;
static int test1();
static int test2();

int main(int argc, char const *argv[])
{
    return test2();
}

static int test2()
{
    constexpr auto MAX_BUF_SIZE{64};
    cout << "current = " << getpid() << "\n";

    int pipefd[2]{};
    const auto ret {pipe(pipefd)};

    if (-1 == ret){
        cerr << "[ERROR] pipe()\n";
        exit(EXIT_FAILURE);
    }
    
    const auto fd_flags{fcntl(pipefd[0],F_GETFL,O_NONBLOCK)};
    if (-1 == fd_flags){
        cerr << "[ERROR] fcntl(1)\n";
        close(pipefd[0]),close(pipefd[1]);
        exit(EXIT_FAILURE);
    }
    
    if(-1 == fcntl(pipefd[0],F_SETFL,fd_flags | O_NONBLOCK)){ /*set noblock*/
        cerr << "[ERROR] fcntl(2)\n";
        close(pipefd[0]),close(pipefd[1]);
        exit(EXIT_FAILURE);
    }

    auto cpid{fork()};

    if (-1 == cpid){
        cerr << "[ERROR] fork()\n";
        close(pipefd[0]),close(pipefd[1]);
        exit(EXIT_FAILURE);
    }else if (!cpid){
        
        cpid = getpid();
        cout << "child pid = " << cpid << "\n";

        close(pipefd[1]);

        for(;;){

            char buf[MAX_BUF_SIZE]{};
            const auto rbytes{read(pipefd[0],buf,sizeof(buf))};

            if (-1 == rbytes){

                if (EWOULDBLOCK != errno){
                    cerr << "[ERROR] read(...)\n";
                    close(pipefd[0]);
                    exit(EXIT_FAILURE);
                }
            }else{
                cout << "read size = " << rbytes << "\n";
                cout << "child(" << cpid << ") buf " << buf << "\n";
            }

            if (!strcmp(buf,"quit")){
                cout << "child proc exit...\n";
                break;
            }
        }

        close(pipefd[0]);
        exit(EXIT_SUCCESS);

    }else{

        close(pipefd[0]);

        for(;;){

            usleep(100);
            string buf;
            cout << "Please input : ";
            cin >> buf;

            if (buf.size() > MAX_BUF_SIZE){
                cin.clear();
                cerr << "Input failed. Buffer overflow.\n";
                continue;
            }

            const auto wbytes{write(pipefd[1],buf.c_str(),buf.size())};
            if (-1 == wbytes){
                cerr << "[ERROR] write(...)\n";
                close(pipefd[1]);
                exit(EXIT_FAILURE);
            }
            
            if (buf == "quit"){
                cout << "parent proc exit...\n";
                break;
            }
        }

        close(pipefd[1]);
  
        int status{};
        waitpid(cpid,&status,0);
        cout << WEXITSTATUS(status) << "\n";
    }

    return 0;
}


static int test1()
{

    int rpipefd[2]{};
    int wpipefd[2]{};

    const auto ret1{pipe(rpipefd)};
    if (-1 == ret1){
        cerr << "[ERRPR] rpipe()\n";
        exit(EXIT_FAILURE);
    }

    const auto ret2{pipe(wpipefd)};
    if(-1 == ret2){
        cerr << "[ERRPR] wpipe()\n";
        close(rpipefd[0]),close(rpipefd[1]);
        exit(EXIT_FAILURE);
    }

    cout << "rpipefd[0] = " << rpipefd[0] << " , rpipefd[1] = " << rpipefd[1] << " , wpipefd[0] = " << wpipefd[0] << " , wpipefd[1] = " << wpipefd[1] << "\n";

    const auto child_pid {fork()};

    if (child_pid < 0)
    {
        cerr << "parent [ERROR] fork()\n";
        close(rpipefd[0]),close(rpipefd[1]);
        close(wpipefd[0]),close(wpipefd[1]);
        exit(EXIT_FAILURE);

    }else if (!child_pid){

        const auto cpid {getpid()};
        cout << "child pid = " << cpid << "\n";

        cout << cpid << " close(wpipefd[1]) = " << close(wpipefd[1]) << "\n";
        char rbuffer[64]{};
        const auto rbytes{read(wpipefd[0],rbuffer,sizeof(rbuffer))};
        cout << "child rbytes = " << rbytes << "\n";
        if (-1 == rbytes){
            cerr << "child [ERROR] read\n";
            close(wpipefd[0]);
            exit(EXIT_FAILURE);
        }
        cout << cpid << " close(wpipefd[0]) = " << close(wpipefd[0]) << "\n";

        cout << rbuffer << "\n";

        cout << cpid << " close(rpipefd[0]) = " << close(rpipefd[0]) << "\n";
        constexpr auto wbuffer{"child send hello,pipe"};
        const auto wrbytes{write(rpipefd[1],wbuffer,strlen(wbuffer))};
        cout << "child wrbytes = " << wrbytes << "\n";
        if (-1 == wrbytes){
            cerr << "child [ERROR] write\n";
            close(rpipefd[1]);
            exit(EXIT_FAILURE);
        }

        cout << cpid << " close(rpipefd[1]) = " << close(rpipefd[1]) << "\n";
        exit(EXIT_SUCCESS);

    }else{
        
        const auto ppid_{getpid()};
        cout << "current pid = " << ppid_ << "\n";

        cout << ppid_ << " close(wpipefd[0]) = " << close(wpipefd[0]) << "\n";
        constexpr auto wbuffer{"parent send hello,pipe"};
        const auto wrbytes{write(wpipefd[1],wbuffer,strlen(wbuffer))};
        cout << "parent wrbytes = " << wrbytes <<"\n";
        if (-1 == wrbytes){
            cerr << "parent [ERROR] write\n";
            close(wpipefd[1]);
            exit(EXIT_FAILURE);
        }
        cout << ppid_ << " close(wpipefd[1]) = " << close(wpipefd[1]) << "\n";

        cout << ppid_ << " close(rpipefd[1]) = " << close(rpipefd[1]) << "\n";
        char rbuffer[64]{};
        const auto rbytes{read(rpipefd[0],rbuffer,sizeof(rbuffer))};
        cout << "parent rbytes = " << rbytes <<"\n";
        if (-1 == rbytes){
            cerr << "parent [ERROR] read\n";
            close(rpipefd[0]);
            exit(EXIT_FAILURE);
        }
        cout << ppid_ << " close(rpipefd[0]) = " << close(rpipefd[0]) << "\n";

        cout << rbuffer << "\n";
        wait(nullptr);
    }
    
    return 0;
}

