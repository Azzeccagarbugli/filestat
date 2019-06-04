#ifndef _TREEH_
#define _TREEH_

typedef struct analisisentry AnalisisEntry;

struct analisisentry
{
    char *analisis;
    AnalisisEntry *nextAnalisis;
};

typedef struct pathentry PathEntry;

struct pathentry
{
    char *path;
    AnalisisEntry *analisis;
    PathEntry *nextPath;
};

AnalisisEntry *emptyAnalisis();
PathEntry *emptyPath();
PathEntry *createNewPath(char *pt);
int isAnalisisEmpty(AnalisisEntry *entry);
int isPathEmpty(PathEntry *node);
PathEntry *addPathAndAnalisis(PathEntry *entry, char *path, char *analisis);
PathEntry *getNextPath(PathEntry *entry);
AnalisisEntry *getFirstAnalisis(PathEntry *entry);
AnalisisEntry *getNextAnalisis(AnalisisEntry *entry);
PathEntry *getPathEntry(PathEntry *entry, char *path);

#endif