#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../include/scan.h"
#include "../include/main.h"
#include <sys/stat.h>
#include <sys/types.h>

struct ScanInfo scan_info = {0, 0, 0, 0, 0, 0, 0};

void increaseMonitorati()
{
    info.nr_monitorati++;
};
void increaseLink()
{
    info.nr_link++;
}
void increaseDirectory()
{
    info.nr_directory++;
}

void updateDimMedia()
{
    info.dim_media = info.dim_totale / info.nr_monitorati;
}

void updateDimMax(int data)
{
    if (data > info.dim_max)
    {
        info.dim_max = data;
    }
};
void updateDimMin(int data)
{
    if (data < info.dim_min)
    {
        info.dim_min = data;
    }
}
void increaseDimTotale(int data)
{
    info.dim_totale = info.dim_totale + data;
    updateDimMedia();
    updateDimMax(data);
    updateDimMin(data);
}

void readInputFile(FILE *input)
{
    char line[1024];
    while (fgets(line, sizeof line, input) != NULL)
    {
        analisiSingolaRiga(strtok(line, "\n"));
    }
}

void analisiSingolaRiga(char *riga)
{
    printf("\nLine: %s \n", riga);
    char *path = (char *)calloc(strlen(riga), sizeof(char));
    int isR = 0;
    int isL = 0;
    int pathRead = 0;
    char *token;
    for (token = strtok(riga, " "); token; token = strtok(NULL, " "))
    {
        if (strcmp(token, "r") == 0)
        {
            bR = 1;
        }
        else if (strcmp(token, "l") == 0)
        {
            bL = 1;
        }
        else if (!pathRead)
        {
            strcpy(path, token);
            pathRead = 1;
        }
    }
    scanFile(path, isR, isL);
    free(path);
}

int startScan(FILE *input, FILE *output)
{
    readInputFile(input);
}

int scanFile(char *path, int isR, int isL)
{
    struct stat currentStat;
    if (isL)
    {
        if (stat(path, &currentStat) < 0)
            return 0;
    }
    else
    {
        if (lstat(path, &currentStat) < 0)
            return 0;
    }

    
}