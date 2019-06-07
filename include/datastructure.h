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
PathEntry *createNewPath(char *);
int isAnalisisEmpty(AnalisisEntry *);
int isPathEmpty(PathEntry *);
PathEntry *addPathAndAnalisis(PathEntry *, char *, char *);
PathEntry *getNextPath(PathEntry *);
AnalisisEntry *getFirstAnalisis(PathEntry *);
AnalisisEntry *getNextAnalisis(AnalisisEntry *);
PathEntry *getPathEntry(PathEntry *, char *);

#endif