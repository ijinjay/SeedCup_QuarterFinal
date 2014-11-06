#include "htmlcss2RenderTree.h"

static pRenderNode initANewRenderNode(void) {
	RenderNode *newN = (pRenderNode)malloc(sizeof(RenderNode));
	newN->domNode = NULL;
	newN->css = NULL;
	newN->pCH = NULL;
	newN->drawFunction = NULL;
	return newN;
}
void freeRenderTree(pRenderNode head) {
	freeDOMTree(head->domNode);
	freeCssList(head->css);
	freeDraw(head->pCH);
	free(head);
}
RenderNode *generateRenderTree(char *html, char *css) {
	RenderNode *head = initANewRenderNode();
	head->domNode = generateDOMTree(html);
	head->css = handleCss(css); 
	// TODO 
	head->pCH = initDrawContext();
	return head;
}
char *getWebText(RenderNode *head) {
	char *webText = (char *)malloc( MAXHTMLLEN*2*sizeof(char));
	strcpy(webText, "To beautiful you");
	return webText;
}
void drawPNG(pRenderNode head) {
	printf("draw a png!\n");
}
