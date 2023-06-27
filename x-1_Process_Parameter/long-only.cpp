#include <stdio.h>
#include <getopt.h>
#include <iostream>
#include <unistd.h>
int main(int argc, char* argv[])
{
    constexpr option long_options[] {
        { "test",    required_argument, nullptr, 't' },
        { "n",       required_argument, nullptr, 'n' }, 
        { nullptr,   0,                 nullptr,  0  },
    };

    int option_index{};

    while( true ){

        int c { getopt_long_only(argc, argv, "d:f", long_options, &option_index) };

        if( -1 == c ) {
            break;
        }

        std::cout << "c = " << c << "(" << static_cast<const char>(c) <<")" << 
            ", optopt = " <<  optopt << "("<< static_cast<const char>(optopt) << ")" << 
            ", optarg = " << (optarg? optarg : "(null)") << "\n";
    }

    if( optind < argc ){

        std::cout << "parameters:\n";
        
        while (optind < argc){
            std::cout << argv[optind++] << "\n";
        }
    }

   return 0;
}
