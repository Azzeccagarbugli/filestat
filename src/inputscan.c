#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <linux/limits.h>
#include <unistd.h>
#include "../include/main.h"
#include "../include/inputscan.h"
#include "../include/datastructure.h"
#include "../include/scan.h"

PathEntry *inputLineAnalisis(char *, PathEntry *);
PathEntry *scanFilePath(char *, int, int, PathEntry *);
PathEntry *addFileAnalisis(struct stat *, char *, PathEntry *);
int checkLength(struct stat *);
int checkUID(struct stat *);
int checkGID(struct stat *);
int checkOptions(struct stat *);
char *findLastOf(char *, char);
char *getLinkAbsPath(char *);
PathEntry *directoryAnalisis(struct stat *, PathEntry *, int, int, char *);

/**
 * Gestione completa delle analisi delle informazioni presenti sul file di input trattato dal programma.
 * 
 * :param input: puntatore al file di input da cui leggere le informazioni sulle analisi passate
 * :param entry: puntatore alla struttura dati PathEntry in cui inserire le informazioni recuperate grazie al file di input
 * :return: puntatore alla struttura dati PathEntry aggiornata
 */
PathEntry *readInputFile(FILE *input, PathEntry *entry)
{
    size_t t = 0;
    char *line = NULL;
    for (ssize_t read = getline(&line, &t, input); read >= 0; read = getline(&line, &t, input))
    {
        entry = inputLineAnalisis(strtok(line, "\r\n"), entry);
    }

    free(line);
    return entry;
}

/**
 * Metodo che legge interamente le linee presenti all'interno del file di input.
 * 
 * :param riga: puntatore all'array di caratteri contenente la riga del file desiderata di cui si vuole effettuare la lettura
 * :param entry: puntatore alla struttura dati PathEntry in cui inserire le informazioni recuperate grazie all'analisi della riga
 * :return: puntatore alla struttura dati PathEntry aggiornata
 */
PathEntry *inputLineAnalisis(char *riga, PathEntry *entry)
{
    char *path;
    int isR = 0;
    int isL = 0;
    int spaceRead = 0;
    char *token;
    for (token = strtok(riga, " "); token; token = strtok(NULL, " "))
    {
        if (strcmp(token, "r") == 0)
            isR = 1;
        else if (strcmp(token, "l") == 0)
            isL = 1;
        else if (!spaceRead)
        {
            path = strdup(token);
            if (path[strlen(path) - 1] == '\\')
            {
                spaceRead = 1;
                path[strlen(path) - 1] = ' ';
            }
        }
        else
        {
            if (token[strlen(token) - 1] == '\\')
                token[strlen(token) - 1] = ' ';
            path = realloc(path, sizeof(path) + sizeof(token) + 1);
            strcat(path, token);
        }
    }

    entry = scanFilePath(path, isR, isL, entry);
    free(path);
    return entry;
}

/**
 * Analisi completa di un singolo file.
 * 
 * :param path: puntatore all'array di caratteri contenente path del file da analizzare
 * :param isR: flag per l'analisi ricorsiva del file in caso sia una directory
 * :param isL: flag utile in caso il file analizzato sia un link, di cui vengono analizzate le informazioni senza far riferimento al file referenziato
 * :param entry: puntatore alla struttura dati PathEntry in cui inserire le informazioni recuperate grazie all'analisi del file
 * :return: puntatore alla struttura dati PathEntry aggiornata
 */
PathEntry *scanFilePath(char *path, int isR, int isL, PathEntry *entry)
{
    struct stat *currentStat = (struct stat *)malloc(sizeof(struct stat));

    if (lstat(path, currentStat) < 0)
    {
        if (options.verbose_flag)
            printf("Non sono riuscito ad effettuare l'analisi sul file al path: %s\n", path);
        return entry;
    }

    if (isL == 1)
        stat(path, currentStat);
    if (checkOptions(currentStat))
        entry = S_ISLNK(currentStat->st_mode) ? addFileAnalisis(currentStat, getLinkAbsPath(path), entry) : addFileAnalisis(currentStat, realpath(path, NULL), entry);

    if (isR)
        entry = directoryAnalisis(currentStat, entry, isR, isL, path);

    free(currentStat);
    return entry;
}

