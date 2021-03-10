//
//
#include"includes.h"
#include <cairo-pdf.h>

int cairo_pdf(int argc, char * argv[])
{
	cairo_surface_t * surface;
	cairo_t * cr;
	
	surface = cairo_pdf_surface_create("pdffile.pdf", 320, 48);
	cr = cairo_create(surface);
	
	cairo_set_source_rgb(cr, 0.627, 0, 0);
	cairo_select_font_face(cr, "AdobeHeitiStd", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 24.0);
	
	cairo_move_to(cr, 10.0, 34.0);
	cairo_show_text(cr, "的");
	
	cairo_show_page(cr);
	
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
	
	return 0;
}
