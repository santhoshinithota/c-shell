#include "ls.h"
#include "headers.h"
#include "handleHome.h"
#include "prompt.h"

#define BLK "\e[0;30m"
#define BLU "\x1b[34m"
#define GRN "\x1b[32m"
#define RED "\x1b[31m"
#define COLOR_RESET "\x1b[0m"

void mySwap(char *X, char *Y)
{
    char temp[1000];
    strcpy(temp, X);
    strcpy(X, Y);
    strcpy(Y, temp);
    return;
}

void printTimeUpdated(struct stat info, char *nameOfFile)
{
    struct passwd *pw = getpwuid(info.st_uid);
    struct group *gr = getgrgid(info.st_gid);
    char lastUpdated[100];
    printDirectoryInformation(info);
    strftime(lastUpdated, 25, "%b %d %H:%M ", localtime(&(info.st_mtime)));

    printf("%0.3hu %-8.8s %-8.8s  %-8lld %s ", info.st_nlink, pw->pw_name, gr->gr_name, info.st_size, lastUpdated);
    
    struct stat sb;
    if (S_ISDIR(info.st_mode))
    {
        printf(BLU);
    }
    else if (access(nameOfFile, X_OK) == 0) {
       printf(GRN);
    }
    else
    {
        // printf(RED);
    }
    printf("%s\n", nameOfFile);
    printf(COLOR_RESET);
    return;
}

void printDirectoryOnly(char Directory[])
{
    struct dirent *de;
    DIR *dr = opendir(Directory);
    if (dr == NULL)
    {
        return;
    }
    while ((de = readdir(dr)) != NULL)
    {
        char path[100];
        path[0] = '\0';
        strcpy(path, Directory);
        strcat(path, "/");
        strcat(path, de->d_name);
        if (de->d_type == DT_DIR && strcmp(de->d_name, ".."))
            printf("%s\n", path);
        if (de->d_name[0] == '.')
            continue;
        printDirectoryOnly(path);
    }
}

void printDirectoryInformation(struct stat info)
{
    printf((S_ISDIR(info.st_mode)) ? "d" : "-");
    printf((info.st_mode & S_IRUSR) ? "r" : "-");
    printf((info.st_mode & S_IWUSR) ? "w" : "-");
    printf((info.st_mode & S_IXUSR) ? "x" : "-");
    printf((info.st_mode & S_IRGRP) ? "r" : "-");
    printf((info.st_mode & S_IWGRP) ? "w" : "-");
    printf((info.st_mode & S_IXGRP) ? "x" : "-");
    printf((info.st_mode & S_IROTH) ? "r" : "-");
    printf((info.st_mode & S_IWOTH) ? "w" : "-");
    printf((info.st_mode & S_IXOTH) ? "x   " : "-   ");
    return;
}

void sortForLs(char name[1000][500], struct stat stats[], int index)
{
    for (int j = 0; j < index; j++)
    {
        for (int i = 0; i < index - 1; i++)
        {
            if (strcasecmp(name[i], name[i + 1]) > 0)
            {
                mySwap(name[i], name[i + 1]);
                struct stat tmp = stats[i];
                stats[i] = stats[i + 1];
                stats[i + 1] = tmp;
            }
        }
    }
    return;
}

void listFilesAndDirectories(char *list_[], int len)
{
    char list[1000][1000];
    for (int i = 0; i < len; i++)
        strcpy(list[i], list_[i]);
    int flag = 0;
    for (int i = 1; i < len; i++)
    {
        if (!strcmp(list[i], "-a")) //-a is for all files including hidden
            flag |= 1;
        else if (!strcmp(list[i], "-l")) //-l is for showing extra info
            flag |= 2;
        else if (!strcmp(list[i], "-al") || !strcmp(list[i], "-la"))
            flag = 3;
        else if (list[i][0] == '-') {
            int k = 1;
            while (list[i][k] != '\0'){
                if (list[i][k] == 'l') flag |= 2;
                else if (list[i][k] == 'a') flag |= 1;
                else {
                    flag = 0;
                    break;
                }
                k++;
            }
        }
        else if (list[i][0] == '~')
        {
            handleHome(list[i], HOME);
        }
    }
    struct stat firstStat[10000], secondStat[10000];
    char firstName[1000][500], secondName[1000][500];
    int firstIndex = 0, secondIndex = 0;
    for (int i = 1; i < len; i++)
    {
        if (!strcmp(list[i], "-a"))
            continue;
        else if (!strcmp(list[i], "-l"))
            continue;
        else if (!strcmp(list[i], "-al") || !strcmp(list[i], "-la"))
            continue;
        struct stat info;
        int ret = stat(list[i], &info);
        if (ret == -1)
        {
            printf("No such file or directory %s\n", list[i]);
            continue;
        }
        if (S_ISREG(info.st_mode))
        {
            firstStat[firstIndex++] = info;
            strcpy(firstName[firstIndex - 1], list[i]);
        }
        else
        {
            secondStat[secondIndex++] = info;
            strcpy(secondName[secondIndex - 1], list[i]);
        }
    }
    sortForLs(firstName, firstStat, firstIndex);
    sortForLs(secondName, secondStat, secondIndex);
    if (firstIndex == 0 && secondIndex == 0)
    {
        strcpy(secondName[0], ".");
        secondIndex++;
        struct stat info;
        int ret = stat(".", &info);
        if (ret == -1)
        {
            printf("!!!Error!!!");
        }
        secondStat[secondIndex - 1] = info;
    }
    for (int i = 0; i < firstIndex; i++)
    {
        if (flag & (1 << 1))
            printTimeUpdated(firstStat[i], firstName[i]);
        else
            printf("%s\n", firstName[i]);
    }
    for (int i = 0; i < secondIndex; i++)
    {
        if (secondIndex + firstIndex > 1)
            printf("%s:\n", secondName[i]);
        DIR *d;
        struct dirent *dir;
        d = opendir(secondName[i]);
        char tempName[1000][500];
        struct stat tempStats[10000];
        int tempIndex = 0;
        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                if (!(flag & 1))
                {
                    if (dir->d_name[0] == '.')
                        continue;
                }
                strcpy(tempName[tempIndex++], dir->d_name);
                char path[1000];
                path[0] = '\0';
                strcpy(path, secondName[i]);
                strcat(path, "/");
                strcat(path, dir->d_name);
                stat(path, &tempStats[tempIndex - 1]);
            }
            closedir(d);
            sortForLs(tempName, tempStats, tempIndex);
            for (int i = 0; i < tempIndex; i++)
            {
                if (!(flag & (1 << 1)))
                    printf("%s\n", tempName[i]);
                else {
                    printTimeUpdated(tempStats[i], tempName[i]);
                }
            }
        }
        else
        {
            printf("!Error!");
        }
    }
    return;
}
