#ifndef _HTML2DOM_H
#define _HTML2DOM_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <regex.h>
#include "cssHandle.h"

enum TAG {
    NO_TAG = 0,
    HEAD_TAG,
    BODY_TAG,
    SPAN_TAG,
    DIV_TAG,
    P_TAG,
    STRONG_TAG,
    H_TAG,
    EM_TAG,
    LINK_TAG,
    TEXT_TAG,
    DOCUMENT_TAG
};

typedef struct st_style {
    char offsetLeft[10];
    char offsetTop[10];
    char display[10];
    char position[10];
    char width[10];
    char height[10];
    char padding[4][10];
    char border[4][10];
    char margin[4][10];
    char left[10];
    char right[10];
    char top[10];
    char bottom[10];
    char color[10];
    char line_height[10];
    char font_size[10];
    char font_style[10];
    char font_weight[10];
    char text_align[10];
    char line_break[10];
}st_style;

#ifndef MAXSONNUM
#define MAXSONNUM 20
#endif

#ifndef MAXHTMLLEN
#define MAXHTMLLEN 10000
#endif
#ifndef CSSSTYLEMAXNUM
#define CSSSTYLEMAXNUM 100
#endif
typedef struct DOMNode {
    enum TAG        tag;
    struct DOMNode  *sonNodes[MAXSONNUM];
    int             sonNum;
    struct DOMNode  *fatherNode;
    // 节点属性相关
    st_style        style;
    char            ID[20];
    char            classes[20][20];
    int             classNum;
    char *          text; // 如果是文本，需要申请内存
    // 节点适用的css样式数组
    DOMCSSES *csses;

}DOMTree, *pDOMNode;

typedef struct NodeStack {
    enum TAG    tag;
    char        stylecontent[50];
}NodeStack;

typedef struct MultiResult{
    char results[10][50];
    int  num;
} MultiResult, *pMultiResult;
typedef struct CSSURL {
    char urls[10][30];
    int  urlNum;
}CSSURL, *pCSSURL;

// 通过html产生CSS的路径,会对html进行更改
extern CSSURL *parseCSSURL(char **pHTML);
// 通过解析解析过head标签后的html产生DOM树
extern DOMTree *generateDOMTree(const char *HTML);
// 释放DOM树占用的内存
extern void freeDOMTree(DOMTree *pHead);
// 打印DOMTree
extern void printDOMTree(DOMTree *pHead);
// 
extern char *getTagName(int tag);
extern void print2File(DOMTree *pNode, FILE *f,char **pstr);
// 打印节点的style
extern void printDOMNode(pDOMNode pNode, FILE *f);
#endif // _HTML2DOM_H
