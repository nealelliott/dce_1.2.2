/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dumbwindows.c,v $
 * Revision 1.1.67.1  1996/10/02  17:50:47  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:53  damon]
 *
 * Revision 1.1.62.3  1994/08/03  16:11:04  ruby
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  16:56:52  ruby]
 * 
 * Revision 1.1.62.2  1994/06/09  14:09:47  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:26  annie]
 * 
 * Revision 1.1.62.1  1994/02/04  20:20:24  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:13:54  devsrc]
 * 
 * Revision 1.1.60.1  1993/12/07  17:26:23  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:31:58  jaffe]
 * 
 * Revision 1.1.4.5  1993/01/21  19:38:47  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  19:52:53  zeliff]
 * 
 * Revision 1.1.4.4  1992/11/24  17:51:21  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  18:04:50  bolinger]
 * 
 * Revision 1.1.4.3  1992/09/15  13:14:24  jaffe
 * 	Transarc delta: jaffe-sync-with-aug-31-osf-sources 1.4
 * 	  Selected comments:
 * 	    Pick up changes from last submission to the OSF.
 * 	    remove errno definition, since its not used.
 * 	    remove commondefs.idl.  This was done incompletely in another delta
 * 	[1992/09/14  19:52:15  jaffe]
 * 
 * Revision 1.1.4.2  1992/08/31  19:58:34  jaffe
 * 	*** empty log message ***
 * 	[1992/08/31  15:19:18  jaffe]
 * 
 * Revision 1.1.2.3  1992/06/02  19:19:48  garyf
 * 	remove OSF/1 conditional
 * 	[1992/06/02  19:10:45  garyf]
 * 
 * Revision 1.1.2.2  1992/05/20  19:54:24  mason
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
 * 	[1992/05/20  11:34:55  mason]
 * 
 * Revision 1.1  1992/01/19  02:41:46  devrcs
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

/*------------------------------------------------------------------------
 * dumbwindows.c
 *
 * Description:
 *	Implementation of the gator dumb window facility.
 *
 * Author:
 *	Ed. Zayas
 *	Transarc Corporation
 *------------------------------------------------------------------------*/

#include <dcedfs/gtxdumbwin.h>			/*Interface definition*/
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

int dumb_debug;				/*Is debugging turned on?*/
static char mn[] = "gator_dumbwindows";	/*Module name*/

/*
 * Version of standard operations for a dumb window.
 */
struct gwinops dumb_gwinops = {
    gator_dumbgwin_box,
    gator_dumbgwin_clear,
    gator_dumbgwin_destroy,
    gator_dumbgwin_display,
    gator_dumbgwin_drawline,
    gator_dumbgwin_drawrectangle,
    gator_dumbgwin_drawchar,
    gator_dumbgwin_drawstring,
    gator_dumbgwin_invert,
    gator_dumbgwin_getchar,
    gator_dumbgwin_getdimensions,
    gator_dumbgwin_wait,
};

struct gwinbaseops gator_dumb_gwinbops = {
    gator_dumbgwin_create,
    gator_dumbgwin_cleanup
};

/*
 * Macros to map pixel positions to row & column positions.
 * (Note: for now, they are the identity function!!)
 */
#define	GATOR_MAP_X_TO_COL(w, x)    (x)
#define	GATOR_MAP_Y_TO_LINE(w, y)   (y)

/*------------------------------------------------------------------------
 * gator_dumbgwin_init
 *
 * Description:
 *	Initialize the dumb window package.
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

int gator_dumbgwin_init(adebug)
    int adebug;

{ /*gator_dumbgwin_init*/

    static char	rn[] = "gator_dumbgwin_init";	/*Routine name*/

    /*
     * Remember if we'll be doing debugging, init dumb and clear the
     * standard screen.
     */
    dumb_debug = adebug;

    if (dumb_debug)
	dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    /*
     * We always return success here.
     */
    return(0);

} /*gator_dumbgwin_init*/

