#include <cstdio>
#include <getopt.h>
#include <iostream>

int main(int argc, char* argv[])
{
    int option_index{},flag{-1};

    const option long_options[] {
        { "add",     required_argument, &flag, 'a' },
        { "delete",  required_argument, &flag, 'd' }, 
        { "clear",   no_argument,       &flag, 'c' },
        { "list",    optional_argument, &flag, 'l' },
        { nullptr,   0,                 nullptr,0  },
    };

    while( true ){

        int c { getopt_long(argc, argv, "n:ml::", long_options, &option_index) };
        
        if( -1 == c ) {
            break;
        }

        std::cout << "flag = " << flag << "(" << static_cast<char>(flag) << ")" << 
            ", c = " << c << "(" << static_cast<char>(c) << ")" << "\n";
 
        if( c ){
            if( '?' != c ){

                std::cout << "short: option: " << static_cast<char>(c) << ", value : " << (optarg ? optarg : "(null)") << "\n";
            }
        }else{

            std::cout << "long: option: " << ( long_options[option_index].name ? long_options[option_index].name : "(null)" ) << 
                ", value: " << (optarg ? optarg : "(null)") << "\n";
        }
    }

    if( optind < argc ){

        std::cout << "parameters:\n";

        while (optind < argc){
            std::cout << argv[optind++] << "\n";
        }
    }

   return 0;
}
