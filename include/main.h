#ifndef _MAINH_
#define _MAINH_

typedef struct oinfo OptInfo;

struct oinfo
{
    int verbose_flag;
    int stat_flag;
    int report_flag;

    int history_flag;
    char *history_path;

    int user_flag;
    char *uID;

    int group_flag;
    char *gID;

    int length_flag;
    long int min_length;
    long int max_length;

    int noscan_flag;
};

extern OptInfo options;

int getLengthArg(char *);
int getHistoryPath(char *);
int parseOpt(int, char **);
void parsePaths(int, char **);
void filesBetween(char *);

#endif