#ifndef LS_H
#define LS_H
#include "headers.h"

void listFilesAndDirectories(char *list_[], int len);
void sortForLs(char name[1000][500], struct stat stats[], int index);
void printTimeUpdated(struct stat info, char *nameOfFile);
void printDirectoryOnly(char Directory[]);
void printDirectoryInformation(struct stat info);
void mySwap(char *X, char *Y);

#endif
