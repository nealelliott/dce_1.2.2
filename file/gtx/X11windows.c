/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: X11windows.c,v $
 * Revision 1.1.75.1  1996/10/02  17:50:39  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:44  damon]
 *
 * Revision 1.1.70.3  1994/08/03  16:08:56  ruby
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  16:54:54  ruby]
 * 
 * Revision 1.1.70.2  1994/06/09  14:09:42  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:21  annie]
 * 
 * Revision 1.1.70.1  1994/02/04  20:20:19  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:13:52  devsrc]
 * 
 * Revision 1.1.68.1  1993/12/07  17:26:20  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:31:15  jaffe]
 * 
 * Revision 1.1.5.3  1993/01/21  19:38:32  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  19:52:38  zeliff]
 * 
 * Revision 1.1.5.2  1992/11/24  17:51:07  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  18:04:38  bolinger]
 * 
 * Revision 1.1.3.4  1992/05/20  19:54:12  mason
 * 	Transarc delta: vijay-ot3289-gtx-fix-lock-calls 1.2
 * 	  Files modified:
 * 	    gtx: Makefile, X11windows.c, dumbwindows.c, frame.c
 * 	    gtx: gtxtextcb.h, input.c, lightobject.c, objects.c
 * 	    gtx: textcb.c, textobject.c
 * 	  Selected comments:
 * 	    change the lock_ calls to afslk_ calls.
 * 	    fix a duplicate declaration
 * 	    remove a spurious cml diff line. This line was not deleted upon a merge
 * 	    and gave some compilations problems.
 * 	[1992/05/20  11:34:32  mason]
 * 
 * Revision 1.1.3.3  1992/01/24  03:45:32  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:13:30  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  19:49:38  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:34:12  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:17:11  devsrc
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
 * gator_X11windows.c
 *
 * Description:
 *	Implementation of the gator X11 window facility.
 *
 * Author:
 *	Ed. Zayas
 *	Transarc Corporation
 *------------------------------------------------------------------------*/

#include <dcedfs/gtxX11win.h>		/*Interface definition*/
#include <stdio.h>			/*Standard I/O package*/

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsgtxmac.h>
#include <dfsgtxsvc.h>
#include <dfsgtxmsg.h>

int X11_debug;				/*Is debugging turned on?*/
static char mn[] = "gator_X11windows";	/*Module name*/

/*
 * Version of standard operations for a X11 window.
 */
struct gwinops X11_gwinops = {
    gator_X11gwin_box,
    gator_X11gwin_clear,
    gator_X11gwin_destroy,
    gator_X11gwin_display,
    gator_X11gwin_drawline,
    gator_X11gwin_drawrectangle,
    gator_X11gwin_drawchar,
    gator_X11gwin_drawstring,
    gator_X11gwin_invert,
    gator_X11gwin_getchar,
    gator_X11gwin_getdimensions,
    gator_X11gwin_wait,
};

struct gwinbaseops gator_X11_gwinbops = {
    gator_X11gwin_create,
    gator_X11gwin_cleanup
};

/*
 * Macros to map pixel positions to row & column positions.
 * (Note: for now, they are the identity function!!)
 */
#define	GATOR_MAP_X_TO_COL(w, x)    (x)
#define	GATOR_MAP_Y_TO_LINE(w, y)   (y)

/*------------------------------------------------------------------------
 * gator_X11gwin_init
 *
 * Description:
 *	Initialize the X11 window package.
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

int gator_X11gwin_init(adebug)
    int adebug;

{ /*gator_X11gwin_init*/

    static char	rn[] = "gator_X11gwin_init";	/*Routine name*/

    /*
     * Remember if we'll be doing debugging, init X11 and clear the
     * standard screen.
     */
    X11_debug = adebug;

    if (X11_debug)
	dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    /*
     * We return success, fill this routine it at some point.
     */
    return(0);

} /*gator_X11gwin_init*/

