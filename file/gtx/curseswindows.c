/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: curseswindows.c,v $
 * Revision 1.1.76.1  1996/10/02  17:50:44  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:51  damon]
 *
 * Revision 1.1.71.3  1994/08/03  16:10:28  ruby
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  16:56:16  ruby]
 * 
 * Revision 1.1.71.2  1994/06/09  14:09:46  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:24  annie]
 * 
 * Revision 1.1.71.1  1994/02/04  20:20:21  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:13:53  devsrc]
 * 
 * Revision 1.1.69.1  1993/12/07  17:26:21  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:31:36  jaffe]
 * 
 * Revision 1.1.5.3  1993/01/21  19:38:43  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  19:52:48  zeliff]
 * 
 * Revision 1.1.5.2  1992/11/24  17:51:17  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  18:04:44  bolinger]
 * 
 * Revision 1.1.3.3  1992/01/24  03:45:34  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:13:34  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  19:49:44  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:34:17  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:17:15  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
*/
/*
 * (C) Copyright Transarc Corporation 1989
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

/*------------------------------------------------------------------------
 * gator_curseswindows.c
 *
 * Description:
 *	Implementation of the gator curses window facility.
 *
 * Author:
 *	Ed. Zayas
 *	Transarc Corporation
 *------------------------------------------------------------------------*/

#include <dcedfs/param.h>
#include <curses.h>			    /*Curses library*/
#ifdef sys_aix_22
#define WINDOW struct _win_st
#endif
#include <sys/types.h>
#include <sys/file.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <pthread.h>

#include <dcedfs/gtxcurseswin.h>		/*Interface definition*/
#include <dcedfs/gtxobjects.h>
#include <dcedfs/gtxframe.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsgtxmac.h>
#include <dfsgtxsvc.h>
#include <dfsgtxmsg.h>

int curses_debug;			    /*Is debugging turned on?*/
static char mn[] = "gator_curseswindows";   /*Module name*/

/*
 * Version of standard operations for a curses window.
 */
struct gwinops curses_gwinops = {
    gator_cursesgwin_box,
    gator_cursesgwin_clear,
    gator_cursesgwin_destroy,
    gator_cursesgwin_display,
    gator_cursesgwin_drawline,
    gator_cursesgwin_drawrectangle,
    gator_cursesgwin_drawchar,
    gator_cursesgwin_drawstring,
    gator_cursesgwin_invert,
    gator_cursesgwin_getchar,
    gator_cursesgwin_getdimensions,
    gator_cursesgwin_wait,
};

struct gwinbaseops gator_curses_gwinbops = {
    gator_cursesgwin_create,
    gator_cursesgwin_cleanup,
};


/*
 * Macros to map pixel positions to row & column positions.
 * (Note: for now, they are the identity function!!)
 */
#define	GATOR_MAP_X_TO_COL(w, x)    (x)
#define	GATOR_MAP_Y_TO_LINE(w, y)   (y)

