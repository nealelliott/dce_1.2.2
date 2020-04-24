/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: screen_test.c,v $
 * Revision 1.1.44.1  1996/10/02  17:51:34  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:40:22  damon]
 *
 * Revision 1.1.39.5  1994/09/28  21:22:09  sasala
 * 	bos output incorrect -missing output, OT 12280, S12Y
 * 	[1994/09/28  21:20:19  sasala]
 * 
 * Revision 1.1.39.4  1994/08/03  16:15:51  ruby
 * 	Fixed RCS problem
 * 	[1994/08/03  16:06:55  ruby]
 * 
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  17:00:54  ruby]
 * 
 * Revision 1.1.41.2  1994/06/29  11:38:11  devsrc
 * 	merged with bl-10
 * 
 * Revision 1.1.41.1  1994/04/28  17:36:21  mbs
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:34  mbs]
 * 
 * Revision 1.1.39.3  1994/07/13  22:28:31  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:38:11  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:34  mbs]
 * 
 * Revision 1.1.39.2  1994/06/09  14:10:40  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:52  annie]
 * 
 * Revision 1.1.39.1  1994/02/04  20:20:38  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:14:01  devsrc]
 * 
 * Revision 1.1.37.1  1993/12/07  17:26:33  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:34:05  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/19  16:04:10  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:14:01  cjd]
 * 
 * Revision 1.1.2.2  1992/11/24  17:52:40  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  18:06:11  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:41:27  devrcs
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

/*--------------------------------------------------------------------------------
 * screen_test: A test of the gator screen operations.
 *
 *	Ed. Zayas
 *	Transarc Corporation
 *--------------------------------------------------------------------------------*/

#include <gtxwindows.h>	    /*Generalized window interface*/
#include <gtxcurseswin.h>    /*Curses window interface*/
#include <gtxdumbwin.h>	    /*Dumb terminal window interface*/
#include <gtxX11win.h>	    /*X11 window interface*/
#include <dcedfs/cmd.h>		    /*Command interpretation library*/

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsgtxmac.h>
#include <dfsgtxsvc.h>
#include <dfsgtxmsg.h>

dce_svc_handle_t   gtx_svc_handle;

