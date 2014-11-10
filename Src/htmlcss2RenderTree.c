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
	if (head->css != NULL)
		freeCssList(head->css);
	free(head);
}
static int getSinglePriority(DOMTree *domNode, char *content) {
	switch(content[0]) {
		case '#':
			if (!strcmp(content+1, domNode->ID)) 
				return 100;
			break;
		case '.':
				for (int i = 0; i < domNode->classNum; ++i) {
					if (!strcmp(content+1, domNode->classes[i])) 
						return 10;
				} break;
		case ' ':
		case '>':
			if (!strcmp(content + 1, getTagName((domNode->tag))))
				return 1;break;
		default:
			if (!strcmp(content, getTagName(domNode->tag))) {
				return 1;
			}
		break;
	}
	return 0;
}
static int typePriority(DOMTree **currentDom, char *currentStr, int *priority, int ifFather, int ifInclude, int ifBro) {
	// 处理 '>'
	if (ifFather) {
		int tempPriority;
		if((tempPriority = getSinglePriority((*currentDom)->fatherNode, currentStr)) == 0){
			(*priority) = 0;
			return 0;
		}
		else { 
			(*priority) += tempPriority;
			(*currentDom) = (*currentDom)->fatherNode;
		}
	}
	// 若前一次特殊符号为' '
	else if (ifInclude) {
		int tempPriority;
		// 若当前节点还有父节点时

		while((*currentDom)->fatherNode != NULL) {
			//若匹配成功，则停止向上遍历
			if ((tempPriority = getSinglePriority((*currentDom)->fatherNode, currentStr)) != 0) {
				(*priority) += tempPriority;
				break;
			}
			//若匹配不成功，继续向上遍历查找
			(*currentDom) = (*currentDom)->fatherNode;
		}
		// 遍历所有还未找到，则不匹配
		if (tempPriority == 0) {
			(*priority) = 0;
			return 0;
		}
		(*currentDom) = (*currentDom)->fatherNode;
	}
	// 若特殊符号位'.'或'#',并且前一个字符为非特殊符号
	else if (ifBro) {
		int tempPriority;
		// until here is right
		if ((tempPriority = getSinglePriority((*currentDom), currentStr))== 0) {
			(*priority) = 0;
			return 0;
		}
		(*priority) += tempPriority;
	}
	else {
		int tempPriority;
		int ifAddone = 0;
		if ((currentStr[0] == ' ') || (currentStr[0] == '>')) {
			ifAddone = 1;
		}
		if ((tempPriority = getSinglePriority((*currentDom), currentStr + ifAddone)) == 0) {
			(*priority) = 0;
			return 0;
		}
		(*priority) += tempPriority;
	}
	return (*priority);
}

