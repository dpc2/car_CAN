////
////
//
//#include "includes.h"
//#include "display.h"
//cairo_t * cr = NULL;
//GtkWidget * a_da = NULL;
//
//static gboolean draw_callback(GtkWidget *widget, cairo_t * cr1, gpointer data)
//{
//	static int x = 0;
//	static guint width, height;
//	static GtkStyleContext *context;
//	static GdkRGBA color;
//	static GdkRGBA color_1 = {0, 0, 1, 1};
//	/*
//	 * struct _GdkRGBA
//	 {
//	 gdouble red;
//	 gdouble green;
//	 gdouble blue;
//	 gdouble alpha;
//	 };
//	 *
//	 */
//
//	if(x == 0)
//	{
//		cr = gdk_cairo_create(gtk_widget_get_window(widget));
//
//		context = gtk_widget_get_style_context(widget);
//		width = gtk_widget_get_allocated_width(widget);
//		height = gtk_widget_get_allocated_height(widget);
//		gtk_render_background(context, cr, 0, 0, width, height);
//		gtk_style_context_get_color(context, gtk_style_context_get_state(context), &color);
//
//		cairo_set_source_rgba(cr, 1, 1, 0, 1);
//		cairo_paint(cr);
//		x++;
//	}
//
//	printf("2 ....... %d       %d       cr = %p  \n",
//			gtk_widget_get_allocated_width(widget),
//			gtk_widget_get_allocated_height(widget),
//			cr);
//
////	color1.red = 0;
////	color1.green = 0;
////	color1.blue = 1;
////	color1.alpha = 1;
//	gdk_cairo_set_source_rgba(cr, &color_1);
//	cairo_set_line_width(cr, 1);
////	cairo_arc(cr, width / 2.0, height / 2.0, MIN(width,height) / 2.0, 0, 2 * G_PI);
////	cairo_set_source_rgba(cr, 0, 0, 1, 1);
//
//	static int a = 0;
//	static int b = 0;
//	cairo_move_to(cr, a, b);
//
//	a = random() % 200;
//	b = random() % 200;
//	cairo_line_to(cr, a, b);
//
//	cairo_stroke(cr);
////	cairo_stroke_preserve(cr);
//
////	cairo_fill(cr);
//
////	cairo_set_source_rgb(cr, 1, 1, 1);
////	cairo_paint(cr);
//
////
////	static int x = 0;
////	if(x == 0)
////	{
////		a_surface = gdk_window_create_similar_surface(
////				gtk_widget_get_window(widget),
////				CAIRO_CONTENT_COLOR, /* CAIRO_CONTENT_COLOR_ALPHA */
////				gtk_widget_get_allocated_width(widget), /* gtk_widget_get_allocated_width(widget) */
////				gtk_widget_get_allocated_height(widget)); /* gtk_widget_get_allocated_height(widget) */
////		a_cr = cairo_create(a_surface);
////		x++;
////	}
////	printf("2 ....... %d       %d   a_surface = %p  a_cr = %p  \n",
////			gtk_widget_get_allocated_width(widget),
////			gtk_widget_get_allocated_height(widget),
////			a_surface,
////			a_cr);
////
////	cairo_set_source_rgb(a_cr, 0, 0, 1);
////
////	static int a = 0;
////	static int b = 0;
////	cairo_move_to(a_cr, a, b);
////
////	a = random() % 200;
////	b = random() % 200;
////	cairo_line_to(a_cr, a, b);
////
////// 	cairo_stroke(cr);
////	cairo_stroke_preserve(a_cr);
//
//	return FALSE;
//
////	a_cr = gdk_cairo_create(a_surface);
////	cairo_set_line_join();
////	gtk_widget_queue_draw_area(a_da, 0, 0, 600, 600);
//}
//
//int aaaa(GtkWidget *do_widget)
//{
//
//	static GtkWidget *window = NULL;
//
//	if(window == NULL)
//	{
//		window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//		gtk_window_set_screen(GTK_WINDOW(window), gtk_widget_get_screen(do_widget));
//		gtk_window_set_title(GTK_WINDOW(window), "a");
//		gtk_container_set_border_width(GTK_CONTAINER(window), 8);
//
//		GtkWidget * frame = gtk_frame_new("a");
//		gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);
//		gtk_container_add(GTK_CONTAINER(window), frame);
//
//		a_da = gtk_drawing_area_new();
//		gtk_widget_set_size_request(a_da, 500, 500);
//		gtk_container_add(GTK_CONTAINER(frame), a_da);
//		g_signal_connect(G_OBJECT(a_da), "draw", G_CALLBACK(draw_callback), NULL);
//
////		gtk_widget_set_app_paintable(a_da, TRUE);
////		cairo_paint(a_cr);
////		cairo_destroy(a_cr);
//
//	}
//
//	if(gtk_widget_get_visible(window) == FALSE)
//	{
////		gtk_window_fullscreen(GTK_WINDOW(window));	// title will not exist
//		gtk_widget_show_all(window);
//	}
//	else
//	{
//		gtk_widget_destroy(window);
//		window = NULL;
//	}
//
//	return 0;
//}
//
//void aaaa_draw(void)
//{
//	cairo_surface_t *a_surface = NULL;
//	cairo_t * a_cr = NULL;
//
////	GtkAllocation allocation;
////	gtk_widget_get_allocation(a_da, &allocation);
//
//	a_surface = gdk_window_create_similar_surface(
//			gtk_widget_get_window(a_da),
//			CAIRO_CONTENT_COLOR, /* CAIRO_CONTENT_COLOR_ALPHA */
//			gtk_widget_get_allocated_width(a_da), /* gtk_widget_get_allocated_width(widget) */
//			gtk_widget_get_allocated_height(a_da)); /* gtk_widget_get_allocated_height(widget) */
//
//	a_cr = cairo_create(a_surface);
//
//	printf("....... %d       %d   a_surface = %p  a_cr = %p  \n",
//			gtk_widget_get_allocated_width(a_da),
//			gtk_widget_get_allocated_height(a_da),
//			a_surface,
//			a_cr);
//
////	cairo_set_source_rgb(a_cr, 1, 1, 1);
////	cairo_paint(a_cr);
//
//	int i = 10;
//	cairo_move_to(a_cr, i, i);
//
////	cairo_set_source_surface(a_cr, a_surface, 0, 0);
//
//	while (1)
//	{
//		i++;
//		cairo_line_to(a_cr, i, i);
//		cairo_move_to(a_cr, i, i);
//		cairo_stroke(a_cr);
//
////		gtk_widget_queue_draw_area (a_da, 0, 0, 600, 600);
//		usleep(100 * 1000);
//	}
//
//	return;
//}
//
