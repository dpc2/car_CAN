//
//

#include <includes.h>

#define COUNT 10

typedef struct
{

	double x;
	double y;
} point_t;

point_t point_ar[COUNT];

GtkWidget * paint(GtkWidget * do_widget)
{
//	GdkEventExpose * e;
//	GdkEventButton * b;
	GDK_BUTTON_PRESS_MASK;


//	gtk_widget_queue_draw();
//	gtk_widget_queue_draw_region();

//	 cairo_arc();


	
//	GtkDrawingArea draw_area1;

	static GtkWidget *window = NULL;

	if(window == NULL)
	{
		window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_screen(GTK_WINDOW(window), gtk_widget_get_screen(do_widget));
		gtk_window_set_title(GTK_WINDOW(window), "display");
		//		gtk_widget_set_size_request(window, 600, 600);
		gtk_window_fullscreen(GTK_WINDOW(window));	// title will not exist
		gtk_container_set_border_width(GTK_CONTAINER(window), 3);

		int width, height;
		gtk_window_get_size(GTK_WINDOW(window), &width, &height);  // 获取窗口的宽度与高度尺寸


		cairo_t *cr;
		GtkWidget * draw_area2 = gtk_drawing_area_new();
//
////		GdkWindow * gdkwindow;
//		cr = gdk_cairo_create();

		{
			cairo_set_source_rgb(cr, 0, 0, 1);
			cairo_set_line_width(cr, 0.5);
			cairo_move_to(cr, 0, 0);

			int i;
			for(i = 0; i < COUNT; i++)
			{
				cairo_line_to(cr, point_ar[i].x, point_ar[i].y);
				cairo_move_to(cr, point_ar[i].x, point_ar[i].y);  // 用 cairo_move_to() 和 cairo_line_to() 函数在 cr 中定义绘图路径 (path)
			}
		}

		cairo_stroke(cr);  // cairo_stroke() 函数会将 cr 中的路径绘制出来
		cairo_destroy(cr);


//		cairo_paint_with_alpha ();
//		g_timeout_add (50, (GSourceFunc) time_handler, (gpointer) window);  // 创建一个计时器，其回调函数为 time_handler ()，每隔 50ms 便会被调用一次
		cairo_set_source_rgb(cr, 0, 0, 0);
		cairo_set_line_width(cr, 14);
//		cairo_public void cairo_rectangle (cairo_t *cr, double x, double y, double width, double height);
//		cairo_rect//	cr = gdk_cairo_create(widget->window);angle(cr, 20, 20, 120, 80);
//		cairo_scale(cr, 1, 0.7);

		cairo_surface_t * surface;
//		cairo_t * x = cairo_create(surface);

		/*
		 * 	cairo_rel_move_to();
		 * 	cairo_line_to（cr，0.5，0.375）;
		 *		cairo_rel_line_to（cr，0.25，-0.125）;
		 * 开始一个新的子路径。 在这个调用之后，当前点将被（x，y）偏移。
		 *	给定当前点（x，y），cairo_rel_move_to（cr，dx，dy）在逻辑上等同于cairo_move_to（cr，x + dx，y + dy）。
		 *	在没有当前点的情况下调用此函数是一个错误。 这样做会导致cr关闭，状态为 CAIRO_STATUS_NO_CURRENT_POINT。
		 *
		 *
		 *
		 *	cairo_close_path (cr);
		 *	开罗也可以通过绘制直线到当前子路径的开头来关闭路径。 这条直线可以用于多边形的最后一个边，但不能直接用于基于曲线的形状。
		 *	开罗也可以通过绘制直线到当前子路径的开头来关闭路径。 封闭的路径与开放路径截然不同：它是一条连续的路径，没有开始或结束。
		 *	开罗也可以通过绘制直线到当前子路径的开头来关闭路径。 封闭的路径没有线帽，因为没有地方放置它们。
		 *
		 *
		 */
		cairo_set_line_width (cr, 0.1);

		cairo_save (cr);
		cairo_scale (cr, 0.5, 1);
		cairo_arc (cr, 0.5, 0.5, 0.40, 0, 2 * M_PI);
		cairo_stroke (cr);

		cairo_translate (cr, 1, 0);
		cairo_arc (cr, 0.5, 0.5, 0.40, 0, 2 * M_PI);
		cairo_restore (cr);
		cairo_stroke (cr);



//		cairo_paint();
//		cairo_paint_with_alpha (cr, 0.5);
//		gtk_widget_queue_draw_region();
		cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);
		cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
		gtk_widget_set_app_paintable(window, TRUE);
		cairo_stroke_preserve(cr);
//		cairo_fill_preserve();

		/*
		 * 与cairo_stroke（）不同，cairo_stroke_preserve（）保留cairo上下文中的路径。
请参阅cairo_set_line_width（)
cairo_set_line_join（）
cairo_set_line_cap（）
cairo_set_dash（）
cairo_stroke_preserve（）
		 */

		{
//			cairo_mask() and cairo_mask_surface()

			/* #cairo_pattern_t在绘制到曲面上时表示一个源。#cairo_pattern_t有不同的子类型，用于不同类型的源;
			 * 例如，cairo_pattern_create_rgb（）创建一个实体不透明颜色的图案。
			 *
			 */
			cairo_pattern_t *linpat, *radpat;

			linpat = cairo_pattern_create_linear (0, 0, 1, 1);
			cairo_pattern_add_color_stop_rgb (linpat, 0, 0, 0.3, 0.8);
			cairo_pattern_add_color_stop_rgb (linpat, 1, 0, 0.8, 0.3);

			radpat = cairo_pattern_create_radial (0.5, 0.5, 0.25, 0.5, 0.5, 0.75);
			cairo_pattern_add_color_stop_rgba (radpat, 0, 0, 0, 0, 1);
			cairo_pattern_add_color_stop_rgba (radpat, 0.5, 0, 0, 0, 0);

			cairo_set_source (cr, linpat);
			cairo_mask (cr, radpat);



		}



	}
//cairo_reference();
//cairo_destroy();

	if(gtk_widget_get_visible(window) == FALSE)
	{
//		gtk_window_fullscreen(GTK_WINDOW(window));	// title will not exist
		gtk_widget_show_all(window);
	}
	else
	{
		gtk_widget_destroy(window);
		window = NULL;
	}

	return window;





}



gboolean time_handler (GtkWidget * widget)
{
//	if (widget->window == NULL)
	return FALSE;

	gtk_widget_queue_draw (widget);
	return TRUE;
}
//time_handler () 函数的实现，它的主要作用是调用 gtk_widget_queue_draw () 函数，实现窗口的重绘 (发出 expose 事件)。当该函数返回值为 FALSE 时，计时器便停止工作。
