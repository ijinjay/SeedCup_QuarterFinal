#include "cssHandle.h"

typedef struct rule
{
    char name[15];
    int namePos;
    char value[30];
    int valPos;
    struct rule* next;
}rule;
typedef struct ruleList {
    rule* head;             
    int type;
}ruleList;
static char attributes[18][15]= {
    "display",      //1
    "position",     //2
    "width",        //3
    "height",       //4
    "top",          //5
    "bottom",       //6
    "left",         //7
    "right",        //8
    "padding",      //9
    "border",       //10
    "margin",       //11
    "color",        //12
    "font-size",    //13
    "line-height",  //14
    "text-align",   //15
    "font-style",   //16
    "font-weight",  //17
    "line-break",   //18
};

static int getErrorPos(char value[], int pos);
static int handleRule(cssNode* c, rule* r, int i);
static int freeRuleList(ruleList *rules);

DOMCSSES *initADomCSS(void) {
    DOMCSSES *newNode = (pDOMCSSES)malloc(sizeof(DOMCSSES));
    for (int i = 0; i < 100; ++i) {
        newNode->cssStyle[i] = NULL;
        newNode->priorities[i] = 0;
    }
    newNode->cssStyleNum = 0;
    return newNode;
}
static pSelectNode initASelectNode(void) {
    selectNode *newNode = (pSelectNode)malloc(sizeof(selectNode));
    newNode->type = 0;
    newNode->nodeNum = 0;
    return newNode;
}

