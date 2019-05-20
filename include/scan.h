struct ScanInfo
{
    int nr_monitorati;
    int nr_link;
    int nr_directory;
    int dim_totale;
    int dim_media;
    int dim_max;
    int dim_min;
};

extern struct ScanInfo scan_info;
void readInputFile(FILE *);
void analisiSingolaRiga(char *);
int startScan(FILE *, FILE *);