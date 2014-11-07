#ifndef _CSSHANDLE_H
#define _CSSHANDLE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum cssType {
    typeSingle      = 0,
    typeMulti       = 1,
    typeParent      = 2,
    typeInclude     = 3,
    typeAnd         = 4,
};
enum NodeType {
    classNode = 1,
    idNode = 2,
    elementNode = 3,
};
typedef struct node {
    enum NodeType type;
    char   name[30];
    struct node* next;
}node;
typedef struct nodeList {
    node* head;
    int   type;
    int   priority;
}nodeList;

typedef struct cssNode {
    struct nodeList* nodes; 
    enum cssType type;
    char display[10];
    char position[10];
    char width[10];
    char height[10];
    char top[10];
    char bottom[10];
    char left[10];
    char right[10];
    char padding[4][10];
    char border[4][10];
    char margin[4][10];
    char color[10];
    char fontSize[10];
    char lineHeight[10];
    char textAlign[10];
    char fontStyle[10];
    char fontWeight[10];
    char lineBreak[10];
    struct cssNode *next;
    unsigned defineFlag:18;
}cssList, cssNode;

extern cssList* handleCss(const char* buffer);
extern void freeCssList(cssList* csss);
int getDefineState(const char* att, cssNode* css);
//extern void
#endif // _CSSHANDLE_H

