#include "prompt.h"
#include "death.h"
#include "history.h"
#include "handleHome.h"
#include "headers.h"
#include "ls.h"
#include "proclore.h"

#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define BLU "\e[0;34m"
#define RESET "\x1b[0m"

char HOME[1000];
int cnt;
int childIds[100];
char ChildNames[100][256];
char pDir[1000];
int ForegroundTime = -1;
char* foreProcess;
char months[100][100];

int max(int x, int y)
{
	if (x > y)
		return x;
	return y;
}

char *getUserName()
{
	uid_t UID = geteuid();
	struct passwd *PW = getpwuid(UID);
	return PW->pw_name;
}

void printCurrentDirectory()
{
	char CURR_DIR[1000];
	getcwd(CURR_DIR, sizeof(CURR_DIR));
	printf("%s\n", CURR_DIR);
	return;
}

void changeDirectory(char Directory[])
{
	char CURR_DIR[1000], temp[1000];
	getcwd(CURR_DIR, sizeof(CURR_DIR));
	strcpy(temp, pDir);
	strcpy(pDir, CURR_DIR);
	if (chdir(Directory) == -1)
	{
		printf("No such File or Directory %s\n", Directory);
		strcpy(pDir, temp);
	}
	return;
}

void printContentDirectory(char Directory[])
{
	struct dirent *de;
	DIR *dr = opendir(Directory);
	if (dr == NULL)
	{
		return;
	}
	while ((de = readdir(dr)) != NULL)
	{
		char path[1000];
		path[0] = '\0';
		strcpy(path, Directory);
		strcat(path, "/");
		strcat(path, de->d_name);
		if (strcmp(de->d_name, ".."))
			printf("%s\n", path);
		if (de->d_name[0] == '.')
			continue;
		printContentDirectory(path);
	}
	return;
}

void printFilesOnly(char Directory[])
{
	struct dirent *de;
	DIR *dr = opendir(Directory);
	if (dr == NULL)
	{
		return;
	}
	while ((de = readdir(dr)) != NULL)
	{
		char path[1000];
		path[0] = '\0';
		strcpy(path, Directory);
		strcat(path, "/");
		strcat(path, de->d_name);
		if (de->d_type == DT_REG)
			printf("%s\n", path);
		if (de->d_name[0] == '.')
			continue;
		printFilesOnly(path);
	}
}

void FindFile(char Directory[], char File[])
{
	struct dirent *de;
	DIR *dr = opendir(Directory);
	if (dr == NULL)
	{
		return;
	}
	while ((de = readdir(dr)) != NULL)
	{
		char path[1000];
		path[0] = '\0';
		strcpy(path, Directory);
		strcat(path, "/");
		strcat(path, de->d_name);
		if (!strcmp(de->d_name, File))
		{
			printf("%s\n", path);
		}
		if (de->d_name[0] == '.')
			continue;
		FindFile(path, File);
	}
}

void seek(char *list_[], int len)
{
    int type = 0;
    int e_flag = 0;
    char file[1000], directory[1000], list[1000][1000];
    for (int i = 0; i < len; i++)
        strcpy(list[i], list_[i]);
    
    getcwd(directory, sizeof(directory));
    file[0] = '\0';

    for (int i = 1; i < len; i++)
    {
        if (!strcmp(list[i], "-d"))
            type |= 1;
        else if (!strcmp(list[i], "-f"))
            type |= 2;
        else if (!strcmp(list[i], "-e"))
            e_flag = 1;
        else if (file[0] == '\0')
            strcpy(file, list[i]);
        else
        {
            handleHome(list[i], HOME);
            strcpy(directory, list[i]);
        }
    }

    if (type == 3)
    {
        printf("Invalid flags!\n");
        return;
    }

    int matches = 0;
    char last_match[1000];
    last_match[0] = '\0';

    void search(const char *dir, const char *target, int type, int *matches, char *last_match)
    {
        DIR *d = opendir(dir);
        if (!d) return;

        struct dirent *entry;
        while ((entry = readdir(d)) != NULL)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            char path[1000];
            snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name);

            struct stat statbuf;
            if (stat(path, &statbuf) == -1)
                continue;

            int is_dir = S_ISDIR(statbuf.st_mode);

            if (strcmp(entry->d_name, target) == 0 || strcmp(target, ".") == 0)
            {
                if ((type == 0) || (type == 1 && is_dir) || (type == 2 && !is_dir))
                {
                    (*matches)++;
                    strcpy(last_match, path);
                    char relative_path[1000];
                    if (strlen(path) > strlen(directory) + 1)
                        strcpy(relative_path, path + strlen(directory) + 1);
                    else
                        strcpy(relative_path, path);
                    
                    if (is_dir)
                        printf(BLU "./%s\n" RESET, relative_path);
                    else
                        printf(GRN "./%s\n" RESET, relative_path);
                }
            }

            if (is_dir)
                search(path, target, type, matches, last_match);
        }
        closedir(d);
    }

    search(directory, file, type, &matches, last_match);

    if (matches == 0)
    {
        printf("No match found!\n");
    }
    else if (e_flag && matches == 1)
    {
        struct stat statbuf;
        if (stat(last_match, &statbuf) == 0)
        {
            if (S_ISDIR(statbuf.st_mode))
            {
                if (access(last_match, X_OK) == 0)
                {
                    chdir(last_match);
                    printf("%s/\n", last_match);
                }
                else
                {
                    printf("Missing permissions for task!\n");
                }
            }
            else
            {
                if (access(last_match, R_OK) == 0)
                {
                    FILE *file = fopen(last_match, "r");
                    if (file)
                    {
                        char buffer[1024];
                        while (fgets(buffer, sizeof(buffer), file))
                        {
                            printf("%s", buffer);
                        }
                        fclose(file);
                    }
                }
                else
                {
                    printf("Missing permissions for task!\n");
                }
            }
        }
    }
}

