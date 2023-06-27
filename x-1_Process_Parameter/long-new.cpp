#include <stdio.h>
#include <getopt.h>
#include <iostream>

int main(int argc, char* argv[])
{
    constexpr option long_options[] {
        { "add",     required_argument, nullptr, 'a' },
        { "delete",  required_argument, nullptr, 'd' }, 
        { "clear",   no_argument,       nullptr, 'c' },
        { "list",    optional_argument, nullptr, 'l' },
        { nullptr,   0,                 nullptr,  0  }
    };

    int option_index{};

    while( true ){

        int c {getopt_long(argc, argv, "a:d:cl::", long_options, &option_index)};
        
        if( -1 == c) {
            break;
        }

        std::cout << "c = " << c << "("<< static_cast<char>(c) << ")" << 
            ", optopt = " << optopt << "("<< static_cast<char>(optopt) << ")" << 
            ", optarg = " << (optarg ? optarg : "(null)") << "\n";
    }

    if( optind < argc ){

        std::cout << "parameters:\n";
        
        while (optind < argc){
            std::cout << argv[optind++] << "\n";
        }
    }

   return 0;
}

