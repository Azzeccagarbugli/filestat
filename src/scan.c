#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../include/scan.h"
#include "../include/main.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

struct ScanInfo scan_info = {0, 0, 0, 0, 0, 0, 0};

int startScan(FILE *input, FILE *output)
{
    readInputFile(input);
    printf("\n###\n");
}

void increaseMonitorati()
{
    scan_info.nr_monitorati++;
};
void increaseLink()
{
    scan_info.nr_link++;
}
void increaseDirectory()
{
    scan_info.nr_directory++;
}

void updateDimMedia()
{
    scan_info.dim_media = scan_info.dim_totale / scan_info.nr_monitorati;
}

void updateDimMax(int data)
{
    if (data > scan_info.dim_max)
    {
        scan_info.dim_max = data;
    }
};
void updateDimMin(int data)
{
    if (data < scan_info.dim_min)
    {
        scan_info.dim_min = data;
    }
}
void increaseDimTotale(int data)
{
    scan_info.dim_totale = scan_info.dim_totale + data;
    updateDimMedia();
    updateDimMax(data);
    updateDimMin(data);
}

void readInputFile(FILE *input)
{
    int t = 2;
    char *line = (char *)calloc(t, sizeof(char));
    char *temp_line = (char *)calloc(t, sizeof(char));
    while (fgets(temp_line, t * sizeof(char), input))
    {
        strcat(line, temp_line);
        if (strchr(line, '\n'))
        {
            line[strlen(line) - 1] = '\0';
            analisiSingolaRiga(line);
            free(line);
            line = (char *)calloc(t, sizeof(char));
        }
        else
        {
            line = (char *)realloc(line, strlen(line) + t);
        }
    };
    analisiSingolaRiga(line);
    free(line);
    free(temp_line);
}

void analisiSingolaRiga(char *riga)
{
    char *path = (char *)calloc(strlen(riga), sizeof(char));
    int isR = 0;
    int isL = 0;
    int pathRead = 0;
    char *token;
    for (token = strtok(riga, " "); token; token = strtok(NULL, " "))
    {
        printf("Token analizzato: %s-\n", token);
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

    printf("Path: %s\n", path);
    printf("R: %d\n", isR);
    printf("L: %d\n", isL);
    scanFile(path, isR, isL);
    free(path);
}

int scanFile(char *path, int isR, int isL)
{
    printf("Inizio scansione file con path: %s\n", path);
    // printf("Il path corrente è: %s\n", path);
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    struct stat currentStat;
    if (isL == 1)
    {
        // printf("Del link analizzo il file referenziato\n");
        if (stat(path, &currentStat) < 0)
            return 0;
    }
    else if (isL == 0)
    {
        //  printf("Del link analizzo il file link\n");
        if (lstat(path, &currentStat) < 0)
            return 0;
    }
    if (isR && S_ISDIR(currentStat.st_mode))
    {
        //  printf("IL PERCORSO: %s E' UNA DIRECTORY IN CUI ENTRARE RICORSIVAMENTE\n", path);

        DIR *dir;
        struct dirent *entry;
        dir = opendir(path);
        while (entry = readdir(dir))
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            char copy[256];
            strcpy(copy, path);
            strcat(strcat(copy, "/"), entry->d_name);
            scanFile(copy, isR, isL);
        }
        closedir(dir);
    }

    char dinfo = S_ISDIR(currentStat.st_mode) ? 'd' : '-';
    char irusr = currentStat.st_mode & S_IRUSR ? 'r' : '-';
    char iwusr = currentStat.st_mode & S_IWUSR ? 'w' : '-';
    char ixusr = currentStat.st_mode & S_IXUSR ? 'x' : '-';
    char irgrp = currentStat.st_mode & S_IRGRP ? 'r' : '-';
    char iwgrp = currentStat.st_mode & S_IWGRP ? 'w' : '-';
    char ixgrp = currentStat.st_mode & S_IXGRP ? 'x' : '-';
    char iroth = currentStat.st_mode & S_IROTH ? 'r' : '-';
    char iwoth = currentStat.st_mode & S_IWOTH ? 'w' : '-';
    char ixoth = currentStat.st_mode & S_IXOTH ? 'x' : '-';
    struct passwd *pwsUID = getpwuid(currentStat.st_uid);
    struct group *grpGID = getgrgid(currentStat.st_gid);
    printf("# <Path: %s>\n\n", path);
    printf("<Data: %s> <User: %s> <Group : %s> <Diritti: %c%c%c%c%c%c%c%c%c%c> <Last access: %s> <Last change: %s> <Last mod: %s> <Links: %ld>\n\n###\n\n",
           strtok(asctime(timeinfo), "\n"),
           pwsUID->pw_name,
           grpGID->gr_name,
           dinfo,
           irusr,
           iwusr,
           ixusr,
           irgrp,
           iwgrp,
           ixgrp,
           iroth,
           iwoth,
           ixoth,
           ctime(&currentStat.st_atime), ctime(&currentStat.st_mtime), ctime(&currentStat.st_ctime), currentStat.st_nlink);

    return 1;
}