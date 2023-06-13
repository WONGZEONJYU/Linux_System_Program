#include <cstdio>
#include <cstdlib>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    printf("original:\n");
    
    printf("%s=%s\n", "TEST1", getenv(const_cast<char*>("TEST1")));
    printf("%s=%s\n", "TEST2", getenv(const_cast<char*>("TEST2")));
    printf("%s=%s\n", "TEST3", getenv(const_cast<char*>("TEST3")));
    
    putenv(const_cast<char*>("TEST1"));//clear "TEST1" env
    putenv(const_cast<char*>("TEST2=NEW-VALUE"));
    putenv(const_cast<char*>("TEST3=CREATE NEW"));

    printf("new:\n");
    
    printf("%s=%s\n", "TEST1", getenv(const_cast<char*>("TEST1")));
    printf("%s=%s\n", "TEST2", getenv(const_cast<char*>("TEST2")));
    printf("%s=%s\n", "TEST3", getenv(const_cast<char*>("TEST3")));

    printf("changed:\n");

    int i {};

    while (environ[i]){
        printf("exec = %d, %s\n", getpid(), environ[i++]);
    }
    return 0;
}
