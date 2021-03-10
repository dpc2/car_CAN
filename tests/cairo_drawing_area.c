//
//

#include"includes.h"

static GtkWidget *window = NULL;
/* Pixmap for scribble area, to store current scribbles */
static cairo_surface_t *surface = NULL;

/* Create a new surface of the appropriate size to store our scribbles */
static gboolean scribble_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
	if(surface != NULL)
	{
		cairo_surface_destroy(surface);
	}

	/* typedef struct _cairo_rectangle_int {
	 int x, y;
	 int width, height;
	 } cairo_rectangle_int_t;

	 * typedef 	cairo_rectangle_int_t	GdkRectangle;
	 * typedef 	GdkRectangle	   		GtkAllocation;
	  */
//
//	GtkAllocation allocation;
//	gtk_widget_get_allocation(widget, &allocation);

//	gtk_widget_get_allocated_width(widget);
//	gtk_widget_get_allocated_height(widget);

	surface = gdk_window_create_similar_surface(
			gtk_widget_get_window(widget),
			CAIRO_CONTENT_COLOR, /* CAIRO_CONTENT_COLOR_ALPHA */
			gtk_widget_get_allocated_width(widget),		/* gtk_widget_get_allocated_width(widget) */
			gtk_widget_get_allocated_height(widget));	/* gtk_widget_get_allocated_height(widget) */

	/* Initialize the surface to white */
	cairo_t *cr;
	cr = cairo_create(surface);

	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);

	cairo_destroy(cr);

	/* We've handled the configure event, no need for further processing. */
	return TRUE;
}

/* Redraw the screen from the surface */
static gboolean scribble_draw(GtkWidget *widget, cairo_t *cr, gpointer data)  // scribble: 涂
{
	cairo_set_source_surface(cr, surface, 0, 0);
	cairo_paint(cr);

	return FALSE;
}

/* Draw a rectangle on the screen */
static void draw_brush(GtkWidget *widget, gdouble x, gdouble y)
{
	GdkRectangle update_rect;
	cairo_t *cr;

	update_rect.x = x - 3;
	update_rect.y = y - 3;
	update_rect.width = 6;
	update_rect.height = 6;

	/* Paint to the surface, where we store our state */
	cr = cairo_create(surface);  // create one other cr for the surface

	gdk_cairo_rectangle(cr, &update_rect);   // rectangle: /ˈrɛkˌtæŋɡəl/ 矩形
	cairo_fill(cr);

	cairo_destroy(cr);

	/* Now invalidate the affected region of the drawing area. */
	gdk_window_invalidate_rect(gtk_widget_get_window(widget), &update_rect, FALSE);
}

static gboolean scribble_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	if(surface == NULL)
	{
		return FALSE; /* paranoia check, in case we haven't gotten a configure event */
	}
	if(event->button == GDK_BUTTON_PRIMARY)
	{
		draw_brush(widget, event->x, event->y);
	}
	/* We've handled the event, stop processing */
	return TRUE;
}

static gboolean scribble_motion_notify_event(GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
	int x, y;
	GdkModifierType state;

	if(surface == NULL)
	{
		return FALSE; /* paranoia check, in case we haven't gotten a configure event */
	}

	/* This call is very important; it requests the next motion event.
	 If you don't call gdk_window_get_pointer() you'll only get
	 a single motion event. The reason is that we specified
	 GDK_POINTER_MOTION_HINT_MASK to gtk_widget_set_events().
	 If we hadn't specified that, we could just use event->x, event->y
	 as the pointer location. But we'd also get deluged in events.
	 By requesting the next event as we handle the current one,
	 we avoid getting a huge number of events faster than we
	 can cope.
	 */

	gdk_window_get_device_position(event->window, event->device, &x, &y, &state);

	if(state & GDK_BUTTON1_MASK)
	{
		draw_brush(widget, x, y);
	}

	/* We've handled it, stop processing */
	return TRUE;
}

static void close_window(void)
{
	window = NULL;

	if(surface != NULL)
	{
		cairo_surface_destroy(surface);
	}
	surface = NULL;
}

GtkWidget * cairo_drawing_area(GtkWidget *do_widget)
{

	GtkWidget *frame;
	GtkWidget *da;


	if(window == NULL)
	{
		window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_screen(GTK_WINDOW(window), gtk_widget_get_screen(do_widget));
		gtk_window_set_title(GTK_WINDOW(window), "ddddddddddddd");
		gtk_container_set_border_width(GTK_CONTAINER(window), 8);
		g_signal_connect(window, "destroy", G_CALLBACK (close_window), NULL);

		frame = gtk_frame_new(NULL);
		gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);
		gtk_container_add(GTK_CONTAINER(window), frame);

		da = gtk_drawing_area_new();
		gtk_widget_set_size_request(da, 300, 200);
		gtk_container_add(GTK_CONTAINER(frame), da);


		/* Signals used to handle backing surface */

		g_signal_connect(da, "draw", G_CALLBACK (scribble_draw), NULL);
		g_signal_connect(da, "configure-event", G_CALLBACK (scribble_configure_event), NULL);

		/* Event signals */

		g_signal_connect(da, "motion-notify-event", G_CALLBACK (scribble_motion_notify_event), NULL);
		g_signal_connect(da, "button-press-event", G_CALLBACK (scribble_button_press_event), NULL);

		/* Ask to receive events the drawing area doesn't normally
		 * subscribe to
		 */
		gtk_widget_set_events(da, gtk_widget_get_events(da)
				| GDK_LEAVE_NOTIFY_MASK
				| GDK_BUTTON_PRESS_MASK
				| GDK_POINTER_MOTION_MASK
				| GDK_POINTER_MOTION_HINT_MASK);

	}

	if(gtk_widget_get_visible(window) == FALSE)
	{
		gtk_widget_show_all(window);
	}
	else
	{
		gtk_widget_destroy(window);
	}

	puts("cairo_drawing");
	return window;
}

