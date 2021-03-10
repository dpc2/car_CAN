//
//
//

#include "includes.h"

int screen_param(int argc, char *argv[])
{
	GdkScreen *gdk_screen;
	gint screen_width, screen_height;
	
	gtk_init(&argc, &argv);
	gdk_screen = gdk_screen_get_default();
	
	screen_width = gdk_screen_get_width(gdk_screen);
	screen_height = gdk_screen_get_height(gdk_screen);
	
	printf("screen_width = %d\n", screen_width);
	printf("screen_height= %d\n", screen_height);
	
	return 0;
}
