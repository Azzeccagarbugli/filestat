#include <stdio.h>
#include <stdlib.h>
#include "../include/datastructure.h"
#include "../include/main.h"
#include <string.h>

RecordNode *addRecord(RecordNode *node, char *path, char *record);
RecordNode *addPath(RecordNode *node, char *path);
void printNodeIfAnalisis(RecordNode *node);
RecordNode *emptyNode()
{
    return NULL;
};

RecordNode *createNewNode(char *value, int isPath)
{
    RecordNode *newNode = malloc(sizeof(RecordNode));
    newNode->value = (char *)malloc(strlen(value) + 2 * sizeof(char));
    strcpy(newNode->value, value);
    newNode->nextPath = NULL;
    newNode->nextRecord = NULL;
    newNode->isPath = isPath;
    // printf("Nodo con valore %s aggiunto\n", newNode->value);
    return newNode;
};

RecordNode *addPath(RecordNode *node, char *path)
{
    if (isEmpty(node))
    {
        // printf("Il path %s non era presente ed è stato aggiunto\n", path);
        return createNewNode(path, 1);
    }
    if (strcmp(node->value, path) == 0)
    {
        // printf("Il path %s era già presente\n", path);
        return node;
    }
    else
    {
        node->nextPath = addPath(node->nextPath, path);
        return node;
    }
}

RecordNode *addRecord(RecordNode *node, char *path, char *record)
{
    if (isEmpty(node))
    {
        // printf("Ho aggiunto %s al percorso: %s\n", record, path);
        return createNewNode(record, 0);
    }
    else
    {
        if ((strcmp(node->value, path) == 0) || (node->isPath == 0))
        {
            node->nextRecord = addRecord(node->nextRecord, path, record);
            return node;
        }
        else
        {
            node->nextPath = addRecord(node->nextPath, path, record);
            return node;
        }
    }
}

RecordNode *addRecordByPath(RecordNode *node, char *path, char *record)
{
    node = addPath(node, path);
    node = addRecord(node, path, record);
    // printf("Aggiunta analisi a path terminata\n");
    return node;
};

int isEmpty(RecordNode *node)
{
    return (node == NULL);
};

void printInOrder(RecordNode *node)
{
    if (isEmpty(node))
    {
        return;
    }
    else
    {
        if (node->isPath)
        {
            printf("# ");
        }
        printf("%s\n", node->value);
        if (node->nextRecord == NULL)
        {
            printf("###\n");
        }
        printInOrder(node->nextRecord);
        printInOrder(node->nextPath);
    }
}

RecordNode *getNodeByPath(RecordNode *current, char *value)
{
    if (isEmpty(current))
    {
        //Il nodo col path cercato non esiste
        return NULL;
    }
    if (strcmp(current->value, value) == 0)
    {
        return current;
    }
    else
    {
        return getNodeByPath(current->nextPath, value);
    }
};

void freeNode(RecordNode *node)
{
    if (node != NULL)
    {
        free(node->value);
        free(node);
    }
}

void printOnFile(RecordNode *node, FILE *output)
{
    if (isEmpty(node))
    {
        return;
    }
    else
    {
        if (node->isPath)
        {
            fprintf(output, "%s ", "#");
            if (opt_info.noscan_flag)
            {
                printf("# ");
            }
        }
        fprintf(output, "%s\n", node->value);
        RecordNode *nextRecord = node->nextRecord;
        RecordNode *nextPath = node->nextPath;

        if (opt_info.noscan_flag)
        {
            printf("%s\n", node->value);
        }

        freeNode(node);
        if (nextRecord == NULL)
        {
            fprintf(output, "###\n", NULL);
            if (opt_info.noscan_flag)
            {
                printf("###\n");
            }
        }
        printOnFile(nextRecord, output);
        printOnFile(nextPath, output);
    }
}

void printHistory(RecordNode *root, char *path)
{
    printf("Cronologia del file al path %s\n", realpath(path, NULL));
    RecordNode *current = getNodeByPath(root, realpath(path, NULL));
    if (current == NULL)
    {
        printf("Non esiste cronologia di tale file nel file di output specificato\n");
    }
    else
        while (current != NULL)
        {
            printNodeIfAnalisis(current);
            current = current->nextRecord;
        }
}

void printNodeIfAnalisis(RecordNode *node)
{
    if(node->isPath == 0){
        printf("%s\n", node->value);
    }
}