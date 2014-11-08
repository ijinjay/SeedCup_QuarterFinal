#include "html2DOM.h"

// 初始化style为默认参数
static void initANewStyle(st_style *style) {
    strcpy((*style).offsetLeft,    "0");
    strcpy((*style).offsetTop,     "0");
    strcpy((*style).display,       "inline");
    strcpy((*style).position,      "static");
    strcpy((*style).width,         "auto");
    strcpy((*style).height,        "auto");
    strcpy((*style).padding[0],    "0");
    strcpy((*style).padding[1],    "0");
    strcpy((*style).padding[2],    "0");
    strcpy((*style).padding[3],    "0");
    strcpy((*style).border[0],     "1px");
    strcpy((*style).border[1],     "1px");
    strcpy((*style).border[2],     "1px");
    strcpy((*style).border[3],     "1px");
    strcpy((*style).margin[0],     "0");
    strcpy((*style).margin[1],     "0");
    strcpy((*style).margin[2],     "0");
    strcpy((*style).margin[3],     "0");
    strcpy((*style).left,          "auto");
    strcpy((*style).right,         "auto");
    strcpy((*style).top,           "auto");
    strcpy((*style).bottom,        "auto");
    strcpy((*style).color,         "#000");
    strcpy((*style).line_height,   "1.2");
    strcpy((*style).font_size,     "16px");
    strcpy((*style).font_style,    "normal");
    strcpy((*style).font_weight,   "normal");
    strcpy((*style).text_align,    "left");
    strcpy((*style).line_break,    "normal");
}
// 新建一个DOM节点
static pDOMNode initANewDOMNode(void) {
    pDOMNode newNode = (pDOMNode)malloc(sizeof(DOMTree));
    newNode->tag = NO_TAG;
    initANewStyle(&(newNode->style));
    newNode->sonNum = 0;
    for (int i = 0; i < MAXSONNUM; ++i) {
        newNode->sonNodes[i] = NULL;
        strcpy(newNode->classes[i], "");
    }
    newNode->ID[0] = '\0';
    newNode->classNum = 0;
    newNode->fatherNode = NULL;
    newNode->text = NULL;
    newNode->csses = NULL;
    return newNode;
}
// 将一个节点添加为另一个节点的儿子节点
static void addNodeToParent(pDOMNode *parrent, pDOMNode *son) {
    (*parrent)->sonNodes[(*parrent)->sonNum ++] = (*son);
    (*son)->fatherNode = (*parrent);
}
// 释放内存
void freeDOMTree(DOMTree *pHead) {
    for (int i = 0; i < pHead->sonNum; ++i) {
        freeDOMTree(pHead->sonNodes[i]);
    }
    // 将css样式的信息释放掉
    if (pHead->csses != NULL)
        free(pHead->csses);
    // 纯文本需要额外释放文本占用的内存
    if (pHead->tag == TEXT_TAG)
        free(pHead->text);
    free(pHead);
}
// 打印DOMTree
void printDOMTree(DOMTree *pHead) {
    for (int i = 0; i < pHead->sonNum; ++i) {
        printDOMTree(pHead->sonNodes[i]);
    }
    if (pHead->tag != TEXT_TAG && pHead->tag != DOCUMENT_TAG) {
        printf("**************************************\n");
        for (int i = 0; i < pHead->csses->cssStyleNum; ++i) {
            printf("%s's prio is %d\n", getTagName(pHead->tag), pHead->csses->priorities[i]);
        }
    }
    // printDOMNode(pHead);
}
char *getTagName(int tag) {
    switch(tag) {
        case HEAD_TAG:  return "head";
        case BODY_TAG:  return "body";
        case SPAN_TAG:  return "span";
        case DIV_TAG:   return "div";
        case P_TAG:     return "p";
        case STRONG_TAG:return "strong";
        case H_TAG:     return "h";
        case EM_TAG:    return "em";
        case LINK_TAG:  return "link";
        case TEXT_TAG:  return "text";
        case DOCUMENT_TAG: return "document";
        default: break;
    }
    return "unknown";
}
// 打印节点style内容
void printDOMNode(pDOMNode pNode, FILE *f) {
    fprintf(f, "\toffsetLeft: %s;\n", pNode->style.offsetLeft);
    fprintf(f, "\toffsetTop: %s;\n", pNode->style.offsetTop);
    fprintf(f, "\tdisplay: %s;\n", pNode->style.display);
    fprintf(f, "\tposition: %s;\n", pNode->style.position);
    fprintf(f, "\twidth: %s;\n", pNode->style.width);
    fprintf(f, "\theight: %s;\n", pNode->style.height);
    fprintf(f, "\tpadding: %s %s %s %s;\n",  pNode->style.padding[0],
                                        pNode->style.padding[1],
                                        pNode->style.padding[2],
                                        pNode->style.padding[3]);
    fprintf(f, "\tborder: %s %s %s %s;\n",   pNode->style.border[0],
                                        pNode->style.border[1],
                                        pNode->style.border[2],
                                        pNode->style.border[3]);
    fprintf(f, "\tmargin: %s %s %s %s;\n",   pNode->style.margin[0],
                                        pNode->style.margin[1],
                                        pNode->style.margin[2],
                                        pNode->style.margin[3]);
    fprintf(f, "\tleft: %s;\n", pNode->style.left);
    fprintf(f, "\tright: %s;\n", pNode->style.right);
    fprintf(f, "\ttop: %s;\n", pNode->style.top);
    fprintf(f, "\tbottom: %s;\n", pNode->style.bottom);
    fprintf(f, "\tcolor: %s;\n", pNode->style.color);
    fprintf(f, "\tline-height: %s;\n", pNode->style.line_height);
    fprintf(f, "\tfont-size: %s;\n", pNode->style.font_size);
    fprintf(f, "\tfont-style: %s;\n", pNode->style.font_style);
    fprintf(f, "\tfont-weight: %s;\n", pNode->style.font_weight);
    fprintf(f, "\ttext-align: %s;\n", pNode->style.text_align);
    fprintf(f, "\tline-break: %s;\n", pNode->style.line_break);
    fprintf(f, "}");
} 

