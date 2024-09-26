#include "proclore.h"
#include "handleHome.h"
#include "prompt.h"
#include "headers.h"

void proclore(pid_t pid) {
    char path[50], mid[50], exePath[200];
    char processInfo[100];
    sprintf(mid, "/proc/%d", pid);
    strcpy(path, mid);
    strcat(path, "/status");

    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Status file couldn't be opened for PID %d\n", pid);
        return;
    }

    char state = 'R';
    char memory[100] = "";
    int processGroup = -1;

    while (fgets(processInfo, sizeof(processInfo), fp) != NULL) {
        if (strncmp(processInfo, "State:", strlen("State:")) == 0) {
            for (int i = 6; i < strlen(processInfo); i++) {
                if (processInfo[i] == ' ' || processInfo[i] == ':' || processInfo[i] == '\t') {
                    continue;
                }
                state = processInfo[i];
                break;
            }
        }

        if (strncmp(processInfo, "VmRSS:", strlen("VmRSS:")) == 0) {
            sscanf(processInfo, "VmRSS: %s", memory);
        }

        if (strncmp(processInfo, "Tgid:", strlen("Tgid:")) == 0) {
            sscanf(processInfo, "Tgid: %d", &processGroup);
        }
    }
    fclose(fp);

    sprintf(mid, "/proc/%d/exe", pid);
    readlink(mid, exePath, sizeof(exePath));

    printf("pid : %d\n", pid);
    printf("process status : %c\n", state);
    printf("process group : %d\n", processGroup);
    printf("virtual memory : %s kB\n", memory);
    printf("executable path : %s\n", exePath);
}
