/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: object_test.c,v $
 * Revision 1.1.42.1  1996/10/02  17:51:31  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:40:19  damon]
 *
 * Revision 1.1.37.5  1994/09/28  21:22:08  sasala
 * 	bos output incorrect -missing output, OT 12280, S12Y
 * 	[1994/09/28  21:20:18  sasala]
 * 
 * Revision 1.1.37.4  1994/08/03  16:14:53  ruby
 * 	Fixed RCS problem
 * 	[1994/08/03  16:06:45  ruby]
 * 
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  16:59:51  ruby]
 * 
 * Revision 1.1.39.2  1994/06/29  11:38:10  devsrc
 * 	merged with bl-10
 * 
 * Revision 1.1.39.1  1994/04/28  17:36:18  mbs
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:33  mbs]
 * 
 * Revision 1.1.37.3  1994/07/13  22:28:30  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:38:10  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:33  mbs]
 * 
 * Revision 1.1.37.2  1994/06/09  14:10:35  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:50  annie]
 * 
 * Revision 1.1.37.1  1994/02/04  20:20:33  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:13:58  devsrc]
 * 
 * Revision 1.1.35.1  1993/12/07  17:26:30  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:33:33  jaffe]
 * 
 * Revision 1.1.2.2  1993/01/19  16:04:03  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:13:51  cjd]
 * 
 * Revision 1.1  1992/01/19  02:41:21  devrcs
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
 * object_test: A test of the gator object operations.
 *
 *	Ed. Zayas
 *	Transarc Corporation
 *--------------------------------------------------------------------------------*/

#include <gtxscreenobj.h>	/*Gator screen object interface*/
#include <gtxtextobj.h>		/*Gator text object interface*/
#include <gtxlightobj.h>	/*Gator light object interface*/
#include <gtxwindows.h>		/*Gator generic window package*/
#include <gtxcurseswin.h>	/*Gator curses window package*/
#include <gtxdumbwin.h>		/*Gator dumb terminal window package*/
#include <gtxX11win.h>		/*Gator X11 window package*/
#include <stdio.h>		/*Standard I/O stuff*/
#include <cmd.h>		/*Command interpretation library*/

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsgtxmac.h>
#include <dfsgtxsvc.h>
#include <dfsgtxmsg.h>

/*
 * Command line parameter indicies.
 */
#define	P_PACKAGE   0
#define	P_DEBUG	    1

static char pn[] = "object_test";   /*Program name*/
static int object_debug = 0;	    /*Is debugging turned on?*/


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



/*--------------------------------------------------------------------------------
 * test_objects
 *
 * Description:
 *	Routine that does the actual testing of gator objects.
 *
 * Arguments:
 *	int pkg : Number of windowing package to use.
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

static int test_objects(pkg)
    int pkg;

{ /*test_objects*/

    static char	rn[] = "test_objects";	        /*Routine name*/
    register int code;				/*Return code*/
    struct onode_initparams oi_params;          /*Init params*/
    struct gwin_initparams wi_params;		/*Window initialization params*/
#if 0
    /*We don't need these, do we?*/
    struct gator_cursesgwin_params c_crparams;	/*Curses window creation params*/
    struct gator_dumbgwin_params d_crparams;	/*Dumb terminal window creation params*/
    struct gator_X11gwin_params x_crparams;	/*X11 window creation params*/