void initMonths()
{
	for (int mon = 1; mon <= 12; mon++)
	{
		switch (mon)
		{
		case 1:
			strcpy(months[mon], "Jan");
			break;
		case 2:
			strcpy(months[mon], "Feb");
			break;
		case 3:
			strcpy(months[mon], "Mar");
			break;
		case 4:
			strcpy(months[mon], "Apr");
			break;
		case 5:
			strcpy(months[mon], "May");
			break;
		case 6:
			strcpy(months[mon], "Jun");
			break;
		case 7:
			strcpy(months[mon], "Jul");
			break;
		case 8:
			strcpy(months[mon], "Aug");
			break;
		case 9:
			strcpy(months[mon], "Sep");
			break;
		case 10:
			strcpy(months[mon], "Oct");
			break;
		case 11:
			strcpy(months[mon], "Nov");
			break;
		case 12:
			strcpy(months[mon], "Dec");
			break;
		}
	}
	return;
}

void executeFront(char *list[], int len)
{
	time_t t;
	t = time(NULL);
	pid_t processId;
	processId = fork();
	if (processId < 0) // Error Handling
	{
		perror("!!!Error!!!");
	}
	else if (processId > 0) // This is parent
	{
		int stat;
		waitpid(processId, &stat, WUNTRACED);
		while (!WIFEXITED(stat) && !WIFSIGNALED(stat))
			waitpid(processId, &stat, WUNTRACED);
		t = time(NULL) - t;
		if ((int)t >= 1)
		{
			ForegroundTime = t;
			foreProcess = strdup(list[0]);
		}
	}
	else // This is child
	{
		list[len] = NULL;
		if (execvp(list[0], list) == -1)
		{
			char buffer[256];
			sprintf(buffer, "Invalid Command '%s'", list[0]);
			perror(buffer);
		}
	}
	return;
}

