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
    int    type;
    char   name[20];
    struct node* next;
}node;
typedef struct nodeList {
    node* head;             //head节点无数据，只有next
    int   type;
    int   priority;         //优先级
}nodeList;
typedef struct cssNode {
    struct nodeList* nodes; //选择器选择的结点链
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
}cssNode;
typedef struct cssList
{
    cssNode* head;          //head节点无数据，只有next
}cssList;
typedef struct rule
{
    char name[15];
    int namePos;
    char value[30];
    int valPos;
    struct rule* next;
}rule;
typedef struct ruleList {
    rule* head;             //head节点无数据，只有next
    int type;
}ruleList;
static char attributes[18][15]= {
    "display",      //1
    "position",     //2
    "width",        //3
    "height",       //4
    "top",          //5
    "bottom",       //6
    "left",         //7
    "right",        //8
    "padding",      //9
    "border",       //10
    "margin",       //11
    "color",        //12
    "font-size",    //13
    "line-height",  //14
    "text-align",   //15
    "font-style",   //16
    "font-weight",  //17
    "line-break",   //18
};
extern cssList* handleCss(const char* buffer);
extern void freeCssList(cssList* csss);

#endif // _CSSHANDLE_H

