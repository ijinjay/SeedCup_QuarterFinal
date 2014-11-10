
## 程序设计及各模块的功能说明
### 整体结构
![1.png](1.png)
###文件读写模块
为了从所有文件夹中读写文件，我们首先通过系统调用获取所有应处理的目录名，然后对指定目录中的文件进行读写。
这里，我们将目录名全部写到一个文件中，然后依次处理目录中的文件；对于html文件，我们处理”index.html”，对于css文件，我们通过html来确定文件名，并且进行处理。
CSS解析模块
在CSS解析模块，我们将解析部分分为两个部分进行，分别是选择器部分和规则部分。对于选择器部分，我们将所有的选择器存在一个字符串中，在后续阶段进行处理；而规则则放置在一个链中，方便我们进行调用。
选择器处理
选择器部分，我们将用‘ , ’分隔的一个选择器提取出来，将其中的元素名存储在数组中，并且记录元素的个数。为此，我们定义了如下的数据结构：

    typedef struct selectNode {
        enum cssType type;
        char   name[10][30];    //选择器元素数组
        int    nodeNum;         //选择器元素个数
    }selectNode, *pSelectNode;
### html2DOM
将html转换为DOM树。我们的做法是先对html进行预处理，删除多余的空格，变成统一的风格。

    <link href="test.css">
    <div class="fs jl" id="fjsa"></div>
如上所示，所有的多余空格都会被删去。

然后我们使用正则表达式去匹配得到标签的类和ID。如

    <link href=\"[^\"]*\">   #可以解析css文件名
    class=\"[^\"]*\"         #可以解析类名
然后我们保存节点的信息。

DOM节点的结构为：

    typedef struct DOMNode {
        enum TAG        tag;
        struct DOMNode  *sonNodes[MAXSONNUM];
        int             sonNum;
        struct DOMNode  *fatherNode;
        // 节点属性相关
        st_style        style;
        char            ID[20];
        char            classes[20][20];
        int             classNum;
        char *          text; // 如果是文本，需要申请内存
        // 节点适用的css样式数组
        DOMCSSES *csses;
        int inheritStyle[7];
    }DOMTree, *pDOMNode;
### CSS规则处理
在规则处理模块，我们通过’{’和’}’作为起点和终点，对于一组规则进行解析。我们定义了如下数据结构，将每条规则存储在其中。

    typedef struct rule
    {
        char name[15];      //  存储规则名
        int namePos;
        char value[30];     //  存储规则值
        int valPos;
        struct rule* next;  //  指向下一条规则
    }rule;
**错误规则处理**

在CSS规则中，可能会出现规则之中没有用”；”隔开的情况，针对这种情况，我们进行了如下处理：在没有正确的匹配到”；”时，如果匹配到了其他特殊符号，如“：”，“}”，则根据后一条规则的属性名，可以确定”；”的位置，从而对CSS规则进行分割。

CSS节点生成
在得到CSS选择器和CSS规则链后，我们对其进行分析，形成统一的数据结构，方便我们进行后续处理。在这个阶段，我们识别出每条规则的属性名和属性值，并存储选择器。为此我们定义了CSS节点数据结构，以方便进行存储。

    typedef struct cssNode {
        selectNode *snodes; 
        char display[10];
        char position[10];
        char width[10];
        char height[10];
        char top[10];
        char bottom[10];
        char left[10];
        char right[10];
        char padding[4][10];
        char border[4][10];
        char margin[4][10];
        char color[10];
        char font_size[10];
        char line_height[10];
        char text_align[10];
        char font_style[10];
        char font_weight[10];
        char line_break[10];
        struct cssNode *next;
        unsigned defineFlag:18;
    }cssList, cssNode;
特别的，这里我们应用了位段来标记有哪些属性被定义了，这样在分析阶段就可以避免错误的赋值。
CSS计算模块
在对DOM树进行赋值之后，我们通过选择器的优先级对每个树节点中的CSS链进行排序，并且根据排序后的链对其属性值进行赋值。
在赋值之后，我们通过三次遍历来对树中的节点进行处理：第一次，我们从根开始向下进行先序遍历，并且记录所有不能确定的节点；第二次，我们从底向上进行遍历，对所有没有确定值，或者需要撑大的节点进行赋值；第三次，我们通过先序遍历，计算出每个节点的offset值。

## 自评
本次难度较大，没能很好的完成。
## 附加信息
代码托管地址：[2014种子杯复赛](https://github.com/jinjaysnow/SeedCup_QuarterFinal)

