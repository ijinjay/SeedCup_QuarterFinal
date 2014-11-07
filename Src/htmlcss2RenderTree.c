#include "htmlcss2RenderTree.h"

static pRenderNode initANewRenderNode(void) {
	RenderNode *newN = (pRenderNode)malloc(sizeof(RenderNode));
	newN->domNode = NULL;
	newN->css = NULL;
	for (int i = 0; i < 100; ++i) 
		newN->csses[i] = NULL;
	newN->cssNum = 0;
	return newN;
}
void freeRenderTree(pRenderNode head) {
	freeDOMTree(head->domNode);
	// 可能没有css文件
	printf("free css start \n");
	if (head->css != NULL)
		freeCssList(head->css);
	printf("free render tree start \n");
	free(head);
}
// 单个元素样式比较
/*static int singleNodeCmp(node nnode, pDOMNode domNode){
	if (nnode.type == elementNode)
		return ((strcmp(nnode.name, getTagName(domNode->tag))==0) && (domNode->cssStyleNum <= CSSSTYLEMAXNUM ));
	else if (nnode.type == idNode) 
		return ((domNode->ID != NULL) && (strcmp(nnode.name, domNode->ID) == 0));
	else if (nnode.type == classNode)
		for (int i = 0; i < domNode->classNum; ++i) { 
			if (strcmp(domNode->classes[i], nnode.name) == 0)
				return 1;
		}
	return 0;
}*/
// 多元素样式比较
/*static int multiNodeCmp(node nnode, pDOMNode domNode) {
	node *pNode = &nnode;
	while(pNode != NULL) {
		if (singleNodeCmp((*pNode), domNode))
			return 1;
		pNode = pNode->next;
	}
	return 0;
}
static int parentNodeCmp(node nnode, pDOMNode domNode) {


	return 0;
}
static int includeNodeCmp(node nnode, pDOMNode domNode) {

	return 0;
}
static int andNodeCmp(node nnode, pDOMNode domNode) {

	return 0;
}*/

/*static void addCSSStyle2DOM(const cssList *csss, struct DOMNode **ppNode) {
	for (int i = 0; i < (*ppNode)->sonNum; ++i) {
		addCSSStyle2DOM(csss, &((*ppNode)->sonNodes[i]));
	}
	// 最多有CSSSTYLEMAXNUM个规则能匹配到一个节点
	cssNode **cssStyle = (cssNode **)malloc(CSSSTYLEMAXNUM * sizeof(cssNode *));
	// 得到第一个可用的css样式节点
	// cssNode *currentCss = csss->next;
	// 遍历整个css链表,将符合规则的css节点地址放入cssStyle;
	while(currentCss != NULL) {
		switch(currentCss->type) {
			case typeSingle:
				if (singleNodeCmp((*(currentCss->nodes->head)), (*ppNode)))
					cssStyle[(*ppNode)->cssStyleNum ++] = currentCss;
				break;
			case typeMulti:
				if (multiNodeCmp((*(currentCss->nodes->head)), (*ppNode)))
					cssStyle[(*ppNode)->cssStyleNum ++] = currentCss;
				break;
			case typeParent:
				if (parentNodeCmp((*(currentCss->nodes->head)), (*ppNode)))
					cssStyle[(*ppNode)->cssStyleNum ++] = currentCss;
				break;
			case typeInclude:
				if (includeNodeCmp((*(currentCss->nodes->head)), (*ppNode)))
					cssStyle[(*ppNode)->cssStyleNum ++] = currentCss;
				break;
			case typeAnd:
				if (andNodeCmp((*(currentCss->nodes->head)), (*ppNode)))
					cssStyle[(*ppNode)->cssStyleNum ++] = currentCss;
				break;
			default:
				break;
		}
		currentCss = currentCss->next;
	}
}
*/
RenderNode *generateRenderTree(char *html, char *css) {
	RenderNode *head = initANewRenderNode();
	head->domNode = generateDOMTree(html);
	// 可能没有css文件
	if (strlen(css) != 0) {
		head->css = handleCss(css); 
		printf("Start print css\n");
		printCSS(head->css);

		// TODO 
		// 获得第一个body节点
		// struct DOMNode *bodyNode = head->domNode->sonNodes[0];
		// addCSSStyle2DOM(head->css, &bodyNode);
	}
	return head;
}
char *getWebText(RenderNode *head) {
	char *webText = (char *)malloc( MAXHTMLLEN*2*sizeof(char));
	strcpy(webText, "To beautiful you");
	return webText;
}
void drawPNG(pRenderNode head) {
	CairoHandle *pCH = initDrawContext();
	st_style style;
    strcpy(style.offsetLeft,    "0px");
    strcpy(style.offsetTop,     "0px");
    strcpy(style.width,         "800px");
    strcpy(style.height,        "600px");
    strcpy(style.border[0],     "10px");
    strcpy(style.border[1],     "10px");
    strcpy(style.border[2],     "10px");
    strcpy(style.border[3],     "10px");
    strcpy(style.margin[0],     "10px");
    strcpy(style.margin[1],     "10px");
    strcpy(style.margin[2],     "10px");
    strcpy(style.margin[3],     "10px");
    strcpy(style.color,         "#00ffff");
    drawBorder(pCH, style);
    strcpy(style.offsetLeft,    "50px");
    strcpy(style.offsetTop,     "50px");
    strcpy(style.color,         "#0c0");
    drawBorder(pCH, style);
    strcpy(style.offsetLeft,    "30px");
    strcpy(style.offsetTop,     "30px");
    strcpy(style.color,         "#cf0");
    drawBorder(pCH, style);

// test draw text
    strcpy(style.offsetLeft,    "500px");
    strcpy(style.offsetTop,     "300px");
    strcpy((style).font_style,    "normal");
    strcpy((style).font_weight,   "normal");
    strcpy((style).text_align,    "left");
    strcpy((style).line_break,    "normal");
    strcpy((style).font_size,    "20px");
    strcpy(style.color,         "#c00");

    drawText(pCH, "To beautiful you", style);
    strcpy(style.offsetLeft,    "150px");
    strcpy(style.offsetTop,     "150px");
    strcpy((style).font_style,    "italic");
    strcpy((style).font_weight,   "bold");
    drawText(pCH, "I'm Jin Jay", style);
    writeDrawFile(pCH, "test.png");
    printf("draw a png\n");

	freeDraw(pCH);
}
