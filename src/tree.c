#include <stdio.h>
#include <stdlib.h>
#include "../include/tree.h"
#include <string.h>

RecordNode *emptyNode()
{
    return NULL;
};

RecordNode *createNewNode(char *value, RecordNode *left, RecordNode *right, int isPath)
{
    RecordNode *newNode = (RecordNode *)malloc(sizeof(RecordNode));
    char *temp = (char *)calloc(strlen(value), sizeof(char));
    strcpy(temp, value);
    newNode->value = temp;
    newNode->nextPath = left;
    newNode->nextRecord = right;
    newNode->isPath = isPath;
    return newNode;
};

RecordNode *addPath(RecordNode *node, char *path)
{
    if (isEmpty(node))
    {
        // printf("Il path inserito non era presente\n");
        return createNewNode(path, NULL, NULL, 1);
    }
    if (strcmp(node->value, path) == 0)
    {
        // printf("Il path inserito era già presente\n");
        return node;
    }
    else
    {

        // printf("Il path inserito non era in questo nodo\n");
        RecordNode *new = createNewNode(node->value, addPath(node->nextPath, path), node->nextRecord, node->isPath);
        free(node);
        return new;
    }
}

RecordNode *addRecordByPath(RecordNode *node, char *path, char *record)
{
    if (isEmpty(node))
    {
        // printf("Ho aggiunto %s al percorso: %s\n", record, path);
        RecordNode *new = createNewNode(record, NULL, NULL, 0);
        free(node);
        return new;
    }

    if ((strcmp(node->value, path) == 0) || (node->isPath != 1))
    {
        RecordNode *new = createNewNode(node->value, node->nextPath, addRecordByPath(node->nextRecord, path, record), node->isPath);
        free(node);
        return new;
    }
    else
    {
        RecordNode *new = createNewNode(node->value, addRecordByPath(node->nextPath, path, record), node->nextRecord, node->isPath);
        free(node);
        return new;
    }
}

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
        if(node->isPath){
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
        freeNode(node->nextPath);
        freeNode(node->nextRecord);
        free(node->value);
        free(node);
    }
}