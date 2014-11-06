/****************************************************
 * Author: Jin Jay
 * Created On: 2014-10-23
 * Last-Modified: 2014-11-2
 */

#include "SeedCup.h"

int main(int argc, char const *argv[]) {
    char *data = (char *)malloc(MAXHTMLLEN * sizeof(char));
    readCssFile(data, "test/test.html");
    CSSURL *cssurl = parseCSSURL(&data);
    for (int i = 0; i < cssurl->urlNum; ++i) {
        printf("%s\n", cssurl->urls[i]);
    }
    free(cssurl);
    printf("%s\n", data);
    DOMTree *tree = generateDOMTree(data);
    printDOMTree(tree);
    freeDOMTree(tree);
    free(data);

    CairoHandle *pCaiHandle = initDrawContext();

    Pos a, b;
    a.x = 10; a.y = 10;
    b.x = 1000; b.y = 700;

    drawBorder(pCaiHandle, a, b);
    cairo_set_source_rgb (pCaiHandle->cr, 0.627, 0, 0);
    cairo_select_font_face (pCaiHandle->cr, "Microsoft YaHei Mono",
                            CAIRO_FONT_SLANT_NORMAL,
                            CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size (pCaiHandle->cr, 24.0);

    cairo_move_to (pCaiHandle->cr, 10.0, 34.0);
    cairo_show_text (pCaiHandle->cr, "To Beautiful You! 靳杰");

    cairo_surface_write_to_png (pCaiHandle->surface, "image.png");

    freeDraw(pCaiHandle);
	return 0;
}
