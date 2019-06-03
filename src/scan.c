#ifdef __linux__
#include <linux/limits.h>
#else
#include <limits.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../include/scan.h"
#include "../include/main.h"
#include "../include/datastructure.h"
#include <sys/stat.h>
//#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

ScanInfo stats = {0, 0, 0, 0, 0, 0, 0};

RecordNode *readOutputFile(FILE *, RecordNode *);
RecordNode *readInputFile(FILE *, RecordNode *);
RecordNode *analisiSingolaRiga(char *, RecordNode *);
RecordNode *scanFilePath(char *, int, int, RecordNode *);
RecordNode *addFileAnalisis(struct stat *, char *, RecordNode *);
void printOutput(FILE *output, RecordNode *node);
void printOnFile(RecordNode *node, FILE *output);
void cleanFile(FILE *output);
int checkLength(struct stat *file);
int checkUID(struct stat *file);
int checkGID(struct stat *file);
int checkOptions(struct stat *file);
void updateStats(struct stat *file);
void printStats();
char *find_last_of(char *str, char c);
char *getAbsPath(char *path);

int startScan(FILE *input, FILE *output)
{
    RecordNode *data = malloc(sizeof(RecordNode));
    data = emptyNode();
    data = readOutputFile(output, data);
    if (opt_info.history_flag)
    {
        printHistory(data, opt_info.history_path);
    }
    data = readInputFile(input, data);
    printOutput(output, data);
    if (opt_info.stat_flag)
    {
        printStats();
    }
}

RecordNode *readOutputFile(FILE *output, RecordNode *data)
{
    size_t t = 1;
    char *line = NULL;
    char *currentPath = NULL;
    for (ssize_t read = getline(&line, &t, output); read >= 0; read = getline(&line, &t, output))
    {
        if (line[0] == '#' && line[1] == ' ')
        {
            free(currentPath);
            currentPath = malloc(strlen(line) + 1);
            strcpy(currentPath, strtok(line + 2, "\r\n"));
        }
        else if (line[0] != '#' && line[1] != '#' && line[2] != '#')
        {
            data = addRecordByPath(data, currentPath, strtok(line, "\r\n"));
        }
    }

    free(line);
    free(currentPath);
    return data;
}
void increaseMonitorati()
{
    stats.nr_monitorati++;
};
void increaseLink()
{
    stats.nr_link++;
}
void increaseDirectory()
{
    stats.nr_directory++;
}

void updateDimMedia()
{
    stats.dim_media = stats.dim_totale / stats.nr_monitorati;
}

void updateDimMax(int data)
{
    if (data > stats.dim_max)
    {
        stats.dim_max = data;
    }
};
void updateDimMin(int data)
{
    if (data < stats.dim_min)
    {
        stats.dim_min = data;
    }
}
void increaseDimTotale(int data)
{
    stats.dim_totale = stats.dim_totale + data;
    updateDimMedia();
    updateDimMax(data);
    updateDimMin(data);
}

RecordNode *readInputFile(FILE *input, RecordNode *data)
{
    size_t t = 2;
    char *line = NULL;
    for (ssize_t read = getline(&line, &t, input); read >= 0; read = getline(&line, &t, input))
    {
        data = analisiSingolaRiga(strtok(line, "\r\n"), data);
    }
    free(line);
    return data;
}

RecordNode *analisiSingolaRiga(char *riga, RecordNode *data)
{
    char *path = (char *)calloc(strlen(riga), sizeof(char));
    int isR = 0;
    int isL = 0;
    int pathRead = 0;
    char *token;
    for (token = strtok(riga, " "); token; token = strtok(NULL, " "))
    {
        if (strcmp(token, "r") == 0)
        {
            isR = 1;
        }
        else if (strcmp(token, "l") == 0)
        {
            isL = 1;
        }
        else if (!pathRead)
        {
            strcpy(path, token);
            pathRead = 1;
        }
    }

    data = scanFilePath(path, isR, isL, data);
    free(path);
    return data;
}

