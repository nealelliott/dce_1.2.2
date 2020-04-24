/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: objects.c,v $
 * Revision 1.1.73.1  1996/10/02  17:51:32  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:40:21  damon]
 *
 * Revision 1.1.68.3  1994/08/03  16:15:24  ruby
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  17:00:22  ruby]
 * 
 * Revision 1.1.68.2  1994/06/09  14:10:37  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:51  annie]
 * 
 * Revision 1.1.68.1  1994/02/04  20:20:35  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:14:00  devsrc]
 * 
 * Revision 1.1.66.1  1993/12/07  17:26:31  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:33:49  jaffe]
 * 
 * Revision 1.1.5.3  1993/01/19  16:04:07  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:13:56  cjd]
 * 
 * Revision 1.1.5.2  1992/11/24  17:52:34  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  18:06:05  bolinger]
 * 
 * Revision 1.1.3.2  1992/05/20  19:55:18  mason
 * 	Transarc delta: jess-correct-1.30-serror 1.1
 * 	  Files modified:
 * 	    cm: cm_dcache.c; gtx: objects.c, textcb.c, textobject.c
 * 	  Selected comments:
 * 	    To correct syntax errors
 * 	    extern long errno cahnged to extern int errno.
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
 * 	    see above
 * 	[1992/05/20  11:36:48  mason]
 * 
 * Revision 1.1  1992/01/19  02:41:47  devrcs
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
 * objects.c
 *
 * Description:
 *	Implementation of the gator object interface.
 *
 * Author:
 *	Ed. Zayas
 *	Transarc Corporation
 *------------------------------------------------------------------------*/

#include <dcedfs/gtxobjects.h>	/*Interface for this module*/
#include <dcedfs/gtxtextobj.h>	/*Text object interface*/
#include <dcedfs/gtxlightobj.h>	/*Light object interface*/
#include <dcedfs/gtxobjdict.h>	/*Object dictionary module*/
#include <stdio.h>		/*Standard I/O stuff*/

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsgtxmac.h>
#include <dfsgtxsvc.h>
#include <dfsgtxmsg.h>

/*
 * Number of known gator object types.
 */
#define GATOR_NUM_OBJTYPES 3

static char mn[] = "gator_objects";		/*Module name*/
int objects_debug;			        /*Is debugging output on?*/

int (*on_create[GATOR_NUM_OBJTYPES])();         /*Array of ptrs to creation functions*/
struct onodeops objops[GATOR_NUM_OBJTYPES];	/*Per-type op arrays*/

/*--------------------------------------------------------------------------------
 * gator_objects_init
 *
 * Description:
 *	Initialize the gator object package.
 *
 * Arguments:
 *	struct onode_initparams *params: Initialization parameters.
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
 *	
 *--------------------------------------------------------------------------------*/

int gator_objects_init(params)
    struct onode_initparams *params;

{ /*gator_objects_init*/

    static char	rn[] = "gator_objects_init";	/*Routine name*/
    static int initialized = 0;                 /*Have we been called?*/
    register int code;				/*Return code*/

    /*
     * If we've already been called, just return.
     */
    if (initialized) {
      initialized++;
      if (objects_debug)
	dce_svc_printf(GTX_S_OJ_CALLED_MORE_THAN_ONCE_MSG, mn, rn, initialized);
      return(0);
    }

    /*
     * Remember our debugging level.
     */
    objects_debug = params->i_debug;

    /*
     * Set up the onode op array, one entry for each known gator object type.
     */
    if (objects_debug)
      dce_svc_printf(GTX_S_OJ_SET_OBJOPS_ARRAY_MSG, mn, rn);
    objops[GATOR_OBJ_TEXT]   = gator_text_ops;
    objops[GATOR_OBJ_LIGHT]  = gator_light_ops;

    /*
      * Initialize the object dictionary.
      */
    if (objects_debug)
      dce_svc_printf(GTX_S_OJ_INIT_OBJECT_DICTIONARY_MSG, mn, rn);
    code = gator_objdict_init(objects_debug);
    if (code) {
	dce_svc_printf(GTX_S_OJ_FAILED_INIT_OBJECT_DICTIONARY_MSG, mn, rn, code);
	return(code);
    }

    /*
      * Initialize the chosen window package.  Remember the base window
      * is accessible as gator_basegwin.
      */
    if (objects_debug) {
      dce_svc_printf(GTX_S_OJ_INIT_GATOR_WINDOW_MSG, mn, rn, params->i_gwparams->i_type);
      dce_svc_printf(GTX_S_OJ_WINDOW_INIT_PARAMETERS_MSG, params->i_gwparams->i_type, params->i_gwparams->i_x, params->i_gwparams->i_y, params->i_gwparams->i_width, params->i_gwparams->i_height, params->i_gwparams->i_debug);
    }
    code = gw_init(params->i_gwparams);
    if (code) {
      dce_svc_printf(GTX_S_OJ_FAILED_GATOR_WINDOW_INIT_MSG, mn, rn, params->i_gwparams->i_type, code);
      return(code);
    }

    /*
      * Set up the array of creation functions.
      */
    if (objects_debug)
      dce_svc_printf(GTX_S_OJ_INIT_GATOR_OBJECT_CREATION_MSG, mn, rn);
    on_create[GATOR_OBJ_TEXT]   = gator_text_create;
    on_create[GATOR_OBJ_LIGHT]  = gator_light_create;