/*------------------------------------------------------------------------
 * gator_cursesgwin_init
 *
 * Description:
 *	Initialize the curses window package.
 *
 * Arguments:
 *	int adebug: Is debugging turned on?
 *
 * Returns:
 *	0 on success,
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_cursesgwin_init(adebug)
    int adebug;

{ /*gator_cursesgwin_init*/

    static char	rn[] = "gator_cursesgwin_init";	/*Routine name*/
    struct gator_cursesgwin *c_data;		/*Ptr to curses-specific data*/

    /*
     * Remember if we'll be doing debugging, then init the curses package.
     */
    curses_debug = adebug;

    if (curses_debug)
	dce_svc_printf(GTX_S_CD_CALLING_INITSCR_MSG, mn, rn);
    initscr();

    /*
     * Fill out the base window structure for curses.
     */
    if (curses_debug)
	dce_svc_printf(GTX_S_CD_ALLOCATE_CURSES_BYTES_MSG, mn, rn, sizeof(struct gator_cursesgwin));
    c_data = (struct gator_cursesgwin *) malloc(sizeof(struct gator_cursesgwin));
    if (c_data == (struct gator_cursesgwin *)0){
	dce_svc_printf(GTX_S_CD_NO_CURSES_BYTES_MSG, mn, rn, sizeof(struct gator_cursesgwin));
	return(-1);
    }

    /*
     * Fill in the curses-specific base window info.  We assume that chars are 8x13.
     */
    c_data->wp		  = stdscr;
    c_data->charwidth	  = 8;
    c_data->charheight	  = 13;
    c_data->box_vertchar  = '|';
    c_data->box_horizchar = '-';

    /*
     * Fill in the generic base window info.
     */
    gator_basegwin.w_type    = GATOR_WIN_CURSES;
    gator_basegwin.w_x	     = 0;
    gator_basegwin.w_y	     = 0;
    gator_basegwin.w_width   = c_data->charwidth  * COLS;
    gator_basegwin.w_height  = c_data->charheight * LINES;
    gator_basegwin.w_changed = 0;
    gator_basegwin.w_op	     = &curses_gwinops;
    gator_basegwin.w_parent  = (struct gwin *)0;

    /*
     * Plug the private data into the generic part of the base window.
     */
    gator_basegwin.w_data = (int *)c_data;

    /*
     * Now, set the terminal into the right mode for handling input
     */
    raw();	/* curses raw mode */

    /* init the frame */
    gator_basegwin.w_frame = gtxframe_Create();

    /*
     * Clear out the screen and return the good news.
     */
    wclear(((struct gator_cursesgwin *)(gator_basegwin.w_data))->wp);
    return(0);

} /*gator_cursesgwin_init*/

/*------------------------------------------------------------------------
 * gator_cursesgwin_create
 *
 * Description:
 *	Create a curses window (incorrectly).
 *
 * Arguments:
 *	struct gator_cursesgwin_params *params : Ptr to creation parameters.
 *
 * Returns:
 *	Ptr to the created curses window if successful,
 *	Null ptr otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

struct gwin *gator_cursesgwin_create(params)
    struct gator_cursesgwin_params *params;

{ /*gator_cursesgwin_create*/

    static char	rn[] = "gator_cursesgwin_create";   /*Routine name*/
    struct gwin	*newgwin;			    /*Ptr to new curses window*/
    struct gator_cursesgwin *c_data;		    /*Ptr to curses-specific data*/
    WINDOW *newcursgwin;			    /*Ptr to new curses window*/

    if (curses_debug)
	dce_svc_printf(GTX_S_CD_ALLOCATING_BYTES_FOR_GWIN_MSG, mn, rn, sizeof(struct gwin));
    newgwin = (struct gwin *) malloc(sizeof(struct gwin));
    if (newgwin == (struct gwin *)0) {
	dce_svc_printf(GTX_S_CD_CANT_ALLOCATE_BYTES_FOR_GWIN_MSG, mn, rn, sizeof(struct gwin), errno);
	return((struct gwin *)0);
    }

    newgwin->w_type	= GATOR_WIN_CURSES;
    newgwin->w_x	= params->gwin_params.cr_x;
    newgwin->w_y	= params->gwin_params.cr_y;
    newgwin->w_width	= params->gwin_params.cr_width;
    newgwin->w_height	= params->gwin_params.cr_height;
    newgwin->w_changed	= 1;
    newgwin->w_op	= &curses_gwinops;
    newgwin->w_parent	= params->gwin_params.cr_parentwin;

    if (curses_debug)
	dce_svc_printf(GTX_S_CD_ALLOCATE_PRIVATE_CURSES_MSG, mn, rn, sizeof(struct gator_cursesgwin));
    c_data = (struct gator_cursesgwin *) malloc(sizeof(struct gator_cursesgwin));
    if (c_data == (struct gator_cursesgwin *)0){
	dce_svc_printf(GTX_S_CD_CANT_ALLOCATE_PRIVATE_CURSES_MSG, mn, rn, sizeof(struct gator_cursesgwin));
	free(newgwin);
	return((struct gwin *)0);
    }

    newcursgwin	= newwin(newgwin->w_height,	/*Number of lines*/
			  newgwin->w_width,	/*Number of columns*/
			  newgwin->w_y,		/*Beginning y value*/
			  newgwin->w_x);	/*Beginning x value*/
    if (newcursgwin == (WINDOW *)0) {
	dce_svc_printf(GTX_S_CD_NEWIN_FAILED_MSG, mn, rn);
	free(newgwin);
	free(c_data);
	return((struct gwin *)0);
    }

    /*
     * Now, fill in the curses-specific window info.
     */
    c_data->wp		  = newcursgwin;
    c_data->charwidth	  = params->charwidth;
    c_data->charheight	  = params->charheight;
    c_data->box_vertchar  = params->box_vertchar;
    c_data->box_horizchar = params->box_horizchar;

    /*
     * Plug in a frame at the top-level.
     */
    newgwin->w_frame = gtxframe_Create();

    /*
     * Plug the curses private data into the generic window object, then
     * return the new window's info.
     */
    newgwin->w_data = (int *) c_data;
    return(newgwin);

} /*gator_cursesgwin_create*/

