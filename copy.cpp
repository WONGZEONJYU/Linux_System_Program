#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

/*通过linux系统调用open read wrire close复制文件*/
static int test1(char const *argv[])
{
    cout << "begin " << __FUNCTION__ << "\n";

    auto src_file {argv[1]},dst_file{argv[2]};
    
    const auto src_fd{open64(src_file,O_RDONLY)},
        dst_fd{open64(dst_file,O_CREAT|O_RDWR|O_TRUNC,0777)};

    if (-1 == src_fd){
        cerr << "src file open error...\n";
        return -1;
    }

    if (-1 == dst_fd){
        cerr << "dst file open error...\n";
        return -1;
    }

    lseek64(src_fd,0,SEEK_SET);
    lseek64(dst_fd,0,SEEK_SET);

    for(;;){
        
        constexpr auto B_SIZE{256};
        char buf[B_SIZE]{};
        
        auto rbytes {read(src_fd,buf,B_SIZE)};
        if ((-1 == rbytes)){
            cerr << "read error...\n";
            exit(-1);
        }
        if (!rbytes){
            break;
        }

        auto wbytes {write(dst_fd,buf,rbytes)};
        if (-1 == wbytes){
            cerr << "write error...\n";
            exit(-1);
        }
    }

    close(src_fd);
    close(dst_fd);

    cout << "end " << __FUNCTION__ << "\n";
    cout << "copy suceess\n";

    return 0;
}

/*通过标准io的fgets fputs复制文件*/
static int test2(char const *argv[])
{
    cout << "begin " << __FUNCTION__ << "\n";

    auto src_file {argv[1]},dst_file{argv[2]};
    
    const auto src_f{fopen64(src_file,"rb")},
        dst_f{fopen64(dst_file,"wb+")};

    if (!src_f){
        cerr << src_file << " file open error...\n";
        return -1;
    }

    if (!dst_f){
        cerr << dst_file <<" file open error...\n";
        return -1;
    }

    for (;;){
        const auto ch{fgetc(src_f)};

        if (EOF == ch){
            if (!errno){
                break;
            }else{
                cerr << "read error...\n";
                exit(-1);
            }
        }

        if (EOF == fputc(ch,dst_f)){
            if (errno){
                cerr << "write error...\n";
                exit(-1);
            }
        }
    }

    fclose(src_f);
    fclose(dst_f);
    
    cout << "end " << __FUNCTION__ << "\n";
    cout << "copy suceess\n";
    
    return 0;
}

/*通过标准io的fgetc fputc复制文件*/
static int test3(char const *argv[])
{
    cout << "begin " << __FUNCTION__ << "\n";

    auto src_file {argv[1]},dst_file{argv[2]};
    
    const auto src_f{fopen64(src_file,"rb")},
        dst_f{fopen64(dst_file,"wb+")};

    if (!src_f){
        cerr << src_file << " file open error...\n";
        return -1;
    }

    if (!dst_f){
        cerr << dst_file <<" file open error...\n";
        return -1;
    }
    
    for (;;){
        
        constexpr auto B_SIZE{256};
        char buf[B_SIZE]{};

        if (!fgets(buf,B_SIZE,src_f)){
            if (errno){
                cerr << "read error...\n";
                exit(-1);
            }else{
                break;
            }
        }

        if (EOF == fputs(buf,dst_f));{
            if (errno){
                cerr << "write error...\n";
                exit(-1);
            }
        }
    }

    fclose(src_f);
    fclose(dst_f);
    
    cout << "end " << __FUNCTION__ << "\n";

    cout << "copy suceess\n";

    return 0;
}

/*通过fread fwrire复制文件*/
static int test4(char const *argv[])
{
    cout << "begin " << __FUNCTION__ << "\n";

    auto src_file {argv[1]},dst_file{argv[2]};
    
    const auto src_f{fopen64(src_file,"rb")},
        dst_f{fopen64(dst_file,"wb+")};

    if (!src_f){
        cerr << src_file << " file open error...\n";
        return -1;
    }

    if (!dst_f){
        cerr << dst_file <<" file open error...\n";
        return -1;
    }

        
    for (;;){

        constexpr auto B_SIZE{64};
        int buf[B_SIZE]{};
        constexpr auto type_size{sizeof(*buf)};

        const auto rbytes {fread(buf,type_size,B_SIZE,src_f)};
        
        if ((!rbytes) || (rbytes < B_SIZE)){

            if (errno){
                cerr << "read error...\n";
                exit(-1);
            }else{
                break;
            }
        }

        const auto wbytes{fwrite(buf,type_size,B_SIZE,dst_f)};
        if ((!wbytes) || (wbytes < B_SIZE)){
            if (errno){
                cerr << "write error...\n";
                exit(-1);
            }
        }
    }
    
    fclose(src_f);
    fclose(dst_f);
    
    cout << "end " << __FUNCTION__ << "\n";
    cout << "copy suceess\n";

    return 0;
}

int main(int argc, char const *argv[])
{
    if (argc < 3){
        cerr << "parm less...\n";
        return -1;
    }

    return test4(argv);
}
