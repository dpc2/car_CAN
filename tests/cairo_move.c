//
//
#include <includes.h>

static gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	cairo_t *cr;

//	cr = gdk_cairo_create(widget->window);
//	cr = gdk_cairo_create(widget->priv);

	cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
	cairo_rectangle(cr, 20, 20, 80, 50);
	cairo_stroke_preserve(cr);
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_fill(cr);

	cairo_translate(cr, 100, 100);

	cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
	cairo_rectangle(cr, 20, 20, 80, 50);
	cairo_stroke_preserve(cr);
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_fill(cr);


	cairo_destroy(cr);

	return FALSE;
}

int cairo_move(int argc, char *argv[])
{
	GtkWidget *window;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	g_signal_connect(window, "expose-event", G_CALLBACK (on_expose_event), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);   // where the window will appear
	gtk_window_set_default_size(GTK_WINDOW(window), 300, 230);

	gtk_widget_set_app_paintable(window, TRUE);

	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}