#endif 0
    struct gator_light_crparams light_crparams; /*Light creation params*/
    char helpstring1[128];                       /*Help string to use*/
    char helpstring2[128];                      /*Help string to use*/
    char helpstring3[128];                      /*Help string to use*/
    char helpstring4[128];                      /*Help string to use*/
    char helpstringt1[128];                     /*Help string to use*/
    struct onode *light_onp1;                   /*Ptr to light onode*/
    struct onode *light_onp2;                   /*Ptr to another light onode*/
    struct onode *light_onp3;                   /*Ptr to another light onode*/
    struct onode *light_onp4;                   /*Ptr to another light onode*/
    struct onode *text_onp1;			/*Ptr to text onode*/
    int i;                                      /*Generic loop variable*/
    int setting;                                /*Current light setting*/
    struct gator_textobj_params text_crparams;	/*Text creation params*/
    char s[128];				/*Another string*/
    struct gwin_strparams strparams;		/*String-drawing params*/

    /*
     * Initialize the chosen gator window package.
     */
    if (object_debug)
      dce_svc_printf(GTX_S_OT_INIT_WINDOWS_MSG, pn, rn, pkg);
    wi_params.i_type   = pkg;
    wi_params.i_x      =   0;
    wi_params.i_y      =   0;
    wi_params.i_width  =  80;
    wi_params.i_height = 200;
    wi_params.i_debug  = object_debug;

    /*
      * Set up the basic onode initialization parameters, throwing in
      * the graphics-specific stuff.
      */
    oi_params.i_debug = object_debug;
    oi_params.i_gwparams = &wi_params;
    code = gator_objects_init(&oi_params);
    if (code) {
	dce_svc_printf(GTX_S_CANT_INIT_GATOR_OBJECTS_MSG, pn, rn, pkg, code);
	return(code);
    }

    /*
      * Set up some light objects and put them up on the screen.
      */
 
    light_crparams.onode_params.cr_type       = GATOR_OBJ_LIGHT;

    light_crparams.onode_params.cr_x          = 10;
    light_crparams.onode_params.cr_y          = 10;
    light_crparams.onode_params.cr_width      = 10;
    light_crparams.onode_params.cr_height     = 1;
    light_crparams.onode_params.cr_window     = &gator_basegwin;
    light_crparams.onode_params.cr_home_obj   = (struct onode *)0;
    light_crparams.onode_params.cr_prev_obj   = (struct onode *)0;
    light_crparams.onode_params.cr_parent_obj = (struct onode *)0;

    light_crparams.appearance = 0;
    light_crparams.flashfreq  = 0;
 
    test_objects_text(&light_crparams, helpstring1, 1);

    light_crparams.label_x = 0;
    light_crparams.label_y = 0;

    light_onp1 = gator_objects_create((struct onode_createparams *)(&light_crparams));
    if (light_onp1 == (struct onode *)0) {
      dce_svc_printf(GTX_S_OT_CREATE_OBJECT_FAILED_MSG, pn, rn);
      exit(-1);
    }

    light_crparams.onode_params.cr_x = 10;
    light_crparams.onode_params.cr_y = 12;

    test_objects_text(&light_crparams, helpstring2, 2);

    light_onp2 = gator_objects_create((struct onode_createparams *)(&light_crparams));
    if (light_onp2 == (struct onode *)0) {
      dce_svc_printf(GTX_S_OT_CREATE_OBJECT_FAILED_MSG, pn, rn);
      exit(-1);
    }

    light_crparams.onode_params.cr_x = 10;
    light_crparams.onode_params.cr_y = 14;

    test_objects_text(&light_crparams, helpstring3, 3);

    light_onp3 = gator_objects_create((struct onode_createparams *)(&light_crparams));
    if (light_onp3 == (struct onode *)0) {
      dce_svc_printf(GTX_S_OT_CREATE_OBJECT_FAILED_MSG, pn, rn);
      exit(-1);
    }

    light_crparams.onode_params.cr_x = 21;
    light_crparams.onode_params.cr_y = 10;

    test_objects_text(&light_crparams, helpstring4, 4);

    light_onp4 = gator_objects_create((struct onode_createparams *)(&light_crparams));
    if (light_onp4 == (struct onode *)0) {
      dce_svc_printf(GTX_S_OT_CREATE_OBJECT_FAILED_MSG, pn, rn);
      exit(-1);
    }

    /*
      * Create a text object, too.
      */

      {
	char              *s;

	s = dce_sprintf(gtx_s_ot_t1);
	strcpy(helpstringt1, s);
	free(s);

	s = dce_sprintf(gtx_s_ot_t11);
	strcpy(text_crparams.onode_params.cr_name, s);
	free(s);
    }

    text_crparams.onode_params.cr_type	     = GATOR_OBJ_TEXT;

    text_crparams.onode_params.cr_x	     = 30;
    text_crparams.onode_params.cr_y	     = 10;
    text_crparams.onode_params.cr_width	     = 35;
    text_crparams.onode_params.cr_height     =  7;
    text_crparams.onode_params.cr_window     = &gator_basegwin;
    text_crparams.onode_params.cr_home_obj   = (struct onode *)0;
    text_crparams.onode_params.cr_prev_obj   = (struct onode *)0;
    text_crparams.onode_params.cr_parent_obj = (struct onode *)0;
    text_crparams.onode_params.cr_helpstring = helpstringt1;
    text_crparams.maxEntries 	   =  7;
    text_crparams.maxCharsPerEntry = 35;

    text_onp1 = gator_objects_create((struct onode_createparams *)(&text_crparams));
    if (text_onp1 == (struct onode *)0) {
      dce_svc_printf(GTX_S_CANT_CREATE_TEXT_OBJECT_MSG, pn, rn);
      exit(-1);
    }
    OOP_DISPLAY(text_onp1);
    sleep(2);

    /*
      * Now that we have our lights, turn them on and off a few times.
      */
    setting = 1;
    sprintf(s, "%s", "ABCD");
    strparams.x		= 0;
    strparams.y		= 0;
    strparams.s		= s;
    strparams.highlight	= 0;

    for (i=0; i<10; i++) {
      code = gator_light_set(light_onp1, setting);
      if (code)
	dce_svc_printf(GTX_S_OT_CANT_SET_GATOR_LIGHT_MSG, pn, rn, light_onp1, setting, (setting? "ON" : "OFF"));
      else
	OOP_DISPLAY(light_onp1);

      code = gator_light_set(light_onp2, setting);
      if (code)
	dce_svc_printf(GTX_S_OT_CANT_SET_GATOR_LIGHT_MSG, pn, rn, light_onp2, setting, (setting? "ON" : "OFF"));
      else
	OOP_DISPLAY(light_onp2);

      code = gator_light_set(light_onp3, setting);
      if (code)
	dce_svc_printf(GTX_S_OT_CANT_SET_GATOR_LIGHT_MSG, pn, rn, light_onp3, setting, (setting? "ON" : "OFF"));
      else
	OOP_DISPLAY(light_onp3);

      code = gator_light_set(light_onp4, setting);
      if (code)
	dce_svc_printf(GTX_S_CANT_SET_GATOR_LIGHT_MSG, pn, rn, light_onp4, setting, (setting? "ON" : "OFF"));
      else
	OOP_DISPLAY(light_onp4);
      setting = (setting? 0 : 1);

      sleep(1);

      WOP_DRAWSTRING(text_onp1->o_window, &strparams);
      strparams.x++;
      strparams.y++;
      sleep(1);

    } /*Flash loop*/

    OOP_DISPLAY(text_onp1);

    /*
      * Start writing stuff to our text object.
      */
    {
    char *dcesptr;
    dcesptr = dce_sprintf(gtx_s_ot_first_line);
    strcpy(s , dcesptr);
    free(dcesptr);
    }
    code = gator_text_Write(text_onp1, s, strlen(s), 0, 1);
    if (code)
      dce_svc_printf(GTX_S_OT_CANT_WRITE_TO_TEXT_OBJECT_MSG, pn, rn, s, strlen(s), text_onp1, code);
    sleep(2);

    {
    char *dcesptr;
    dcesptr = dce_sprintf(gtx_s_ot_this_the);
    strcpy(s , dcesptr);
    free(dcesptr);
    }
    code = gator_text_Write(text_onp1, s, strlen(s), 0, 0);
    if (code)
      dce_svc_printf(GTX_S_OT_CANT_WRITE_TO_TEXT_OBJECT_MSG, pn, rn, s, strlen(s), text_onp1, code);
    sleep(2);

    {
    char *dcesptr;
    dcesptr = dce_sprintf(gtx_s_ot_second_line);
    strcpy(s , dcesptr);
    free(dcesptr);
    }
    code = gator_text_Write(text_onp1, s, strlen(s), 0, 1);
    if (code)
      dce_svc_printf(GTX_S_OT_CANT_WRITE_TO_TEXT_OBJECT_MSG, pn, rn, s, strlen(s), text_onp1, code);
    sleep(2);

    {
    char *dcesptr;
    dcesptr = dce_sprintf(gtx_s_ot_highlighted_third);
    strcpy(s , dcesptr);
    free(dcesptr);
    }
    code = gator_text_Write(text_onp1, s, strlen(s), 1, 1);
    if (code)
      dce_svc_printf(GTX_S_OT_CANT_WRITE_TO_TEXT_OBJECT_MSG, pn, rn, s, strlen(s), text_onp1, code);
    sleep(2);

    {
    char *dcesptr;
    dcesptr = dce_sprintf(gtx_s_ot_fourth_line);
    strcpy(s , dcesptr);
    free(dcesptr);
    }
    code = gator_text_Write(text_onp1, s, strlen(s), 0, 1);
    if (code)
      dce_svc_printf(GTX_S_OT_CANT_WRITE_TO_TEXT_OBJECT_MSG, pn, rn, s, strlen(s), text_onp1, code);
    sleep(2);

    {
    char *dcesptr;
    dcesptr = dce_sprintf(gtx_s_ot_fifth_line);
    strcpy(s , dcesptr);
    free(dcesptr);
    }
    code = gator_text_Write(text_onp1, s, strlen(s), 0, 1);
    if (code)
      dce_svc_printf(GTX_S_OT_CANT_WRITE_TO_TEXT_OBJECT_MSG, pn, rn, s, strlen(s), text_onp1, code);
    sleep(2);

    {
    char *dcesptr;
    dcesptr = dce_sprintf(gtx_s_ot_sixth_line);
    strcpy(s , dcesptr);
    free(dcesptr);
    }
    code = gator_text_Write(text_onp1, s, strlen(s), 0, 1);
    if (code)
      dce_svc_printf(GTX_S_OT_CANT_WRITE_TO_TEXT_OBJECT_MSG, pn, rn, s, strlen(s), text_onp1, code);
    sleep(2);

    {
    char *dcesptr;
    dcesptr = dce_sprintf(gtx_s_ot_seventh_line);
    strcpy(s , dcesptr);
    free(dcesptr);
    }
    code = gator_text_Write(text_onp1, s, strlen(s), 0, 1);
    if (code)
      dce_svc_printf(GTX_S_OT_CANT_WRITE_TO_TEXT_OBJECT_MSG, pn, rn, s, strlen(s), text_onp1, code);
    sleep(4);

    /*
      * Now, try to scroll the sucker.
      */
    for (i = 0; i<10; i++) {
      code = gator_text_Scroll(text_onp1, 1, GATOR_TEXT_SCROLL_UP);
      if (code)
      dce_svc_printf(GTX_S_OT_CANT_SCOLL_UP_MSG, pn, rn, text_onp1);
      sleep(2);
    }

    for (i = 0; i < 10; i++) {
      code = gator_text_Scroll(text_onp1, 2, GATOR_TEXT_SCROLL_DOWN);
      dce_svc_printf(GTX_S_OT_CANT_SCROLL_DOWN_MSG, pn, rn, text_onp1);
      sleep(2);
    }

    /*
      * Before leaving, we clean up our windows.
      */
    WOP_CLEANUP(&gator_basegwin);

} /*test_objects*/

