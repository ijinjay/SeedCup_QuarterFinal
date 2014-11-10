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

extern int px2int(const char *px) {
	int pixel = (int)strtol(px, NULL, 10);
	//printf("pixel is %d\n", pixel);
	if (strcmp(px, "auto") == 0)
		return 0;
	return pixel;
}
extern int em2int(const char *em, int fontsize) {
	int emNum = (int)strtol(em, NULL, 10);
	return emNum * fontsize;
}
int percent2int(const char *percent, int fatherwidth) {
	int percentNum = (int)strtol(percent, NULL , 10);
	return (int)((percentNum / 100.0)* fatherwidth);
}
int len2int(const char *str, int fontsize, int fatherwidth) {
	int strLen = strlen(str);
	if (strLen > 2 && strcmp(str + strLen - 2, "em") == 0)
		return em2int(str, fontsize);
	if (strLen > 2 && strcmp(str + strLen -2, "px") == 0)
		return px2int(str);
	if (str[strLen - 1] == '%')
		return percent2int(str, fatherwidth);
	return (int)strtol(str, NULL, 10);
}
void drawBorder(pCairoHandle pCH, st_style style) {
	Color rgb = str2Color(style.color);
	cairo_set_source_rgb(pCH->cr, rgb.R, rgb.G, rgb.B);
	double border_left 	= px2int(style.offsetLeft)
					   	+ px2int(style.margin[3])
					   	+ px2int(style.border[3]) /2 ;
	double border_top	= px2int(style.offsetTop)
					   	+ px2int(style.margin[0])
					   	+ px2int(style.border[0]) /2 ;
	double border_right = px2int(style.offsetLeft)
						+ px2int(style.padding[3])
						+ px2int(style.padding[1])
						+ px2int(style.margin[3])
					   	+ px2int(style.width)
					   	+ px2int(style.border[1]) /2 ;
	double border_down	= px2int(style.offsetTop)
					   	+ px2int(style.height)
					   	+ px2int(style.margin[0])
					   	+ px2int(style.border[0])
					   	+ px2int(style.border[3])
					   	+ px2int(style.border[0])
					   	+ px2int(style.border[2]) /2 ;
	// 左上到右上
	cairo_set_line_width(pCH->cr, px2int(style.border[0]));
	cairo_move_to(pCH->cr, border_left, border_top);
	cairo_line_to(pCH->cr, border_right, border_top);
	// 右上到右下
	cairo_set_line_width(pCH->cr, px2int(style.border[1]));
	cairo_move_to(pCH->cr, border_right, border_top);
	cairo_line_to(pCH->cr, border_right, border_down);
	// 右下到左下
	cairo_set_line_width(pCH->cr, px2int(style.border[2]));
	cairo_move_to(pCH->cr, border_right, border_down);
	cairo_line_to(pCH->cr, border_left, border_down);
	// 左下到左上
	cairo_set_line_width(pCH->cr, px2int(style.border[3]));
	cairo_move_to(pCH->cr, border_left, border_down);
	cairo_line_to(pCH->cr, border_left, border_top);
	// 显示线
	cairo_stroke(pCH->cr);
}

void drawText(pCairoHandle pCH, const char *text, st_style style) {
	Color rgb = str2Color(style.color);
	cairo_set_source_rgb(pCH->cr, rgb.R, rgb.G, rgb.B);
	// 可能需要换行
	int font_italic = CAIRO_FONT_SLANT_NORMAL;
	int font_weight = CAIRO_FONT_WEIGHT_NORMAL;

	if (strcmp(style.line_break, "normal") == 0) {
		if (strcmp(style.font_style, "italic") == 0)
			font_italic = CAIRO_FONT_SLANT_ITALIC;
		if (strcmp(style.font_weight, "bold") == 0)
			font_weight = CAIRO_FONT_WEIGHT_BOLD;
		cairo_select_font_face(pCH->cr, "arial", font_italic, font_weight);
		cairo_set_font_size(pCH->cr, px2int(style.font_size));
		cairo_move_to(pCH->cr, px2int(style.offsetLeft), px2int(style.offsetTop) + px2int(style.height));
		cairo_show_text(pCH->cr, text);
	}

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
