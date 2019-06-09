#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include "../include/main.h"
#include "../include/scan.h"

#define DEFAULT_INPUT_PATH "./filestat.in"
#define DEFAULT_OUTPUT_PATH "./filestat.db"

/**
 * Struct in cui viene eseguito lo storage delle possibili opzioni. 
 */
OptInfo options = {0, 0, 0, 0, NULL, 0, NULL, 0, NULL, 0, 0, 0, 0};

/**
 * Puntatori di natura FILE in cui vengono settati i file di I/O che verranno utilizzati. 
 * Quest'ultimi potranno essere scelti dall'utente o impostati di default.
 */
static FILE *file_input;
static FILE *file_output;


/**
 * Main necessario all'avvio del programma.
 */
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

    if (options.report_flag)
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

/**
 * Apertura dei file di input e di output basandosi sulle scelte dell'utente.
 * :param argc: parametro passato come argomento a main
 * :param argv: parametro passato come argomento a main
 */
void parsePaths(int argc, char **argv)
{
    if (!((argc > 2) && (access(argv[argc - 2], F_OK) == 0)) 
    || (strcmp(argv[argc - 2], argv[argc - 1]) == 0) 
    || (strcmp("-h", argv[argc - 3]) == 0) 
    || (strcmp("--history", argv[argc - 3]) == 0))
    {
        file_input = fopen(DEFAULT_INPUT_PATH, "a+");
        printf("Come file di input è stato aperto il predefinito\n");
    }
    else
    {
        file_input = fopen(argv[argc - 2], "r");
        printf("Come file di input è stato aperto quello specificato come argomento\n");
    }
    if (!((argc > 2) && (access(argv[argc - 1], F_OK) == 0)) 
    || (strcmp(argv[argc - 2], argv[argc - 1]) == 0) 
    || (strcmp("-h", argv[argc - 2]) == 0) 
    || (strcmp("--history", argv[argc - 2]) == 0))
    {
        file_output = fopen(DEFAULT_OUTPUT_PATH, "a+");
        printf("Come file di output è stato aperto il predefinito\n");
    }
    else
    {
        file_output = fopen(argv[argc - 1], "r+");
        printf("Come file di output è stato aperto quello specificato come argomento\n");
    }

    fseek(file_input, 0, SEEK_SET);
    fseek(file_output, 0, SEEK_SET);
}

/**
 * Metodo necessario al debug per la visualizzazione dello struct delle opzioni.
 */
void printOpt()
{
    printf("Verbose Flag: %d\n", options.verbose_flag);
    printf("Stat Flag: %d\n", options.stat_flag);
    printf("Report flag: %d\n", options.report_flag);
    printf("History flag: %d\n", options.history_flag);
    printf("History path: %s\n", options.history_path);
    printf("User flag: %d\n", options.user_flag);
    printf("User ID: %s\n", options.uID);
    printf("Group flag: %d\n", options.group_flag);
    printf("Group ID: %s\n", options.gID);
    printf("Length flag: %d\n", options.length_flag);
    printf("Min length: %ld\n", options.min_length);
    printf("Max lenght: %ld\n", options.max_length);
    printf("Noscan: %d\n", options.noscan_flag);
}

/**
 * Viene effettuata la lettura delle opzioni inserite da linea di comando.
 * 
 * :param argc: parametro passato come argomento a main
 * :param argv: parametro passato come argomento a main
 * :return: 0 in caso di errore, non-zero in caso di successo
 */
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
            options.verbose_flag = 1;
            break;
        case 's':
            options.stat_flag = 1;
            break;
        case 'r':
            options.report_flag = 1;
            break;
        case 'h':
            options.history_flag = 1;
            if (!getHistoryPath(optarg))
            {
                return 0;
            };
            break;
        case 'u':
            options.user_flag = 1;
            options.uID = (char *)calloc(strlen(optarg), sizeof(char));
            strcpy(options.uID,
                   optarg);
            break;
        case 'g':
            options.group_flag = 1;
            options.gID = (char *)calloc(strlen(optarg), sizeof(char));
            strcpy(options.gID,
                   optarg);
            break;
        case 'l':
            options.length_flag = 1;
            if (!getLengthArg(optarg))
            {
                return 0;
            };
            break;
        case 'n':
            options.noscan_flag = 1;
            break;
        }
    }
    return 1;
}

/**
 * Metodo ausiliario all'impostazione del flag -l/--length.
 * :param arg: puntatore ad un array di caratteri contenente i valori delle lunghezze
 * :return: 1 in caso di successo
 */
int getLengthArg(char *arg)
{
    char *token = strtok(arg, ":");

    if (arg[0] == ':')
    {
        options.min_length = 0;
        options.max_length = atoi(token);
    }
    else
    {
        options.min_length = atoi(token);
        token = strtok(NULL, ":");
        options.max_length = (token != NULL) ? atoi(token) : 0;
    }
    if (options.max_length != 0 &&
        options.min_length > options.max_length)
    {
        perror("I valori inseriti con -l non vanno bene\n");
        exit(EXIT_FAILURE);
    }

    return 1;
}

/**
 * Metodo che restituisce la cronologia dei path analizzati grazie all'opzione -h/--history.
 * 
 * :param arg: puntatore ad un array di caratteri contenente il path che si desidera analizzare
 * :return: 1 in caso di successo
 */
int getHistoryPath(char *arg)
{
    printf("LOL\n");
    options.history_path = (char *)malloc(strlen(arg));
    strcpy(options.history_path,arg);

    return 1;
}
