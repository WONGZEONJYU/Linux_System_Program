#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <string>

/*
选项 合法：返回值 为 选项字符，optarg 指向 选项值字符串
选项 不合法：返回字符“ ? ”，optopt 保存 当前选项字符（错误）
选项 合法但缺少选项值：返回“ : ”，optopt 保存 当前选项字符（错误）
默认情况下：getopt(…)对argv进行重排，所有操作数位于最后位置

optstring规则 的扩展定义
起始字符可以是 “ ：，+，- 或省略 ”
省略：出现选项错误时，程序中通过“ : ”或“ ? ”进行处理并给出默认错误提示
“ : ” ：错误提示开关，程序中通过返回值“ : ”或“ ? ”进行处理（无默认错误提示）
“ + ”：提前停止开关，遇见操作数时，返回 -1，认为选项处理完毕（后续都是操作数）
“ - ” ：不重排开关，遇见操作数时，返回 1，optarg指向操作数字符串
组合: "+:" or "-:"

*/

int main(int argc, char* argv[])
{
    int c{},iflg{},fflg{},sflg{};

    char* fvalue{},*svalue{};

    while( -1 != (c = getopt(argc, argv, "f:is::")) ) {

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
                svalue = optarg;
                break;
            case '?':
                //printf("Unknow option: -%c\n", optopt);
                std::cout << "Unknow option: -" << static_cast<char>(optopt) << "\n";
                break;
            case ':':
                //printf("-%c missing option argument\n", optopt);
                std::cout << "-" << static_cast<char>(optopt) << " missing option argument\n";
                break;
            case 1:
                //printf("inter: %s\n", optarg);
                std::cout << "inter: " << (optarg ? optarg : "(null)") << "\n";
                break;
            default:
                //printf("ret = %d\n", c);
                std::cout << "ret = " << c << "\n";
                break;
        }
    }

    std::cout << "fflg = " << fflg << ", fvalue = " << (fvalue ? fvalue : "(null)") << 
            ", iflg = " << iflg << ", sflg = " << sflg << ", svalue = " << (svalue ? svalue : "(null)") << "\n";

    for(int i {optind}; i < argc; i++){
        std::cout << "parameter: " << (argv[i] ? argv[i] : "(null)") << "\n";
    }

    return 0;
}
