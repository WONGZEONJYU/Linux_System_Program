#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <cstring>
#include <string>
#include <iostream>
#include <fcntl.h>

using namespace std;
static int test1();

inline static constexpr auto FIFO_NAME {"./fifo"};

int main(int argc, char const *argv[])
{
    return test1();
}

static int test1()
{
    const auto fd{open(FIFO_NAME,O_RDWR)};
    if (-1 == fd){
        cerr << "[ERROR] open(...)\n";
        exit(EXIT_FAILURE);
    }
    
    char buffer[64]{};
    const auto rbytes{read(fd,buffer,sizeof(buffer))};
    if (rbytes > 0){
        cout << "rbytes : " << buffer << "\n";
    }
    
    close(fd);
    return 0;
}
