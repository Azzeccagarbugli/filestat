#include <stdio.h>
#include "filestat.h"
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>

int numeroMonitorati = 0;
int numeroLink = 0;
int numeroDirectory = 0;
int dimensioneTotale = 0;
int dimensioneMedia = 0;
int dimensioneMassima = 0;
int dimensioneMinima = 0;

void updateDimensioneMedia(){
    dimensioneMedia = dimensioneTotale/(numeroDirectory+numeroLink+numeroMonitorati);
}

void updateDimensioneMassima(int newMax){
    if(newMax > dimensioneMassima) dimensioneMassima = newMax;
}

void updateDimensioneMinima(int newMin){
    if(newMin < dimensioneMinima) dimensioneMinima = newMin;
}

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

int index_option(int nrElem, char *options[], char *optionToSearch){
    for(int i = 0; i<nrElem; i++){
        if(strcmp(options[i], optionToSearch)){
            return i;
        }
    }
    return -1;
}

readPath(char *path, int isR, int isL){
    struct stat *infoSingleFile;
    if(l){
        if(lstat(path, infoSingleFile) < 0) return -1;
    } else {
        if(stat(path, infoSingleFile) < 0) return -1;
    }
    
}

int main(int argc, char const *argv[])
{
    if(argc < 1){
        return 1;
    }

    char const *options[argc - 3];
    for(int i = 0; i< argc - 3 ; i++){
        options[i] = argv[i+1];
        printf(options[i]);
    }

    FILE *fileInput;
    if(strcmp(get_filename_ext(argv[argc - 2]),"in")){
      fileInput = fopen(argv[argc - 2], "r");
    } else {
        fileInput = fopen("filestat.in", "r");
    }

    FILE *fileOutput;
    if(strcmp(get_filename_ext(argv[argc - 1]),"db")){
      fileOutput = fopen(argv[argc - 1], "a");
    } else {
        fileOutput = fopen("filestat.db", "a");
    }

    char *currentPath;
    
    fcloseall(void);
    return 0;
}
