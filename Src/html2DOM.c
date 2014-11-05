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
    newNode->classNum = 0;
    newNode->fatherNode = NULL;
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
    free(pHead);
}
// 打印DOMTree
void printDOMTree(DOMTree *pHead) {
    for (int i = 0; i < pHead->sonNum; ++i) {
        printDOMTree(pHead->sonNodes[i]);
    }
    printDOMNode(pHead);
}
static char *getTagName(int tag) {
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
void printDOMNode(pDOMNode pNode) {
    printf("%s{\n", getTagName(pNode->tag));
    printf("\toffsetLeft: %s;\n", pNode->style.offsetLeft);
    printf("\toffsetTop: %s;\n", pNode->style.offsetTop);
    printf("\tdisplay: %s;\n", pNode->style.display);
    printf("\tposition: %s;\n", pNode->style.position);
    printf("\twidth: %s;\n", pNode->style.width);
    printf("\theight: %s;\n", pNode->style.height);
    printf("\tpadding: %s %s %s %s;\n",  pNode->style.padding[0],
                                        pNode->style.padding[1],
                                        pNode->style.padding[2],
                                        pNode->style.padding[3]);
    printf("\tborder: %s %s %s %s;\n",   pNode->style.border[0],
                                        pNode->style.border[1],
                                        pNode->style.border[2],
                                        pNode->style.border[3]);
    printf("\tmargin: %s %s %s %s;\n",   pNode->style.margin[0],
                                        pNode->style.margin[1],
                                        pNode->style.margin[2],
                                        pNode->style.margin[3]);
    printf("\tleft: %s;\n", pNode->style.left);
    printf("\tright: %s;\n", pNode->style.right);
    printf("\ttop: %s;\n", pNode->style.top);
    printf("\tbottom: %s;\n", pNode->style.bottom);
    printf("\tcolor: %s;\n", pNode->style.color);
    printf("\tline-height: %s;\n", pNode->style.line_height);
    printf("\tfont-size: %s;\n", pNode->style.font_size);
    printf("\tfont-style: %s;\n", pNode->style.font_style);
    printf("\tfont-weight: %s;\n", pNode->style.font_weight);
    printf("\ttext-align: %s;\n", pNode->style.text_align);
    printf("\tline-break: %s;\n", pNode->style.line_break);
    printf("}\n");
} 

// pHTMLl中的多余空格删去保留一个空格
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
    free((*pHTML));
    (*pHTML) = preHTML;
}

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
    }
    return r;
}

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
// 将含有属性的标签内容分解成tag,class,id
static pDOMNode str2tag(const char *str) {
    pDOMNode newNode = initANewDOMNode();
    int len = strlen(str);
    char tagname[10];
    int tagIndex = 0;
    for (int i = 0; i < len; ++i) {
        if (str[i] == ' ')
            break;
        else
            tagname[tagIndex ++] = tolower(str[i]);
    }
    tagname[tagIndex] = '\0';
    newNode->tag = tagname2tag(tagname);
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
    printf("parse tag %s end.\n", tagname);
    return newNode;
}
static int errorStack[100];
static int errorTop = 0;
static void pushError(int error) {
    errorStack[errorTop ++] = error;
}
static int popError(void) {
    return errorStack[-- errorTop];
}
static void changeAB(int *a, int *b) {
    int temp;
    temp = (*a);
    (*a) = (*b);
    (*b) = (*a);
}
DOMTree *generateDOMTree(const char *HTML) {
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
                    // printf("text is %s\n", str);
                    newNode = initANewDOMNode();
                    newNode->tag = TEXT_TAG;
                    parrent = topNode();
                    addNodeToParent(&parrent, &newNode);
                    break;
                }
        }
    }

    printf("errorTop is :%d\n", errorTop);
    return tree;
}