/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: lightobject.c,v $
 * Revision 1.1.60.1  1996/10/02  17:51:28  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:40:17  damon]
 *
 * Revision 1.1.55.3  1994/08/03  16:13:45  ruby
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  16:58:40  ruby]
 * 
 * Revision 1.1.55.2  1994/06/09  14:10:30  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:48  annie]
 * 
 * Revision 1.1.55.1  1994/02/04  20:20:29  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:13:57  devsrc]
 * 
 * Revision 1.1.53.1  1993/12/07  17:26:28  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:32:57  jaffe]
 * 
 * Revision 1.1.4.5  1993/01/19  16:03:51  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:13:39  cjd]
 * 
 * Revision 1.1.4.4  1992/11/24  17:52:20  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  18:05:54  bolinger]
 * 
 * Revision 1.1.4.3  1992/09/15  13:14:27  jaffe
 * 	Transarc delta: jaffe-sync-with-aug-31-osf-sources 1.4
 * 	  Selected comments:
 * 	    Pick up changes from last submission to the OSF.
 * 	    include errno.h instead of using an extern definition.
 * 	    remove commondefs.idl.  This was done incompletely in another delta
 * 	[1992/09/14  19:52:42  jaffe]
 * 
 * Revision 1.1.4.2  1992/08/31  20:05:35  jaffe
 * 	*** empty log message ***
 * 	[1992/08/31  15:19:24  jaffe]
 * 
 * Revision 1.1.2.3  1992/06/02  19:19:53  garyf
 * 	cleanup errno reference & OSF/1 conditional
 * 	[1992/06/02  19:10:51  garyf]
 * 
 * Revision 1.1.2.2  1992/05/20  19:55:03  mason
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
 * 	[1992/05/20  11:36:25  mason]
 * 
 * Revision 1.1  1992/01/19  02:41:49  devrcs
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
 * lightobject.c
 *
 * Description:
 *	Implementation of the gator light object.
 *
 * Author:
 *	Ed. Zayas
 *	Transarc Corporation
 *------------------------------------------------------------------------*/

#include <dcedfs/gtxlightobj.h>		/*Interface for this module*/
#include <stdio.h>                      /*Standard I/O stuff*/

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

/*Externally-advertised array of light onode operations*/
struct onodeops gator_light_ops = {
    gator_light_destroy,
    gator_light_display,
    gator_light_release
};

#include <errno.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsgtxmac.h>
#include <dfsgtxsvc.h>
#include <dfsgtxmsg.h>

static char mn[] = "gator_lightobject";   /*Module name*/

/*------------------------------------------------------------------------
 * gator_light_create
 *
 * Description:
 *	Create a gator light object.
 *
 * Arguments:
 *      struct onode *light_onp : Ptr to the light onode to fill out.
 *	struct onode_createparams *params : Generic ptr to creation
 *	    parameters.
 *
 * Returns:
 *	Zero if successful,
 *	Error value otherwise.
 *
 * Environment:
 *	The base onode fields have already been set.  Lights are turned
 *	off at creation.
 *
 * Side Effects:
 *	Creates and initializes the light private data area, including
 *	a window string-drawing parameter structure.  These areas are
 *	garbage-collected upon failure.
 *------------------------------------------------------------------------*/

int gator_light_create(light_onp, params)
    struct onode *light_onp;
    struct onode_createparams *params;

{ /*gator_light_create*/

    static char	rn[] = "gator_light_create";   /*Routine name*/
    struct gator_light_crparams *light_params; /*My specific creation params*/
    struct gator_lightobj *light_data;         /*Ptr to private data*/
    struct gwin_strparams *light_strparams;    /*Light label params*/

    light_params = (struct gator_light_crparams *)params;
    if (objects_debug) {
      dce_svc_printf(GTX_S_LO_PRIVATE_DATE_PASSED_MSG, mn, rn);
      dce_svc_printf(GTX_S_LO_DATA_PASSED_MSG, light_params->appearance, light_params->flashfreq, light_params->label_x, light_params->label_y, light_params->label);
    }

    /*
      * Allocate the private data area, including the lower-level
      * structure, then fill it in.
      */
    light_data = (struct gator_lightobj *)malloc(sizeof(struct gator_lightobj));
    if (light_data == (struct gator_lightobj *)0) {
      dce_svc_printf(GTX_S_LO_CANT_ALLOCATE_PRIVATE_DATA_MSG, mn, rn, sizeof(struct gator_lightobj), errno);
      return(errno);
    }

