#ifndef __PROMPT_H__
#define __PROMPT_H__

extern char HOME[1000];
extern int cnt;
extern int childIds[100];
extern char ChildNames[100][256];
extern char pDir[1000];
extern int ForegroundTime;
extern char* foreProcess;
extern char months[100][100];

void prompt(char *HOME, int fgtime, char* foreProcess);

#endif
