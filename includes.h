#pragma once

#include    <stdio.h>
#include    <stdlib.h>
//#include	<stdarg.h>
//#include	<stdint-gcc.h>
#include	<stdint.h>
#include	<stdbool.h>

#include 	<unistd.h>
#include    <string.h>
#include    <ctype.h>
#include 	<errno.h>
#include 	<time.h>
#include	<math.h>

#include	<sys/time.h>
#include 	<sys/mman.h>
#include 	<sys/socket.h>
#include 	<sys/types.h>
#include	<sys/sysinfo.h>
#include 	<sys/ipc.h>
#include 	<sys/msg.h>
#include	<sys/shm.h>
#include	<sys/reboot.h>
#include 	<sys/ioctl.h>

#include	<linux/kd.h>
#include 	<linux/fb.h>
#include	<linux/can.h>
#include	<linux/can/raw.h>
#include	<linux/stddef.h>
#include 	<fcntl.h>

#include	"signal.h"
#include 	<pthread.h>

#include	<net/if.h>
#include 	<netdb.h>
#include 	<netinet/in.h>
#include 	<arpa/inet.h>
#include 	<termio.h>
#include	<asm-generic/errno-base.h>

#include	<gtk/gtk.h>
#include	<gdk-pixbuf-2.0/gdk-pixbuf/gdk-pixbuf.h>
#include	<glib.h>
#include	<glib/gi18n.h>
#include	<cairo/cairo.h>
#include	"sqlite3.h"

#define	STRING_LENGTH							(100)				// string length
//#define X 800									// screen width
//#define Y 480									// screen height
#define	DRAW_AREA_X_MARGIN						(30)  // draw in right of X_MARGIN
#define	DRAW_AREA_Y_MARGIN						(40)
#define	RULER_AREA_X_MARGIN						(5)  // draw in right of X_MARGIN
#define	RULER_AREA_Y_MARGIN						(DRAW_AREA_Y_MARGIN)
#define	SHORT_LINE_LENGTH						(7)

//#define	MESSAGE_COUNT							(20)			// id count

#define	MAX_PIX_COUNT_OF_ONE_S					(1000)		// 1000 pixel/s
#define	MIN_PIX_COUNT_OF_ONE_S					(50)			// 1000 pixel/s

#define	PIX_COUNT_OF_ONE_S						(100)													// 100 pixel/s
#define	S_COUNT_OF_ONE_PIX						(1 / (double)PIX_COUNT_OF_ONE_S)				// 0.01 s/pixel
#define	PIX_COUNT_OF_ONE_MS						((double)PIX_COUNT_OF_ONE_S / 1000)			// 0.1 pixel/Ms
#define	MS_COUNT_OF_ONE_PIX						(1 / (double)PIX_COUNT_OF_ONE_MS)			// 10 Ms/pixel

#define	FRAME_COUNT_OF_ONE_S					(50)													// frame count of each id every second
#define	FRAME_COUNT_OF_ONE_MS					(FRAME_COUNT_OF_ONE_S / 1000)					// frame count of each id every millisecond
#define	FRAME_RECEIVE_TEST_USLEEP				(1000 * 1000 / FRAME_COUNT_OF_ONE_S)		// micro_second, usleep(FRAME_RECEIVE_TEST_USLEEP)

#define	MILLISECOND_OF_SHOW_ENTRY_DELAY			(500)			// millisecond
#define	MILLISECOND_OF_DRAW_LINES_DELAY  		(500)			// millisecond, 1 <= DRAW_LINES_DELAY <= draw_area_width * MS_COUNT_OF_ONE_PIX

//#define PRINT

typedef unsigned char	u8;
typedef unsigned short	u16;
typedef unsigned int	u32;

