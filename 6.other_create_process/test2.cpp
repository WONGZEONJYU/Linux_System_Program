#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "current pid: " << getpid() << "\n";
    int result {system("pstree -A -p -s $$")};
    std::cout << "result = " << result << "\n";
    return 0;
}