static void test_objects_text(pl, hs, number)

    struct gator_light_crparams  *pl;
    char                         *hs;
    int                          number;

    {
    char                         *s;

    s = dce_sprintf(gtx_s_ot_help_string, number);
    strcpy(hs, s);
    free(s);

    pl->onode_params.cr_helpstring = hs;

    s = dce_sprintf(gtx_s_ot_light_name, number);
    strcpy(pl->onode_params.cr_name, s);
    free(s);

    s = dce_sprintf(gtx_s_ot_light_label, number);
    strcpy(pl->label, s);
    free(s);
    }


/*------------------------------------------------------------------------
 * object_testInit
 *
 * Description:
 *	Routine that is called when object_test is invoked, responsible
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
 *------------------------------------------------------------------------*/

static int object_testInit(as, arock)
    struct cmd_syndesc *as;
    char *arock;

{ /*object_testInit*/

    static char	rn[] = "object_testInit";   /*Routine name*/
    int	wpkg_to_use;			    /*Window package to use*/

    if (as->parms[P_DEBUG].items != 0)
	object_debug = 1;
    wpkg_to_use = atoi(as->parms[P_PACKAGE].items->data);
    dce_svc_printf(GTX_S_OT_USING_GRAPHICS_PACKAGE_MSG, pn, rn, wpkg_to_use);
    switch (wpkg_to_use) {
	case GATOR_WIN_CURSES:
	    dce_svc_printf(GTX_S_OT_CURSES_MSG);
	    break;
	case GATOR_WIN_DUMB:
	    dce_svc_printf(GTX_S_OT_DUMB_TERMINAL_MSG);
	    break;
	case GATOR_WIN_X11:
	    dce_svc_printf(GTX_S_OT_X11_MSG);
	    break;
	default:
	    dce_svc_printf(GTX_S_OT_ILLEGAL_GRAPHICS_MSG, wpkg_to_use);
	    exit(-1);
    } /*end switch (wpkg_to_use)*/

    /*
     * Now, drive the sucker.
     */
    test_objects(wpkg_to_use);

    /*
     * We initialized (and ran) correctly, so return the good news.
     */
    return(0);

} /*object_testInit*/

main(argc, argv)
    int argc;
    char **argv;

{ /*main*/

    static char	rn[] = "main";		/*Routine name*/
    register long code;			/*Return code*/
    register struct cmd_syndesc	*ts;	/*Ptr to cmd line syntax descriptor*/


    initialize_svc();


    /*
     * Set up the commands we understand.
     */
    ts = cmd_CreateSyntax("initcmd", object_testInit, 0,
			  "Initialize the program");
    cmd_AddParm(ts, "-package", CMD_SINGLE, CMD_REQUIRED,
		"Graphics package to use");
    cmd_AddParm(ts, "-debug",   CMD_FLAG,   CMD_OPTIONAL,
		"Turn debugging on");

    /*
     * Parse command-line switches & execute the test, then get the heck
     * out of here.
     */
    code = cmd_Dispatch(argc, argv);
    if (code) {
	dce_svc_printf(GTX_S_OT_CMD_DISPATCH_FAILED_MSG, pn, rn, code);
	exit(1);
    }

} /*main*/
