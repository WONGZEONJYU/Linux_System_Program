#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>

static int g_init {255};
static float g_uninit;

static void text()
{

}

int main(int argc, char const *argv[])
{
    static double s_init {0.255};
    static double s_uninit;
    int i {};
    int* p {reinterpret_cast<int*>(malloc(4))};

    printf("argv[0] = %p\n", argv[0]);
    printf("&i = %p\n", &i);
    printf("p = %p\n", p);
    printf("&g_uninit = %p\n", &g_uninit);
    printf("&s_uninit = %p\n", &s_uninit);
    printf("&g_init = %p\n", &g_init);
    printf("&s_init = %p\n", &s_init);
    printf("text = %p\n", text);
    
    free(p);

    return 0;
}