void print2File(DOMTree *pNode, FILE *f, char **pstr) {
    // 第一个是document节点，可以直接遍历子节点
    char finalStr[100];
    finalStr[0] = '\0';
    int sonRank = 0;
    if (strlen(*pstr) != 0){
        sprintf(finalStr, "%s", *pstr);
        printf("%s\n", finalStr);
    }
    for (int i = 0; i < pNode->sonNum; ++i) {
        // 跳过文本节点和display等于none的节点
        char tempStr[100];
        if (strlen(finalStr) != 0)
            strcpy(tempStr, finalStr);
        if (    (pNode->sonNodes[i]->tag != TEXT_TAG) 
            &&  (strcmp(pNode->sonNodes[i]->style.display, "none") != 0) ) {
            if (pNode->sonNodes[i]->tag != BODY_TAG) {
                // printf("enter son node --- %s\n", tempStr);
                fprintf(f, "\n\n");
                sprintf(tempStr, "%s>%s", tempStr,getTagName(pNode->sonNodes[i]->tag));
                // 打印出节点
                if (strlen(pNode->sonNodes[i]->ID) != 0)
                    sprintf(tempStr, "%s#%s", tempStr,pNode->sonNodes[i]->ID);
                for (int j = 0; j < pNode->sonNodes[i]->classNum; ++j) {
                    sprintf(tempStr, "%s.%s", tempStr,pNode->sonNodes[i]->classes[j]);
                }
                sprintf(tempStr, "%s[%d]", tempStr, sonRank ++);
                sprintf(*pstr, "%s", tempStr);
                fprintf(f, "%s{\n", tempStr);
            }
            else {
                printf("enter body node\n");
                sprintf(tempStr, "body");
                if (strlen(pNode->sonNodes[i]->ID) != 0) {
                    sprintf(tempStr, "%s#%s", tempStr, pNode->sonNodes[i]->ID);
                }
                for (int j = 0; j < pNode->sonNum; ++j)
                    sprintf(tempStr, "%s.%s", tempStr,pNode->sonNodes[i]->classes[j]);
                fprintf(f, "%s{\n", tempStr);
            }
            printDOMNode(pNode, f);
            if (strlen(tempStr) != 0)
                sprintf(*pstr, "%s",tempStr);
        }
        print2File(pNode->sonNodes[i], f, pstr);
    }
}
// HTML中的多余空格删去保留一个空格
static void preparse(char **pHTML) {
    int len = strlen(*pHTML);
    int space = 0;
    char *preHTML = (char *)malloc(MAXHTMLLEN * sizeof(char));
    int j = 0;
    for (int i = 0; i < len; ++i) {
        if (isspace((*pHTML)[i]))
            space ++;
        else {
            if (space != 0) {
                space = 0;
                preHTML[j ++] = ' ';
            }
            preHTML[j ++] = (*pHTML)[i];
        }
    }
    preHTML[j] = '\0';
    // 修正
    len = strlen(preHTML);
    j = 0;
    int enter = 0;
    for (int i = 0; i < len; ++i) {
        if (preHTML[i] == '<') {
            enter = 1;
        }
        if (enter && preHTML[i] == '>') {
            enter = 0;
        }
        if (enter && preHTML[i] == ' ') 
            if (preHTML[i-1] == '=' || preHTML[i+1] == '=') 
                continue;
        (*pHTML)[j++] = preHTML[i];
    }
    (*pHTML)[j] = '\0';
    // 关闭字符串
    free(preHTML);
}
// 多次正则匹配，将多个匹配结果存入pMultiResult
static pMultiResult regexMul(const char *pattern, const char *str, int left, int right) {
    pMultiResult r = (pMultiResult)malloc(sizeof(MultiResult));
    r->num = 0;
    regex_t reg;
    regmatch_t pmatch[1];
    // 模式串编译
    regcomp(&reg, pattern, REG_EXTENDED);
    // 匹配
    int shift = 0;
    int result;
    while((result = regexec(&reg, str + shift, 1, pmatch, 0)) == 0) {
        strncpy(r->results[r->num ++] , 
                str + shift + pmatch[0].rm_so + left,
                pmatch[0].rm_eo - pmatch[0].rm_so - (left + right));
        shift += pmatch[0].rm_eo;
        r->results[r->num -1][pmatch[0].rm_eo - pmatch[0].rm_so - (left + right)] = '\0';
    }
    return r;
}
// 解析head标签中的link标签，返回css文件的路径
CSSURL *parseCSSURL(char **pHTML) {
    preparse(pHTML);
    pCSSURL cssurl = (pCSSURL)malloc(sizeof(CSSURL));
    cssurl->urlNum = 0;
    // int start, end;
    regex_t reg;
    regmatch_t pmatch[1];
    char pattern[] = "<link href=\"[^\"]+\">";
    // 模式串编译
    regcomp(&reg, pattern, REG_EXTENDED);
    // 匹配
    int shift = 0;
    int result;
    while((result = regexec(&reg, (*pHTML) + shift, 1, pmatch, 0)) == 0) {
        strncpy(cssurl->urls[cssurl->urlNum ++], 
                (*pHTML) + shift + pmatch[0].rm_so+12,
                pmatch[0].rm_eo-pmatch[0].rm_so-14);
        (cssurl->urls[cssurl->urlNum-1])[pmatch[0].rm_eo-pmatch[0].rm_so-14] = '\0';
        // printf("css name is %s\n", cssurl->urls[cssurl->urlNum - 1]);
        shift += pmatch[0].rm_eo;
    }
    regcomp(&reg, "<head>.*</head>", REG_EXTENDED);
    if((result = regexec(&reg, (*pHTML), 1, pmatch, 0)) ==0 ) {
        int start = pmatch[0].rm_so;
        int end   = pmatch[0].rm_eo;
        int len   = strlen((*pHTML));
        for (int i = start; i < len - start; ++i) {
            (*pHTML)[i] = (*pHTML)[end+i];
        }
        (*pHTML)[len-start] = '\0';
    }

    return cssurl;
}
// 处理父子节点用的节点堆栈
static pDOMNode stack[100];
static int stackTop = 0;
static void pushNode(pDOMNode node) {
    stack[stackTop ++] = node;
}
static pDOMNode popNode(void) {
    return stack[-- stackTop];
}
static pDOMNode topNode() {
    return stack[stackTop - 1];
}
static void initStack(void) {
    while(stackTop > 0) {
        popNode();
    }
}
// 根据标签名得到标签的枚举值
static int tagname2tag(char *tag) {
    if (!strcmp(tag, "body"))
        return BODY_TAG;
    if (!strcmp(tag, "span"))
        return SPAN_TAG;
    if (!strcmp(tag, "div"))
        return DIV_TAG;
    if (!strcmp(tag, "p"))
        return P_TAG;
    if (!strcmp(tag, "strong"))
        return STRONG_TAG;
    if (!strcmp(tag, "h"))
        return H_TAG;
    if (!strcmp(tag, "em"))
        return EM_TAG;
    if (!strcmp(tag, "link"))
        return LINK_TAG;
    return NO_TAG;
}
// 更改元素默认属性
static void modifyElementStyle(pDOMNode *ppNode) {
    switch((*ppNode)->tag) {
        case HEAD_TAG:
        case LINK_TAG:
            strcpy(((*ppNode)->style).display, "none");break;
        case STRONG_TAG:
            strcpy(((*ppNode)->style).font_weight, "bold");
        case SPAN_TAG:
            strcpy(((*ppNode)->style).display, "inline");break;
        case EM_TAG:
            strcpy(((*ppNode)->style).font_weight, "italic");
            strcpy(((*ppNode)->style).display, "inline");break;
        case BODY_TAG: 
            strcpy(((*ppNode)->style).margin[0], "8px");
            strcpy(((*ppNode)->style).margin[1], "8px");
            strcpy(((*ppNode)->style).margin[2], "8px");
            strcpy(((*ppNode)->style).margin[3], "8px");
            strcpy(((*ppNode)->style).font_size, "16px");
            strcpy(((*ppNode)->style).line_height, "1.2");
        case DIV_TAG:
            strcpy(((*ppNode)->style).display, "block");break;
        case P_TAG:
            strcpy(((*ppNode)->style).margin[0], "1em");
            strcpy(((*ppNode)->style).margin[1], "0");
            strcpy(((*ppNode)->style).margin[2], "1em");
            strcpy(((*ppNode)->style).margin[3], "0");
            strcpy(((*ppNode)->style).display, "block");break;
        case H_TAG:
            strcpy(((*ppNode)->style).font_size, "32px");
            strcpy(((*ppNode)->style).font_weight, "bold");
            strcpy(((*ppNode)->style).margin[0], "0.67em");
            strcpy(((*ppNode)->style).margin[1], "0");
            strcpy(((*ppNode)->style).margin[2], "0.67em");
            strcpy(((*ppNode)->style).margin[3], "0");
            strcpy(((*ppNode)->style).display, "block");break;
        default:
            break;
    }
}
// 将含有属性的标签内容分解成tag,class,id
static pDOMNode str2tag(const char *str) {
    // 初始化一个节点
    pDOMNode newNode = initANewDOMNode();
    int len = strlen(str);
    char tagname[10];
    int tagIndex = 0;
    // 将tag名提取出来
    for (int i = 0; i < len; ++i) {
        if (str[i] == ' ')
            break;
        else
            tagname[tagIndex ++] = tolower(str[i]);
    }
    tagname[tagIndex] = '\0';
    newNode->tag = tagname2tag(tagname);
    // 根据tag类型更改节点默认属性
    modifyElementStyle(&newNode);

    // 解析tag的class和id属性
    MultiResult *c  = regexMul("class=\"[^\"]*\"", str, 7, 1);
    MultiResult *id = regexMul("id=\"[^\"]*\"", str, 4,1);
    // 将class和id属性写入节点
    for (int i = 0; i < c->num; ++i) {
        int classlen = strlen(c->results[i]);
        char tempclass[20];
        int tempLen = 0;
        for (int j = 0; j < classlen; ++j) {
            if ((c->results[i])[j] != ' ') {
                tempclass[tempLen ++] = (c->results[i])[j];
            }
            else {
                tempclass[tempLen] = '\0';
                strcpy(newNode->classes[newNode->classNum ++], tempclass);
                tempLen = 0;
            }
        }
        if (tempLen != 0) {
            tempclass[tempLen] = '\0';
            strcpy(newNode->classes[newNode->classNum ++], tempclass);
        }
    }
    for (int i = 0; i < id->num; ++i) {
        strcpy(newNode->ID, id->results[i]);
    }
    free(c);
    free(id);
    return newNode;
}
// 处理html标签位置错位的堆栈
static int errorStack[100];
static int errorTop = 0;
static void pushError(int error) {
    errorStack[errorTop ++] = error;
}
static int popError(void) {
    return errorStack[-- errorTop];
}
static void initErrorStack(void) {
    while(errorTop > 0) {
        popError();
    }
}
// 更改AB的值
static void changeAB(int *a, int *b) {
    int temp;
    temp = (*a);
    (*a) = (*b);
    (*b) = temp;
}
// 产生DOMTree
DOMTree *generateDOMTree(const char *HTML) {
    // important! 初始化static参数，每次调用时都必须初始化
    if (strlen(HTML) > MAXHTMLLEN) {
        printf("Cannot parse for the length of HTML is too long.\n");
        return NULL;
    }
    initStack();
    initErrorStack();
    // 树节点，节点类型为document
    DOMTree *tree = initANewDOMNode();
    tree->tag = DOCUMENT_TAG;
    pushNode(tree);
    // 解析开始
    int len = strlen(HTML);
    char str[1024];
    int strIndex = 0;
    pDOMNode newNode = NULL;
    pDOMNode parrent = NULL;
    for (int i = 0; i < len; ++i) {
        switch(HTML[i]) {
            case '<': 
                if (isalpha(HTML[i+1])) {
                    // 标签开启状态
                    strIndex = 0;
                    while(HTML[++ i] != '>' && i < len) {
                        str[strIndex ++] = HTML[i];
                    }
                    str[strIndex] = '\0';
                    // 构造新节点
                    newNode = str2tag(str);
                    // 将新节点与其父节点联系起来
                    parrent = topNode();
                    addNodeToParent(&parrent, &newNode);
                    // 当前节点进栈
                    pushNode(newNode);
                }
                else if (HTML[i+1] == '/') {
                    // 标签结束状态
                    strIndex = 0;
                    i++;
                    while(HTML[++i] != '>' && i < len) {
                        str[strIndex ++] = HTML[i];
                    }
                    str[strIndex] = '\0';
                    // 取出栈顶元素，比较后相同便出栈，该标签结束
                    parrent = topNode();
                    if (tagname2tag(str) == parrent->tag) {
                        popNode();
                        for (int j = errorTop - 1; j >= 0; j--) {
                            // 当前错误处理元素与节点的类型一致
                            if (errorStack[j] == topNode()->tag) {
                                popNode();
                                // 修正errorStack
                                if (j != errorTop - 1)
                                    changeAB(&(errorStack[j]), &(errorStack[errorTop - 1]));
                                popError();
                                j ++;
                            }
                        }
                    }
                    else { // 标签关闭错误处理
                        pushError(tagname2tag(str));
                    }
                }
                else
                    printf("this '<' is not a normal state!\n");
                break;
            // 空格就直接忽略，直到是<或者[a-zA-Z0-9]即文字的开头
            case ' ':
                break;
            default: 
                if (topNode()->tag == DOCUMENT_TAG)
                    break;
                else {
                    // 文本状态
                    strIndex = 0;
                    str[strIndex ++] = HTML[i];
                    while(HTML[++i] != '<' && i < len) {
                        str[strIndex ++] = HTML[i];
                    }
                    if (i != len)
                        i --;
                    str[strIndex] = '\0';
                    newNode = initANewDOMNode();
                    newNode->tag = TEXT_TAG;
                    newNode->text = (char *)malloc((strlen(str) + 1) * sizeof(char));
                    strcpy(newNode->text, str);
                    parrent = topNode();
                    addNodeToParent(&parrent, &newNode);
                }
                break;
        }
    }
    // 打印出没有配对成功的标签名
    for (int i = 0; i < errorTop; ++i) {
        printf("%s haven't been matched!\n", getTagName(errorStack[i]));
    }
    return tree;
}