/**
 * Analisi delle entry presenti all'interno di un file directory.
 * 
 * :param dirStat: puntatore all'istanza di struct stat in cui sono contenute le informazioni del file da analizzare
 * :param entry: puntatore alla struttura dati PathEntry in cui inserire le informazioni recuperate grazie all'analisi del file
 * :param isR: flag da passare all'analisi dei file referenziati dalle entry del file directory argomento
 * :param isL: flag da passare all'analisi dei file referenziati dalle entry del file directory argomento
 * :param path: puntatore all'array di caratteri contenente il pathname associato a dirStat
 * :return: puntatore alla struttura dati PathEntry aggiornata
 */
PathEntry *directoryAnalisis(struct stat *dirStat, PathEntry *entry, int isR, int isL, char *path)
{
    if (S_ISDIR(dirStat->st_mode))
    {
        DIR *dir;
        struct dirent *ent;
        dir = opendir(path);
        if (dir)
        {
            while (ent = readdir(dir))
            {
                if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
                    continue;

                char *copy = (char *)malloc(strlen(path) + strlen(ent->d_name) + 2);
                strcpy(copy, path);

                if (copy[strlen(path) - 1] != '/')
                    strcat(copy, "/");

                strcat(copy, ent->d_name);
                entry = scanFilePath(copy, isR, isL, entry);
                free(copy);
            }
        }
        closedir(dir);
    }
    return entry;
}

/**
 * Aggiunta delle informazioni associate all'analisi di un file ad una struttura dati PathEntry.
 * 
 * :param currentStat: puntatore all'istanza di struct stat in cui sono contenute le informazioni del file da analizzare
 * :param path: puntatore all'array di caratteri associato al file da analizzare
 * :param entry: puntatore alla struttura dati PathEntry in cui inserire le informazioni recuperate grazie all'analisi del file
 * :return: puntatore alla struttura dati PathEntry aggiornata
 */
PathEntry *addFileAnalisis(struct stat *currentStat, char *path, PathEntry *entry)
{
    if (!containsPath(entry, path))
    {
        if (options.verbose_flag)
            printf("Inizio ad elaborare il file al path: %s\n", path);
        char dlinfo;
        if (S_ISDIR(currentStat->st_mode))
        {
            dlinfo = 'd';
            if (options.verbose_flag)
                printf("Il file che sto elaborando è una directory\n");

            increaseDirectory();
        }
        else if (S_ISLNK(currentStat->st_mode))
        {
            dlinfo = 'l';
            if (options.verbose_flag)
                printf("Il file che sto elaborando è un link\n");

            increaseLink();
        }
        else
        {
            dlinfo = '-';
        }
        char irusr = currentStat->st_mode & S_IRUSR ? 'r' : '-';
        char iwusr = currentStat->st_mode & S_IWUSR ? 'w' : '-';
        char ixusr = currentStat->st_mode & S_IXUSR ? 'x' : '-';
        char irgrp = currentStat->st_mode & S_IRGRP ? 'r' : '-';
        char iwgrp = currentStat->st_mode & S_IWGRP ? 'w' : '-';
        char ixgrp = currentStat->st_mode & S_IXGRP ? 'x' : '-';
        char iroth = currentStat->st_mode & S_IROTH ? 'r' : '-';
        char iwoth = currentStat->st_mode & S_IWOTH ? 'w' : '-';
        char ixoth = currentStat->st_mode & S_IXOTH ? 'x' : '-';

        struct passwd *pwsUID = getpwuid(currentStat->st_uid);
        struct group *grpGID = getgrgid(currentStat->st_gid);

        char current_time[32];
        char time_last_access[32];
        char time_last_change[32];
        char time_last_chmod[32];

        char size[21];
        sprintf(size, "%ld", currentStat->st_size);
        char *record = malloc(33 + sizeof(pwsUID->pw_name) + sizeof(grpGID->gr_name) + 22 + 11 + 33 + 33 + 33 + 21);

        time_t curtime = time(NULL);
        time(&curtime);

        sprintf(record, "%s %s %s %s %c%c%c%c%c%c%c%c%c%c %s %s %s %ld",
                strtok(ctime_r(&curtime, current_time), "\n"), pwsUID->pw_name, grpGID->gr_name, size,
                dlinfo, irusr, iwusr, ixusr, irgrp, iwgrp, ixgrp, iroth, iwoth, ixoth,
                strtok(ctime_r(&currentStat->st_atime, time_last_access), "\n"),
                strtok(ctime_r(&currentStat->st_mtime, time_last_change), "\n"),
                strtok(ctime_r(&currentStat->st_ctime, time_last_chmod), "\n"),
                currentStat->st_nlink);

        if (options.length_flag || options.group_flag || options.user_flag)
            printf("# Analisi effettuata su: %s\n%s\n", path, record);

        entry = addPathAndAnalisis(entry, path, strtok(record, "\r\n"));
        updateStats(currentStat->st_size);

        if (options.verbose_flag)
            printf("Analisi del file effettuata correttamente\n\n");
        free(record);
    }
    free(path);
    return entry;
}

