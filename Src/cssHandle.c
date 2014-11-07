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

static node* getNode(const char* buffer, int *pos);
static ruleList* getRule(const char* buffer, int *pos);
static int getErrorPos(char value[], int pos);
static cssNode* initCssNode();
static cssNode *ruleParser(nodeList* nodes, ruleList* rules);
static int handleRule(cssNode* c, rule* r, int i);
static int getPriority(node* n);
cssList* handleCss(const char* buffer);
int freeRuleList(ruleList *rules);
int freeNodeList(nodeList* nodes);
void freeCssList(cssList* csss);
/****************************************************
 * �ڲ�����getNode
 * 		����ѡ�����е���Ŀ��
 * ���������
 *		const char *buffer, int *pos
 * ����ֵ��
 *		node* newNode;
 * ע�⣺ ���ص�newNode��Ҫ�ڲ��õ�ʱ���ͷ��ڴ�
 */
static node* getNode(const char* buffer, int *pos) {
    node* newNode = (node*)malloc(sizeof(node));
    int namePos = 0;
    char c = *(buffer + *pos);
    (*pos)++;
    if(c == '#')
        newNode->type = idNode;
    else if(c == '.')
        newNode->type = classNode;
    else if( isalnum(c)){
        newNode->type = elementNode;
        newNode->name[namePos++] = c;
    }
    while(isalnum(c = (*(buffer + *pos))))
    {
        newNode->name[namePos++] = c;
        (*pos)++;
    }
    newNode->name[namePos] = '\0';
    (*pos)--;
    newNode->next = NULL;
    return newNode;
}
/****************************************************
 * �ڲ�����selector
 * 		����ѡ����
 * ���������
 *		const char *buffer, int *pos
 * ����ֵ��
 *		nodeList* newList;
   ����ѡ����ѡ��Ľڵ���
 * ע�⣺ ���ص�newList��Ҫ�ڲ��õ�ʱ���ͷ��ڴ�
 */
//����ѡ����
static nodeList* selector(const char* buffer, int *pos)
{
    char c;
    nodeList* newList = (nodeList*)malloc(sizeof(nodeList));
    newList->priority = 0;
    node* head = (node*)malloc(sizeof(node));
    node* currentNode;
    currentNode = head;
    newList->head = head;
    while((c = *(buffer + *pos))!='{')
    {
        if(c == '#'||c=='.')
        {
            node* newNode;
            if(*pos > 0)
            {
                if(isalnum(*(buffer + *pos - 1)))
                {
                    newList ->type = typeAnd;
                }
            }
            newNode = getNode(buffer, pos);
            currentNode->next = newNode;
            currentNode = newNode;
        }
        else if(isalnum(c))
        {
            node* newNode;
            newNode = getNode(buffer,pos);
            currentNode->next = newNode;
            currentNode = newNode;
        }
        else if(isspace(c))
        {
            if(newList->type != typeMulti && newList->type != typeParent)
            {
                newList->type = typeInclude;
            }
        }
        else if(c == '>')
        {
            newList->type = typeParent;
        }
        else if(c == ',')
        {
            newList->type = typeMulti;
        }
        (*pos)++;
    }
    if(head->next != NULL)
    {
        if(head->next->next == NULL)
            newList->type = typeSingle;
    }
    node* t = newList->head->next;
    while(t)
    {
        printf("%s type:%i\n",t->name,t->type);
        newList->priority += getPriority(t);
        printf("priotrity:%d\n",newList->priority);
        t = t->next;
    }
    printf("%i\n",newList->type);
    return newList;
}
/****************************************************
 * �ڲ�����getRule
 * 		����һ��{}�ڵ����й���
 * ���������
 *		const char *buffer, int *pos
 * ����ֵ��
 *		ruleList* rules;
 * ע�⣺ ���ص�rules��Ҫ�ڲ��õ�ʱ���ͷ��ڴ�
 */