/*------------------------------------------------------------------------
 * gator_cursesgwin_cleanup
 *
 * Description:
 *	Clean up, probably right before the caller exits.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to base window.
 *
 * Returns:
 *	0 on success,
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_cursesgwin_cleanup(gwp)
    struct gwin *gwp;

{ /*gator_cursesgwin_cleanup*/

    static char	rn[] = "gator_cursesgwin_cleanup";  /*Routine name*/
    struct gator_cursesgwin *cwp;		    /*Curses private area ptr*/

    cwp = (struct gator_cursesgwin *)(gwp->w_data);

    /*
     * Cleaning up in curses is extremely easy - one simple call.  We also
     * want to clear the screen before we go.
     */
    if (curses_debug)
	dce_svc_printf(GTX_S_CD_CALLING_WCLEAR_MSG, mn, rn, cwp->wp);
    wclear(cwp->wp);
    wrefresh(cwp->wp);

    /*
     * Now, set the terminal back into normal mode.
     */
    noraw();

    if (curses_debug)
	dce_svc_printf(GTX_S_CD_CALLING_ENDWIN_MSG, mn, rn);
    endwin();

    return(0);

} /*gator_cursesgwin_cleanup*/

/*------------------------------------------------------------------------
 * gator_cursesgwin_box
 *
 * Description:
 *	Draw a box around the given curses window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the curses window to draw
 *	                   a box around.
 *
 * Returns:
 *	0 on success,
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_cursesgwin_box(gwp)
    struct gwin *gwp;

{ /*gator_cursesgwin_box*/

    static char	rn[] = "gator_cursesgwin_box";	/*Routine name*/
    struct gator_cursesgwin *cwp;		/*Ptr to curses private area*/

    cwp = (struct gator_cursesgwin *)(gwp->w_data);
    if (curses_debug)
	dce_svc_printf(GTX_S_CD_CALLING_BOX_MSG, mn, rn, cwp->wp);
    box(cwp->wp, cwp->box_vertchar, cwp->box_horizchar);

    return(0);

} /*gator_cursesgwin_box*/

/*------------------------------------------------------------------------
 * gator_cursesgwin_clear
 *
 * Description:
 *	Clear out the given curses window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the curses window to clear out.
 *
 * Returns:
 *	0 on success,
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_cursesgwin_clear(gwp)
    struct gwin *gwp;

{ /*gator_cursesgwin_clear*/

    static char	rn[] = "gator_cursesgwin_clear";    /*Routine name*/
    struct gator_cursesgwin *cwp;		    /*Ptr to curses private area*/

    /*
     * Clearing windows is very easy in curses; just one call will do it.
     */
    cwp = (struct gator_cursesgwin *)(gwp->w_data);
    if (curses_debug)
	dce_svc_printf(GTX_S_CD_CALLING_WCLEAR_MSG, mn, rn, cwp->wp);
    wclear(cwp->wp);

    return(0);

} /*gator_cursesgwin_clear*/

/*------------------------------------------------------------------------
 * gator_cursesgwin_destroy
 *
 * Description:
 *	Destroy the given curses window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the curses window to destroy.
 *
 * Returns:
 *	0 on success,
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_cursesgwin_destroy(gwp)
    struct gwin *gwp;

{ /*gator_cursesgwin_destroy*/

    static char	rn[] = "gator_cursesgwin_destroy";  /*Routine name*/
    struct gator_cursesgwin *cwp;		    /*Ptr to curses private area*/

    cwp = (struct gator_cursesgwin *)(gwp->w_data);
    if (curses_debug)
	dce_svc_printf(GTX_S_CD_CALLING_DELWIN_MSG, mn, rn, cwp->wp);
    delwin(cwp->wp);

    return(0);

} /*gator_cursesgwin_destroy*/

