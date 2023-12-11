#include <unistd.h>
#include <sys/wait.h>
#include <poll.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <limits.h>
#include "mysignal.hpp"

using namespace std;
using namespace wtd;

static constexpr auto FIFO_NAME {"./fifo"};
static constexpr auto BUFFER_SIZE{64};

int main(int argc, char const *argv[])
{
    const auto pipe_fd{open(FIFO_NAME,O_RDWR)};

    if (pipe_fd < 0){
        cerr << "[ERROR] open(...)\n";
        exit(EXIT_FAILURE);
    }
    
    auto m{MySignal::Create(SIGINT,SA_RESTART,[&](){
        close(pipe_fd);
        exit(EXIT_SUCCESS);
    })};

    pollfd pfds{.fd = pipe_fd,.events = POLL_IN};

    for(;;){

        const auto ret{poll(&pfds,1,1000)};

        if (ret > 0){
            
            if(pfds.revents & POLLIN){
                char buffer[BUFFER_SIZE]{};
                const auto rsize{read(pipe_fd,buffer,BUFFER_SIZE)};
                if (rsize < 0){
                    cerr << "[ERROR] read(...)\n";
                    close(pipe_fd);
                    exit(EXIT_FAILURE);
                }
                cout << "buffer : "<< (buffer ? buffer : "null") << "\n";
            }

        }else if(!ret){
            cout << "timeout...\n";
        }else{
            cerr << "[ERROR] poll(...)\n";
            exit(EXIT_FAILURE);
        }   
    }

    close(pipe_fd);

    return 0;
}