/*------------------------------------------------------------------------
 * gator_dumbgwin_create
 *
 * Description:
 *	Create a dumb window.
 *
 * Arguments:
 *	struct gator_dumbgwin_params *params : Ptr to creation parameters.
 *
 * Returns:
 *	Ptr to the created dumb window if successful,
 *	Null ptr otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

struct gwin *gator_dumbgwin_create(params)
    struct gator_dumbgwin_params *params;

{ /*gator_dumbgwin_create*/

    static char	rn[] = "gator_dumbgwin_create";	/*Routine name*/

    if (dumb_debug)
	dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    /*
      * Return failure here, fill this routine in at some point.
      */
    return((struct gwin *)0);

} /*gator_dumbgwin_create*/

/*------------------------------------------------------------------------
 * gator_dumbgwin_cleanup
 *
 * Description:
 *	Create a dumb window.
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

int gator_dumbgwin_cleanup(gwp)
    struct gwin *gwp;

{ /*gator_dumbgwin_cleanup*/

    static char	rn[] = "gator_dumbgwin_cleanup";  /*Routine name*/

    if (dumb_debug)
	dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    /*
     * Return success here, fill this routine in at some point.
     */
    return(0);

} /*gator_dumbgwin_cleanup*/

/*------------------------------------------------------------------------
 * gator_dumbgwin_box
 *
 * Description:
 *	Draw a box around the given dumb window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the dumb window to draw
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

int gator_dumbgwin_box(gwp)
    struct gwin *gwp;

{ /*gator_dumbgwin_box*/

    static char	rn[] = "gator_dumbgwin_box";	/*Routine name*/

    if (dumb_debug)
	dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    /*
      * Return success here, fill in the routine at some point.
      */
    return(0);

} /*gator_dumbgwin_box*/

/*------------------------------------------------------------------------
 * gator_dumbgwin_clear
 *
 * Description:
 *	Clear out the given dumb window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the dumb window to clear out.
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

int gator_dumbgwin_clear(gwp)
    struct gwin *gwp;

{ /*gator_dumbgwin_clear*/

    static char	rn[] = "gator_dumbgwin_clear";	/*Routine name*/

    if (dumb_debug)
	dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    /*
      * Return success, fill in this routine at some point.
      */
    return(0);

} /*gator_dumbgwin_clear*/

/*------------------------------------------------------------------------
 * gator_dumbgwin_destroy
 *
 * Description:
 *	Destroy the given dumb window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the dumb window to destroy.
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

int gator_dumbgwin_destroy(gwp)
    struct gwin *gwp;

{ /*gator_dumbgwin_destroy*/

    static char	rn[] = "gator_dumbgwin_destroy";    /*Routine name*/

    if (dumb_debug)
	dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    return(0);

} /*gator_dumbgwin_destroy*/

/*------------------------------------------------------------------------
 * gator_dumbgwin_display
 *
 * Description:
 *	Display/redraw the given dumb window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the dumb window to draw.
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

int gator_dumbgwin_display(gwp)
    struct gwin *gwp;

{ /*gator_dumbgwin_display*/

    static char	rn[] = "gator_dumbgwin_display";    /*Routine name*/

    if (dumb_debug)
	dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    /*
      * Return success, fill in this routine at some point.
      */
    return(0);

} /*gator_dumbgwin_display*/

/*------------------------------------------------------------------------
 * gator_dumbgwin_drawline
 *
 * Description:
 *	Draw a line between two points in the given dumb
 *	window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the dumb window in which
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

int gator_dumbgwin_drawline(gwp, params)
    struct gwin *gwp;
    struct gwin_lineparams *params;

{ /*gator_dumbgwin_drawline*/

    static char	rn[] = "gator_dumbgwin_drawline"; /*Routine name*/

    if (dumb_debug)
      dce_svc_printf(GTX_S_ROUTINE_IS_NOOPED_MSG, mn, rn);

    /*
      * Return success, fill in this routine at some point.
      */
    return(0);

} /*gator_dumbgwin_drawline*/

/*------------------------------------------------------------------------
 * gator_dumbgwin_drawrectangle
 *
 * Description:
 *	Draw a rectangle in the given dumb window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the dumb window in which
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

int gator_dumbgwin_drawrectangle(gwp, params)
    struct gwin *gwp;
    struct gwin_rectparams *params;

{ /*gator_dumbgwin_drawrectangle*/

    static char	rn[] = "gator_dumbgwin_drawrectangle";    /*Routine name*/

    if (dumb_debug)
	dce_svc_printf(GTX_S_ROUTINE_IS_NOOPED_MSG, mn, rn);

    /*
      * Return success, fill in this routine at some point.
      */
    return(0);

} /*gator_dumbgwin_drawrectangle*/

