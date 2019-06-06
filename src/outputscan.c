#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../include/outputscan.h"
#include "../include/datastructure.h"
#include "../include/main.h"

/**
 * Gestione completa delle analisi delle informazioni presenti sul file di output trattato dal programma.
 * :param output: puntatore al file di output da cui leggere le informazioni sulle analisi passate
 * :param data: puntatore alla struttura dati PathEntry in cui inserire le informazioni recuperate dal file di output
 */
PathEntry *readOutputFile(FILE *output, PathEntry *data)
{
    size_t t = 0;
    char *line = NULL;
    char *currentPath = NULL;
    for (ssize_t read = getline(&line, &t, output); read >= 0; read = getline(&line, &t, output))
    {
        if (line[0] == '#' && line[1] == ' ')
        {
            free(currentPath);
            currentPath = strdup(strtok(line + 2, "\r\n"));
        }
        else if (line[0] != '#' && line[1] != '#' && line[2] != '#')
        {
            data = addPathAndAnalisis(data, currentPath, strtok(line, "\r\n"));
        }
    }

    free(line);
    free(currentPath);
    return data;
}