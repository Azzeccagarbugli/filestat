#include <stdio.h>
#include <stdlib.h>
#include "../include/tree.h"
#include <string.h>


RecordNode *addRecord(RecordNode *node, char *path, char *record);
RecordNode *addPath(RecordNode *node, char *path);
RecordNode *emptyNode()
{
    return NULL;
};

RecordNode *createNewNode(char *value, int isPath)
{
    RecordNode *newNode = (RecordNode *)malloc(sizeof(RecordNode));

    newNode->value = (char *)malloc((strlen(value)+1) * sizeof(char));
    strcpy(newNode->value, value);
    newNode->nextPath = NULL;
    newNode->nextRecord = NULL;
    newNode->isPath = isPath;
    return newNode;
};

RecordNode *addPath(RecordNode *node, char *path)
{
    if (isEmpty(node))
    {
        printf("Il path %s non era presente ed è stato aggiunto\n", path);
        return createNewNode(path, 1);
    }
    if (strcmp(node->value, path) == 0)
    {
        printf("Il path %s era già presente\n", path);
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
        printf("Ho aggiunto %s al percorso: %s\n", record, path);
        return createNewNode(record, 0);
    }
    else
    {
        if((strcmp(node->value, path) == 0) || (node->isPath == 0)){
            node->nextRecord = addRecord(node->nextRecord, path, record);
            return node;
        } else {
            //node->nextPath = addPath(node->nextPath, path);
            node->nextPath = addRecord(node->nextPath, path, record);
            return node;
        }
    }
}

RecordNode *addRecordByPath(RecordNode *node, char *path, char *record)
{
    node = addPath(node, path);
    node = addRecord(node, path, record);
    printf("Aggiunta analisi a path terminata\n");
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

int pathExist(RecordNode *node, char *path)
{
    if (isEmpty(node))
    {
        return 0;
    }
    else if (node->isPath)
    {
        if (strcmp(node->value, path) == 0)
        {
            return 1;
        }
        else
        {
            return pathExist(node->nextPath, path);
        }
    }
    else
    {
        return 1;
    }
}
void freeTree(RecordNode *node)
{
    if (node != NULL)
    {
        freeTree(node->nextPath);
        freeTree(node->nextRecord);
        free(node->value);
        free(node);
    }
}