    /*
     * Finally, return the good news.
     */
    return(0);

} /*gator_objects_init*/

/*--------------------------------------------------------------------------------
 * gator_objects_create
 *
 * Description:
 *	Create an onode of the given type.
 *
 * Arguments:
 *	struct onode_createparams *params: Ptr to creation params.
 *
 * Returns:
 *	Ptr to newly-created onode if successful,
 *	Null pointer otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *--------------------------------------------------------------------------------*/

struct onode *gator_objects_create(params)
    struct onode_createparams *params;

{ /*gator_objects_create*/

    static char	rn[] = "gator_objects_create";	/*Routine name*/
    register int code;				/*Return code*/
    struct onode *new_onode;                    /*Ptr to new onode*/

    if (objects_debug) {
      dce_svc_printf(GTX_S_OJ_CREATE_ONODE_MSG, mn, rn, params->cr_type, params->cr_name);
      dce_svc_printf(GTX_S_OJ_ONODE_ORIGIN_MSG, params->cr_x, params->cr_y);
      dce_svc_printf(GTX_S_OJ_ONODE_WIDTH_MSG, params->cr_width, params->cr_height);
      dce_svc_printf(GTX_S_OJ_ONODE_HELPSTRING_MSG, params->cr_helpstring);
      dce_svc_printf(GTX_S_OJ_ONODE_WINDOW_STRUCT_MSG, params->cr_window);
    }

    if (objects_debug)
      dce_svc_printf(GTX_S_OJ_ALLOCATE_ONODE_BYTES_MSG, mn, rn, sizeof(struct onode));
    new_onode = (struct onode *)malloc(sizeof(struct onode));
    if (new_onode == (struct onode *)0) {
      dce_svc_printf(GTX_S_OJ_CANT_ALLOCATE_ONODE_BYTES_MSG, mn, rn, sizeof(struct onode), errno);
      return((struct onode *)0);
    }

    /*
      * Fill in the onode fields we can do right away.
      * **** Don't do anything with cr_helpstring yet - eventually,
      *      we'll create a scrollable text help object with it ****
      */
    if (objects_debug)
      dce_svc_printf(GTX_S_OJ_FILL_ONODE_FIELDS_MSG, mn, rn, sizeof(struct onode));
    new_onode->o_type     = params->cr_type;
    strcpy(new_onode->o_name, params->cr_name);
    new_onode->o_x        = params->cr_x;
    new_onode->o_y        = params->cr_y;
    new_onode->o_width    = params->cr_width;
    new_onode->o_height   = params->cr_height;
    new_onode->o_changed  = 1;
    new_onode->o_refcount = 1;
    new_onode->o_window   = params->cr_window;
    new_onode->o_op       = &(objops[params->cr_type]);
    new_onode->o_home     = params->cr_home_obj;
    new_onode->o_help     = (struct onode *)0;
    new_onode->o_nextobj  = (struct onode *)0;
    new_onode->o_upobj    = params->cr_parent_obj;
    new_onode->o_downobj  = (struct onode *)0;

    /*
      * Call the proper routine to initialize the private parts of the
      * given object.
      */
    if (objects_debug)
      dce_svc_printf(GTX_S_OJ_CREATE_GATOR_OBJECT_MSG, mn, rn, params->cr_type);
    code = (on_create[params->cr_type])(new_onode, params);
    if (code) {
      if (objects_debug)
	dce_svc_printf(GTX_S_OJ_FAILED_CREATE_GATOR_OBJECT_MSG, mn, rn, code, params->cr_type);
      free(new_onode);
      return((struct onode *)0);
    }

    /*
      * Set the links on the parent and previous objects, if so directed.
      */
    if (params->cr_prev_obj != (struct onode *)0) {
      if (objects_debug)
	dce_svc_printf(GTX_S_OJ_LINK_ONODE_OBJECT_MSG, mn, rn, params->cr_prev_obj, params->cr_prev_obj->o_nextobj);
      params->cr_prev_obj->o_nextobj = new_onode;
    }
    if (params->cr_parent_obj != (struct onode *)0) {
      if (objects_debug)
	dce_svc_printf(GTX_S_OJ_DOWNLINK_ONODE_OBJECT_MSG, mn, rn, params->cr_parent_obj, params->cr_parent_obj->o_downobj);
      params->cr_parent_obj->o_downobj = new_onode;
    }

    /*
      * Return the location of the completely-initialized onode object.
      */
    return(new_onode);

} /*gator_objects_create*/

/*--------------------------------------------------------------------------------
 * gator_objects_lookup
 *
 * Description:
 *	
 *
 * Arguments:
 *	char *onode_name: Onode string name to find.
 *
 * Returns:
 *	Ptr to onode matching the given name if one exists,
 *	Null pointer otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *--------------------------------------------------------------------------------*/

struct onode *gator_objects_lookup(onode_name)
    char *onode_name;

{ /*gator_objects_lookup*/

    static char	rn[] = "gator_objects_lookup";	/*Routine name*/

    /*
     * Life is very simple here - just call the dictionary routine.
     */
    if (objects_debug)
      dce_svc_printf(GTX_S_OJ_LOOKUP_GATOR_OBJECT_MSG, mn, rn, onode_name);
    return(gator_objdict_lookup(onode_name));

} /*gator_objects_lookup*/
