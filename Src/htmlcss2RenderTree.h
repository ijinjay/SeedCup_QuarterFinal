#ifndef _HTMLCSS2RENDERTREE_H
#define _HTMLCSS2RENDERTREE_H 
#include "html2DOM.h"
#include "draw.h"
#include "cssHandle.h"

typedef struct RenderNode {
	DOMTree *domNode;
	cssList *css;		 // 供头结点使用
	cssNode *csses[100]; // 供一般节点使用
	int cssNum;
} RenderNode, *pRenderNode;
enum eic {
	typeElement,
	typeID,
	typeClass
};

extern RenderNode *generateRenderTree(char *html, char *css);
extern char *getWebText(RenderNode *head);
extern void freeRenderTree(pRenderNode head);
extern void drawPNG(pRenderNode head);
#endif // _HTMLCSS2RENDERTREE_H
