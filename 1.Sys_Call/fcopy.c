#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int file_copy(const char* dst, const char* src)
{
    int dfd = open(dst, O_WRONLY|O_CREAT, 0600);
    int sfd = open(src, O_RDONLY);
    int ret = 0;
    
    if( dfd == -1 )
    {
        ret = -1;
    }
    else if( sfd == -1 )
    {
        ret = -2;
    }
    else if( (dfd != -1) && (sfd != -1) )
    {
        char buf[512] = {0};
        int len = 0;

        while( (len = read(sfd, buf, sizeof(buf))) > 0 )
        {
            write(dfd, buf, len);
        }

        close(dfd);
        close(sfd);
    }
    
    return ret;
}

int main(int argc, const char* argv[])
{
    if( argc > 2 )
    {
        if( file_copy(argv[1], argv[2]) == 0) 
        {
            printf("copy completed\n");
        }
        else
        {
            printf("copy failed\n");
        }   
    }
    else
    {
        printf("parameter error\n");
    }

    // printf("argv[1] : %s\r\n" ,argv[1]);
    // printf("argv[2] : %s\r\n" ,argv[2]);
    return 0;
}
