#ifndef _HTMLCSS2RENDERTREE_H
#define _HTMLCSS2RENDERTREE_H 
#include "html2DOM.h"
#include "draw.h"
#include "cssHandle.h"

typedef struct RenderNode {
	DOMTree *domNode;
	cssList *css;
	CairoHandle *pCH;
	void (*drawFunction)(pCairoHandle pCH,st_style style);
} RenderNode, *pRenderNode;

extern RenderNode *generateRenderTree(char *html, char *css);
extern char *getWebText(RenderNode *head);
extern void freeRenderTree(pRenderNode head);
extern void drawPNG(pRenderNode head);
#endif // _HTMLCSS2RENDERTREE_H
