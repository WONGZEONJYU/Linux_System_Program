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
    int master {posix_openpt(O_RDWR)}; /*gnome-terminal*/
    
    if (master > 0){

        grantpt(master);    /*获取设备使用权限*/

        unlockpt(master);   /*解锁设备，为读写做准备*/

        std::cout << "Slave : " << ptsname(master) << "\n"; /*获取伪终端从设备名字*/

        int i {},c {};

        char rx{},rxbuf[128]{},txbuf[sizeof(rxbuf) * 2]{};

        while (1 == (c = read(master,&rx,1))){  /*每次读取一个字节*/

            if ('\r' == rx){    /*遇到回车代表命令一行的结束*/

                rxbuf[i] = 0;

                sprintf(txbuf,"from slave: %s\r",rxbuf); /*show on screen*/

                write(master,txbuf,strlen(txbuf)); /*keyboaed input*/

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
