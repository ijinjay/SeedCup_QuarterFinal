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
// 
static int getSinglePriority(DOMTree *domNode, char *content) {
	switch(content[0]) {
		case '#':
			if (!strcmp(content, domNode->ID))
				return 100;
		case '.':
				for (int i = 0; i < domNode->classNum; ++i) {
					if (!strcmp(content, domNode->classes[i]))
						return 10;
				}
		default:
			if (!strcmp(content, getTagName(domNode->tag)))
				return 1;
		break;
	}
	return 0;
}
// 单个元素样式比较，返回优先级
static int singleNodeCmp(char *name, DOMTree *domNode){
	// spos记录特殊符号的位置
	int spos[10];
	// posNum记录特殊符号的个数
	int posNum = 0;
	spos[0] = 0;
	for (int i = 0; i < strlen(name); ++i) {
		if (name[i] == '.'
			|| name[i] == '#'
			|| name[i] == '>'
			|| name[i] == ' ') {
			spos[posNum ++] = i;
		}
	}
	// 将字符串的结尾位置记录为特殊位置
	spos[posNum] = strlen(name);
	// 记录相邻选择器元素的关系
	int ifFather = 0;
	int ifBro = 0;
	int ifInclude = 0;
	// 优先级
	int priority = 0;
	DOMTree *currentDom = domNode;
	// 解析整个字符串
	for (int i = posNum - 1; i >= 0; --i) {
		char currentStr[20];
		int j = 0;
		// 两个特殊符号在一起，需要跳过
		if (spos[i] == spos[i+1] - 1)
			continue;
		for (int k = spos[i]; k < spos[i + 1]; ++k)
			currentStr[j ++] = name[k];
		currentStr[j] = '\0';
		// 若特殊符号为'>'
		if (ifFather) {
			int tempPriority;
			if((tempPriority = getSinglePriority(domNode->fatherNode, currentStr)) == 0){
				priority = 0;
				break;
			}
			else { 
				priority += tempPriority;
				currentDom = domNode->fatherNode;
			}
		}
		// 若特殊符号为' '
		else if (ifInclude) {
			int tempPriority;
			// 若当前节点还有父节点时
			while(currentDom->fatherNode != NULL) {
				//若匹配成功，则停止向上遍历
				if ((tempPriority = getSinglePriority(currentDom->fatherNode, currentStr)) != 0) {
					priority += tempPriority;
					break;
				}
				//若匹配不成功，继续向上遍历查找
				currentDom = currentDom->fatherNode;
			}
			// 遍历所有还未找到，则不匹配
			if (tempPriority == 0) {
				priority = 0;
				break;
			}
		}
		// 若特殊符号位'.'或'#',并且前一个字符为非特殊符号
		else if (ifBro) {
			int tempPriority;
			if ((tempPriority = getSinglePriority(currentDom, currentStr) == 0)) {
				priority = 0;
				break;
			}
			priority += tempPriority;
		}
		else {
			int tempPriority;
			if ((tempPriority = getSinglePriority(currentDom, currentStr)) == 0) {
				priority = 0;
				break;
			}
			priority += tempPriority;
		}
		// 更新标记位
		if ((currentStr[0] != ' ') && (currentStr[0] != '>'))
			getSinglePriority(domNode, currentStr);
		if (spos[i] > 0 && (name[spos[i]] == '>' || name[spos[i]-1] == '>'))
			ifFather = 1;
		else
			ifFather = 0;
		if (spos[i] > 0 && (name[spos[i]] == ' ' || name[spos[i]-1] == ' '))
			ifInclude = 1;
		else
			ifInclude = 0;
		if (spos[i] > 0 && isalnum(name[spos[i] - 1]) 
						&& name[spos[i]] != ' '
						&& name[spos[i]] != '>')
			ifBro = 1;
		else
			ifBro = 0;
	}
	if (spos[0] == 0) {
		priority = getSinglePriority(currentDom, name);
	}
	else {
		if (priority == 0)
			return 0;
		else {
			int temp;
			char currentStr[20];
			int j = 0;
			for (int i = 0; i < spos[0]; ++i)
				currentStr[j ++] = name[i];
			currentStr[j] = '\0';
			if ((temp = getSinglePriority(currentDom, currentStr)) == 0) 
				return 0;
			priority += temp;
			return priority;
		}
	}
	return 0;
}
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
*/

static void addCSSStyle2DOM(const cssList *csss, struct DOMNode **ppNode) {
	for (int i = 0; i < (*ppNode)->sonNum; ++i) {
		addCSSStyle2DOM(csss, &((*ppNode)->sonNodes[i]));
	}
	if ((*ppNode)->tag == TEXT_TAG)
		return;
	// 最多有CSSSTYLEMAXNUM个规则能匹配到一个节点
	DOMCSSES *domcccc = (DOMCSSES *)malloc(sizeof(DOMCSSES));
	// 得到第一个可用的css样式节点
	cssNode *currentCss = csss->next;
	// 遍历整个css链表,将符合规则的css节点地址放入cssStyle;
	int priority = 0;
	currentCss = currentCss->next;
	while(currentCss != NULL) {
		if (currentCss->snodes != NULL) {
			printf("not NULL\n");
			for (int i = 0; i < currentCss->snodes->nodeNum; ++i) {
				if ((priority = singleNodeCmp(currentCss->snodes->name[i], *ppNode)) != 0) {
					domcccc->cssStyle[domcccc->cssStyleNum ++] = currentCss;
					domcccc->priorities[domcccc->cssStyleNum - 1] = priority;
				}
			}
		}
		currentCss = currentCss->next;
	}
	(*ppNode)->csses = domcccc;
}

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
		struct DOMNode *bodyNode = head->domNode->sonNodes[0];
		addCSSStyle2DOM(head->css, &bodyNode);
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
