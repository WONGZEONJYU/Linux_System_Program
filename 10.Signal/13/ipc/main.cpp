#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

static int g_type {-1},g_length{-1},g_current{-1},g_obj_sig{};
static char* g_data {};

static void ipc_data_handler(char* data,const int length)
{
    std::cout << "data length: "<<  (length - 1) << ", ipc data: "<< (data ? data : "(null)") << "\n";
}

static void signal_handler(const int sig,siginfo_t* info,void*)
{
    if (sig == g_obj_sig){

        const auto data {info->si_value.sival_int};

        if (-1 == g_current){

            g_type = data & 0xffff;
            g_length = (data >> 16) & 0xffff;
            g_current = 0;
            g_data = reinterpret_cast<char*>(malloc(g_length));

            if (!g_data) {
                exit(-1);
            }

        }else{

            int i {};

            while ((i < 4) && (g_current < g_length)){

                g_data[g_current++] = ((data >> (i * 8)) & 0xFF);
                ++i;
            }
        }

        if (g_current == g_length){

            ipc_data_handler(g_data, g_length);
            g_type = -1;
            g_length = -1;
            g_current = -1;
            free(g_data);
        }
    }
}

int main(int argc,char* argv[])
{
    std::cout << "current pid (" << getpid() << ") ...\n";

    struct sigaction act {};
    act.sa_sigaction = signal_handler;
    act.sa_flags = SA_SIGINFO | SA_RESTART;
    g_obj_sig = atoi(argv[1]);

    sigaddset(&act.sa_mask,g_obj_sig);

    sigaction(g_obj_sig,&act,nullptr);

    while (true){
        sleep(1);
    }

    return 0;
}

