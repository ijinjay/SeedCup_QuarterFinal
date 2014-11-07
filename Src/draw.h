/**
 * Author: Jin Jay
 * Created On: 2014-11-5
 * Last Modified: 2014-11-5
 */
#ifndef _DRAW_H
#define _DRAW_H 

#include <stdlib.h>
#include <cairo/cairo.h>
#include "html2DOM.h"

typedef struct CairoHandle {
	cairo_t *cr;
	cairo_surface_t *surface;
}CairoHandle, *pCairoHandle;

typedef struct Pos {
	int x;
	int y;
} Pos;

// 初始化绘图上下文
extern CairoHandle *initDrawContext(void);
extern void drawText(pCairoHandle pCH, const char *text, st_style style);
extern void drawBorder(pCairoHandle pCH, st_style style);
// 保存绘制的图形
extern int writeDrawFile(pCairoHandle pCH, const char *filename);
// 结束绘图
extern void freeDraw(pCairoHandle ph);
#endif // _DRAW_H
