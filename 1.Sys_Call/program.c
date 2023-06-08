
void print(const char* s, int l);
void exit(int code);

void program()
{
    print("Hello World!\n", 13);
    exit(0);
}

void print(const char* s, int l)
{
    asm volatile (
        "movl $4, %%eax\n"  //指定编号为4的系统调用(sys_write)
        "movl $1, %%ebx\n"  //指定 sys_write的输出目标，1为标准输出
        "movl %0, %%ecx\n"  //指定输出字符串地址
        "movl %1, %%edx\n"  //指定输出字符串长度
        "int $0x80     \n"  //执行系统调用
        :                   //忽略输出参数
        : "r"(s), "r"(l)
        : "eax", "ebx", "ecx", "edx"  //保留寄存器，不用于关联变量
    );
}

void exit(int code)
{
    asm volatile (
        "movl $1, %%eax\n"
        "movl %0, %%ebx\n"
        "int $0x80     \n" 
        :
        : "r"(code)
        : "eax", "ebx"
    );
}
