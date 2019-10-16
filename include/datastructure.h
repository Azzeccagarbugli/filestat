#ifndef _TREEH_
#define _TREEH_

typedef struct analysisentry AnalysisEntry;

struct analysisentry
{
    char *analysis;
    AnalysisEntry *nextAnalysis;
};

typedef struct pathentry PathEntry;

struct pathentry
{
    char *path;
    AnalysisEntry *analysis;
    PathEntry *nextPath;
};

AnalysisEntry *emptyAnalysis();
PathEntry *emptyPath();
PathEntry *createNewPath(char *);
int isAnalysisEmpty(AnalysisEntry *);
int isPathEmpty(PathEntry *);
PathEntry *addPathAndAnalysis(PathEntry *, char *, char *);
PathEntry *getNextPath(PathEntry *);
AnalysisEntry *getFirstAnalysis(PathEntry *);
AnalysisEntry *getNextAnalysis(AnalysisEntry *);
PathEntry *getPathEntry(PathEntry *, char *);
int containsPath(PathEntry *, char *);

#endif