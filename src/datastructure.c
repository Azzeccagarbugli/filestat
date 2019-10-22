#include <stdio.h>
#include <stdlib.h>
#include "../include/datastructure.h"
#include "../include/main.h"
#include <string.h>

AnalysisEntry *createNewAnalysis(char *an);
AnalysisEntry *addAnalysisByAnalysisEntry(AnalysisEntry *entry, char *newanalysis);
PathEntry *addAnalysis(PathEntry *pathentry, char *path, char *newanalysis);
PathEntry *addPath(PathEntry *, char *);

/**
 * Ottenimento di un elemento vuoto della struttura dati AnalysisEntry.
 * 
 * :return: puntatore ad un'istanza vuota di AnalysisEntry
 */
AnalysisEntry *emptyAnalysis(void)
{
    return NULL;
}

/**
 * Ottenimento di un elemento vuoto della struttura dati PathEntry.
 * 
 * :return: puntatore ad un'istanza vuota di PathEntry
 */
PathEntry *emptyPath(void)
{
    return NULL;
}

/**
 * Creazione di un nuovo elemento della struttura dati AnalysisEntry.
 * 
 * :param an: puntatore all'array di caratteri contenente la stringa contentente l'analisi da inserire all'interno dell'elemento
 * :return: puntatore ad un nuovo elemento di AnalysisEntry
 */
AnalysisEntry *createNewAnalysis(char *an)
{
    AnalysisEntry *newEntry = malloc(sizeof(AnalysisEntry));
    newEntry->analysis = strdup(an);
    newEntry->nextAnalysis = emptyAnalysis();
    return newEntry;
}

/**
 * Creazione di un nuovo elemento della struttura dati PathEntry.
 * 
 * :param pt: puntatore all'array di caratteri contenente il path da inserire all'interno dell'elemento
 * :return: puntatore ad un nuovo elemento di PathEntry
 */
PathEntry *createNewPath(char *pt)
{
    PathEntry *newEntry = malloc(sizeof(PathEntry));
    newEntry->path = strdup(pt);
    newEntry->analysis = emptyAnalysis();
    newEntry->nextPath = emptyPath();
    return newEntry;
}

/**
 * Verifica che un puntatore ad AnalysisEntry corrisponda ad un elemento vuoto.
 * 
 * :param entry: puntatore all'elemento AnalysisENtry da analizzare
 * :return: 0 in caso di errore, non-zero in caso di successo
 */
int isAnalysisEmpty(AnalysisEntry *entry)
{
    return (entry == NULL);
}

/**
 * Verifica che un puntatore ad PathEntry corrisponda ad un elemento vuoto.
 * 
 * :param entry: puntatore all'elemento PathEntry da analizzare
 * :return: 0 in caso di errore, non-zero in caso di successo
 */
int isPathEmpty(PathEntry *entry)
{
    return (entry == NULL);
}

/**
 * Aggiunta di un nuovo path all'interno di una struttura dati PathEntry.
 * Metodo ausiliario ad addPathAndAnalysis(PathEntry*, char*, char*).
 * 
 * :param entry: puntatore alla struttura dati PathEntry da aggiornare
 * :param newpath: puntatore all'array di caratteri contenente il path da inserire
 * :return: puntatore alla struttura PathEntry aggiornata
 */
PathEntry *addPath(PathEntry *entry, char *newpath)
{
    if (isPathEmpty(entry))
    {
        return createNewPath(newpath);
    }
    else if (strcmp(entry->path, newpath) == 0)
    {
        return entry;
    }
    else
    {
        entry->nextPath = addPath(entry->nextPath, newpath);
        return entry;
    }
}

/**
 * Aggiunta di una nuova analisi all'interno di una struttura AnalysisEntry.
 * Metodo ausiliario ad addAnalysis(PathEntry*, char*, char*).
 * 
 * :param entry: puntatore alla struttura dati AnalysisEntry da aggiornare
 * :param newanalysis: puntatore all'array di caratteri contenente l'analisi da inserire
 * :return: puntatore alla struttura AnalysisEntry aggiornata
 */
