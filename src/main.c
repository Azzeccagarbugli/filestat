#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/main.h"
#include "../include/scan.h"
#include <time.h>

#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define DEFAULT_INPUT_PATH "./filestat.in"
#define DEFAULT_OUTPUT_PATH "./filestat.db"

OptInfo opt_info = {0, 0, 0, 0, NULL, 0, NULL, 0, NULL, 0, 0, 0, 0};

static FILE *file_input;
static FILE *file_output;

struct stat file_stats;

int main(int argc, char **argv)
{
    clock_t timer_app;
    timer_app = clock();
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
    fflush(file_input);
    fflush(file_output);
    fclose(file_input);
    fclose(file_output);
    
    if (opt_info.report_flag)
    {
        printf("\nReport finale: \n");
        printf("Numero file elaborati: %ld\n", stats.nr_monitorati);
        timer_app = clock() - timer_app;
        double time_taken = ((double)timer_app) / CLOCKS_PER_SEC;
        printf("Tempo di elaborazione: %f secondi\n", time_taken);
        printf("Dimensione massima: %ld bytes\n", stats.dim_max);
    }
    
    return 1;
}

void parsePaths(int argc, char **argv)
{
    if ((!((argc > 2) && (access(argv[argc - 2], F_OK) == 0)) || (strcmp(argv[argc - 2], argv[argc - 1]) == 0)))
    {
        file_input = fopen(DEFAULT_INPUT_PATH, "a+");
        printf("Come file di input è stato aperto il predefinito\n");
    }
    else
    {
        file_input = fopen(argv[argc - 2], "a+");
        printf("Come file di input è stato aperto quello specificato come argomento\n");
    }
    if ((!((argc > 2) && (access(argv[argc - 1], F_OK) == 0)) || (strcmp(argv[argc - 2], argv[argc - 1]) == 0) || (strcmp("-h", argv[argc - 2]) == 0) || (strcmp("--history", argv[argc - 2]) == 0)))
    {
        file_output = fopen(DEFAULT_OUTPUT_PATH, "a+");
        printf("Come file di output è stato aperto il predefinito\n");
    }
    else
    {
        file_output = fopen(argv[argc - 1], "a+");
        printf("Come file di output è stato aperto quello specificato come argomento\n");
    }
    
    fseek(file_input, 0, SEEK_SET);
    fseek(file_output, 0, SEEK_SET);
}

void printOpt()
{
    printf("Verbose Flag: %d\n", opt_info.verbose_flag);
    printf("Stat Flag: %d\n", opt_info.stat_flag);
    printf("Report flag: %d\n", opt_info.report_flag);
    printf("History flag: %d\n", opt_info.history_flag);
    printf("History path: %s\n", opt_info.history_path);
    printf("User flag: %d\n", opt_info.user_flag);
    printf("User ID: %s\n", opt_info.uID);
    printf("Group flag: %d\n", opt_info.group_flag);
    printf("Group ID: %s\n", opt_info.gID);
    printf("Length flag: %d\n", opt_info.length_flag);
    printf("Min length: %ld\n", opt_info.min_length);
    printf("Max lenght: %ld\n", opt_info.max_length);
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
            opt_info.uID = (char *)calloc(strlen(optarg), sizeof(char));
            strcpy(opt_info.uID, optarg);
            break;
        case 'g':
            opt_info.group_flag = 1;
            opt_info.gID = (char *)calloc(strlen(optarg), sizeof(char));
            strcpy(opt_info.gID, optarg);
            break;
        case 'l':
            opt_info.length_flag = 1;
            if (!getLengthArg(optarg))
            {
                return 0;
            };
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
        exit(EXIT_FAILURE);
    }
    return 1;
}

int getHistoryPath(char *arg)
{
    opt_info.history_path = (char *)calloc(strlen(arg), sizeof(char));
    strcpy(opt_info.history_path, arg);
    if (access(opt_info.history_path, F_OK) == -1)
    {
        perror("Il file inserito con -h non esiste in questa directory\n");
        exit(EXIT_FAILURE);
    }
    return 1;
}
