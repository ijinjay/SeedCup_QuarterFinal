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
	return 0;
}
