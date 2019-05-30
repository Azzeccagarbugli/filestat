
#include "../include/datastructure.h"
#include <sys/stat.h>
#ifndef _SCANH_
#define _SCANH_
typedef struct sinfo ScanInfo;
struct sinfo
{
    long int nr_monitorati;
    long int nr_link;
    long int nr_directory;
    long int dim_totale;
    long int dim_media;
    long int dim_max;
    long int dim_min;
};

extern ScanInfo stats;
int startScan(FILE *, FILE *);


#endif