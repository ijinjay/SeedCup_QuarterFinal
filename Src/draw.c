#include "draw.h"

CairoHandle * initDrawContext(void) {
	CairoHandle *chandle = (pCairoHandle)malloc(sizeof(CairoHandle));
	chandle->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1024, 768);
	chandle->cr = cairo_create(chandle->surface);
	return chandle;
}

typedef struct Color {
	unsigned char R;
	unsigned char G;
	unsigned char B;
}Color;

static Color str2Color(const char *str) {
	int len = strlen(str);
	Color c;
	unsigned int number = (unsigned int)strtol(str+1, NULL, 16);
	printf("%d\n", number);
	if (len == 4) {
		c.R = (unsigned char)(((number & 0xf00)>>8) + ((number & 0xf00)>>4));
		c.G = (unsigned char)(((number & 0x0f0)>>4) + ((number & 0x0f0)));
		c.B = (unsigned char)(((number & 0x00f)<<4) + ((number & 0x00f)));
	}
	else {
		c.R = (unsigned char)((number & 0xff0000)>>16);
		c.G = (unsigned char)((number & 0x00ff00)>>8);
		c.B = (unsigned char)((number & 0x0000ff));
	}
	return c;
}

static int px2int(const char *px) {
	int pixel = (int)strtol(px, NULL, 10);
	return pixel;
}

void drawBorder(pCairoHandle pCH, st_style style) {
	Color rgb = str2Color(style.color);
	printf("%d, %d, %d\n", rgb.R, rgb.G, rgb.B);
	cairo_set_source_rgb(pCH->cr, rgb.R, rgb.G, rgb.B);
	printf("%dpx %dpx\n", px2int("0px"), px2int("100px"));
	cairo_set_line_width(pCH->cr, px2int(style.line_height));
	// cairo_move_to(pCH->cr, leftTop.x, leftTop.y);
	// cairo_line_to(pCH->cr, rightDown.x, leftTop.y);
	// cairo_move_to(pCH->cr, rightDown.x, leftTop.y);
	// cairo_line_to(pCH->cr, rightDown.x, rightDown.y);
	// cairo_move_to(pCH->cr, rightDown.x, rightDown.y);
	// cairo_line_to(pCH->cr, leftTop.x, rightDown.y);
	// cairo_move_to(pCH->cr, leftTop.x, rightDown.y);
	// cairo_line_to(pCH->cr, leftTop.x, leftTop.y);
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
