#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include "../include/outputscan.h"
#include "../include/datastructure.h"
#include "../include/main.h"

/**
 * Gestione completa delle analisi delle informazioni presenti sul file di output trattato dal programma.
 * 
 * 
 * :param output: puntatore al file di output da cui leggere le informazioni sulle analisi passate
 * :param data: puntatore alla struttura dati PathEntry in cui inserire le informazioni recuperate dal file di output
 * :return: puntatore alla struttura dati PathEntry aggiornata
 */
PathEntry *readOutputFile(FILE *output, PathEntry *data)
{
    if (options.noscan_flag)
        printf("# Verranno stampate di seguito le informazioni presenti sul file di output: \n");
    size_t t = 0;
    char *line = NULL;
    char *currentPath = NULL;
    for (ssize_t read = getline(&line, &t, output); read >= 0; read = getline(&line, &t, output))
    {
        if (line[0] == '#' && line[1] == ' ')
        {
            currentPath = strdup(strtok(line + 2, "\r\n"));
            if (options.noscan_flag)
                printf("# %s\n", currentPath);
        }
        else if (line[0] != '#' && line[1] != '#' && line[2] != '#' && currentPath != NULL)
        {
            if (options.noscan_flag)
                printf("%s", line);
            data = addPathAndAnalisis(data, currentPath, strtok(line, "\r\n"));
        }
        else
        {
            if (options.noscan_flag)
                printf("###\n");
            free(currentPath);
            currentPath = NULL;
        }
    }

    if (options.noscan_flag)
        printf("# Fine della stampa delle informazioni presenti sul file di output\n\n");
    free(line);
    free(currentPath);
    return data;
}