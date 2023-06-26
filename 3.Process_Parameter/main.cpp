#include <cstdio>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int c{},iflg{},fflg{},sflg{};

    char* fvalue {};

    while( (c = getopt(argc, argv, "f:is"))  != -1 ) {

        switch( c ) {
            case 'f':
                fflg = 1;
                fvalue = optarg;
                break;
            case 'i':
                iflg = 1;
                break;
            case 's':
                sflg = 1;
                break;
            case '?':
                printf("Unknow option: -%c\n", optopt);
                break;
            case ':':
                printf("-%c missing option argument\n", optopt);
                break;
            case 1:
                printf("inter: %s\n", optarg);
                break;
            default:
                printf("ret = %d\n", c);
                break;
        }
    }

    printf("fflg = %d, fvalue = %s, iflg = %d, sflg = %d\n", fflg, fvalue, iflg, sflg);
    
    for(int i {optind}; i < argc; i++){
        printf("parameter: %s\n", argv[i]);
    }

    return 0;
}