static ruleList* getRule(const char* buffer, int *pos)
{
    char c;
    ruleList* rules = (ruleList*)malloc(sizeof(ruleList));
    rule* head= (rule*)malloc(sizeof(rule));
    head->next = NULL;
    rule* currentRule = head;
    rules->head = head;
    (*pos)++;
    while((c = *(buffer + *pos))!='}')
    {
        while(isspace(c = *(buffer + *pos))) (*pos)++;
        rule* newRule = (rule*)malloc(sizeof(rule));
        newRule->valPos = 0;
        newRule->namePos = 0;
        //����������
        while(isalnum(c = *(buffer + *pos))||(c = *(buffer +*pos))=='-')
        {
            newRule->name[newRule->namePos++] = c;
            (*pos)++;
        }
         //�����հ׷�
        while(isspace(c = *(buffer + *pos)))
        {
            (*pos)++;
        }
        if((c = *(buffer + *pos)) == ':')
        {
            newRule->name[newRule->namePos++] = '\0';
            (*pos)++;
        }
        //�����հ׷�
        while(isspace(c = *(buffer + *pos)))
        {
            (*pos)++;
        }
        //��������ֵ
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
        //printf("%s\n",newRule->name);
        //printf("%s\n",newRule->value);
        while(isspace(c = *(buffer + *pos))) (*pos)++;
    }
    //rule* r = rules->head;
    //while(r->next != NULL)
    //{
        //r = r->next;
        //printf("%s\n",r->name);
        //printf("%s\n",r->value);
    //}
    return rules;
}
/****************************************************
 * �ڲ�����getErrorPos
 * 		ȷ��ȱ�ٵ�;��λ��
 * ���������
 *		const char *buffer, int *pos
 * ����ֵ��
 *		0��Ѱ��ʧ��
        i: ;Ӧ�ô��ڵ�λ��
 */
static int getErrorPos(char value[], int pos)
{
    for(int i = 0; i < pos; i++)
    {
        for(int j = 0; j < 18; j++)
        {
            if(strcmp((value + i), attributes[j]) == 0)
                return i;
        }
    }
    return 0;
}
/****************************************************
 * �ڲ�����initCssNode
 * 		��ʼ��һ���µ�CssNode
 * ���������
 *		��
 * ����ֵ��
 *		cssNode newCssNode
 */
static cssNode* initCssNode()
{
     cssNode* newCssNode = (cssNode*)malloc(sizeof(cssNode));
     newCssNode->type = 0;
     newCssNode->next = NULL;
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
     return newCssNode;
}
/****************************************************
 * �ڲ�����ruleParser
 * 		����nodes��rules,�޸�cssNode�ڵ�
 * ���������
 *		nodeList* nodes, ruleList* rules;
 * ����ֵ��
 *		cssNode* newCssNode
 * ע�⣺ ���ص�newCssNode��Ҫ�ڲ��õ�ʱ���ͷ��ڴ�
          �˺������rules�нڵ���ͷ�
 */
static cssNode *ruleParser(nodeList* nodes, ruleList* rules)
{
    cssNode* newCssNode = initCssNode();
    rule* currentRule;
    if(rules->head->next != NULL) currentRule = rules->head->next;
    while(currentRule != NULL)
    {
        //�������հ׷�
        int valueLen = strlen(currentRule->value);
        for(int i = valueLen - 1; i >= 0; i--)
        {
            if(isspace(currentRule->value[i]))
                currentRule->value[i] = '\0';
            else
                break;
        }
        for(int i = 0; i < 18; i++)
        {
            if(strcmp(currentRule->name, attributes[i]) == 0)
            {
                handleRule(newCssNode, currentRule, i);
                break;
            }
        }
        currentRule = currentRule->next;
    }
    node* currentNode;
    if(nodes->head->next != NULL)
    {
        currentNode = nodes->head->next;
        if(currentNode->next == NULL)
        {
            newCssNode->type = typeSingle;
        }
        newCssNode->nodes = nodes;
    }
    freeRuleList(rules);
    return newCssNode;
}
/****************************************************
 * handleCss
 * 		��һ�������ڵ�Css��Ϣ���д���������洢��CssList��
        ͨ��CssList��ȡcss�ļ���Ϣ
 * �������:
 *		const char *buffer
 * ����ֵ��
 *		cssList* newCssList
 * ע�⣺ ���ص�rules��Ҫ�ڲ��õ�ʱ���ͷ��ڴ�
 */
