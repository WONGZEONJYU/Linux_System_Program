#include <unistd.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

static constexpr auto PIPENAME{"./fifo"};
static constexpr auto MAXEVENTS{10};

int main(int argc, char const *argv[])
{
    const auto epoll_fd{epoll_create1(EPOLL_CLOEXEC)};

    if (epoll_fd < 0){
        cerr << "[ERROR] epoll_create1(...)\n";
        exit(EXIT_FAILURE);
    }
    const auto pipe_fd{open64(PIPENAME,O_RDWR | O_NONBLOCK)};
    if (pipe_fd < 0){
        cerr << "[ERROR] open64(...)\n";
        exit(EXIT_FAILURE);
    }
    
    epoll_event e{};
    e.data.fd = pipe_fd;
    e.events = EPOLLIN | EPOLLET;
    
    if (epoll_ctl(epoll_fd,EPOLL_CTL_ADD,pipe_fd,&e) < 0){
        cerr << "[ERROR] epoll_ctl(...)\n";
        exit(EXIT_FAILURE);
    }
    
    for(;;){
        epoll_event ev_ready[MAXEVENTS]{};
        const auto ready{epoll_wait(epoll_fd,ev_ready,MAXEVENTS,1000)};
        if (ready > 0){

            bool quit{};

            for(int i {};i < ready;++i){
                cout << "fd : " << ev_ready[i].data.fd << " , events : " << 
                ((ev_ready[i].events & EPOLLIN) ? "EPOLLIN" : "") << " " << 
                ((ev_ready[i].events & EPOLLHUP) ? "EPOLLHUP" : "") << " " << 
                ((ev_ready[i].events & EPOLLERR) ? "EPOLLERR" : "") << "\n";

                if (ev_ready[i].events & EPOLLIN){
                    char buffer[64]{};
                    const auto rsize{read(ev_ready[i].data.fd,buffer,64)};
                    if (rsize < 0){
                        cerr << "[ERROR] read(...)\n";
                        close(epoll_fd);
                        close(pipe_fd);
                        exit(EXIT_FAILURE);
                    }

                    buffer[rsize] = 0;
                    cout << "read size : " << rsize << " , buffer : " << buffer << "\n";

                    if ("quit" == string(buffer)){
                        cout << "quiting...\n";
                        quit = true;
                        break;
                    }

                }else if(ev_ready[i].events & (EPOLLHUP | EPOLLERR)){
                    cerr << "closing fd : " << ev_ready[i].data.fd << "\n";
                    close(ev_ready[i].data.fd);
                }
            }

            if(quit){break;}

        }else if(!ready){
            cout << "timeout...\n";
        }else{
            cerr << "[ERROR] epoll_wait(...)\n";
            close(epoll_fd);
            close(pipe_fd);
            exit(EXIT_FAILURE);
        }
    }

    close(epoll_fd);
    close(pipe_fd);
    return 0;
}
