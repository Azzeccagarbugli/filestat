#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/main.h"
#include "../include/scan.h"

static int verbose_flag;
static int stat_flag;
static int report_flag;

static int history_flag;
static char *history_path;

static int user_flag;
static int uID;

static int group_flag;
static int gID;

static int length_flag;
static int min_length;
static int max_length;

static int noscan_flag;

static FILE *file_input;
static FILE *file_output;
static char default_input_path[] = "./filestat.in";
static char default_output_path[] = "./filestat.db";

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

    readInputFile(file_input);
    fclose(file_input);
    fclose(file_output);
    return 1;
}

void parsePaths(int argc, char **argv)
{
    file_input = ((argc > 2) && (access(argv[argc - 2], F_OK) == 0)) ? fopen(argv[argc - 2], "r") : fopen(default_input_path, "r");
    file_output = ((argc > 2) && (access(argv[argc - 1], F_OK) == 0)) ? fopen(argv[argc - 1], "r+") : fopen(default_output_path, "r+");

    printf("\n");
}

void printOpt()
{
    printf("Verbose Flag: %d\n", verbose_flag);
    printf("Stat Flag: %d\n", stat_flag);
    printf("Report flag: %d\n", report_flag);
    printf("History flag: %d\n", history_flag);
    printf("History path: %s\n", history_path);
    printf("User flag: %d\n", user_flag);
    printf("User ID: %d\n", uID);
    printf("Group flag: %d\n", group_flag);
    printf("Group ID: %d\n", gID);
    printf("Length flag: %d\n", length_flag);
    printf("Min length: %d\n", min_length);
    printf("Max lenght: %d\n", max_length);
    printf("Noscan: %d\n", noscan_flag);
    printf("\n");
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
        {"noscan", no_argument, &noscan_flag, 1},
        {0, 0, 0, 0}};

    int c = 0;

    if (argc < 1)
    {
        return 0;
    }

    int option_index = 0;
    while (c != -1)
    {
        c = getopt_long(argc, argv, "vsrh:u:g:l:", long_options, &option_index);
        switch (c)
        {
        case 'v':
            verbose_flag = 1;
            break;
        case 's':
            stat_flag = 1;
            break;
        case 'r':
            report_flag = 1;
            break;
        case 'h':
            history_flag = 1;
            if (!getHistoryPath(optarg))
            {
                return 0;
            };
            break;
        case 'u':
            user_flag = 1;
            uID = atoi(optarg);
            break;
        case 'g':
            group_flag = 1;
            gID = atoi(optarg);
            break;
        case 'l':
            length_flag = 1;
            if (!getLengthArg(optarg))
            {
                return 0;
            };
            break;
        case 1:
            noscan_flag = 1;
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
        min_length = 0;
        max_length = atoi(token);
    }
    else
    {
        min_length = atoi(token);
        token = strtok(NULL, ":");
        max_length = (token != NULL) ? atoi(token) : 0;
    }
    if (max_length != 0 && min_length > max_length)
    {
        perror("I valori inseriti con -l non vanno bene\n");
        return 0;
    }
    return 1;
}

int getHistoryPath(char *arg)
{
    struct stat sb;
    history_path = (char *)calloc(strlen(arg), sizeof(char));
    strcpy(history_path, arg);
    if (access(history_path, F_OK) == -1)
    {
        perror("Il file inserito con -h non esiste in questa directory\n");
        return 0;
    }
    return 1;
}