cssList* handleCss(const char* buffer)
{
    int pos = 0;
    cssList *newCssList = initCssNode();
    cssNode* head = initCssNode();
    newCssList->next = head;
    cssNode* currentNode;
    currentNode = head;
    while(buffer[pos]!='\0') {
        nodeList* nl = selector(buffer, &pos);
        ruleList* rl = getRule(buffer, &pos);
        cssNode *cssNewNode = ruleParser(nl, rl);
        currentNode->next = cssNewNode;
        currentNode = cssNewNode;
        pos++;
        while(isspace(buffer[pos])) pos++;
    }
    return newCssList;
}
/****************************************************
 * �ڲ�����handleRule
 * 		��ʶ��Ĺ�����д����洢��cssNode��
 * ���������
 *		cssNode* c, rule* r, int i
 * ����ֵ��
 *		0: �ɹ�
        1��ʧ��
 */
static int handleRule(cssNode* c, rule* r, int i)
{
    printf("attribute:%s value:%s\n",r->name,r->value);
    switch(i)
    {
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
                while(t<len)
                {
                    if(isspace(r->value[t]))
                    {
                        c->border[count][pos] = '\0';
                        count++;
                        pos = 0;
                    }
                    else
                    {
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
                while(t<len)
                {
                    if(isspace(r->value[t]))
                    {
                        c->margin[count][pos] = '\0';
                        count++;
                        pos = 0;
                    }
                    else
                    {
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
/****************************************************
 * freeRuleList
 * 		�ͷ�ruleList�еĽ���������
 * ���������
 *		ruleList *rules
 */
int freeRuleList(ruleList *rules)
{
    if(rules->head->next == NULL)
    {
        free(rules);
    }
    else
    {
        rule* currentRule = rules->head->next;
        rule* temp;
        while(currentRule)
        {
            temp = currentRule;
            currentRule = currentRule->next;
            free(temp);
        }
        free(rules);
    }
    return 0;
}
/****************************************************
 * freeNodeList
 * 		�ͷ�NodeList�еĽ��
 * ���������
 *		nodeList *nodes
 */
int freeNodeList(nodeList* nodes)
{
    if(nodes->head == NULL)
    {
        printf("err");
    }
    else if(nodes->head->next == NULL)
    {
        //free(nodes);
        printf("test");
    }
    else
    {
        node* currentNode = nodes->head->next;
        node* temp;
        while(currentNode)
        {
            temp = currentNode;
            currentNode = currentNode->next;
            free(temp);
        }
        //free(nodes);
    }
    return 0;
}
/****************************************************
 * freeRuleList
 * 		�ͷ�cssList�еĽ���������
 * ���������
 *		cssList *csss
 */
void freeCssList(cssList* csss) {
    if(csss->next == NULL) {
        printf("NULL");
        free(csss);
    }
    else {
        cssNode* currentNode = csss->next;
        cssNode* temp;
        while(currentNode != NULL) {
            temp = currentNode;
            currentNode = currentNode->next;
            freeNodeList(temp->nodes);
            free(temp);
        }
        free(csss);
    }
}
/****************************************************
 * �ڲ�����getPriority
 * 		��ȡ�����ڵ�����ȼ�Ȩ��
 * ���������
 *		node* n
 * ����ֵ��
 *		���ȼ� int p
 */
static int getPriority(node* n)
{
    int p;
    if(n->type == classNode) p = 10;
    else if(n->type == idNode) p = 100;
    else if(n->type == elementNode) p = 1;
    return p;
}