/*------------------------------------------------------------------------
 * gator_X11gwin_create
 *
 * Description:
 *	Create a X11 window.
 *
 * Arguments:
 *	struct gator_X11gwin_params *params : Ptr to creation parameters.
 *
 * Returns:
 *	Ptr to the created X11 window if successful,
 *	Null ptr otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

struct gwin *gator_X11gwin_create(params)
    struct gator_X11gwin_params *params;

{ /*gator_X11gwin_create*/

    static char	rn[] = "gator_X11gwin_create";	/*Routine name*/

    if (X11_debug)
	dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    return((struct gwin *)0);

} /*gator_X11gwin_create*/

/*------------------------------------------------------------------------
 * gator_X11gwin_cleanup
 *
 * Description:
 *	Create a X11 window.
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

int gator_X11gwin_cleanup(gwp)
    struct gwin *gwp;

{ /*gator_X11gwin_cleanup*/

    static char	rn[] = "gator_X11gwin_cleanup";  /*Routine name*/

    if (X11_debug)
	dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    return(0);

} /*gator_X11gwin_cleanup*/

/*------------------------------------------------------------------------
 * gator_X11gwin_box
 *
 * Description:
 *	Draw a box around the given X11 window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the X11 window to draw
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

int gator_X11gwin_box(gwp)
    struct gwin *gwp;

{ /*gator_X11gwin_box*/

    static char	rn[] = "gator_X11gwin_box";	/*Routine name*/

    if (X11_debug)
	dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    return(0);

} /*gator_X11gwin_box*/

/*------------------------------------------------------------------------
 * gator_X11gwin_clear
 *
 * Description:
 *	Clear out the given X11 window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the X11 window to clear out.
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

int gator_X11gwin_clear(gwp)
    struct gwin *gwp;

{ /*gator_X11gwin_clear*/

    static char	rn[] = "gator_X11gwin_clear";    /*Routine name*/

    if (X11_debug)
	dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    return(0);

} /*gator_X11gwin_clear*/

/*------------------------------------------------------------------------
 * gator_X11gwin_destroy
 *
 * Description:
 *	Destroy the given X11 window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the X11 window to destroy.
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

int gator_X11gwin_destroy(gwp)
    struct gwin *gwp;

{ /*gator_X11gwin_destroy*/

    static char	rn[] = "gator_X11gwin_destroy";  /*Routine name*/

    if (X11_debug)
	dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    return(0);

} /*gator_X11gwin_destroy*/

/*------------------------------------------------------------------------
 * gator_X11gwin_display
 *
 * Description:
 *	Display/redraw the given X11 window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the X11 window to draw.
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

int gator_X11gwin_display(gwp)
    struct gwin *gwp;

{ /*gator_X11gwin_display*/

    static char	rn[] = "gator_X11gwin_display";  /*Routine name*/

    if (X11_debug)
	dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    return(0);

} /*gator_X11gwin_display*/

/*------------------------------------------------------------------------
 * gator_X11gwin_drawline
 *
 * Description:
 *	Draw a line between two points in the given X11
 *	window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the X11 window in which
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

int gator_X11gwin_drawline(gwp, params)
    struct gwin *gwp;
    struct gwin_lineparams *params;

{ /*gator_X11gwin_drawline*/

    static char	rn[] = "gator_X11gwin_drawline"; /*Routine name*/

    if (X11_debug)
	dce_svc_printf(GTX_S_ROUTINE_IS_NOOPED_MSG, mn, rn);

    return(0);

} /*gator_X11gwin_drawline*/

/*------------------------------------------------------------------------
 * gator_X11gwin_drawrectangle
 *
 * Description:
 *	Draw a rectangle in the given X11 window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the X11 window in which
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

int gator_X11gwin_drawrectangle(gwp, params)
    struct gwin *gwp;
    struct gwin_rectparams *params;

{ /*gator_X11gwin_drawrectangle*/

    static char	rn[] = "gator_X11gwin_drawrectangle";    /*Routine name*/

    if (X11_debug)
	dce_svc_printf(GTX_S_ROUTINE_IS_NOOPED_MSG, mn, rn);

    return(0);

} /*gator_X11gwin_drawrectangle*/

