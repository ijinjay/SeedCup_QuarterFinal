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

    cairo_surface_t *surface;
    cairo_t *cr;

    surface =
        cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 320, 48);
    cr = cairo_create (surface);

    cairo_set_source_rgb (cr, 0.627, 0, 0);
    cairo_select_font_face (cr, "Adobe Heiti Std",
                            CAIRO_FONT_SLANT_NORMAL,
                            CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size (cr, 24.0);

    cairo_move_to (cr, 10.0, 34.0);
    cairo_show_text (cr, "To Beautiful You! 靳杰");

    cairo_surface_write_to_png (surface, "image.png");

    cairo_destroy (cr);
    cairo_surface_destroy (surface);
	return 0;
}
