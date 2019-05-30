
#ifndef _TREEH_

#define _TREEH_
typedef struct treenode RecordNode;
struct treenode{
    char *value;
    int isPath;
    RecordNode *nextPath;
    RecordNode *nextRecord;
};

RecordNode* emptyNode();
RecordNode* createNewNode(char* value, int isPath);
RecordNode *addRecordByPath(RecordNode *node, char *path, char *record);
int isEmpty(RecordNode* node);
RecordNode *getNodeByPath(RecordNode *current, char *value);
void printInOrder(RecordNode *node);
void freeNode(RecordNode *node);
void printOnFile(RecordNode *node, FILE *output);
void printHistory(RecordNode *root, char *path);


#endif