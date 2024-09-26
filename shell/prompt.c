#include "prompt.h"
#include "headers.h"


#define BLUE "\x1b[34m"
#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define COLOR_RESET "\x1b[0m"
#define COLOR_BOLD  "\e[1m"
#define COLOR_OFF   "\e[m"

void prompt(char *HOME, int fgtime, char* foreProcess)
{
    char NAME[1000], SYSTEM[1000], CURR_DIR[1000];
    memset(NAME, '\0', sizeof(NAME));
    // getlogin_r(NAME, sizeof(NAME));
    __uid_t uid = geteuid();
    struct passwd* pw = getpwuid(uid);
    strcpy(NAME, pw->pw_name);

    gethostname(SYSTEM, sizeof(SYSTEM));
    getcwd(CURR_DIR, sizeof(CURR_DIR));
    if (strlen(CURR_DIR) >= strlen(HOME))
    {
        bool ok = true;
        for (int i = 0; i < strlen(HOME); i++)
        {
            if (HOME[i] != CURR_DIR[i])
            {
                ok = false;
            }
        }
        if (ok)
        {
            char *final;
            final = CURR_DIR;
            final += strlen(HOME) - 1;
            final[0] = '~';
            if (fgtime == -1)
                printf("<"GREEN COLOR_BOLD "%s@%s:" COLOR_BOLD BLUE "%s" COLOR_RESET "> ", NAME, SYSTEM, final);
            else
                printf("<"GREEN COLOR_BOLD "%s@%s:" COLOR_BOLD BLUE "%s" COLOR_RESET " %s: " RED COLOR_BOLD "%ds" COLOR_RESET "> ", NAME, SYSTEM, final, foreProcess, fgtime);
        }
        else
        {
            if (fgtime == -1)
                printf("<"GREEN COLOR_BOLD "%s@%s:" COLOR_BOLD BLUE "%s" COLOR_RESET "> ", NAME, SYSTEM, CURR_DIR);
            else
                printf("<"GREEN COLOR_BOLD "%s@%s:" COLOR_BOLD BLUE "%s" COLOR_RESET " %s: " RED COLOR_BOLD "%ds" COLOR_RESET "> ", NAME, SYSTEM, CURR_DIR, foreProcess, fgtime);
        }
    }
    else
    {
        if (fgtime == -1)
            printf("<"GREEN COLOR_BOLD "%s@%s:" COLOR_BOLD BLUE "%s" COLOR_RESET "> ", NAME, SYSTEM, CURR_DIR);
        else
            printf("<"GREEN COLOR_BOLD "%s@%s:" COLOR_BOLD BLUE "%s" COLOR_RESET " %s: " RED COLOR_BOLD "%ds" COLOR_RESET "> ", NAME, SYSTEM, CURR_DIR, foreProcess, fgtime);
    }
    fflush(stdout);
    return;
}