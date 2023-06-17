#define _XOPEN_SOURCE 600
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>

int main(int argc, char *argv[])
{
    int master {posix_openpt(O_RDWR)}; // gnome-terminal
    
    if (master > 0){

        grantpt(master);

        unlockpt(master);

        std::cout << "Slave : " << ptsname(master) << "\n";

        int i {},c {};

        char rx{},rxbuf[128]{},txbuf[sizeof(rxbuf) * 2]{};

        while (1 == (c = read(master,&rx,1))){

            if ('\r' == rx){

                rxbuf[i] = 0;

                sprintf(txbuf,"from slave: %s\r",rxbuf);    //show on screen

                write(master,txbuf,strlen(txbuf));      //keyboaed input

                i = 0;
 
            }else{
                rxbuf[i++] = rx;
            }
        }

        close(master);
    }else{
        std::cout << "create pty error ...\n";
    }

    return 0;
}
