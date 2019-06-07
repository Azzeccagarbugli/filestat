#ifdef __linux__
#include <linux/limits.h>
#else
#include <limits.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "../include/scan.h"
#include "../include/main.h"
#include "../include/datastructure.h"
#include "../include/outputscan.h"
#include "../include/inputscan.h"

void printOnOutput(FILE *, PathEntry *);
void cleanFile(FILE *);
void printStats();
void printHistory(PathEntry *, char *);
void printOnFile(PathEntry *, FILE *);
void freeAnalisis(AnalisisEntry *);
void freePath(PathEntry *);

/**
 * Struct di riferimento per le informazioni complessive raccolte sui file, quali:
 * - numero di file monitorati;
 * - numero di link incontrati;
 * - numero di directory incontrate;
 * - dimensione totale dei file;
 * - dimensione media dei file;
 * - dimensione massima dei file;
 * - dimensione minima (in byte) dei file.
 */
ScanInfo stats = {0, 0, 0, 0, 0, 0, 0};

/**
 * Avvio delle operazioni di scan e di recupero delle informazioni richieste dal programma.
 * :param input: puntatore al file di input trattato nel programma
 * :param output: puntatore al file di output trattato nel programma
 */
int startScan(FILE *input, FILE *output)
{
    PathEntry *data = malloc(sizeof(PathEntry));

    data = emptyPath();
    data = readOutputFile(output, data);

    if (options.history_flag)
    {
        printHistory(data, options.history_path);
    }

    data = readInputFile(input, data);
    printOnFile(data, output);
    freePath(data);
    if (options.stat_flag)
    {
        printStats();
    }
}

/**
 * Incremento numero di file monitorati e di cui è stata aggiunta la relativa analisi.
 */
void increaseMonitorati()
{
    stats.nr_monitorati++;
}

/**
 * Incremento numero di link incontrati.
 */
void increaseLink()
{
    stats.nr_link++;
}

/**
 * Incremento numero di directory.
 */
void increaseDirectory()
{
    stats.nr_directory++;
}

/**
 * Aggiornamento valore dimensione media dei file monitorati.
 */
void updateDimMedia()
{
    stats.dim_media = stats.dim_totale / stats.nr_monitorati;
}

/**
 * Aggiornamento valore dimensione massima dei file monitorati.
 * 
 * :param data: valore della dimensione che si desidera aggiungere
 */
void updateDimMax(int data)
{
    if (data > stats.dim_max)
    {
        stats.dim_max = data;
    }
}

/**
 * Aggiornamento valore dimensione minima dei file monitorati.
 * 
 * :param data: valore della dimensione che si desidera aggiungere
 */
void updateDimMin(int data)
{
    if (data < stats.dim_min)
    {
        stats.dim_min = data;
    }
}

/**
 * Aggiornamento valore dimensione totale dei file monitorati.
 * 
 * :param data: valore della dimensione che si desidera aggiungere
 */
void increaseDimTotale(int data)
{
    stats.dim_totale = stats.dim_totale + data;
    updateDimMedia();
    updateDimMax(data);
    updateDimMin(data);
}

/**
 * Reset completo di un file.
 * 
 * :param target: puntatore al file si cui si desidera effettuare i reset
 */
void cleanFile(FILE *target)
{
    fflush(target);
    ftruncate(fileno(target), 0);
    fseek(target, 0, SEEK_SET);
    fflush(target);
}

/**
 * Gestione completa della stampa delle informazioni contenute in una struttura PathEntry su un file di output.
 * 
 * :param target: puntatore al file di output su cui si vogliono inserire le informazioni
 * :param entry: puntatore alla struttura dati da cui si vogliono leggere le informazioni da stampare
 */
void printOnOutput(FILE *target, PathEntry *entry)
{
    cleanFile(target);
    printOnFile(entry, target);
    fflush(target);
}

/**
 * Aggiornamento completo di tutte le informazioni riguardo i file che sono stati monitorati.
 * 
 * :param size: valore della dimensione che si desidera aggiungere
 */
void updateStats(long int size)
{
    increaseMonitorati();
    increaseDimTotale(size);
}

/**
 * Metodo che stampa a video il report completo delle informazioni acquisite.
 */
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

/**
 * Stampa sullo standard output delle informazioni relative alle analisi passate effettuate su un file.
 * 
 * :param entry: puntatore alla struttura dati da cui leggere le informazioni
 * :param path: puntatore all'array di caratteri contenente il associato al file di cui si vuole conoscere la cronologia
 */
void printHistory(PathEntry *entry, char *path)
{
    PathEntry *pEntry = getPathEntry(entry, path);
    if (isPathEmpty(pEntry))
    {
        printf("Non esiste cronologia di tale file nel file di output specificato\n");
    }
    else
    {
        printf("### Cronologia del file al path: %s\n", path);
        for (AnalisisEntry *curanalisis = getFirstAnalisis(pEntry); !isAnalisisEmpty(curanalisis); curanalisis = getNextAnalisis(curanalisis))
        {
            printf("%s\n", curanalisis->analisis);
        }
        printf("### Fine cronologia\n");
    }
}

/**
 * Stampa completa delle informazioni presenti in una struttura dati PathEntry su di un file.
 *
 * :param pathentry: puntatore all'entry associato al file di cui si vogliono stampare le informazioni sullo standard output
 * :param file: puntatore al il file su cui si vogliono inserire le informazioni
 */
void printOnFile(PathEntry *pathentry, FILE *file)
{
    if (options.noscan_flag)
        printf("# Verranno stampate di seguito le informazioni presenti sul file di output: \n");

    for (PathEntry *curpath = pathentry; !isPathEmpty(curpath); curpath = getNextPath(curpath))
    {
        fprintf(file, "# %s\n", curpath->path);
        if (options.noscan_flag)
        {
            printf("# %s\n", curpath->path);
        }
        for (AnalisisEntry *curanalisis = getFirstAnalisis(curpath); !isAnalisisEmpty(curanalisis); curanalisis = getNextAnalisis(curanalisis))
        {
            fprintf(file, "%s\n", curanalisis->analisis);
            if (options.noscan_flag)
            {
                printf("%s\n", curanalisis->analisis);
            }
        }
        fprintf(file, "###\n", NULL);
        if (options.noscan_flag)
        {
            printf("###\n");
        }
    }
    fprintf(file, "###\n", NULL);
    if (options.noscan_flag)
    {
        printf("###\n");
    }

    if (options.noscan_flag)
        printf("# Fine della stampa delle informazioni presenti sul file di output\n\n");
}

/**
 * Gestione completa delle operazioni di rilascio delle risorse da operare su una struttura dati PathEntry.
 * :param entry: puntatore struttura dati su cui effetturare l'operazione completa di rilascio delle risorse
 */
void freePath(PathEntry *entry)
{
    if (!isPathEmpty(entry))
    {
        free(entry->path);
        freeAnalisis(getFirstAnalisis(entry));
        free(getNextPath(entry));
        free(entry);
    }
}

/**
 * Gestione completa delle operazioni di rilascio delle risorse da operare su una struttura dati AnalisisEntry.
 * :param entry: puntatore alla struttura dati su cui effetturare l'operazione completa di rilascio delle risorse
 */
void freeAnalisis(AnalisisEntry *entry)
{
    if (!isAnalisisEmpty(entry))
    {
        free(entry->analisis);
        free(getNextAnalisis(entry));
        free(entry);
    }
}