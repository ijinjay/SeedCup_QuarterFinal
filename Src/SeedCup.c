/****************************************************
 * Author: Jin Jay
 * Created On: 2014-10-23
 * Last-Modified: 2014-11-6
 */

#include "SeedCup.h"

int main(int argc, char const *argv[]) {
    directory *dirlist = getDirs();
    directory *head = dirlist->next;
    while(head != NULL) {
        // 读取html文件
        char *data = (char *)malloc(MAXHTMLLEN * sizeof(char));
        if (readHtml(data, head->dir) == -1) {
            printf("cannot get the html file !\n");
            head = head->next;
            continue;
        }
        // 从head标签中解析css文件名
        CSSURL *cssurl = parseCSSURL(&data);
        char *css = (char *)malloc(MAXHTMLLEN * 2 * sizeof(char));
        char *cssTemp = (char *)malloc(MAXHTMLLEN * sizeof(char));
        // 初始化css
        css[0] = '\0';
        // 读取css文件
        for (int i = 0; i < cssurl->urlNum; ++i) {
            if(readCss(cssTemp, head->dir, cssurl->urls[i]) == -1) {
                printf("cannot read css file!\n");
                continue;
            }
            strcat(css, cssTemp);
        }
        free(cssTemp);
        free(cssurl);
        // 开始解析html和css文件
        RenderNode *renderHead = generateRenderTree(data, css);
        printDOMTree(renderHead->domNode);
        char *webText = getWebText(renderHead);
        writeFile(webText, "web.txt");
        drawPNG(renderHead);
        free(webText);

    /*    CairoHandle *pCaiHandle = initDrawContext();
        strcpy(renderHead->domNode->style.color, "#fff");
        drawBorder(pCaiHandle, renderHead->domNode->style);
        cairo_set_source_rgb (pCaiHandle->cr, 0.627, 0, 0);
        cairo_select_font_face (pCaiHandle->cr, "Microsoft YaHei Mono",
                                CAIRO_FONT_SLANT_NORMAL,
                                CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size (pCaiHandle->cr, 24.0);

        cairo_move_to (pCaiHandle->cr, 10.0, 34.0);
        cairo_show_text (pCaiHandle->cr, "To Beautiful You! 靳杰");

        cairo_surface_write_to_png (pCaiHandle->surface, "image.png");

        freeDraw(pCaiHandle);
    */

        freeRenderTree(renderHead);
        free(data);
        free(css);
        head = head->next;
    }

    freeDir(dirlist);
	return 0;
}
