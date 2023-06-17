#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>

int main(int argc, char const *argv[])
{
    int slave {open(argv[1],O_RDWR)};   //shell

    if (slave > -1)
    {
        constexpr char SENDDATA[] {"WONGZEONJYU\r"};

        write(slave,SENDDATA,strlen(SENDDATA));

        sleep(1);

        char buf[128]{};

        int len (read(slave,buf,sizeof(buf) - 1));

        buf[(len > 0)? len : 0] = 0;

        std::cout << "Read: "<< buf <<"\n";

        close(slave);

    }else{
        std::cout << "open slave error ...\n";
    }

    return 0;
}
