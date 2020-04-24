/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gtxcurseswin.h,v $
 * Revision 1.1.12.1  1996/10/02  17:50:57  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:56  damon]
 *
 * Revision 1.1.7.1  1994/06/09  14:09:50  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:30  annie]
 * 
 * Revision 1.1.5.3  1993/01/19  16:02:32  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:12:32  cjd]
 * 
 * Revision 1.1.5.2  1992/11/24  17:51:39  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  18:05:09  bolinger]
 * 
 * Revision 1.1.3.4  1992/05/28  22:09:50  garyf
 * 	fix incorrect include
 * 	[1992/05/28  22:06:45  garyf]
 * 
 * Revision 1.1.3.3  1992/01/24  03:45:38  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:13:41  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  19:49:57  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:34:27  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:17:22  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
*/
/*
 * (C) Copyright Transarc Corporation 1989
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

#ifndef __gator_curseswindows_h
#define	__gator_curseswindows_h  1

#include <dcedfs/gtxwindows.h>  /*Base gator window dfns*/
#undef IN                /* curses needs this! */
#include <curses.h>	    /*Curses library*/

/*Value for gwin w_type field*/
#define	GATOR_WIN_CURSES    2

/*Private data for a curses gwin*/
struct gator_cursesgwin {
    WINDOW *wp;		/*Window pointer*/
    int	charwidth;	/*Character width in pixels*/
    int	charheight;	/*Character height in pixels*/
    char box_vertchar;	/*Vertical char for boxing purposes*/
    char box_horizchar;	/*Horizontal char for boxing purposes*/
};

/*Curses gwin's creation parameters*/
struct gator_cursesgwin_params {
    struct gwin_createparams gwin_params;   /*Basic params for the window*/
    int	charwidth;			    /*Character width in pixels*/
    int	charheight;			    /*Character height in pixels*/
    char box_vertchar;			    /*Vertical char for boxing purposes*/
    char box_horizchar;			    /*Horizontal char for boxing purposes*/
};

/*Curses initialization routine*/

extern int gator_cursesgwin_init();
    /*
     * Summary:
     *    Initialize the curses window package.
     *
     * Args:
     *	  int adebug: Is debugging turned on?
     *
     * Returns:
     *	  0 on success,
     *	  Error value otherwise.
     */

/*Curses window's creation routine*/

extern struct gwin *gator_cursesgwin_create();
    /*
     * Summary:
     *    Create a curses window.
     *
     * Args:
     *	  struct gator_cursesgwin_params *params : Ptr to creation parameters.
     *
     * Returns:
     *	  Ptr to the created curses window structure if successful,
     *	  Error value otherwise.
     */

/*Curses cleanup routine*/

extern int gator_cursesgwin_cleanup();
    /*
     * Summary:
     *    Clean up after the curses window package.
     *
     * Args:
     *	  struct gwin *gwp : Ptr to base window.
     *
     * Returns:
     *	  0 on success,
     *	  Error value otherwise.
     */

extern struct gwinbaseops gator_curses_gwinbops;

/*Curses window's routines*/

extern int gator_cursesgwin_box();
    /*
     * Summary:
     *    Draw a box around the given curses window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the curses window to draw
     *	                            a box around.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_cursesgwin_clear();
    /*
     * Summary:
     *    Clear out the given curses window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the curses window to clear
     *	                            out.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_cursesgwin_destroy();
    /*
     * Summary:
     *    Destroy the given curses window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the curses window to destroy.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_cursesgwin_display();
    /*
     * Summary:
     *    Display/redraw the given curses window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the curses window to draw.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_cursesgwin_drawline();
    /*
     * Summary:
     *    Draw a line between two points in the given curses
     *    window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the curses window in which
     *	                            the line is to be drawn.
     *	 struct gwin_lineparams *params : Ptr to other params.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_cursesgwin_drawrectangle();
    /*
     * Summary:
     *    Draw a rectangle in the given curses window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the curses window in which
     *	                            the rectangle is to be drawn.
     *	 struct gwin_rectparams *params : Ptr to other params.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_cursesgwin_drawchar();
    /*
     * Summary:
     *    Draw a character in the given curses window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the curses window in which
     *	                            the character is to be drawn.
     *	 struct gwin_charparams *params : Ptr to other params.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_cursesgwin_drawstring();
    /*
     * Summary:
     *    Draw a string in the given curses window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the curses window in which
     *	                            the string is to be drawn.
     *	 struct gwin_strparams *params : Ptr to other params.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_cursesgwin_invert();
    /*
     * Summary:
     *    Invert a region in the given curses window.
     *
     * Args:
     *	 struct gwin *gwp : Ptr to the curses window in which
     *	                            the inverted region lies.
     *	 struct gwin_invparams *params : Ptr to other params.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_cursesgwin_getchar();

extern int gator_cursesgwin_getdimensions();

extern int gator_cursesgwin_wait();

#endif /* __gator_curseswindows_h */