/*------------------------------------------------------------------------
 * gator_cursesgwin_display
 *
 * Description:
 *	Display/redraw the given curses window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the curses window to draw.
 *
 * Returns:
 *	0 on success,
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_cursesgwin_display(gwp)
    struct gwin *gwp;

{ /*gator_cursesgwin_display*/

    struct gator_cursesgwin *cwp;	/*Curses private area ptr*/

    cwp = (struct gator_cursesgwin *)(gwp->w_data);

    wclear(cwp->wp);	/* clear screen */
    gtxframe_Display(gwp->w_frame, gwp);	/* display the frame */
    wrefresh(cwp->wp);	/* redraw the guy */
    return(0);

} /*gator_cursesgwin_display*/

/*------------------------------------------------------------------------
 * gator_cursesgwin_drawline
 *
 * Description:
 *	Draw a line between two points in the given curses
 *	window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the curses window in which
 *	                           the line is to be drawn.
 *	struct gwin_lineparams *params : Ptr to other params.
 *
 * Returns:
 *	0 on success,
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_cursesgwin_drawline(gwp, params)
    struct gwin *gwp;
    struct gwin_lineparams *params;

{ /*gator_cursesgwin_drawline*/

    static char	rn[] = "gator_cursesgwin_drawline"; /*Routine name*/

    if (curses_debug)
	dce_svc_printf(GTX_S_ROUTINE_IS_NOOPED_MSG, mn, rn);

    return(0);

} /*gator_cursesgwin_drawline*/

/*------------------------------------------------------------------------
 * gator_cursesgwin_drawrectangle
 *
 * Description:
 *	Draw a rectangle in the given curses window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the curses window in which
 *	                           the rectangle is to be drawn.
 *	struct gwin_rectparams *params : Ptr to other params.
 *
 * Returns:
 *	0 on success,
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_cursesgwin_drawrectangle(gwp, params)
    struct gwin *gwp;
    struct gwin_rectparams *params;

{ /*gator_cursesgwin_drawrectangle*/

    static char	rn[] = "gator_cursesgwin_drawrectangle";    /*Routine name*/

    if (curses_debug)
	dce_svc_printf(GTX_S_ROUTINE_IS_NOOPED_MSG, mn, rn);

    return(0);

} /*gator_cursesgwin_drawrectangle*/

/*------------------------------------------------------------------------
 * gator_cursesgwin_drawchar
 *
 * Description:
 *	Draw a character in the given curses window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the curses window in which
 *	                           the character is to be drawn.
 *	struct gwin_charparams *params : Ptr to other params.
 *
 * Returns:
 *	0 on success,
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_cursesgwin_drawchar(gwp, params)
    struct gwin *gwp;
    struct gwin_charparams *params;

{ /*gator_cursesgwin_drawchar*/

    static char	rn[] = "gator_cursesgwin_drawchar"; /*Routine name*/
    struct gator_cursesgwin *cwp;		    /*Ptr to curses private area*/
    int	curses_x, curses_y;			    /*Mapped x,y positions*/

    cwp = (struct gator_cursesgwin *)(gwp->w_data);
    curses_x = GATOR_MAP_X_TO_COL(cwp, params->x);
    curses_y = GATOR_MAP_Y_TO_LINE(cwp, params->y);
    if (curses_debug)
	dce_svc_printf(GTX_S_CD_DRAWING_CHAR_MSG, mn, rn, params->c, cwp->wp, params->x, params->y, curses_y, curses_x, (params->highlight ? ", using standout mode" : ""));
    wmove(cwp->wp, curses_y, curses_x);
    if (params->highlight)
	wstandout(cwp->wp);
    waddch(cwp->wp, params->c);
    if (params->highlight)
	wstandend(cwp->wp);

    return(0);

} /*gator_cursesgwin_drawchar*/