/**
 * Verifica che un file rispetti le condizioni espresse con il flag ``-l/--length``.
 * 
 * :param file: puntatore al file di cui verificare le condizioni
 * :return: 0 in caso di esito negativo della verifica, 1 in caso di esito positivo della verifica
 */
int checkLength(struct stat *file)
{
    if (!options.length_flag)
    {
        return 1;
    }
    else if (options.max_length >= file->st_size && options.min_length <= file->st_size)
    {
        return 1;
    }
    else if (options.max_length == 0 && options.min_length <= file->st_size)
    {
        return 1;
    }
    else if (options.min_length == 0 && options.max_length >= file->st_size)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * Verifica che un file rispetti le condizioni espresse con il flag ``-u/--user``.
 * 
 * :param file: puntatore file di cui verificare le condizioni
 * :return: 0 in caso di esito negativo della verifica, 1 in caso di esito positivo della verifica
 */
int checkUID(struct stat *file)
{
    if (!options.user_flag)
    {
        return 1;
    }
    else
    {
        struct passwd *pwsUID = getpwuid(file->st_uid);
        return (strcmp(options.uID, pwsUID->pw_name) == 0);
    }
}

/**
 * Verifica che un file rispetti le condizioni espresse con il flag ``-g/--group``.
 * 
 * :param file: puntatore al file di cui verificare le condizioni
 * :return: 0 in caso di esito negativo della verifica, 1 in caso di esito positivo della verifica
 */
int checkGID(struct stat *file)
{
    if (!options.group_flag)
    {
        return 1;
    }
    else
    {
        struct group *grpGID = getgrgid(file->st_gid);
        return (strcmp(options.gID, grpGID->gr_name) == 0);
    }
}

/**
 * Verifica che un file rispetti le condizioni espresse con i flag ``-l/--length``, ``-u/--user`` e ``-g/--group``.
 * 
 * :param file: puntatore al file di cui verificare le condizioni
 * :return: 0 in caso di esito negativo della verifica, 1 in caso di esito positivo della verifica
 */
int checkOptions(struct stat *file)
{
    return (checkLength(file) && checkUID(file) && checkGID(file));
}

/**
 * Permette di ottenere il puntatore all'ultima occorrenza di un carattere presente in una stringa.
 * Metodo ausiliario a ``getLinkAbsPath(char*)``.
 * 
 * :param str:  puntetore all'array di caratteri di cui effettuare l'analisi
 * :param c: carattere di cui si vuole ottenere il puntatore all'ultima occorrenza
 * :return: puntatore all'ultima occorrenza all'interno della stringa passata come argomento del carattere passato come argomento
 */
char *findLastOf(char *str, char c)
{
    for (char *i = str + strlen(str); i >= str; i--)
        if (*i == c)
            return i;
    return NULL;
}

/**
 * Metodo necessario al get del pathname assoluto di un link.
 * 
 * :param path: puntatore all'array di caratteri contentente il filename del link di cui si vuole ottenere il percorso assoluto
 * :return: la stringa contenente il percorso assoluto del link passato come parametro locale
 */
char *getLinkAbsPath(char *path)
{
    char *name;
    char *tmp;
    char *absPath = malloc(PATH_MAX);

    tmp = findLastOf(path, '/');
    if (tmp == NULL)
    {
        name = strdup(path);
        getcwd(absPath, PATH_MAX);
    }
    else
    {
        name = strdup(tmp + 1);
        *tmp = '\0';
        realpath(path, absPath);
    }

    strcat(absPath, "/");
    strcat(absPath, name);
    free(name);
    return absPath;
}