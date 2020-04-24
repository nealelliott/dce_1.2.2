/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gtxX11win.h,v $
 * Revision 1.1.11.1  1996/10/02  17:50:55  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:56  damon]
 *
 * Revision 1.1.6.1  1994/06/09  14:09:49  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:29  annie]
 * 
 * Revision 1.1.4.4  1993/01/19  16:02:28  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:12:27  cjd]
 * 
 * Revision 1.1.4.3  1992/11/24  17:51:33  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  18:05:02  bolinger]
 * 
 * Revision 1.1.4.2  1992/08/31  19:58:47  jaffe
 * 	Transarc delta: jaffe-ot4658-fix-compiler-warnings-in-102-1.42 1.1
 * 	  Selected comments:
 * 	    cleaned up compiler warnings.
 * 	    remove text after endif
 * 	[1992/08/30  02:48:59  jaffe]
 * 
 * Revision 1.1.2.2  1992/05/28  22:09:46  garyf
 * 	fix incorrect include
 * 	[1992/05/28  22:06:39  garyf]
 * 
 * Revision 1.1  1992/01/19  02:41:36  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * (C) Copyright Transarc Corporation 1989
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

#ifndef __gator_X11windows_h
#define	__gator_X11windows_h  1

#include <dcedfs/gtxwindows.h>	/*Base gator window dfns */

/*Value for gwin w_type field*/
#define	GATOR_WIN_X11    3

/*Private data for a X11 gwin*/
#if 0
struct gator_X11gwin {
    WINDOW *wp;		/*Window pointer*/
    int	charwidth;	/*Character width in pixels*/
    int	charheight;	/*Character height in pixels*/
    char box_vertchar;	/*Vertical char for boxing purposes*/
    char box_horizchar;	/*Horizontal char for boxing purposes*/
};
#endif

/*X11 gwin's creation parameters*/
struct gator_X11gwin_params {
    struct gwin_createparams gwin_params;   /*Basic params for the window*/
    char box_vertchar;			    /*Vertical char for boxing purposes*/
    char box_horizchar;			    /*Horizontal char for boxing purposes*/
};

/*X11 initialization routine*/

extern int gator_X11gwin_init();
    /*
     * Summary:
     *    Initialize the X11 window package.
     *
     * Args:
     *	  int adebug: Is debugging turned on?
     *
     * Returns:
     *	  0 on success,
     *	  Error value otherwise.
     */

/*X11 window's creation routine*/

extern struct gwin *gator_X11gwin_create();
    /*
     * Summary:
     *    Create a X11 window.
     *
     * Args:
     *	  struct gator_X11gwin_params *params : Ptr to creation parameters.
     *
     * Returns:
     *	  Ptr to the created X11 window structure if successful,
     *	  Error value otherwise.
     */

/*X11 cleanup routine*/

extern int gator_X11gwin_cleanup();
    /*
     * Summary:
     *    Clean up after the X11 window package.
     *
     * Args:
     *	  struct gwin *gwp : Ptr to base window.
     *
     * Returns:
     *	  0 on success,
     *	  Error value otherwise.
     */

extern struct gwinbaseops gator_X11_gwinbops;

/*X11 window's routines*/

extern int gator_X11gwin_box();
    /*
     * Summary:
     *    Draw a box around the given X11 window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the X11 window to draw
     *	                            a box around.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_X11gwin_clear();
    /*
     * Summary:
     *    Clear out the given X11 window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the X11 window to clear
     *	                            out.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_X11gwin_destroy();
    /*
     * Summary:
     *    Destroy the given X11 window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the X11 window to destroy.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_X11gwin_display();
    /*
     * Summary:
     *    Display/redraw the given X11 window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the X11 window to draw.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_X11gwin_drawline();
    /*
     * Summary:
     *    Draw a line between two points in the given X11
     *    window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the X11 window in which
     *	                            the line is to be drawn.
     *	 struct gwin_lineparams *params : Ptr to other params.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_X11gwin_drawrectangle();
    /*
     * Summary:
     *    Draw a rectangle in the given X11 window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the X11 window in which
     *	                            the rectangle is to be drawn.
     *	 struct gwin_rectparams *params : Ptr to other params.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_X11gwin_drawchar();
    /*
     * Summary:
     *    Draw a character in the given X11 window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the X11 window in which
     *	                            the character is to be drawn.
     *	 struct gwin_charparams *params : Ptr to other params.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_X11gwin_drawstring();
    /*
     * Summary:
     *    Draw a string in the given X11 window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the X11 window in which
     *	                            the string is to be drawn.
     *	 struct gwin_strparams *params : Ptr to other params.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_X11gwin_invert();
    /*
     * Summary:
     *    Invert a region in the given X11 window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the X11 window in which
     *	                            the inverted region lies.
     *	 struct gwin_invparams *params : Ptr to other params.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_X11gwin_getchar();

extern int gator_X11gwin_getdimensions();

extern int gator_X11gwin_wait();

#endif /*  __gator_X11windows_h */