/*------------------------------------------------------------------------
 * gator_dumbgwin_drawchar
 *
 * Description:
 *	Draw a character in the given dumb window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the dumb window in which
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

int gator_dumbgwin_drawchar(gwp, params)
    struct gwin *gwp;
    struct gwin_charparams *params;

{ /*gator_dumbgwin_drawchar*/

    static char	rn[] = "gator_dumbgwin_drawchar"; /*Routine name*/

    if (dumb_debug)
	dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    /*
      * Return success, fill in this routine at some point.
      */
    return(0);

} /*gator_dumbgwin_drawchar*/

/*------------------------------------------------------------------------
 * gator_dumbgwin_drawstring
 *
 * Description:
 *	Draw a string in the given dumb window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the dumb window in which
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

int gator_dumbgwin_drawstring(gwp, params)
    struct gwin *gwp;
    struct gwin_strparams *params;

{ /*gator_dumbgwin_drawstring*/

    static char	rn[] = "gator_dumbgwin_drawstring";	/*Routine name*/

    if (dumb_debug)
      dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    /*
      * Return success, fill in this routine at some point.
      */
    return(0);

} /*gator_dumbgwin_drawstring*/

/*------------------------------------------------------------------------
 * gator_dumbgwin_invert
 *
 * Description:
 *	Invert a region in the given dumb window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the dumb window in which
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

int gator_dumbgwin_invert(gwp, params)
    struct gwin *gwp;
    struct gwin_invparams *params;

{ /*gator_dumbgwin_invert*/

    static char	rn[] = "gator_dumbgwin_invert";   /*Routine name*/

    if (dumb_debug)
	dce_svc_printf(GTX_S_ROUTINE_IS_NOOPED_MSG, mn, rn);

    /*
      * Return success, fill in this routine at some point.
      */
    return(0);

} /*gator_dumbgwin_invert*/

/*------------------------------------------------------------------------
 * gator_dumbgwin_getchar
 *
 * Description:
 *	Pick up a character from the given window.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the dumb window to listen to.
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

int gator_dumbgwin_getchar(gwp)
    struct gwin *gwp;

{ /*gator_dumbgwin_getchar*/

    static char	rn[] = "gator_dumbgwin_getchar";   /*Routine name*/

    if (dumb_debug)
	dce_svc_printf(GTX_S_ROUTINE_IS_NOOPED_MSG, mn, rn);

    return(-1);

} /*gator_dumbgwin_getchar*/

/*------------------------------------------------------------------------
 * gator_dumbgwin_getdimensions
 *
 * Description:
 *	Get the window's X,Y dimensions.
 *
 * Arguments:
 *	struct gwin *gwp	       : Ptr to the dumb window to examine.
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

int gator_dumbgwin_getdimensions(gwp, aparms)
    struct gwin *gwp;
    struct gwin_sizeparams *aparms;

{/*gator_dumbgwin_getdimensions*/

    static char	rn[] = "gator_dumbgwin_getdimensions";   /*Routine name*/

    if (dumb_debug)
	dce_svc_printf(GTX_S_ROUTINE_IS_NOOPED_MSG, mn, rn);

    return(-1);

} /*gator_dumbgwin_getdimensions*/

/*------------------------------------------------------------------------
 * gator_dumbgwin_wait
 *
 * Description:
 *	Wait until input is available.
 *
 * Arguments:
 *	struct gwin *gwp : Ptr to the dumb window to wait on.
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

int gator_dumbgwin_wait(gwp)
    struct gwin *gwp;

{ /*gator_dumbgwin_wait*/

    static char	rn[] = "gator_dumbgwin_wait";   /*Routine name*/

    if (dumb_debug)
	dce_svc_printf(GTX_S_ROUTINE_IS_NOOPED_MSG, mn, rn);

    return(-1);

} /*gator_dumbgwin_wait*/
