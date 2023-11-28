#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <cstring>
#include <string>
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;
static int test1();

inline static constexpr auto FIFO_NAME {"./fifo"};

int main(int argc, char const *argv[])
{
    return test1();
}

static int test1()
{
    const auto ret {access(FIFO_NAME,F_OK)};
    if (-1 == ret){
        mkfifo(FIFO_NAME,0664);
    }

    const auto fd{open(FIFO_NAME,O_RDWR)};
    if (-1 == fd){
        cerr << "[ERROR] open(...)\n";
        exit(EXIT_FAILURE);
    }

    //constexpr auto wbuffer{"fifo pipe"};

    auto current_time {time(nullptr)};
    auto _localtime {localtime(&current_time)};
    constexpr auto MAX_BUFFER_SIZE{80U};
    char timebuffer[MAX_BUFFER_SIZE];
    
    strftime(timebuffer, sizeof(timebuffer), "%Y-%m-%d %H:%M:%S", _localtime);

    std::cout << "当前本地时间: " << timebuffer << std::endl;

    const auto wrbytes{write(fd,timebuffer,strlen(timebuffer))};
    if (wrbytes < 0){
        cerr << "[ERROR] write(...)\n";
        exit(EXIT_FAILURE);
    }
    
    close(fd);
    return 0;
}