/*------------------------------------------------------------------------
 * gator_cursesgwin_drawstring
 *
 * Description:
 *	Draw a string in the given curses window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the curses window in which
 *	                           the string is to be drawn.
 *	struct gwin_strparams *params : Ptr to other params.
 *
 * Returns:
 *	0 on success,
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_cursesgwin_drawstring(gwp, params)
    struct gwin *gwp;
    struct gwin_strparams *params;

{ /*gator_cursesgwin_drawstring*/

    static char	rn[] = "gator_cursesgwin_drawstring";	/*Routine name*/
    struct gator_cursesgwin *cwp;			/*Ptr to curses private area*/
    int	curses_x, curses_y;				/*Mapped x,y positions*/

    cwp = (struct gator_cursesgwin *)(gwp->w_data);
    curses_x = GATOR_MAP_X_TO_COL(cwp, params->x);
    curses_y = GATOR_MAP_Y_TO_LINE(cwp, params->y);
    if (curses_debug)
	dce_svc_printf(GTX_S_CD_DRAWING_STRING_MSG, mn, rn, params->s, cwp->wp, params->x, params->y, curses_y, curses_x, (params->highlight ? ", using standout mode" : ""));
    wmove(cwp->wp, curses_y, curses_x);
    if (params->highlight)
	wstandout(cwp->wp);
    waddstr(cwp->wp, params->s);
    if (params->highlight)
	wstandend(cwp->wp);

    return(0);

} /*gator_cursesgwin_drawstring*/

/*------------------------------------------------------------------------
 * gator_cursesgwin_invert
 *
 * Description:
 *	Invert a region in the given curses window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the curses window in which
 *	                           the inverted region lies.
 *	struct gwin_invparams *params : Ptr to other params.
 *
 * Returns:
 *	0 on success,
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_cursesgwin_invert(gwp, params)
    struct gwin *gwp;
    struct gwin_invparams *params;

{ /*gator_cursesgwin_invert*/

    static char	rn[] = "gator_cursesgwin_invert";   /*Routine name*/

    if (curses_debug)
	dce_svc_printf(GTX_S_ROUTINE_IS_NOOPED_MSG, mn, rn);

    return(0);

} /*gator_cursesgwin_invert*/

/*------------------------------------------------------------------------
 * gator_cursesgwin_getchar
 *
 * Description:
 *	Pick up a character from the given window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the curses window to listen to.
 *
 * Returns:
 *	Value of the character read,
 *	-1 otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_cursesgwin_getchar(gwp)
    struct gwin *gwp;

{ /*gator_cursesgwin_getchar*/

    return(getc(stdin));

} /*gator_cursesgwin_getchar*/

/*------------------------------------------------------------------------
 * gator_cursesgwin_wait
 *
 * Description:
 *	Wait until input is available.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the curses window to wait on.
 *
 * Returns:
 *	0 on success,
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_cursesgwin_wait(gwp)
    struct gwin *gwp;

{ /*gator_cursesgwin_wait*/

    register long code;
    fd_set imask, omask, xmask;
    int fd = fileno(stdin);

    /*
      * If there are already characters waiting, return right away.
     * XXX FIX ME FIX ME XXX
      */
    if (stdin->_cnt > 0)
      return(0);

    /*
      * We have to wait until the stdin file descriptor has input
      * available.
      */
    while (1) {
	FD_ZERO(&omask);
	FD_ZERO(&xmask);
	FD_ZERO(&imask);
	FD_SET(fd, &imask);
	code = select(1 + fd, &imask, &omask, &xmask, (struct timeval *) 0);
	if (code >= 0 && FD_ISSET(fd, &imask))
	  break;
    } /*Wait loop*/

    return(0);

} /*gator_cursesgwin_wait*/

/*------------------------------------------------------------------------
 * gator_cursesgwin_getdimensions
 *
 * Description:
 *	Get the window's X,Y dimensions.
 *
 * Arguments:
 *	struct gwin *gwp	       : Ptr to the curses window to examine.
 *	struct gwin_sizeparams *params : Ptr to the size params to set.
 *
 * Returns:
 *	0 on success,
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_cursesgwin_getdimensions(gwp, aparms)
    struct gwin_sizeparams *aparms;
    struct gwin *gwp;

{ /*gator_cursesgwin_getdimensions*/

    struct gator_cursesgwin *cwp;	/*Curses-specific data*/

    cwp = (struct gator_cursesgwin *)(gwp->w_data);
    aparms->maxx = cwp->wp->_maxx;
    aparms->maxy = cwp->wp->_maxy;

    return(0);

} /*gator_cursesgwin_getdimensions*/
