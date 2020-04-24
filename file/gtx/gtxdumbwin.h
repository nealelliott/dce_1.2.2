/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gtxdumbwin.h,v $
 * Revision 1.1.11.1  1996/10/02  17:50:59  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:57  damon]
 *
 * Revision 1.1.6.1  1994/06/09  14:09:53  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:31  annie]
 * 
 * Revision 1.1.4.4  1993/01/19  16:02:36  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:12:36  cjd]
 * 
 * Revision 1.1.4.3  1992/11/24  17:51:42  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  18:05:15  bolinger]
 * 
 * Revision 1.1.4.2  1992/08/31  20:05:27  jaffe
 * 	Transarc delta: jaffe-ot4658-fix-compiler-warnings-in-102-1.42 1.1
 * 	  Selected comments:
 * 	    cleaned up compiler warnings.
 * 	    remove text after endif
 * 	[1992/08/30  02:49:15  jaffe]
 * 
 * Revision 1.1.2.2  1992/05/28  22:09:57  garyf
 * 	fix incorrect include
 * 	[1992/05/28  22:06:50  garyf]
 * 
 * Revision 1.1  1992/01/19  02:41:37  devrcs
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

#ifndef __gator_dumbwindows_h
#define	__gator_dumbwindows_h  1

#include <dcedfs/gtxwindows.h>		/*Base gator window dfns*/

/*Value for gwin w_type field*/
#define	GATOR_WIN_DUMB    1

/*Private data for a dumb gwin*/
#if 0
struct gator_dumbgwin {
    WINDOW *wp;		/*Window pointer*/
    int	charwidth;	/*Character width in pixels*/
    int	charheight;	/*Character height in pixels*/
    char box_vertchar;	/*Vertical char for boxing purposes*/
    char box_horizchar;	/*Horizontal char for boxing purposes*/
};
#endif

/*Dumb gwin's creation parameters*/
struct gator_dumbgwin_params {
    struct gwin_createparams gwin_params;   /*Basic params for the window*/
    char box_vertchar;			    /*Vertical char for boxing purposes*/
    char box_horizchar;			    /*Horizontal char for boxing purposes*/
};

/*Dumb windows initialization routine*/

extern int gator_dumbgwin_init();
    /*
     * Summary:
     *    Initialize the dumb window package.
     *
     * Args:
     *	  int adebug: Is debugging turned on?
     *
     * Returns:
     *	  0 on success,
     *	  Error value otherwise.
     */

/*Dumb window's creation routine*/

extern struct gwin *gator_dumbgwin_create();
    /*
     * Summary:
     *    Create a dumb window.
     *
     * Args:
     *	  struct gator_dumbgwin_params *params : Ptr to creation parameters.
     *
     * Returns:
     *	  Ptr to the created dumb window structure if successful,
     *	  Error value otherwise.
     */

/*Dumb cleanup routine*/

extern int gator_dumbgwin_cleanup();
    /*
     * Summary:
     *    Clean up after the dumb window package.
     *
     * Args:
     *	  struct gwin *gwp : Ptr to base window.
     *
     * Returns:
     *	  0 on success,
     *	  Error value otherwise.
     */

extern struct gwinbaseops gator_dumb_gwinbops;

/*Dumb window's routines*/

extern int gator_dumbgwin_box();
    /*
     * Summary:
     *    Draw a box around the given dumb window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the dumb window to draw
     *	                            a box around.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_dumbgwin_clear();
    /*
     * Summary:
     *    Clear out the given dumb window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the dumb window to clear
     *	                            out.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_dumbgwin_destroy();
    /*
     * Summary:
     *    Destroy the given dumb window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the dumb window to destroy.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_dumbgwin_display();
    /*
     * Summary:
     *    Display/redraw the given dumb window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the dumb window to draw.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_dumbgwin_drawline();
    /*
     * Summary:
     *    Draw a line between two points in the given dumb
     *    window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the dumb window in which
     *	                            the line is to be drawn.
     *	 struct gwin_lineparams *params : Ptr to other params.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_dumbgwin_drawrectangle();
    /*
     * Summary:
     *    Draw a rectangle in the given dumb window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the dumb window in which
     *	                            the rectangle is to be drawn.
     *	 struct gwin_rectparams *params : Ptr to other params.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_dumbgwin_drawchar();
    /*
     * Summary:
     *    Draw a character in the given dumb window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the dumb window in which
     *	                            the character is to be drawn.
     *	 struct gwin_charparams *params : Ptr to other params.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_dumbgwin_drawstring();
    /*
     * Summary:
     *    Draw a string in the given dumb window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the dumb window in which
     *	                            the string is to be drawn.
     *	 struct gwin_strparams *params : Ptr to other params.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_dumbgwin_invert();
    /*
     * Summary:
     *    Invert a region in the given dumb window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the dumb window in which
     *	                            the inverted region lies.
     *	 struct gwin_invparams *params : Ptr to other params.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_dumbgwin_getchar();
    /* get a character from a window */

extern int gator_dumbgwin_getdimensions();

extern int gator_dumbgwin_wait();

#endif /* __gator_dumbwindows_h */