    light_strparams = (struct gwin_strparams *)malloc(sizeof(struct gwin_strparams));
    if (light_strparams == (struct gwin_strparams *)0) {
      dce_svc_printf(GTX_S_LO_CANT_ALLOCATE_LABEL_MSG, mn, rn, sizeof(struct gwin_strparams), errno);
      free(light_data);
      return(errno);
    }

    /*
      * Now that we have the private structures allocated, set them up.
      */
    light_data->setting        = 0;
    light_data->appearance     = light_params->appearance;
    light_data->flashfreq      = light_params->flashfreq;
    light_data->lasttoggletime = 0;
    strcpy(light_data->label, light_params->label);

    light_strparams->x         = light_onp->o_x + light_params->label_x;
    light_strparams->y         = light_onp->o_y + light_params->label_y;
    light_strparams->s         = light_data->label;
    light_strparams->highlight = 0;
    light_data->llrock         = (int *)light_strparams;

    /*
      * Attach the private data to the onode, then return the happy news.
      */
    light_onp->o_data = (int *)light_data;
    return(0);

} /*gator_light_create*/

/*------------------------------------------------------------------------
 * gator_light_destroy
 *
 * Description:
 *	Destroy a gator light object.
 *
 * Arguments:
 *	struct onode *onp : Ptr to the light onode to delete.
 *
 * Returns:
 *	0: Success
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_light_destroy(onp)
    struct onode *onp;

{ /*gator_light_destroy*/

    /*
      * For now, this is a no-op.
      */
    return(0);

} /*gator_light_destroy*/

/*------------------------------------------------------------------------
 * gator_light_display
 *
 * Description:
 *	Display/redraw a gator light object.
 *
 * Arguments:
 *	struct onode *onp: Ptr to the light onode to display.
 *
 * Returns:
 *	0: Success
 *	Error value otherwise.
 *
 * Environment:
 *	Light objects have a pointer to string-drawing params in the
 *	lower-level rock, with the proper highlighting set according
 *	to whether the light is on or off, so we just have to draw
 *	that string to get the proper effect.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_light_display(onp)
    struct onode *onp;

{ /*gator_light_display*/

    static char	rn[] = "gator_light_display";	/*Routine name*/
    struct gator_lightobj *light_data;          /*Ptr to light obj data*/
    struct gwin_strparams *label_strparams;	/*String-drawing params*/

    /*
      * Draw the label, with proper highlighting depending on whether
      * the light is on.
      */
    light_data = (struct gator_lightobj *)(onp->o_data);
    label_strparams = (struct gwin_strparams *)(light_data->llrock);
    if (objects_debug)
      dce_svc_printf(GTX_S_LO_PRINTING_LABEL_MSG, mn, rn, label_strparams->s, label_strparams->x, label_strparams->y);
    WOP_DRAWSTRING(onp->o_window, label_strparams);
    return(0);

} /*gator_light_display*/

/*------------------------------------------------------------------------
 * gator_light_release
 *
 * Description:
 *	Drop the refcount on a gator light object.
 *
 * Arguments:
 *	struct onode *onp : Ptr to the onode whose refcount is
 *	                             to be dropped.
 *
 * Returns:
 *	0: Success
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_light_release(onp)
    struct onode *onp;

{ /*gator_light_release*/

    /*
      * For now, this is a no-op.
      */
    return(0);

} /*gator_light_release*/

/*------------------------------------------------------------------------
 * gator_light_set
 *
 * Description:
 *	Set the value of the given gator light object.
 *
 * Arguments:
 *	  struct onode *onp : Ptr to the light onode to be set.
 *	  int setting       : Non-zero for ``on'', zero for ``off''.
 *
 * Returns:
 *	0: Success
 *	Error value otherwise.
 *
 * Environment:
 *	We need to set not only the setting field, but the lower-
 *	level structure stored in the rock must have its highlight
 *	field set correctly.
 *
 * Side Effects:
 *	Does NOT redisplay the light object.
 *------------------------------------------------------------------------*/

int gator_light_set(onp, setting)
    struct onode *onp;
    int setting;

{ /*gator_light_set*/

    static char	rn[] = "gator_light_set";	/*Routine name*/
    struct gator_lightobj *light_data;          /*Ptr to light obj data*/
    struct gwin_strparams *label_strparams;	/*String-drawing params*/

    /*
      * Set the object correctly, then set the highlight field in
      * the lower-level rock.
      */
    light_data = (struct gator_lightobj *)(onp->o_data);
    label_strparams = (struct gwin_strparams *)(light_data->llrock);
    if (objects_debug)
      dce_svc_printf(GTX_S_LO_SETTING_OBJECT_MSG, mn, rn, onp, setting, (setting? "ON" : "OFF"));
    light_data->setting        = setting;
    label_strparams->highlight = setting;

    return(0);

} /*gator_light_set*/
