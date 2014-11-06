#include "draw.h"

CairoHandle * initDrawContext(void) {
	CairoHandle *chandle = (pCairoHandle)malloc(sizeof(CairoHandle));
	chandle->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1024, 768);
	chandle->cr = cairo_create(chandle->surface);
	return chandle;
}

void drawBorder(pCairoHandle pCH, Pos leftTop, Pos rightDown) {
	cairo_set_source_rgb(pCH->cr, 0, 0, 0);
	cairo_set_line_width(pCH->cr, 10);
	cairo_move_to(pCH->cr, leftTop.x, leftTop.y);
	cairo_line_to(pCH->cr, rightDown.x, leftTop.y);
	cairo_move_to(pCH->cr, rightDown.x, leftTop.y);
	cairo_line_to(pCH->cr, rightDown.x, rightDown.y);
	cairo_move_to(pCH->cr, rightDown.x, rightDown.y);
	cairo_line_to(pCH->cr, leftTop.x, rightDown.y);
	cairo_move_to(pCH->cr, leftTop.x, rightDown.y);
	cairo_line_to(pCH->cr, leftTop.x, leftTop.y);
	cairo_stroke(pCH->cr);
}

int writeDrawFile(pCairoHandle pCH, const char *filename) {
	cairo_surface_write_to_png(pCH->surface, filename);
	return 1;
}

void freeDraw(pCairoHandle ph) {
	cairo_destroy(ph->cr);
	cairo_surface_destroy(ph->surface);
	free(ph);
}
