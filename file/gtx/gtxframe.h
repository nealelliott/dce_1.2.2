/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gtxframe.h,v $
 * Revision 1.1.9.1  1996/10/02  17:51:01  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:58  damon]
 *
 * Revision 1.1.4.1  1994/06/09  14:09:56  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:32  annie]
 * 
 * Revision 1.1.2.2  1993/01/19  16:02:46  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:12:40  cjd]
 * 
 * Revision 1.1  1992/01/19  02:41:17  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#ifndef __GTX_FRAME_INCL_
#define __GTX_FRAME_INCL_ 1
struct gtxframe_dlist {
    struct gtxframe_dlist *next;
    char *data;
};

struct gtxframe_menu {
    struct gtxframe_menu *next;
    char *name;
    char *cmdString;
};

struct gtx_frame {
    struct keymap_map *keymap;		/*Map for handling keystrokes */
    struct keymap_map *savemap;		/* Map saved during recursive edit */
    struct keymap_state *keystate;	/*Associated key state */
    struct gtxframe_dlist *display;	/*Display list */
    struct gtxframe_menu *menus;	/* Menu list */
    char *messageLine;			/* message line */
    char *promptLine;			/* prompt for a response */
    char *defaultLine;			/* default response */
    struct gwin *window;		/* window we're currently showing on */
    long flags;
};

#define GTXFRAME_NEWDISPLAY	1	/* just did a display string this command */
#define GTXFRAME_RECURSIVEEND	2	/* end recursive edit */
#define GTXFRAME_RECURSIVEERR	4	/* recursive edit failed due to error */

extern struct gtx_frame *gtxframe_Create();
extern struct gtx_frame *gtxframe_GetFrame();

/*
  * The gtxframe_ExitCmd() routine, normally bound to ^C, allows a caller
  * to cleanly exit from its gtx environment, returning the terminal or
  * window to its normal state.  If a non-zero exit value is desired, then
  * the caller should place it in gtxframe_exitValue.
  */
extern int gtxframe_ExitCmd();
extern int gtxframe_exitValue;

#endif /* __GTX_FRAME_INCL_ */
