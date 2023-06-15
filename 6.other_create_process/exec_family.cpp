#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

#define RC(s)   const_cast<char*>(#s)

int main(int argc, char *argv[])
{
    char pids[32]{};
    char* const ps_argv[] {RC(pstree),RC(-A),RC(-p),RC(-s),pids,nullptr};
    char* const ps_envp[] {RC(PATH=/bin:/usr/bin),RC(TEST=WONG),nullptr};

    sprintf(pids,"%d",getpid());

    execl("/bin/pstree", "pstree", "-A", "-p", "-s", pids, nullptr);
    execlp("pstree", "pstree", "-A", "-p", "-s", pids, nullptr);
    execle("/bin/pstree", "pstree", "-A", "-p", "-s", pids, nullptr, ps_envp);
    execv("/bin/pstree", ps_argv);
    execvp("pstree", ps_argv);
    execve("/bin/pstree", ps_argv, ps_envp);

    return 0;
}