void executeBehind(char *list_[], int len)
{
    if (len > 0 && list_[len - 1][strlen(list_[len - 1]) - 1] == '&')
    {
        list_[len - 1][strlen(list_[len - 1]) - 1] = '\0';
        if (strlen(list_[len - 1]) == 0)
        {
            len--;
        }
    }

    // Create a new argument list for execvp
    char *list[1000];
    for (int i = 0; i < len; i++)
    {
        list[i] = list_[i];
    }
    list[len] = NULL; // NULL terminate the argument list

    time_t t;
    t = time(NULL);

    pid_t processId;
    processId = fork();
    if (processId < 0) // Error Handling
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (processId > 0) // This is parent
    {
        childIds[cnt++] = processId;
        strcpy(ChildNames[cnt - 1], list[0]);
        printf("[%d] %d\n", cnt, childIds[cnt - 1]);
    }
    else // This is child
    {
        // Redirect standard input/output to /dev/null to avoid terminal interaction
        freopen("/dev/null", "r", stdin);
        freopen("/dev/null", "w", stdout);
        freopen("/dev/null", "w", stderr);

        if (execvp(list[0], list) == -1)
        {
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
    }
}

void executeBultin(char *list[], int len)
{
	if (strcmp("hop", list[0]) == 0)
	{
		// if (len > 2)
		// {
		// 	printf("!!! Error for hop Command to have more than One Command Line Arguement !!!");
		// 	return;
		// }
		if (len == 1)
		{
			char X[1000];
			strcpy(X, HOME);
			changeDirectory(X);
			return;
		}
		char *parentDirectory;
		char CURR_DIR[1000];

		for (int k = 1; k < len; k++) {
			char* tok = strtok(list[k], "/");
			while (tok != NULL && strlen(tok) != 0) {
				getcwd(CURR_DIR, sizeof(CURR_DIR));
				parentDirectory = dirname(CURR_DIR);
				if (strcmp(tok, "~") == 0)
				{
					char X[1000];
					int j = 0;
					for (int i = 0; i < strlen(HOME); i++)
						X[j++] = HOME[i];
					for (int i = 1; i < strlen(tok); i++)
						X[j++] = tok[i];
					X[j] = '\0';
					changeDirectory(X);
				}
				else if (strcmp(tok, "..") == 0 || strcmp(tok, "..") == 0)
				{
					changeDirectory(parentDirectory);
				}
				else if (strcmp(tok, "-") == 0)
				{
					char Y[1000];
					strcpy(Y, pDir);
					changeDirectory(Y);
					printCurrentDirectory();
				}
				else
					changeDirectory(tok);
				
				tok = strtok(NULL, "/");
			}
		}
	}
	else if (strcmp("echo", list[0]) == 0)
	{
		for (int i = 1; i < len; i++)
			printf("%s ", list[i]);
		printf("\n");
	}
	else if (strcmp("pwd", list[0]) == 0)
	{
		printCurrentDirectory();
	}
	else if (strcmp("log", list[0]) == 0) {
		if (len == 2 && strcmp(list[1], "purge") == 0) {
			clearHistory();
		} else if (len == 3 && strcmp(list[1], "execute") == 0) {
			int index = atoi(list[2]);
			executeHistoryCommand(index);
		} else {
			printHistory();
		}
	}
	else if (strcmp("seek", list[0]) == 0)
	{
		seek(list, len);
	}
	else if (strcmp("reveal", list[0]) == 0)
	{
		listFilesAndDirectories(list, len);
	}
	else if (strcmp("proclore", list[0]) == 0)
	{
		if (len == 1)
			proclore(getpid());
		else
			proclore(atoi(list[1]));
	}
	return;
}

bool builtin(char *X)
{
	if (!strcmp(X, "hop") || !strcmp(X, "pwd") || !strcmp(X, "echo") || !strcmp(X, "log") || !strcmp(X, "seek") || !strcmp(X, "reveal") || !strcmp(X, "proclore"))
		return true;
	return false;
}

void reset()
{
	printf("\033[0m");
}

void tokenizeAndRun(char *inp)
{
	char *f, *s;
	char *t[100];
	char add[1000];
	strcpy(add, inp);
	addToHistory(inp);
	for (char *txtf = strtok_r(inp, ";", &f); txtf != NULL; txtf = strtok_r(NULL, ";", &f))
	{
		int idx = 0;
		for (char *txts = strtok_r(txtf, " \n\t", &s); txts != NULL; txts = strtok_r(NULL, " \n\t", &s))
		{
			t[idx++] = txts;
		}
		if (builtin(t[0]))
			executeBultin(t, idx);
		else
		{
			int start = 0;
			for (int i = 0; i < idx; i++)
			{
				if (t[i][strlen(t[i]) - 1] == '&')
				{
					executeBehind(&t[start], i - start + 1);
					start = i + 1;
				}
			}
			if (start < idx)
			{
				if (builtin(t[start]))
					executeBultin(&t[start], idx - start);
				else
					executeFront(&t[start], idx - start);
			}
		}
	}
	return;
}

void execute(char *inp)
{
	tokenizeAndRun(inp);
	// saveHistory();
	return;
}

void run_terminal()
{
	prompt(HOME, ForegroundTime, foreProcess);
	ForegroundTime = -1;
	foreProcess = "";
	char *INP = NULL;
	size_t len = 0;
	int ret = getline(&INP, &len, stdin);
	if (ret == -1)
	{
		perror("Error Occured Reading Input");
		return;
	}
	if (strcmp(INP, "\n") == 0)
		return;
	if (strcmp(INP, "EXIT\n") == 0)
		exit(0);
	execute(INP);
	return;
}

int main()
{
	loadHistory();
	setvbuf(stdout, NULL, _IONBF, 0);
	signal(SIGCHLD, DEATH);
	getcwd(HOME, sizeof(HOME));
	getcwd(pDir, sizeof(pDir));
	
	printf("\e[1;1H\e[2J");
	
	while (true)
	{
		run_terminal();
	}
	return 0;
}