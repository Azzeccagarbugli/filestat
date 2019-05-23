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
    int uID;

    int group_flag;
    int gID;

    int length_flag;
    int min_length;
    int max_length;

    int noscan_flag;
};

extern OptInfo opt_info;

int getLengthArg(char *);
int getHistoryPath(char *);
void printOpt();
int parseOpt(int, char **);
void parsePaths(int, char **);
void filesBetween(char *);

#endif