
#include "../include/tree.h"
#include <sys/stat.h>
#ifndef _SCANH_
#define _SCANH_
typedef struct sinfo ScanInfo;
struct sinfo
{
    int nr_monitorati;
    int nr_link;
    int nr_directory;
    int dim_totale;
    int dim_media;
    int dim_max;
    int dim_min;
};

extern ScanInfo scan_info;
RecordNode *readOutputFile(FILE *, RecordNode *);
RecordNode *readInputFile(FILE *, RecordNode *);
RecordNode *analisiSingolaRiga(char *, RecordNode *);
int startScan(FILE *, FILE *);
RecordNode *scanFilePath(char *, int, int, RecordNode *);
RecordNode *getStringInfo(struct stat *, char *, RecordNode *);

#endif