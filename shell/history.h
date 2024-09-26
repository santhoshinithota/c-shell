#ifndef __HISTORY_H__
#define __HISTORY_H__

#include <stdlib.h>
#include <stdbool.h>

void addToHistory(char *cmd);
void printHistory();
void clearHistory();
void executeHistoryCommand(int index);
bool isLogCommand(char *cmd);
void tokenizeAndRun(char *inp);

void loadHistory();
void saveHistory();

#endif