RecordNode *scanFilePath(char *path, int isR, int isL, RecordNode *data)
{
    if (opt_info.verbose_flag)
    {
        printf("\nInizio analisi path: %s\n", path);
        printf("R abilitato: %d\n", isR);
        printf("L abilitato: %d\n", isL);
    }

    struct stat *currentStat = (struct stat *)malloc(sizeof(struct stat));

    if (lstat(path, currentStat) < 0)
    {
        if (opt_info.verbose_flag)
            printf("Non sono riuscito ad aprire il file\n");
        return data;
    }

    if (S_ISLNK(currentStat->st_mode))
        increaseLink();

    if (isL == 0 && S_ISLNK(currentStat->st_mode))
    {
        data = scanFilePath(realpath(path, NULL), isR, isL, data);
    }
    else if (S_ISLNK(currentStat->st_mode))
    {
        updateStats(currentStat);
        data = addFileAnalisis(currentStat, getAbsPath(path), data);
    }
    else if (checkOptions(currentStat) && (!opt_info.noscan_flag))
    {
        updateStats(currentStat);
        data = addFileAnalisis(currentStat, realpath(path, NULL), data);
    }

    if (isR && S_ISDIR(currentStat->st_mode))
    {
        DIR *dir;
        struct dirent *entry;
        dir = opendir(path);
        if (dir)
        {
            while (entry = readdir(dir))
            {
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                    continue;

                char *copy = (char *)calloc(strlen(path) + strlen(entry->d_name) + 2, sizeof(char));
                strcpy(copy, path);
                if (copy[strlen(path) - 1] != '/')
                {
                    strcat(copy, "/");
                }
                strcat(copy, entry->d_name);
                data = scanFilePath(copy, isR, isL, data);
                free(copy);
            }
        }
        closedir(dir);
    }

    free(currentStat);
    return data;
}

RecordNode *addFileAnalisis(struct stat *currentStat, char *path, RecordNode *curTree)
{
    char dlinfo;
    if (S_ISDIR(currentStat->st_mode))
    {
        dlinfo = 'd';
    }
    else if (S_ISLNK(currentStat->st_mode))
    {
        dlinfo = 'l';
    }
    else
    {
        dlinfo = '-';
    }
    char irusr = currentStat->st_mode & S_IRUSR ? 'r' : '-';
    char iwusr = currentStat->st_mode & S_IWUSR ? 'w' : '-';
    char ixusr = currentStat->st_mode & S_IXUSR ? 'x' : '-';
    char irgrp = currentStat->st_mode & S_IRGRP ? 'r' : '-';
    char iwgrp = currentStat->st_mode & S_IWGRP ? 'w' : '-';
    char ixgrp = currentStat->st_mode & S_IXGRP ? 'x' : '-';
    char iroth = currentStat->st_mode & S_IROTH ? 'r' : '-';
    char iwoth = currentStat->st_mode & S_IWOTH ? 'w' : '-';
    char ixoth = currentStat->st_mode & S_IXOTH ? 'x' : '-';

    struct passwd *pwsUID = getpwuid(currentStat->st_uid);
    struct group *grpGID = getgrgid(currentStat->st_gid);

    char current_time[32];
    char time_last_access[32];
    char time_last_change[32];
    char time_last_chmod[32];

    char size[21];
    sprintf(size, "%ld", currentStat->st_size);
    char *record = malloc((200 + strlen(pwsUID->pw_name) + strlen(grpGID->gr_name) + strlen(size)) * sizeof(char));

    time_t curtime = time(NULL);
    time(&curtime);

    sprintf(record, "data - %s | uid - %s | gid - %s | dim - %s| perm - %c%c%c%c%c%c%c%c%c%c | acc - %s | change - %s | mod - %s | nlink - %ld",
            strtok(ctime_r(&curtime, current_time), "\n"),
            pwsUID->pw_name,
            grpGID->gr_name,
            size,
            dlinfo,
            irusr,
            iwusr,
            ixusr,
            irgrp,
            iwgrp,
            ixgrp,
            iroth,
            iwoth,
            ixoth,
            strtok(ctime_r(&currentStat->st_atime, time_last_access), "\n"),
            strtok(ctime_r(&currentStat->st_mtime, time_last_change), "\n"),
            strtok(ctime_r(&currentStat->st_ctime, time_last_chmod), "\n"),
            currentStat->st_nlink);

    if (opt_info.length_flag || opt_info.group_flag || opt_info.user_flag)
    {
        printf("# %s\n%s\n###\n", path, record);
    }

    curTree = addRecordByPath(curTree, path, strtok(record, "\r\n"));

    if (opt_info.verbose_flag)
    {
        printf("\nAnalisi del path %s effettuata correttamente\n", path);
    }

    free(record);
    return curTree;
}

