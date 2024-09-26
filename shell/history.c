#include "history.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define LOG_FILE "history"
#define MAX_HISTORY 15

char history[MAX_HISTORY][1000]; 
int current = 0; 
int total = 0;

void loadHistory() {
    FILE *file = fopen(LOG_FILE, "r");
    if (file == NULL) return;

    char line[1000];
    total = 0;
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; 
        if (total >= MAX_HISTORY) {
            // overwrite
            current = (current + 1) % MAX_HISTORY;
        }
        strcpy(history[current], line);
        current = (current + 1) % MAX_HISTORY;
        if (total < MAX_HISTORY) total++;
    }

    fclose(file);
}

void saveHistory() {
    FILE *file = fopen(LOG_FILE, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        int idx = (current - 1 - i + MAX_HISTORY) % MAX_HISTORY;
        if (strlen(history[idx]) > 0) {
            fprintf(file, "%s\n", history[idx]);
        }
    }

    fclose(file);
}

bool isLogCommand(char *cmd) {
    return strstr(cmd, "log") != NULL;
}

void addToHistory(char *cmd) {
    if (isLogCommand(cmd)) return;

    // check duplicates
    if (total > 0 && strcmp(cmd, history[(current + MAX_HISTORY - 1) % MAX_HISTORY]) == 0) {
        return;
    }

    strcpy(history[current], cmd);
    current = (current + 1) % MAX_HISTORY;
    if (total < MAX_HISTORY) total++;

    saveHistory();
}

void printHistory() {
    for (int i = 0; i < total; i++) {
        int idx = (current - 1 - i + MAX_HISTORY) % MAX_HISTORY;
        if (strlen(history[idx]) > 0) {
            printf("%d %s\n", i + 1, history[idx]);
        }
    }
}

void clearHistory() {
    current = 0;
    total = 0;
    remove(LOG_FILE);
}

void executeHistoryCommand(int index) {
    if (index <= 0 || index > total) {
        printf("Invalid history index.\n");
        return;
    }

    int cmdIndex = (current - index + MAX_HISTORY) % MAX_HISTORY;
    printf("Executing: %s\n", history[cmdIndex]);

    char* st = strdup(history[cmdIndex]);
    tokenizeAndRun(st);
}
