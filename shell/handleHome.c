#include "handleHome.h"
#include "headers.h"

void handleHome(char *cur, char *HOME)
{
    int f = strlen(cur);
    int s = strlen(HOME);
    cur[f + s - 1] = '\0';
    char t[1000];
    strcpy(t, cur);
    if (cur[0] == '~')
    {
        for (int j = 0; j < strlen(HOME); j++)
        {
            cur[j] = HOME[j];
        }
        for (int j = 1; j < strlen(t); j++)
        {
            cur[j + s - 1] = t[j];
        }
    }
    return;
}