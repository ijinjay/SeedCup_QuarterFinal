#ifndef _CSSHANDLE_H
#define _CSSHANDLE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum cssType {
    typeSingle      = 0,
    typeMulti       = 1
};

typedef struct selectNode {
    enum cssType type;
    char   name[10][30];
    int    nodeNum;
}selectNode, *pSelectNode;

typedef struct cssNode {
    selectNode *snodes; 
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

typedef struct DOMCSSES {
    cssNode *      cssStyle[100];
    int             cssStyleNum;
    int             priorities[100];
} DOMCSSES, *pDOMCSSES;

extern cssList* handleCss(char* buffer);
extern void freeCssList(cssList* csss);
extern int getDefineState(const char* att, cssNode* css);
extern void printCSS(cssList *csss);
//extern void
#endif // _CSSHANDLE_H

