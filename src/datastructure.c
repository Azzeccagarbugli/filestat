#include <stdio.h>
#include <stdlib.h>
#include "../include/datastructure.h"
#include "../include/main.h"
#include <string.h>

AnalisisEntry *createNewAnalisis(char *an);
AnalisisEntry *addAnalisisByAnalisisEntry(AnalisisEntry *entry, char *newanalisis);
PathEntry *addAnalisis(PathEntry *pathentry, char *path, char *newanalisis);
PathEntry *addPath(PathEntry *, char *);

/**
 * Ottenimento di un elemento vuoto della struttura dati AnalisisEntry.
 * 
 * :return: puntatore ad un'istanza vuota di AnalisisEntry
 */
AnalisisEntry *emptyAnalisis(void)
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
 * Creazione di un nuovo elemento della struttura dati AnalisisEntry.
 * 
 * :param an: puntatore all'array di caratteri contenente la stringa contentente l'analisi da inserire all'interno dell'elemento
 * :return: puntatore ad un nuovo elemento di AnalisisEntry
 */
AnalisisEntry *createNewAnalisis(char *an)
{
    AnalisisEntry *newEntry = malloc(sizeof(AnalisisEntry));
    newEntry->analisis = strdup(an);
    newEntry->nextAnalisis = emptyAnalisis();
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
    newEntry->analisis = emptyAnalisis();
    newEntry->nextPath = emptyPath();
    return newEntry;
}

/**
 * Verifica che un puntatore ad AnalisisEntry corrisponda ad un elemento vuoto.
 * 
 * :param entry: puntatore all'elemento AnalisisENtry da analizzare
 * :return: 0 in caso di errore, non-zero in caso di successo
 */
int isAnalisisEmpty(AnalisisEntry *entry)
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
 * Metodo ausiliario ad addPathAndAnalisis(PathEntry*, char*, char*).
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
 * Aggiunta di una nuova analisi all'interno di una struttura AnalisisEntry.
 * Metodo ausiliario ad addAnalisis(PathEntry*, char*, char*).
 * 
 * :param entry: puntatore alla struttura dati AnalisisEntry da aggiornare
 * :param newanalisis: puntatore all'array di caratteri contenente l'analisi da inserire
 * :return: puntatore alla struttura AnalisisEntry aggiornata
 */
AnalisisEntry *addAnalisisByAnalisisEntry(AnalisisEntry *entry, char *newanalisis)
{
    if (isAnalisisEmpty(entry))
    {
        return createNewAnalisis(newanalisis);
    }
    if (strcmp(entry->analisis, newanalisis) == 0)
    {
        return entry;
    }
    else
    {
        entry->nextAnalisis = addAnalisisByAnalisisEntry(entry->nextAnalisis, newanalisis);
        return entry;
    }
}

/**
 * Aggiunta di una nuova analisi all'interno di una struttura dati PathEntry dato il path a cui corrisponde.
 * Metodo ausiliario ad addPathAndAnalisis(PathEntry*, char*, char*).
 * 
 * :param pathentry: puntatore alla struttura dati PathEntry da aggiornare
 * :param path: puntatore all'array di caratteri contenente il path associato all'analisi da inserire
 * :param newanalisis: ountatore alla stringa contenente l'analisi da inserire
 * :return: puntatore alla struttura PathEntry aggiornata
 */
PathEntry *addAnalisis(PathEntry *pathentry, char *path, char *newanalisis)
{
    if (strcmp(pathentry->path, path) == 0)
    {
        pathentry->analisis = addAnalisisByAnalisisEntry(pathentry->analisis, newanalisis);
        return pathentry;
    }
    else
    {
        pathentry->nextPath = addAnalisis(pathentry->nextPath, path, newanalisis);
        return pathentry;
    }
}

/**
 * Aggiunta di una nuova analisi all'interno di una struttura dati PathEntry dato il path a cui corrisponde.
 * 
 * :param pathentry: puntatore alla struttura dati PathEntry da aggiornare
 * :param path: puntatore all'array di caratteri contenente il path associato all'analisi da inserire
 * :param newanalisis: stringa contenente l'analisi da inserire
 * :return: puntatore alla struttura PathEntry aggiornata
 */
PathEntry *addPathAndAnalisis(PathEntry *entry, char *path, char *analisis)
{
    if (!containsPath(entry, path))
        entry = addPath(entry, path);
    entry = addAnalisis(entry, path, analisis);
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
 * :return: puntatore alla struttura AnalisisEntry richiesta
 */
AnalisisEntry *getFirstAnalisis(PathEntry *entry)
{
    return entry->analisis;
}

/**
 *  Metodo che restituisce l'elemento contentente l'analisi successiva a quella contenuto in un elemento di AnalisisEntry.
 * 
 * :param entry: puntatore all'elemento AnalisisEntry di cui si vuole ottenere il successivo
 * :return: puntatore all'elemento AnalisisEntry richiesta
 */
AnalisisEntry *getNextAnalisis(AnalisisEntry *entry)
{
    return entry->nextAnalisis;
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