void cleanFile(FILE *output)
{
    fflush(output);
    ftruncate(fileno(output), 0);
    fseek(output, 0, SEEK_SET);
    fflush(output);
}

void printOutput(FILE *output, RecordNode *node)
{
    cleanFile(output);
    printOnFile(node, output);
    fprintf(output, "###\n", NULL);
    fflush(output);
}

int checkLength(struct stat *file)
{
    if (!opt_info.length_flag)
    {
        return 1;
    }
    else if (opt_info.max_length >= file->st_size && opt_info.min_length <= file->st_size)
    {
        return 1;
    }
    else if (opt_info.max_length == 0 && opt_info.min_length <= file->st_size)
    {
        return 1;
    }
    else if (opt_info.min_length == 0 && opt_info.max_length >= file->st_size)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int checkUID(struct stat *file)
{
    if (!opt_info.user_flag)
    {
        return 1;
    }
    else
    {
        struct passwd *pwsUID = getpwuid(file->st_uid);
        return (strcmp(opt_info.uID, pwsUID->pw_name) == 0);
    }
}

int checkGID(struct stat *file)
{
    if (!opt_info.group_flag)
    {
        return 1;
    }
    else
    {
        struct group *grpGID = getgrgid(file->st_gid);
        return (strcmp(opt_info.gID, grpGID->gr_name) == 0);
    }
}

int checkOptions(struct stat *file)
{
    return (checkLength(file) && checkUID(file) && checkGID(file));
}

void updateStats(struct stat *file)
{
    increaseMonitorati();
    if (S_ISDIR(file->st_mode))
    {
        increaseDirectory();
    }
    increaseDimTotale(file->st_size);
}

void printStats()
{
    printf("\nStats: \n");
    printf("Numero file monitorati: %ld\n", stats.nr_monitorati);
    printf("Numero di link incontrati: %ld\n", stats.nr_link);
    printf("Numero di directory incontrati: %ld\n", stats.nr_directory);
    printf("Dimensione totale: %ld bytes\n", stats.dim_totale);
    printf("Dimensione media: %ld bytes\n", stats.dim_media);
    printf("Dimensione massima: %ld bytes\n", stats.dim_max);
    printf("Dimensione minima: %ld bytes\n\n", stats.dim_min);
}

char *find_last_of(char *str, char c)
{
    for (char *i = str + strlen(str); i >= str; i--)
        if (*i == c)
            return i;
    return NULL;
}

char *getAbsPath(char *path)
{
    char *name;
    char *tmp;
    char *absPath = malloc(PATH_MAX);

    tmp = find_last_of(path, '/');
    if (tmp == NULL)
    {
        name = strdup(path);
        getcwd(absPath, PATH_MAX);
    }
    else
    {
        name = strdup(tmp + 1);
        *tmp = '\0';
        realpath(path, absPath);
    }

    strcat(absPath, "/");
    strcat(absPath, name);
    free(name);
    return absPath;
}