/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: input.c,v $
 * Revision 1.1.12.1  1996/10/02  17:51:23  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:40:08  damon]
 *
 * Revision 1.1.7.2  1994/08/03  16:13:06  ruby
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  16:58:07  ruby]
 * 
 * Revision 1.1.7.1  1994/06/09  14:10:24  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:45  annie]
 * 
 * Revision 1.1.5.4  1993/01/19  16:03:37  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:13:27  cjd]
 * 
 * Revision 1.1.5.3  1993/01/13  18:08:00  shl
 * 	Transarc delta: jaffe-cleanup-2.3-compiler-warnings 1.8
 * 	  Selected comments:
 * 	    Fix compiler warnings found when moving to OSF/1 1.1.1.
 * 	    call pthread_create with the correct args.
 * 	    Remove the function dacl_SetOwnerControl.  It is not used.
 * 	[1993/01/12  21:05:58  shl]
 * 
 * Revision 1.1.5.2  1992/11/24  17:52:17  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  18:05:47  bolinger]
 * 
 * Revision 1.1.3.4  1992/05/20  19:54:52  mason
 * 	Transarc delta: vijay-ot3289-gtx-fix-lock-calls 1.2
 * 	  Files modified:
 * 	    gtx: Makefile, X11windows.c, dumbwindows.c, frame.c
 * 	    gtx: gtxtextcb.h, input.c, lightobject.c, objects.c
 * 	    gtx: textcb.c, textobject.c
 * 	  Selected comments:
 * 	    change the lock_ calls to afslk_ calls.
 * 	    remove a lwp.h include
 * 	    remove a spurious cml diff line. This line was not deleted upon a merge
 * 	    and gave some compilations problems.
 * 	[1992/05/20  11:35:55  mason]
 * 
 * Revision 1.1.3.3  1992/01/24  03:45:41  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:13:47  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  19:50:07  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:34:35  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:17:28  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
*/
#include <pthread.h>

#include <dcedfs/gtxobjects.h>
#include <dcedfs/gtxwindows.h>
#include <dcedfs/gtxcurseswin.h>
#include <dcedfs/gtxinput.h>
#include <dcedfs/gtxkeymap.h>
#include <dcedfs/gtxframe.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsgtxmac.h>
#include <dfsgtxsvc.h>
#include <dfsgtxmsg.h>

/* define a servicability handle for the dce_svc_printf facility */

DCE_SVC_DEFINE_HANDLE(gtx_svc_handle, gtx_svc_table, "gtx")

/* process input */
pthread_startroutine_t gtx_InputServer(arg)
pthread_addr_t arg;
{
    register struct gwin *awin = (struct gwin *)arg;
    register int tc;
    register long code;
    register struct gtx_frame *tframe;

    WOP_DISPLAY(awin);	/* start off with a clean display */
    while (1) {
	/* get a character from the generic window */
	tframe = awin->w_frame;
	code = WOP_WAIT(awin);
	if (code) {
	    dce_svc_printf(GTX_S_IN_WAIT_FAILURE_MSG, code);
	    exit(1);
	}
	tc = WOP_GETCHAR(awin);
	tframe->flags &= ~GTXFRAME_NEWDISPLAY;	/* OK to clear now */
	if (tc < 0) break;	/* EOF or some such */
	/* otherwise, process the character and go get a new one */
	gtxframe_ClearMessageLine(tframe);
	tframe->flags &= ~(GTXFRAME_RECURSIVEEND | GTXFRAME_RECURSIVEERR);
	keymap_ProcessKey(tframe->keystate, tc, awin);
	tframe = awin->w_frame;	/* in case command changed it */
	if (tframe->flags & GTXFRAME_RECURSIVEEND) {
	    tframe->flags &= ~GTXFRAME_RECURSIVEEND;
	    return 0;
	}
	tframe->flags &= ~GTXFRAME_RECURSIVEEND;
	WOP_DISPLAY(awin);	/* eventually calls gtxframe_Display */
    }
}


struct gwin *gtx_Init(astartInput, atype)
int atype;					/* type of window to create */
int astartInput; {
#ifdef notdef
    PROCESS junk;
#endif
    pthread_t junk;
    pthread_attr_t threadAttr;

    struct onode_initparams oi_params;          /* object init params*/
    struct gwin_initparams wi_params;		/* window initialization params*/
    register struct gwin *twin;
    register long code;
    error_status_t st;

    /* Register gtx messages with the dce_printf facility */

    dce_msg_define_msg_table(gtx__table,
			     sizeof gtx__table / sizeof gtx__table[0],
			     &st);

    if (st != svc_s_ok)

      fprintf(stderr,
	      "Warning Cannot register gtx_table as an incore message table 0x%lx\n",
	      st);

    /* setup the main window structure */
    wi_params.i_type   = GATOR_WIN_CURSES;
    wi_params.i_x      =   0;
    wi_params.i_y      =   0;
    wi_params.i_width  =  80;
    wi_params.i_height = 200;
    wi_params.i_debug  = 0;	/* or 1 if we want debugging done */

    /*
     * Set up the basic onode initialization parameters, throwing in
     * the graphics-specific stuff.
     */
    oi_params.i_debug = 0;	/* or 1 if we want debugging */
    oi_params.i_gwparams = &wi_params;

    code = gator_objects_init(&oi_params);
    if (code) return (struct gwin *) 0;
    if (astartInput)
    {
	code = pthread_attr_create(&threadAttr);
	code = pthread_attr_setstacksize(&threadAttr, 8192);
	/* use default sched priority, which is inherited */
	code = pthread_create(&junk, threadAttr,
			      (pthread_startroutine_t)gtx_InputServer,
			      0);
    }

    /* all done */
    twin = &gator_basegwin;
    return twin;
}

