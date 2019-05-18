#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void analisiSingolaRiga(char *riga)
{
    printf("Line: %s \n", riga);
    char *path = (char *) calloc(strlen(riga), sizeof(char));      
    int bR = 0;
    int bL = 0;
    int pathRead = 0;
    char *token;
    for (token = strtok(riga, " "); token; token = strtok(NULL, " "))
    {
        if (strcmp(token, "r") == 0)
        {
            bR = 1;
        }
        else if (strcmp(token, "l") == 0)
        {
            bL = 1;
        }
        else if (!pathRead)
        {
            strcpy(path, token);
            pathRead = 1;
        }
    }

    printf("Path: %s\n", path);
    printf("R: %d\n", bR);
    printf("L: %d\n", bL);
    free(path);
}

int main(int argc, char const *argv[])
{
    char nome[] = "./filestat.in";
    FILE *input = fopen(nome, "r");
    char line[128];
    while (fgets(line, sizeof line, input) != NULL)
    {
        analisiSingolaRiga(strtok(line, "\n"));
    }
    fclose(input);
    return 1;
}