/*------------------------------------------------------------------------
 * gator_X11gwin_drawchar
 *
 * Description:
 *	Draw a character in the given X11 window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the X11 window in which
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

int gator_X11gwin_drawchar(gwp, params)
    struct gwin *gwp;
    struct gwin_charparams *params;

{ /*gator_X11gwin_drawchar*/

    static char	rn[] = "gator_X11gwin_drawchar"; /*Routine name*/

    if (X11_debug)
	dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    return(0);

} /*gator_X11gwin_drawchar*/

/*------------------------------------------------------------------------
 * gator_X11gwin_drawstring
 *
 * Description:
 *	Draw a string in the given X11 window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the X11 window in which
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

int gator_X11gwin_drawstring(gwp, params)
    struct gwin *gwp;
    struct gwin_strparams *params;

{ /*gator_X11gwin_drawstring*/

    static char	rn[] = "gator_X11gwin_drawstring";	/*Routine name*/

    if (X11_debug)
	dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    return(0);

} /*gator_X11gwin_drawstring*/

/*------------------------------------------------------------------------
 * gator_X11gwin_invert
 *
 * Description:
 *	Invert a region in the given X11 window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the X11 window in which
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

int gator_X11gwin_invert(gwp, params)
    struct gwin *gwp;
    struct gwin_invparams *params;

{ /*gator_X11gwin_invert*/

    static char	rn[] = "gator_X11gwin_invert";   /*Routine name*/

    if (X11_debug)
	dce_svc_printf(GTX_S_ROUTINE_IS_NOOPED_MSG, mn, rn);

    return(0);

} /*gator_X11gwin_invert*/

/*------------------------------------------------------------------------
 * gator_X11gwin_getchar
 *
 * Description:
 *	Pick up a character from the given window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the X11 window to listen to.
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

int gator_X11gwin_getchar(gwp)
    struct gwin *gwp;

{ /*gator_X11gwin_getchar*/

    static char	rn[] = "gator_X11gwin_getchar";   /*Routine name*/

    if (X11_debug)
	dce_svc_printf(GTX_S_ROUTINE_IS_NOOPED_MSG, mn, rn);

    return(-1);

} /*gator_X11gwin_getchar*/

/*------------------------------------------------------------------------
 * gator_X11gwin_getdimensions
 *
 * Description:
 *	Get the window's X,Y dimensions.
 *
 * Arguments:
 *	struct gwin *gwp	       : Ptr to the X11 window to examine.
 *	struct gwin_sizeparams *aparms : Ptr to size params to set.
 *
 * Returns:
 *	0 if successful,
 *	-1 otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_X11gwin_getdimensions(gwp, aparms)
    struct gwin *gwp;
    struct gwin_sizeparams *aparms;

{/*gator_X11gwin_getdimensions*/

    static char	rn[] = "gator_X11gwin_getdimensions";   /*Routine name*/

    if (X11_debug)
	dce_svc_printf(GTX_S_ROUTINE_IS_NOOPED_MSG, mn, rn);

    return(-1);

} /*gator_X11gwin_getdimensions*/

/*------------------------------------------------------------------------
 * gator_X11gwin_wait
 *
 * Description:
 *	Wait until input is available.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the X11 window to wait on.
 *
 * Returns:
 *	0 if successful,
 *	-1 otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_X11gwin_wait(gwp)
    struct gwin *gwp;

{ /*gator_X11gwin_wait*/

    static char	rn[] = "gator_X11gwin_wait";   /*Routine name*/

    if (X11_debug)
	dce_svc_printf(GTX_S_ROUTINE_IS_NOOPED_MSG, mn, rn);

    return(-1);

} /*gator_X11gwin_wait*/
