
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
RecordNode* createNewNode(char* value, RecordNode* left, RecordNode* right, int isPath);
RecordNode *addRecordByPath(RecordNode *node, char *path, char *record);
int isEmpty(RecordNode* node);
RecordNode *getNodeByPath(RecordNode *current, char *value);
void printInOrder(RecordNode *node);
void freeTree(RecordNode *node);
int pathExist(RecordNode *node, char *path);


#endif