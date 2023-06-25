#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>

typedef int OptCall(const char*,const char*);

struct CallHandler
{
    const char opt;
    OptCall* handler;
};

static int A_Handler(const char*,const char*)
{
    int i {};
    while (environ[i]){
        std::cout << environ[i++] << "\n";
    }

    return 0;
}

static int R_Handler(const char* n,const char*)
{
    if (n){
        std::cout << n << "=" << getenv(n) << "\n";
    }else{
        std::cout << "Need environ NAME to read value...\n";
    }
    return 0;
}

static int W_Handler(const char* n,const char* v)
{
    int err {-1};

    if (n && v){
        
        char* kv {reinterpret_cast<char*>(malloc(strlen(n) + strlen(v) + 2))};

        if (kv){
            
            strcpy(kv,n);
            strcat(kv,"=");
            strcat(kv,v);
            err = putenv(kv);

            if (!err){
                std::cout << "New Eviron: " << kv << "\n";
            }else{
                std::cout << "Error on writing new environ value...\n";
            }
        }

        //free(kv);
    }else{
        std::cout << "Need environ NAME to write value...\n";
    }

    return err;
}

static int T_Handler(const char* n,const char* v)
{
    return W_Handler(n,v) || A_Handler(n,v);
}

static constexpr CallHandler g_handler[] {
    {'a',A_Handler},
    {'r',R_Handler},
    {'w',W_Handler},
    {'t',T_Handler},
};

int main(int argc, char *argv[])
{
    int c {};
    char opt{},*name{},*value{};

    while( (c = getopt(argc, argv, "arwtn:v:"))  != -1 ) {

        switch( c ) {
            case 'a':
            case 'r':
            case 'w':
            case 't':
                opt = c;
                break;
            case 'n':
                name = optarg;
                break;
            case 'v':
                value = optarg;
                break;
            default:
                std::cout << "exit!\n\r";
                exit(-1);
                break;
        }
    }

    static constexpr int g_len {sizeof(g_handler)/sizeof(*g_handler)};

    for ( c = 0; c < g_len; c++){
        
        if (opt == g_handler[c].opt){
            g_handler[c].handler(name,value);
            break;
        }
    }

    return 0;
}