// 单个元素样式比较，返回优先级
static int singleNodeCmp(char *name, DOMTree *domNode){
	// spos记录特殊符号的位置
	int spos[20];
	// posNum记录特殊符号的个数
	int posNum = 0;
	spos[0] = 0;
	for (int i = 0; i < strlen(name); ++i) {
		if (name[i] == '.' || name[i] == '#' || name[i] == '>' || name[i] == ' ') {
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
	// 特殊情况
	if (posNum == 0) {
		return getSinglePriority(domNode, name);
	}
	// 解析整个字符串
	for (int i = posNum - 1; i >= 0; --i) {
		// 两个特殊符号在一起，需要跳过
		if (spos[i] == spos[i+1] - 1)
			continue;
		char currentStr[20];
		int j = 0;
		for (int k = spos[i]; k < spos[i + 1]; ++k)
			currentStr[j ++] = name[k];
		currentStr[j] = '\0';
		if (typePriority(&currentDom, currentStr, &priority, ifFather, ifInclude, ifBro) == 0) {
			return 0;
		}
		// 更新标记位	
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
	// 处理#id或者.class这一类情况
	if (spos[0] == 0 && priority != 0 && posNum == 1) {
		// 处理 #id .class这种情况
		return priority;
	}
	// 处理#id#id类似的情况

	// 处理element#id或element.class或element .class或element>#id这种情况
	else {
		if (priority == 0)
			return 0;
		else if(spos[0] != 0) {
			char currentStr[20];
			int j = 0;
			for (int i = 0; i < spos[0]; ++i)
				currentStr[j ++] = name[i];
			currentStr[j] = '\0';
			int temp = 0;
			if ((temp = typePriority(&currentDom, currentStr, &priority, ifFather, ifInclude, ifBro)) == 0)
				return 0;
			// priority += temp;
		}
	}
	return priority;
}
#ifndef UPDATECSSSTYLEATTR
#define UPDATECSSSTYLEATTR(x) {if (getDefineState(#x, (tempNode))) 	\
	strcpy((*domNode)->style.x, tempNode->x);						\
};													 
#endif
#ifndef UPDATEINHERITSTYLE
#define UPDATEINHERITSTYLE(X,y) { 										\
	if ((*domNode)->inheritStyle[ X ] != 1)		{						\
		strcpy((*domNode)->style.y, (*domNode)->fatherNode->style.y);	\
	}\
}
#endif
static void updateCssAtt(DOMTree **domNode) {
	for (int i = 0; i < (*domNode)->csses->cssStyleNum; ++i) {
		cssNode *tempNode = (*domNode)->csses->cssStyle[i];

		UPDATECSSSTYLEATTR(display);
		UPDATECSSSTYLEATTR(position);
		UPDATECSSSTYLEATTR(width);
		UPDATECSSSTYLEATTR(height);
		UPDATECSSSTYLEATTR(bottom);
		UPDATECSSSTYLEATTR(left);
		UPDATECSSSTYLEATTR(right);

		if (getDefineState("margin", tempNode))
			for (int i = 0; i < 4; ++i)
				strcpy((*domNode)->style.margin[i], tempNode->margin[i]);
		if (getDefineState("border", tempNode))
			for (int i = 0; i < 4; ++i)
				strcpy((*domNode)->style.border[i], tempNode->border[i]);
		if (getDefineState("padding", tempNode))
			for (int i = 0; i < 4; ++i)
			{
				strcpy((*domNode)->style.padding[i], tempNode->padding[i]);
			}
		if (getDefineState("color", tempNode)) {
			strcpy((*domNode)->style.color, tempNode->color);
			(*domNode)->inheritStyle[INHERIT_COLOR] = 1;
		}

		if (getDefineState("font-size", tempNode)) {
			strcpy((*domNode)->style.font_size, tempNode->font_size);
			(*domNode)->inheritStyle[INHERIT_FONT_SIZE] = 1;
		}
		if (getDefineState("line-height", tempNode)) {
			strcpy((*domNode)->style.line_height, tempNode->line_height);
			(*domNode)->inheritStyle[INHERIT_LINE_HEIGHT] = 1;
		}
		if (getDefineState("text-align", tempNode)) {
			strcpy((*domNode)->style.text_align, tempNode->text_align);
			(*domNode)->inheritStyle[INHERIT_TEXT_ALIGN] = 1;
		}
		if (getDefineState("font-style", tempNode)) {
			strcpy((*domNode)->style.font_style, tempNode->font_style);
			(*domNode)->inheritStyle[INHERIT_FONT_STYLE] = 1;
		}
		if (getDefineState("font-weight", tempNode)) {
			strcpy((*domNode)->style.font_weight, tempNode->font_weight);
			(*domNode)->inheritStyle[INHERIT_FONT_WEIGHT] = 1;
		}
		if (getDefineState("line-break", tempNode)) {
			strcpy((*domNode)->style.line_break, tempNode->line_break);
			(*domNode)->inheritStyle[INHERIT_LINE_BREAK] = 1;
		}
	}
	UPDATEINHERITSTYLE(INHERIT_COLOR, color);
	UPDATEINHERITSTYLE(INHERIT_FONT_SIZE, font_size);
	UPDATEINHERITSTYLE(INHERIT_FONT_STYLE, font_style);
	UPDATEINHERITSTYLE(INHERIT_FONT_WEIGHT, font_weight);
	UPDATEINHERITSTYLE(INHERIT_LINE_BREAK, line_break);
	UPDATEINHERITSTYLE(INHERIT_LINE_HEIGHT, line_height);
	UPDATEINHERITSTYLE(INHERIT_TEXT_ALIGN, text_align);
}

static void addCSSStyle2DOM(const cssList *csss, struct DOMNode **ppNode) {	
	if ((*ppNode)->tag == TEXT_TAG) {
		// 文本直接继承包含元素的继承属性
		strcpy((*ppNode)->style.color, (*ppNode)->fatherNode->style.color);
		strcpy((*ppNode)->style.font_size, (*ppNode)->fatherNode->style.font_size);
		strcpy((*ppNode)->style.font_style, (*ppNode)->fatherNode->style.font_style);
		strcpy((*ppNode)->style.font_weight, (*ppNode)->fatherNode->style.font_weight);
		strcpy((*ppNode)->style.line_break, (*ppNode)->fatherNode->style.line_break);
		strcpy((*ppNode)->style.line_height, (*ppNode)->fatherNode->style.line_height);
		strcpy((*ppNode)->style.text_align, (*ppNode)->fatherNode->style.text_align);
		return;
	}
	// 最多有CSSSTYLEMAXNUM个规则能匹配到一个节点
	DOMCSSES *domcccc = initADomCSS();
	// 得到第一个可用的css样式节点
	cssNode *currentCss = csss->next;
	// 遍历整个css链表,将符合规则的css节点地址放入cssStyle;
	int priority = 0;
	while(currentCss != NULL) {
		if (currentCss->snodes != NULL) {
			for (int i = 0; i < currentCss->snodes->nodeNum; ++i) {
				if ((priority = singleNodeCmp(currentCss->snodes->name[i], *ppNode)) != 0) {
					domcccc->cssStyle[domcccc->cssStyleNum ++] = currentCss;
					domcccc->priorities[domcccc->cssStyleNum - 1] = priority;
					// 交换排序
					for (int i = domcccc->cssStyleNum - 1; i > 0 ; --i) {
						if (domcccc->priorities[i] < domcccc->priorities[i-1]) {
							cssNode *tempNode = domcccc->cssStyle[i];
							domcccc->cssStyle[i] = domcccc->cssStyle[i-1];
							domcccc->cssStyle[i-1] = tempNode;
							int temp = domcccc->priorities[i];
							domcccc->priorities[i] = domcccc->priorities[i-1];
							domcccc->priorities[i] = domcccc->priorities[i-1];
							domcccc->priorities[i-1] = temp;
						}
					}
				}
			}
		}
		currentCss = currentCss->next;
	}
	(*ppNode)->csses = domcccc;
	updateCssAtt(ppNode);

	for (int i = 0; i < (*ppNode)->sonNum; ++i) {
		addCSSStyle2DOM(csss, &((*ppNode)->sonNodes[i]));
	}
}
static void calculateWidth(DOMTree **ppNode, char* fatherWidth) {
	int fontsize = px2int((*ppNode)->style.font_size);
	int fatherw= px2int(fatherWidth);
	int width = len2int((*ppNode)->style.width, fontsize, fatherw);
	int widthFlag = strcmp((*ppNode)->style.width, "auto"); 
	int mlFlag =  strcmp((*ppNode)->style.margin[3], "auto"); 
	int mrFlag =  strcmp((*ppNode)->style.margin[1], "auto");
	// int lFlag = strcmp((*ppNode)->style.left, "auto");
	// int rFlag = strcmp((*ppNode)->style.right, "auto");
	if(!strcmp((*ppNode)->style.position, "absolute") ){
		// // TODO
		// if(!lFlag && !rFlag && !widthFlag) {
		// 	if(!mlFlag) marginLeft = 0;
		// 	if(!mrFlag) marginRight = 0;
		// 	left = 0;
		// }
		// // else if()
		// // {

		// // }

	}
	else{
		int margin3 = 0,margin1 = 0;
		int border3 = px2int((*ppNode)->style.border[3]);
		int border1 = px2int((*ppNode)->style.border[1]);
		int padding3 = len2int((*ppNode)->style.padding[3], fontsize, fatherw);
		int padding1 = len2int((*ppNode)->style.padding[1], fontsize, fatherw);

		if(widthFlag && mlFlag && mrFlag) {
			strcpy((*ppNode)->style.width, "auto");
		} 
		if(!widthFlag && mlFlag &&mrFlag) {
			int margin3 =  len2int((*ppNode)->style.margin[3], fontsize, fatherw);
			int margin1 =  len2int((*ppNode)->style.margin[1], fontsize, fatherw);
			width = fatherw - margin3 - margin1 - border1 -border3-padding3-padding1;
		}
		else if(widthFlag && !mlFlag && !mrFlag) {
			width = len2int((*ppNode)->style.width, fontsize, fatherw);
			margin3 = (fatherw - width - border3 - border1- padding1 - padding3)/2;
			margin1 = margin3;
		}
		else if(!widthFlag && !mlFlag && mrFlag) {
			margin3 = 0;
			margin1 =len2int((*ppNode)->style.margin[1], fontsize, fatherw);
			width = fatherw - margin1 - padding1 -padding3- border1 -border3;
		}
		else if(!widthFlag && mlFlag && !mrFlag) {
			margin1 = 0;
			margin3 =len2int((*ppNode)->style.margin[3], fontsize, fatherw);
			width = fatherw - margin3 - padding1 -padding3- border1 -border3;
		}
		else if(!widthFlag && !mlFlag &&!mrFlag) {
			margin1 = 0;
			margin3 = 0;
			width = fatherw -padding1 - padding3- border1 - border3;
		}
		else if(widthFlag && !mlFlag && mrFlag) {
			margin3 =len2int((*ppNode)->style.margin[3], fontsize, fatherw);
			width = len2int((*ppNode)->style.width, fontsize, fatherw);
			margin1 =  fatherw-margin3-width-padding1-padding3-border1-border3;
		}
		else if(widthFlag && mlFlag && !mrFlag) {
			margin1 =len2int((*ppNode)->style.margin[1], fontsize, fatherw);
			width = len2int((*ppNode)->style.width, fontsize, fatherw);
			margin3 =  fatherw-margin1-width-padding1-padding3-border1-border3;
		}

		sprintf((*ppNode)->style.margin[3], "%dpx", margin3);
		sprintf((*ppNode)->style.margin[1], "%dpx", margin1);
		sprintf((*ppNode)->style.border[3], "%dpx", border3);
		sprintf((*ppNode)->style.border[1], "%dpx", border1);
		sprintf((*ppNode)->style.padding[3], "%dpx", padding3);
		sprintf((*ppNode)->style.padding[3], "%dpx", padding1);
		sprintf((*ppNode)->style.width, "%dpx", width);
	}
	
}

static void calculateStyle(DOMTree **ppNode) {
	// BODY_TAG没有父节点
	if((*ppNode)->tag != BODY_TAG) {
		DOMTree* fatherNode = (*ppNode)->fatherNode;
		if(!strcmp((*ppNode)->style.display,"none"))
			return ;
		if(!strcmp((*ppNode)->style.display,"inline")) {
			//行内元素等待撑开
			strcpy((*ppNode)->style.height,"auto");
			strcpy((*ppNode)->style.width,"auto");
		}
		if(!strcmp((*ppNode)->style.display,"block")){
			if (!strcmp(fatherNode->style.width, "auto")) {
				strcpy((*ppNode)->style.width, "auto");
			}
			else {
				calculateWidth(ppNode, fatherNode->style.width);
			}
		}
		
		if (strcmp((*ppNode)->style.display, "none") == 0 )
			return;

		if ((*ppNode)->tag == TEXT_TAG) {
			int textlen = strlen((*ppNode)->text);
			int fontsize = px2int((*ppNode)->style.font_size);
			sprintf((*ppNode)->style.width, "%dpx", fontsize * textlen);
			char *str = (*ppNode)->style.line_height;
			int lineHLen = strlen(str);
			int line_height = 0; 
			if (strcmp(str + lineHLen -2, "px") == 0)
				line_height = px2int(str);
			else
				line_height = (int) fontsize * strtof((*ppNode)->style.line_height, NULL);
			sprintf((*ppNode)->style.height, "%dpx", line_height);
			return;
		}
	}

	for (int i = 0; i < (*ppNode)->sonNum; ++i) {
		calculateStyle(&((*ppNode)->sonNodes[i]));
	}
}
static void secondeCalcuStyle(DOMTree **ppNode) {
	for (int i = 0; i < (*ppNode)->sonNum; ++i) {
		secondeCalcuStyle(&((*ppNode)->sonNodes[i]));
	}
	int height = 0;
	int width = 0;
	if (!strcmp((*ppNode)->style.height, "auto")) {
		for (int i = 0; i < (*ppNode)->sonNum; ++i) {
			DOMTree *sonNode = (*ppNode)->sonNodes[i];
			int margin0 = px2int(sonNode->style.margin[0]);
			int margin2 = px2int(sonNode->style.margin[2]);
			int border0 = px2int(sonNode->style.border[0]);
			int border2 = px2int(sonNode->style.border[2]);
			int padding0 = px2int(sonNode->style.padding[0]);
			int padding2 = px2int(sonNode->style.padding[2]);
			int sonheight = px2int(sonNode->style.height);
			height = height + margin0 + margin2 + border0 + border2 + padding0 + padding2 + sonheight;
		}
		sprintf((*ppNode)->style.height, "%dpx", height);
	}
	if (!strcmp((*ppNode)->style.width, "auto")) {
		for (int i = 0; i < (*ppNode)->sonNum; ++i) {
			DOMTree *sonNode = (*ppNode)->sonNodes[i];
			int margin1 = px2int(sonNode->style.margin[1]);
			int margin3 = px2int(sonNode->style.margin[3]);
			int border1 = px2int(sonNode->style.border[1]);
			int border3 = px2int(sonNode->style.border[3]);
			int padding1 = px2int(sonNode->style.padding[1]);
			int padding3 = px2int(sonNode->style.padding[3]);
			int sonwidth = px2int(sonNode->style.width);
			int temp = margin1 + margin3 + border1 + border3 + padding1 + padding3 + sonwidth;
			width = width > temp ? width: temp;
		}
		sprintf((*ppNode)->style.width, "%dpx", width);
	}
}

static void offsetCalcuStyle(DOMTree **ppNode, int left, int top) {
	sprintf((*ppNode)->style.offsetLeft, "%dpx",left);
	sprintf((*ppNode)->style.offsetTop, "%dpx",top);
	int parentLeft = left + px2int((*ppNode)->style.margin[3]) 
					+ px2int((*ppNode)->style.border[3]) 
					+ px2int((*ppNode)->style.padding[3]); 
	int parentTop = top + px2int((*ppNode)->style.margin[0]) 
					+ px2int((*ppNode)->style.border[0]) 
					+ px2int((*ppNode)->style.padding[0]); 
	for (int i = 0; i < (*ppNode)->sonNum; ++i) {
		offsetCalcuStyle(&((*ppNode)->sonNodes[i]), parentLeft, parentTop);

		parentTop = parentTop + px2int((*ppNode)->sonNodes[i]->style.margin[0])  
					+ px2int((*ppNode)->sonNodes[i]->style.border[0])
					+ px2int((*ppNode)->sonNodes[i]->style.padding[0])
					+ px2int((*ppNode)->sonNodes[i]->style.margin[2])  
					+ px2int((*ppNode)->sonNodes[i]->style.border[2])
					+ px2int((*ppNode)->sonNodes[i]->style.padding[2])
					+ px2int((*ppNode)->sonNodes[i]->style.height);
	}
}
RenderNode *generateRenderTree(char *html, char *css, char *fileName) {
	RenderNode *head = initANewRenderNode();
	head->domNode = generateDOMTree(html);
	if (head->domNode == NULL) {
		return NULL;
	}
	// 可能没有css文件
	if (strlen(css) != 0) {
		if (strlen(css) > MAXHTMLLEN) {
			return NULL;
		}
		head->css = handleCss(css); 
		// printCSS(head->css);
		// 获得第一个body节点
		struct DOMNode *bodyNode = head->domNode->sonNodes[0];
		addCSSStyle2DOM(head->css, &bodyNode);
		// TODO 计算 

		calculateStyle(&bodyNode);
		secondeCalcuStyle(&bodyNode);
		calculateStyle(&bodyNode);
		offsetCalcuStyle(&bodyNode, 0, 0);
	}
	return head;
}

static void drawANode(DOMTree *domNode, CairoHandle *pCH) {
	if (domNode->tag != TEXT_TAG)
		drawBorder(pCH, domNode->style);
	else
		drawText(pCH, domNode->text, domNode->style);
	for (int i = 0; i < domNode->sonNum; ++i) {
		drawANode(domNode->sonNodes[i], pCH);
	}
}
void drawPNG(pRenderNode head, const char *fileName) {
	CairoHandle *pCH = initDrawContext();  
    drawANode(head->domNode->sonNodes[0], pCH);
    writeDrawFile(pCH, fileName);
    printf("draw a png\n");

	freeDraw(pCH);
}