AnalysisEntry *addAnalysisByAnalysisEntry(AnalysisEntry *entry, char *newanalysis)
{
    if (isAnalysisEmpty(entry))
    {
        return createNewAnalysis(newanalysis);
    }
    if (strcmp(entry->analysis, newanalysis) == 0)
    {
        return entry;
    }
    else
    {
        entry->nextAnalysis = addAnalysisByAnalysisEntry(entry->nextAnalysis, newanalysis);
        return entry;
    }
}

/**
 * Aggiunta di una nuova analisi all'interno di una struttura dati PathEntry dato il path a cui corrisponde.
 * Metodo ausiliario ad addPathAndAnalysis(PathEntry*, char*, char*).
 * 
 * :param pathentry: puntatore alla struttura dati PathEntry da aggiornare
 * :param path: puntatore all'array di caratteri contenente il path associato all'analisi da inserire
 * :param newanalysis: ountatore alla stringa contenente l'analisi da inserire
 * :return: puntatore alla struttura PathEntry aggiornata
 */
PathEntry *addAnalysis(PathEntry *pathentry, char *path, char *newanalysis)
{
    if (strcmp(pathentry->path, path) == 0)
    {
        pathentry->analysis = addAnalysisByAnalysisEntry(pathentry->analysis, newanalysis);
        return pathentry;
    }
    else
    {
        pathentry->nextPath = addAnalysis(pathentry->nextPath, path, newanalysis);
        return pathentry;
    }
}

/**
 * Aggiunta di una nuova analisi all'interno di una struttura dati PathEntry dato il path a cui corrisponde.
 * 
 * :param pathentry: puntatore alla struttura dati PathEntry da aggiornare
 * :param path: puntatore all'array di caratteri contenente il path associato all'analisi da inserire
 * :param newanalysis: stringa contenente l'analisi da inserire
 * :return: puntatore alla struttura PathEntry aggiornata
 */
PathEntry *addPathAndAnalysis(PathEntry *entry, char *path, char *analysis)
{
    if (!containsPath(entry, path))
        entry = addPath(entry, path);
    entry = addAnalysis(entry, path, analysis);
    return entry;
}

/**
 *  Metodo che restituisce l'elemento contentente il path successivo a quello contenuto in un elemento di PathEntry.
 * 
 * :param entry: puntatore al PathEntry di cui si vuole ottenere il successivo
 * :return: puntatore alla struttura PathEntry richiesta
 */
PathEntry *getNextPath(PathEntry *entry)
{
    return entry->nextPath;
}

/**
 * Metodo che restituisce la prima analisi all'interno della struttura associata al path passato come argomento.
 * 
 * :param entry: puntatore ll PathEntry di cui si vuole analizzare la prima analisi associata
 * :return: puntatore alla struttura AnalysisEntry richiesta
 */
AnalysisEntry *getFirstAnalysis(PathEntry *entry)
{
    return entry->analysis;
}

/**
 *  Metodo che restituisce l'elemento contentente l'analisi successiva a quella contenuto in un elemento di AnalysisEntry.
 * 
 * :param entry: puntatore all'elemento AnalysisEntry di cui si vuole ottenere il successivo
 * :return: puntatore all'elemento AnalysisEntry richiesta
 */
AnalysisEntry *getNextAnalysis(AnalysisEntry *entry)
{
    return entry->nextAnalysis;
}

/**
 * Metodo per ottenere il puntatore all'elemento della struttura dati PathEntry contenente un determinato path.
 * 
 * :param entry: puntatore alla struttura dati in cui effettuare la ricerca
 * :param char: stringa contenente il pathname di cui si vuole ottenere il relativo elemento di PathEntry
 * :return: puntatore alla struttura PathEntry richiesta
 */
PathEntry *getPathEntry(PathEntry *entry, char *path)
{
    if (isPathEmpty(entry))
    {
        return NULL;
    }
    else if (strcmp(entry->path, path) == 0)
    {
        return entry;
    }
    else
    {
        return getPathEntry(entry->nextPath, path);
    }
}

/**
 * Verifica che una struttura dati PathEntry contenga al suo interno un elemento con un path specificato.
 * 
 * :param entry: puntatore alla struttura dati in cui effettuare la ricerca
 * :param char: stringa contenente il pathname di cui si vuole verificare la presenza in un elemento PathEntry
 * :return: 1 se il path è presente in un elemento PathEntry, 0 altrimenti
 */
int containsPath(PathEntry *entry, char *path)
{
    return !(isPathEmpty(getPathEntry(entry, path)));
}