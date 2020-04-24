/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gtxlightobj.h,v $
 * Revision 1.1.11.1  1996/10/02  17:51:07  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:40:01  damon]
 *
 * Revision 1.1.6.1  1994/06/09  14:10:01  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:36  annie]
 * 
 * Revision 1.1.4.3  1993/01/19  16:03:02  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:12:52  cjd]
 * 
 * Revision 1.1.4.2  1992/11/24  17:51:47  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  18:05:20  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/28  22:10:04  garyf
 * 	fix incorrect includes
 * 	[1992/05/28  22:06:56  garyf]
 * 
 * Revision 1.1  1992/01/19  02:41:38  devrcs
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

#ifndef __gator_lightobject_h
#define	__gator_lightobject_h  1

#include <dcedfs/gtxobjects.h>		/*Basic gator object definitions*/

/*Value for onode o_type field*/
#define	GATOR_OBJ_LIGHT	1

/*Light masks*/
#define	GATOR_LIGHTMASK_OUTLINE	   0x1	    /*Outline the light?*/
#define	GATOR_LIGHTMASK_INVVIDEO   0x2	    /*Show light in inverse video?*/
#define	GATOR_LIGHTMASK_FLASH	   0x4	    /*Flash light when turned on?*/
#define	GATOR_LIGHTMASK_FLASHCYCLE 0x8	    /*Current flash cycle*/

#define GATOR_LABEL_CHARS 128		    /*Max chars in light label*/

/*Private data for light onode*/
struct gator_lightobj {
    int	*llrock;			/*Rock for lower-level graphics layer*/
    int setting;                        /*Is light on or off*/
    int	appearance;			/*Bit array describing light's appearance*/
    int	flashfreq;			/*Flashing frequency in msecs*/
    int	lasttoggletime;			/*Last time ``flashed''*/
    char label[GATOR_LABEL_CHARS];      /*Light label*/
};

/*Light object's creation parameters*/
struct gator_light_crparams {
    struct onode_createparams onode_params; /*Creation params for the whole onode*/
    int	appearance;			    /*General appearance*/
    int	flashfreq;			    /*Flash frequency in msecs, if any*/
    char label[GATOR_LABEL_CHARS];          /*Light label*/
    int label_x, label_y;                   /*X,Y offsets for label within light*/
};

/*Light object's creation routine*/

extern int gator_light_create();
    /*
     * Summary:
     *    Create a gator light object.
     *
     * Args:
     *	  struct onode *light_onp : Ptr to the light onode to fill out.
     *	  struct onode_createparams *params : Ptr to creation params.
     *       (Note: this actually points to a gator_light_crparams
     *        structure, but we use the generic version of the ptr)
     *
     * Returns:
     *	  Zero if successful,
     *	  Error value otherwise.
     */

/*Light object's generic onode routines*/

extern int gator_light_destroy();
    /*
     * Summary:
     *    Destroy a gator light object.
     *
     * Args:
     *	 struct onode *onp : Ptr to the light onode to delete.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_light_display();
    /*
     * Summary:
     *    Display/redraw a gator light object.
     *
     * Args:
     *	  struct onode *onp: Ptr to the light onode to display.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_light_release();
    /*
     * Summary:
     *    Drop the refcount on a gator light object.
     *
     * Args:
     *	  struct onode *onp : Ptr to the onode whose refcount is
     *                                   to be dropped.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

/*
 * Additional, light-specific operations.
 */

extern int gator_light_set();
    /*
     * Summary:
     *    Set the value of the given gator light object.
     *
     * Args:
     *	  struct onode *onp : Ptr to the light onode to be set.
     *	  int setting       : Non-zero for ``on'', zero for ``off''.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

/*
 * Set of exported generic light onode operations.
 */
extern struct onodeops gator_light_ops;

#endif /*  __gator_lightobject_h */
