#include <stdio.h>
#include <stdlib.h>
#include "../include/datastructure.h"
#include "../include/main.h"
#include <string.h>

AnalisisEntry *createNewAnalisis(char *an);
PathEntry *addPath(PathEntry *entry, char *newpath);
AnalisisEntry *addAnalisisByAnalisisEntry(AnalisisEntry *entry, char *newanalisis);
PathEntry *addAnalisis(PathEntry *pathentry, char *path, char *newanalisis);

AnalisisEntry *emptyAnalisis()
{
    return NULL;
}

PathEntry *emptyPath()
{
    return NULL;
}

AnalisisEntry *createNewAnalisis(char *an)
{
    AnalisisEntry *newEntry = malloc(sizeof(AnalisisEntry));
    newEntry->analisis = strdup(an);
    newEntry->nextAnalisis = emptyAnalisis();
    return newEntry;
}

PathEntry *createNewPath(char *pt)
{
    PathEntry *newEntry = malloc(sizeof(PathEntry));
    newEntry->path = strdup(pt);
    newEntry->analisis = emptyAnalisis();
    newEntry->nextPath = emptyPath();
    return newEntry;
}

int isAnalisisEmpty(AnalisisEntry *entry)
{
    return (entry == NULL);
}

int isPathEmpty(PathEntry *entry)
{
    return (entry == NULL);
}

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

PathEntry *addPathAndAnalisis(PathEntry *entry, char *path, char *analisis)
{
    entry = addPath(entry, path);
    entry = addAnalisis(entry, path, analisis);
    return entry;
}

PathEntry *getNextPath(PathEntry *entry)
{
    return entry->nextPath;
}

AnalisisEntry *getFirstAnalisis(PathEntry *entry)
{
    return entry->analisis;
}

AnalisisEntry *getNextAnalisis(AnalisisEntry *entry)
{
    return entry->nextAnalisis;
}

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