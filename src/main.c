#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "../include/main.h"
#include "../include/scan.h"
#include <time.h>

struct OptInfo opt_info = {0,0,0,0, NULL, 0,0,0,0,0,0,0,0};

static FILE *file_input;
static FILE *file_output;
static char default_input_path[] = "./filestat.in";
static char default_output_path[] = "./filestat.db";

struct stat file_stats;

DIR *dirp;
struct dirent *dent;

int main(int argc, char **argv)
{
    parsePaths(argc, argv);
    if (!parseOpt(argc, argv))
    {
        return -1;
    }
    else
    {
        printOpt();
    }
    startScan(file_input, file_output);
    fclose(file_input);
    fclose(file_output);
    return 1;
}

void parsePaths(int argc, char **argv)
{
    file_input = ((argc > 2) && (access(argv[argc - 2], F_OK) == 0)) ? fopen(argv[argc - 2], "r") : fopen(default_input_path, "r");
    file_output = ((argc > 2) && (access(argv[argc - 1], F_OK) == 0)) ? fopen(argv[argc - 1], "r+") : fopen(default_output_path, "r+");
}

void printOpt()
{
    printf("Verbose Flag: %d\n", opt_info.verbose_flag);
    printf("Stat Flag: %d\n", opt_info.stat_flag);
    printf("Report flag: %d\n", opt_info.report_flag);
    printf("History flag: %d\n", opt_info.history_flag);
    printf("History path: %s\n", opt_info.history_path);
    printf("User flag: %d\n", opt_info.user_flag);
    printf("User ID: %d\n", opt_info.uID);
    printf("Group flag: %d\n", opt_info.group_flag);
    printf("Group ID: %d\n", opt_info.gID);
    printf("Length flag: %d\n", opt_info.length_flag);
    printf("Min length: %d\n", opt_info.min_length);
    printf("Max lenght: %d\n", opt_info.max_length);
    printf("Noscan: %d\n", opt_info.noscan_flag);
}

int parseOpt(int argc, char **argv)
{
    static struct option long_options[] = {
        {"verbose", no_argument, NULL, 'v'},
        {"stat", no_argument, NULL, 's'},
        {"report", no_argument, NULL, 'r'},
        {"history", required_argument, 0, 'h'},
        {"user", required_argument, 0, 'u'},
        {"group", required_argument, 0, 'g'},
        {"length", required_argument, 0, 'l'},
        {"noscan", no_argument, 0, 'n'},
        {0, 0, 0, 0}};

    int c = 0;

    int option_index = 0;

    if (argc < 1)
    {
        return 0;
    }

    while (c != -1)
    {
        c = getopt_long(argc, argv, "vsrh:u:g:l:", long_options, &option_index);
        switch (c)
        {
        case 'v':
            opt_info.verbose_flag = 1;
            break;
        case 's':
            opt_info.stat_flag = 1;
            break;
        case 'r':
            opt_info.report_flag = 1;
            break;
        case 'h':
            opt_info.history_flag = 1;
            if (!getHistoryPath(optarg))
            {
                return 0;
            };
            break;
        case 'u':
            opt_info.user_flag = 1;
            opt_info.uID = atoi(optarg);
            break;
        case 'g':
            opt_info.group_flag = 1;
            opt_info.gID = atoi(optarg);
            break;
        case 'l':
            opt_info.length_flag = 1;
            if (!getLengthArg(optarg))
            {
                return 0;
            };
            filesBetween("."); // Directory attuale
            break;
        case 'n':
            opt_info.noscan_flag = 1;
            break;
        }
    }
    return 1;
}

int getLengthArg(char *arg)
{
    char *token = strtok(arg, ":");
    if (arg[0] == ':') 
    {
        opt_info.min_length = 0;
        opt_info.max_length = atoi(token);
    }
    else
    {
        opt_info.min_length = atoi(token);
        token = strtok(NULL, ":");
        opt_info.max_length = (token != NULL) ? atoi(token) : 0;
    }
    if (opt_info.max_length != 0 && opt_info.min_length > opt_info.max_length)
    {
        perror("I valori inseriti con -l non vanno bene\n");
        return 0;
    }
    return 1;
}

void filesBetween(char *dir)
{
    dirp = opendir(dir);
    do
    {
        dent = readdir(dirp);
        if (dent)
        {
            if (!stat(dent->d_name, &file_stats))
            {
                // ./filestat -l 32:500
                // Dimensione compresa
                if (opt_info.max_length >= file_stats.st_size && opt_info.min_length <= file_stats.st_size)
                {
                    printf("File name: %-12s \t%-1d bytes\n", dent->d_name, (int) file_stats.st_size);
                }

                // ./filestat -l 32:
                // Dimensione minima
                if (opt_info.max_length == 0 && opt_info.min_length <= file_stats.st_size)
                {
                    printf("File name: %-12s \t%-1d bytes\n", dent->d_name, (int) file_stats.st_size);
                }

                // ./filestat -l :500
                // Dimensione massima
                if (opt_info.min_length == 0 && opt_info.max_length >= file_stats.st_size)
                {
                    printf("File name: %-12s \t%-1d bytes\n", dent->d_name, (int) file_stats.st_size);
                }
            }
            else
            {
                printf("The stat API didn't work for this file\n");
            }
        }
    } while (dent);
    closedir(dirp);
}

int getHistoryPath(char *arg)
{
    opt_info.history_path = (char *)calloc(strlen(arg), sizeof(char));
    strcpy(opt_info.history_path, arg);
    if (access(opt_info.history_path, F_OK) == -1)
    {
        perror("Il file inserito con -h non esiste in questa directory\n");
        return 0;
    }
    return 1;
}