void initialize_svc()
{
  error_status_t st;

  gtx_svc_handle = dce_svc_register(gtx_svc_table,  (idl_char *)"gtx",  &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot register svc 0x%lx\n", st);
  dce_svc_routing((unsigned char *) "NOTICE:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route NOTICE messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "WARNING:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route WARNING messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "ERROR:STDERR:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route ERROR messages  0x%lx\n", st);

  dce_msg_define_msg_table(gtx__table,
        sizeof gtx__table / sizeof gtx__table[0], &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Warning Cannot register gtx incore message table 0x%lx\n", st);
}


/*
 * Command line parameter indicies.
 */
#define	P_PACKAGE   0
#define	P_DEBUG	    1

static char pn[] = "screen_test";   /*Program name*/
static int screen_debug = 0;	    /*Is debugging turned on?*/

/*--------------------------------------------------------------------------------
 * test_this_package
 *
 * Description:
 *	Routine that does the actual testing of the chosen graphics
 *	package.
 *
 * Arguments:
 *	pkg : Number of package to test.
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
 *--------------------------------------------------------------------------------*/

static int test_this_package(pkg)
    int pkg;

{ /*test_this_package*/

    static char	rn[] = "test_this_package";	/*Routine name*/
    register int code;				/*Return code*/
    struct gwin_initparams init_params;		/*Window initialization params*/
    struct gator_cursesgwin_params c_crparams;	/*Curses window creation params*/
    struct gator_dumbgwin_params d_crparams;	/*Dumb terminal window creation params*/
    struct gator_X11gwin_params x_crparams;	/*X11 window creation params*/
    struct gwin	*newwin;			/*New (sub)window*/
    struct gwin_strparams strparams;		/*String-drawing params*/
    struct gwin_charparams charparams;		/*Char-drawing params*/
    char s[128];				/*Test string*/
    int	currx, curry;				/*Sliding values of x & y*/
    int	currhighlight;				/*Highlight this time around?*/

    /*
     * Initialize the gator window package to drive the desired subsystem.
     */
    init_params.i_type	 = pkg;
    init_params.i_x	 =   0;
    init_params.i_y	 =   0;
    init_params.i_width  =  80;
    init_params.i_height = 200;
    init_params.i_debug	 = screen_debug;

    code = gw_init(&init_params);
    if (code) {
	dce_svc_printf(GTX_S_ST_CANT_INIT_GATOR_WINDOWS_MSG, pn, rn, pkg, code);
	return(code);
    }

     
    {
    char *dcesptr;
    dcesptr = (char *) dce_sprintf(gtx_s_st_number_screen_lines, LINES);
    strcpy(s , dcesptr);
    free(dcesptr);
    }
    strparams.x		= 5;
    strparams.y		= LINES / 2;
    strparams.s		= s;
    strparams.highlight	= 0;
    WOP_DRAWSTRING(&gator_basegwin, &strparams);

    {
    char *dcesptr;
    dcesptr = (char *) dce_sprintf(gtx_s_st_number_screen_columns, COLS);
    strcpy(s , dcesptr);
    free(dcesptr);
    }
    strparams.x		= 5;
    strparams.y		= strparams.y + 1;
    strparams.s		= s;
    strparams.highlight	= 1;
    WOP_DRAWSTRING(&gator_basegwin, &strparams);

    /*
     * Draw a set of chars down a diagonal, pausing inbetween.
     */
    currhighlight = 1;
    for (currx = curry = 0; (currx < COLS) && (curry < LINES); currx++, curry++) {
	charparams.x	    = currx;
	charparams.y	    = curry;
	charparams.c	    = 'x';
	charparams.highlight = currhighlight;
	currhighlight = (currhighlight ? 0 : 1);
	WOP_DRAWCHAR(&gator_basegwin, &charparams);
	sleep(1);
    }
    WOP_BOX(&gator_basegwin);

    /*
     * Fill in the new window creation parameters and go for it.
     */
    c_crparams.gwin_params.cr_type      = pkg;
    c_crparams.gwin_params.cr_x	        = 40;
    c_crparams.gwin_params.cr_y         =  5;
    c_crparams.gwin_params.cr_width     = 20;
    c_crparams.gwin_params.cr_height    = 10;
    c_crparams.gwin_params.cr_parentwin = (struct gwin *)(&gator_basegwin);
    c_crparams.charwidth     =  8;
    c_crparams.charheight    = 13;
    c_crparams.box_vertchar  = '|';
    c_crparams.box_horizchar = '-';
    newwin = WOP_CREATE(&c_crparams);
    if (newwin == (struct gwin *)0) {
	dce_svc_printf(GTX_S_ST_CANT_CREATE_WINDOW_MSG, pn, rn);
    }
    else
	if (screen_debug)
	    dce_svc_printf(GTX_S_CREATED_NEW_WINDOW_MSG, pn, rn, newwin);

    /*
     * Draw something to the new window; first, a highlighted banner.
     */
    {
    char *dcesptr;
    dcesptr = (char *) dce_sprintf(gtx_s_st_subwindow);
    strcpy(s , dcesptr);
    free(dcesptr);
    }
    strparams.x		= 1;
    strparams.y		= 1;
    strparams.s		= s;
    strparams.highlight	= 1;
    WOP_DRAWSTRING(newwin, &strparams);

    /*
     * Next, draw an `x' at each corner.
     */
    charparams.c	 = 'x';
    charparams.highlight = 1;
    charparams.x	 = 1;
    charparams.y	 = 2;
    WOP_DRAWCHAR(newwin, &charparams);
    charparams.x	 = 18;
    charparams.y	 = 2;
    WOP_DRAWCHAR(newwin, &charparams);
    charparams.x	 = 1;
    charparams.y	 = 8;
    WOP_DRAWCHAR(newwin, &charparams);
    charparams.x	 = 18;
    charparams.y	 = 8;
    WOP_DRAWCHAR(newwin, &charparams);

    /*
     * Finally, box the sucker.
     */
    WOP_BOX(newwin);

    /*
      * Draw a few other things in the original window.
      */

    {
    char *dcesptr;
    dcesptr = (char *) dce_sprintf(gtx_s_st_number_screen_lines, LINES);
    strcpy(s , dcesptr);
    free(dcesptr);
    }

    strparams.x		= 5;
    strparams.y		= LINES / 2;
    strparams.s		= s;
    strparams.highlight	= 0;
    WOP_DRAWSTRING(&gator_basegwin, &strparams);

    {
    char *dcesptr;
    dcesptr = (char *) dce_sprintf(gtx_s_st_number_screen_columns, COLS);
    strcpy(s , dcesptr);
    free(dcesptr);
    }

    strparams.x		= 5;
    strparams.y		= strparams.y + 1;
    strparams.s		= s;
    strparams.highlight	= 1;
    WOP_DRAWSTRING(&gator_basegwin, &strparams);

    /*
     * Let people admire the handiwork, then clean up.
     */
    WOP_DISPLAY(&gator_basegwin);
    sleep(10);
    WOP_DISPLAY(&gator_basegwin);
    WOP_CLEANUP(&gator_basegwin);

} /*test_this_package*/

/*--------------------------------------------------------------------------------
 * screen_testInit
 *
 * Description:
 *	Routine that is called when screen_test is invoked, responsible
 *	for basic initialization and command line parsing.
 *
 * Arguments:
 *	as	: Command syntax descriptor.
 *	arock	: Associated rock (not used here).
 *
 * Returns:
 *	Zero (but may exit the entire program on error!)
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	Initializes this program.
 *--------------------------------------------------------------------------------*/

static int screen_testInit(as, arock)
    struct cmd_syndesc *as;
    char *arock;

{ /*screen_testInit*/

    static char	rn[] = "screen_testInit";   /*Routine name*/
    int	pkg_to_test;			    /*Which package to test*/

    if (as->parms[P_DEBUG].items != 0)
	screen_debug = 1;
    pkg_to_test = atoi(as->parms[P_PACKAGE].items->data);
    dce_svc_printf(GTX_S_ST_TEST_GRAPHICS_MSG, pn, rn, pkg_to_test);
    switch (pkg_to_test) {
	case GATOR_WIN_CURSES:
	    dce_svc_printf(GTX_S_ST_CURSES_MSG);
	    break;
	case GATOR_WIN_DUMB:
	    dce_svc_printf(GTX_S_ST_DUMB_TERMINAL_MSG);
	    break;
	case GATOR_WIN_X11:
	    dce_svc_printf(GTX_S_ST_X11_MSG);
	    break;
	default:
	    dce_svc_printf(GTX_S_ST_ILLEGAL_GRAPHICS_MSG, pkg_to_test);
    } /*end switch (pkg_to_test)*/

    /*
     * Now, drive the sucker.
     */
    test_this_package(pkg_to_test);

    /*
     * We initialized (and ran) correctly, so return the good news.
     */
    return(0);

} /*screen_testInit*/

main(argc, argv)
    int argc;
    char **argv;

{ /*main*/

    static char	rn[] = "main";		/*Routine name*/
    register long code;			/*Return code*/
    register struct cmd_syndesc	*ts;	/*Ptr to cmd line syntax descriptor*/

    /*
     * There really aren't any opcodes here, but we do want to interpret switches
     * from the command line.  So, all we need do is set up the initcmd ``opcode''.
     */

    initialize_svc();


    ts = cmd_CreateSyntax("initcmd", screen_testInit, 0, "Initialize, interpret command line");
    cmd_AddParm(ts, "-package", CMD_SINGLE, CMD_REQUIRED, "Graphics package to use");
    cmd_AddParm(ts, "-debug",   CMD_FLAG,   CMD_OPTIONAL, "Turn debugging on");

    /*
     * Parse command-line switches & execute the test, then get the heck out of here.
     */
    code = cmd_Dispatch(argc, argv);
    if (code) {
	dce_svc_printf(GTX_S_ST_CMD_DISPATCH_FAILED_MSG, pn, rn, code);
	exit(1);
    }

} /*main*/
