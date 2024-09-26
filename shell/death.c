#include "death.h"
#include "headers.h"
#include "prompt.h"


void DEATH()
{
    int wstat;
    pid_t pid;
    while (true)
    {
        pid = wait3(&wstat, WNOHANG, (struct rusage *)NULL);
        if (pid == 0)
            return;
        else if (pid == -1)
            return;
        else
        {
            for (int i = 0; i < cnt; i++)
            {
                if (childIds[i] == pid)
                {
                    printf(WIFEXITED(wstat)?"\n%s with pid = %d exited normally\n":"\n%s with pid = %d exited abnormally\n", ChildNames[i], childIds[i]);
                    prompt(HOME, ForegroundTime, foreProcess);
                    childIds[i] = -1;
                    return;
                }
            }
        }
    }
}

