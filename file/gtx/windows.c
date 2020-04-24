/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: windows.c,v $
 * Revision 1.1.77.1  1996/10/02  17:51:39  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:40:26  damon]
 *
 * Revision 1.1.72.3  1994/08/03  16:15:59  ruby
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  17:02:28  ruby]
 * 
 * Revision 1.1.72.2  1994/06/09  14:10:48  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:56  annie]
 * 
 * Revision 1.1.72.1  1994/02/04  20:20:45  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:14:03  devsrc]
 * 
 * Revision 1.1.70.1  1993/12/07  17:26:37  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:34:57  jaffe]
 * 
 * Revision 1.1.5.3  1993/01/19  16:04:24  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:14:18  cjd]
 * 
 * Revision 1.1.5.2  1992/11/24  17:53:00  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  18:06:29  bolinger]
 * 
 * Revision 1.1.3.3  1992/01/24  03:45:45  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:13:54  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  19:50:17  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:34:43  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:17:35  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
*/
/*
 * (C) Copyright Transarc Corporation 1989
 * Licensed Maerials - Property of Transarc
 * All Rights Reserved.
 */

/*--------------------------------------------------------------------------------
 * gator_windows.c
 *
 * Description:
 *	Implementation of the gator windows interface.
 *
 * Author:
 *	Ed. Zayas
 *	Transarc Corporation
 *--------------------------------------------------------------------------------*/

#include <dcedfs/gtxwindows.h>	/*Interface for this module*/
#include <dcedfs/gtxcurseswin.h>	/*Interface for the curses module*/
#include <dcedfs/gtxdumbwin.h>	/*Interface for the dumb terminal module*/
#include <dcedfs/gtxX11win.h>	/*Interface for the X11 module*/
#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsgtxmac.h>
#include <dfsgtxsvc.h>
#include <dfsgtxmsg.h>

static char mn[] = "gator_windows";	/*Module name*/
struct gwinbaseops gwinbops;		/*Base window operation fn array*/
struct gwin gator_basegwin;		/*Base gator window*/

/*--------------------------------------------------------------------------------
 * gw_init
 *
 * Description:
 *	Initialize the gator window package.
 *
 * Arguments:
 *	struct gwin_initparams *params : Ptr to initialization params.
 *
 * Returns:
 *	0 on success,
 *	Error value otherwise.
 *
 * Environment:
 *	*** MUST BE THE FIRST ROUTINE CALLED FROM
 *	      THIS PACKAGE ***
 *
 * Side Effects:
 *	Sets up the chosen lower-level graphics package, as well
 *	as the base operation array (gwinbops).  Also sets up the
 *	base window.
 *--------------------------------------------------------------------------------*/

int gw_init(params)
    struct gwin_initparams *params;

{ /*gw_init*/

    static char	rn[] = "gw_init";   /*Routine name*/
    register int code;		    /*Return code*/
    int	gwin_debug;		    /*Is debugging turned on?*/

    /*
     * Remember our debugging level.
     */
    gwin_debug = params->i_debug;

    if (gwin_debug)
      dce_svc_printf(GTX_S_WN_DEBUGGING_ON_MSG, mn, rn);

    /*
     * What we do/call depends on the type of lower-level graphics
     * package we'll be using.
     */
    switch (params->i_type) {
	case GATOR_WIN_DUMB:	/*Dumb terminal*/
	    if (gwin_debug)
		dce_svc_printf(GTX_S_WN_INIT_DUMB_TERMINAL_MSG, mn, rn);
	    gwinbops = gator_dumb_gwinbops;
	    code = gator_dumbgwin_init(gwin_debug);
	    if (code) {
		dce_svc_printf(GTX_S_WN_ERROR_INIT_DUMB_TERMINAL_MSG, mn, rn, code);
		return(code);
	    }
	    break;

	case GATOR_WIN_CURSES:	/*Curses*/
	    if (gwin_debug)
		dce_svc_printf(GTX_S_WN_INIT_CURSES_MSG, mn, rn);
	    gwinbops = gator_curses_gwinbops;
	    code = gator_cursesgwin_init(gwin_debug);
	    if (code) {
		dce_svc_printf(GTX_S_WN_ERROR_INIT_CURSES_MSG, mn, rn, code);
		return(code);
	    }
	    break;

	case GATOR_WIN_X11:	/*X11*/
	    if (gwin_debug)
		dce_svc_printf(GTX_S_WN_INIT_X11_MSG, mn, rn);
	    gwinbops = gator_X11_gwinbops;
	    code = gator_X11gwin_init(params);
	    if (code) {
		dce_svc_printf(GTX_S_WN_ERROR_INIT_X11_MSG, mn, rn, code);
		return(code);
	    }
	    break;

	default:
	    dce_svc_printf(GTX_S_WN_ILLEGAL_GRAPHICS_MSG, mn, rn, params->i_type);
	    dce_svc_printf(GTX_S_WN_LEGAL_CHOICES_MSG);
	    dce_svc_printf(GTX_S_WN_DUMB_TERMINAL_MSG, GATOR_WIN_DUMB);
	    dce_svc_printf(GTX_S_WN_CURSES_MSG, GATOR_WIN_CURSES);
	    dce_svc_printf(GTX_S_WN_X11_MSG, GATOR_WIN_X11);
	    return(-1);
    } /*end switch (params->i_type)*/

    /*
     * Finally, return the good news.
     */
    return(0);

} /*gw_init*/