// 单个或多个选择器
static selectNode *selector(const char* buffer, int *pos) {
    selectNode (*newSelect) = initASelectNode();
    char c;
    int position = *(pos);
    c = buffer[position];
    char oneName[30];
    int namePos = 0;
    if (c == ' ')
        c = buffer[++ position];
    while( c !='{' ) {
        if ( (isalnum(c)) || (c == '#') || c == '.' ) {
            oneName[namePos ++] = c;
        }
        else if (c == ' ') {
            if (   (buffer[position-1] != ',' && buffer[position-1] != '}')
                && (buffer[position+1] != ',' && buffer[position+1] != '{')
                && (buffer[position-1] != '>')
                && (buffer[position+1] != '>') )
                oneName[namePos ++] = c;
        }
        else if (c == ',') {
            oneName[namePos] = '\0';
            strcpy((*newSelect).name[(*newSelect).nodeNum ++], oneName);
            namePos = 0;
            newSelect->type = typeMulti;
        }
        else if (c == '\0') {
            return NULL;
        }
        else {
            oneName[namePos ++] = c;
        }
        c = buffer[++ position];
    }
    oneName[namePos] = '\0';
    strcpy((*newSelect).name[(*newSelect).nodeNum ++], oneName);
    namePos = 0;
    (*pos) = position + 1;

    return newSelect;
}
static ruleList* getRule(const char* buffer, int *pos)
{
    char c;
    ruleList* rules = (ruleList*)malloc(sizeof(ruleList));
    rule* head= (rule*)malloc(sizeof(rule));
    head->next = NULL;
    rule* currentRule = head;
    rules->head = head;
    (*pos)++;
    while((c = *(buffer + *pos))!='}') {
        while(isspace(c = *(buffer + *pos))) (*pos)++;
        rule* newRule = (rule*)malloc(sizeof(rule));
        newRule->valPos = 0;
        newRule->namePos = 0;
        while(isalnum(c = *(buffer + *pos))||(c = *(buffer +*pos))=='-') {
            newRule->name[newRule->namePos++] = c;
            (*pos)++;
        }
        while(isspace(c = *(buffer + *pos))) {
            (*pos)++;
        }
        if((c = *(buffer + *pos)) == ':') {
            newRule->name[newRule->namePos++] = '\0';
            (*pos)++;
        }
        while(isspace(c = *(buffer + *pos))) {
            (*pos)++;
        }
        while(isalnum(c = *(buffer + *pos))||(c = *(buffer +*pos))=='%'||(c = *(buffer +*pos))=='#'||(c = *(buffer +*pos))=='-'||isspace((c = *(buffer +*pos)))) {
            newRule->value[newRule->valPos++] = c;
            (*pos)++;
        }
        if((c = *(buffer + (*pos))) == ':') {
            newRule->value[newRule->valPos] = '\0';
            int errorPos = getErrorPos(newRule->value, newRule->valPos);
            newRule->value[errorPos] = '\0';
            (*pos) = (*pos) - newRule->valPos + errorPos;
            newRule->valPos = errorPos + 1;
        }
        else if((c = *(buffer + (*pos))) == '}') {
            newRule->value[newRule->valPos++] = '\0';
        }
        else if((c = *(buffer + (*pos))) == ';') {
            newRule->value[newRule->valPos++] = '\0';
            (*pos)++;
        }
        else {
            printf("%d",*pos);
            printf("error occur\n");
        }
        newRule->next = NULL;
        currentRule->next = newRule;
        currentRule = newRule;
        while(isspace(c = *(buffer + *pos))) (*pos)++;
    }
    return rules;
}
static int getErrorPos(char value[], int pos) {
    for(int i = 0; i < pos; i++) {
        for(int j = 0; j < 18; j++) {
            if(strcmp((value + i), attributes[j]) == 0)
                return i;
        }
    }
    return 0;
}
static cssNode* initCssNode() {
     cssNode* newCssNode = (cssNode*)malloc(sizeof(cssNode));
     newCssNode->next = NULL;
     newCssNode->snodes = NULL;
     strcpy(newCssNode->display,"inline");
     strcpy(newCssNode->position,"static");
     strcpy(newCssNode->width,"auto");
     strcpy(newCssNode->height,"auto");
     strcpy(newCssNode->top,"auto");
     strcpy(newCssNode->bottom,"auto");
     strcpy(newCssNode->left,"auto");
     strcpy(newCssNode->right,"auto");
     strcpy(newCssNode->padding[0],"0");
     strcpy(newCssNode->padding[1],"0");
     strcpy(newCssNode->padding[2],"0");
     strcpy(newCssNode->padding[3],"0");
     strcpy(newCssNode->border[0],"1px");
     strcpy(newCssNode->border[1],"1px");
     strcpy(newCssNode->border[2],"1px");
     strcpy(newCssNode->border[3],"1px");
     strcpy(newCssNode->margin[0],"0");
     strcpy(newCssNode->margin[1],"0");
     strcpy(newCssNode->margin[2],"0");
     strcpy(newCssNode->margin[3],"0");
     strcpy(newCssNode->color, "#000");
     strcpy(newCssNode->fontSize,"16px");
     strcpy(newCssNode->lineHeight,"1.2");
     strcpy(newCssNode->textAlign,"left");
     strcpy(newCssNode->fontStyle,"normal");
     strcpy(newCssNode->fontWeight,"normal");
     strcpy(newCssNode->lineBreak,"normal");
     newCssNode->defineFlag &= 0x00000;
     return newCssNode;
}
static cssNode *ruleParser(selectNode *snode, ruleList* rules) {
    cssNode* newCssNode = initCssNode();
    rule* currentRule;
    if(rules->head->next != NULL) currentRule = rules->head->next;
    while(currentRule != NULL) {
        int valueLen = strlen(currentRule->value);
        for(int i = valueLen - 1; i >= 0; i--) {
            if(isspace(currentRule->value[i]))
                currentRule->value[i] = '\0';
            else
                break;
        }
        for(int i = 0; i < 18; i++) {
            if(strcmp(currentRule->name, attributes[i]) == 0) {
                handleRule(newCssNode, currentRule, i);
                break;
            }
        }
        currentRule = currentRule->next;
    }
    freeRuleList(rules);
    newCssNode->snodes = snode;
    return newCssNode;
}
// pHTMLl中的多余空格删去保留一个空格
static void preparse(char **pHTML) {
    int len = strlen(*pHTML);
    int space = 0;
    char *preHTML = (char *)malloc( 10000 * sizeof(char));
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
    // 关闭字符串
    preHTML[j] = '\0';
    free((*pHTML));
    (*pHTML) = preHTML;
}
cssList* handleCss(char* buffer)
{
    preparse(&buffer);    
    int pos = 0;
    cssList *newCssList = initCssNode();
    cssNode* currentNode;
    currentNode = newCssList;
    while(buffer[pos]!='\0') {
        selectNode* nl = selector(buffer, &pos);
        if (nl == NULL) {
            break;
        }
        ruleList* rl = getRule(buffer, &pos);
        cssNode *cssNewNode = ruleParser(nl, rl);
        currentNode->next = cssNewNode;
        currentNode = cssNewNode;
        pos++;
    }
    return newCssList;
}
static int handleRule(cssNode* c, rule* r, int i) {
    // printf("attribute:%s value:%s\n",r->name,r->value);
    c->defineFlag |= (0x00001 << i);
    switch(i) {
        case 0: strcpy(c->display, r->value);break;
        case 1: strcpy(c->position, r->value);break;
        case 2: strcpy(c->width, r->value);break;
        case 3: strcpy(c->height, r->value);break;
        case 4: strcpy(c->top, r->value);break;
        case 5: strcpy(c->bottom, r->value);break;
        case 6: strcpy(c->left, r->value);break;
        case 7: strcpy(c->right, r->value);break;
        case 8:
            {
                int count = 0;
                int len = strlen(r->value);
                int t = 0;
                int pos = 0;
                while(t<len)
                {
                    if(isspace(r->value[t]))
                    {
                        c->padding[count][pos] = '\0';
                        count++;
                        pos = 0;
                    }
                    else
                    {
                        c->padding[count][pos] = r->value[t];
                    }
                    t++;
                }
            };break;
        case 9:
            {
                int count = 0;
                int len = strlen(r->value);
                int t = 0;
                int pos = 0;
                while(t<len) {
                    if(isspace(r->value[t])) {
                        c->border[count][pos] = '\0';
                        count++;
                        pos = 0;
                    }
                    else {
                        c->border[count][pos] = r->value[t];
                    }
                    t++;
                }
            };break;
        case 10:
             {
                int count = 0;
                int len = strlen(r->value);
                int t = 0;
                int pos = 0;
                while(t<len) {
                    if(isspace(r->value[t])) {
                        c->margin[count][pos] = '\0';
                        count++;
                        pos = 0;
                    }
                    else {
                        c->margin[count][pos] = r->value[t];
                    }
                    t++;
                }
            };break;
        case 11:strcpy(c->color,r->value);break;
        case 12:strcpy(c->fontSize,r->value);break;
        case 13:strcpy(c->lineHeight,r->value);break;
        case 14:strcpy(c->textAlign,r->value);break;
        case 15:strcpy(c->fontStyle,r->value);break;
        case 16:strcpy(c->fontWeight,r->value);break;
        case 17:strcpy(c->lineBreak,r->value);break;
        default: return 1;break;
    }
    return 0;
}
int freeRuleList(ruleList *rules) {
    if(rules->head->next == NULL) {
        free(rules);
    }
    else {
        rule* currentRule = rules->head->next;
        rule* temp;
        while(currentRule) {
            temp = currentRule;
            currentRule = currentRule->next;
            free(temp);
        }
        free(rules);
    }
    return 0;
}

void freeCssList(cssList* csss) {
    if(csss->next != NULL){
        freeCssList(csss->next);
    }
    if (csss->snodes != NULL) {
        free(csss->snodes);
    }
    free(csss);
}


int getDefineState(const char* att, cssNode* css){
    for(int i = 0; i < 18; i++) {
        if(strcmp(att,attributes[i])==0)   return (css->defineFlag &= (0x00001 << i));
    }
    return -1;
}
void printCSS(cssList *csss) {
    if (csss->next != NULL) {
        printCSS(csss->next);
    }
    if (csss->snodes != NULL) {
        for (int i = 0; i < csss->snodes->nodeNum; ++i) {
            printf("%s\t", csss->snodes->name[i]);
        }
        printf("\n");
    }
}
