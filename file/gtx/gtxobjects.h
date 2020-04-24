/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gtxobjects.h,v $
 * Revision 1.1.11.1  1996/10/02  17:51:11  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:40:02  damon]
 *
 * Revision 1.1.6.1  1994/06/09  14:10:09  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:38  annie]
 * 
 * Revision 1.1.4.3  1993/01/19  16:03:17  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:13:02  cjd]
 * 
 * Revision 1.1.4.2  1992/11/24  17:51:54  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  18:05:26  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/28  22:10:09  garyf
 * 	fix incorrect include
 * 	[1992/05/28  22:07:02  garyf]
 * 
 * Revision 1.1  1992/01/19  02:41:33  devrcs
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

#ifndef __gator_objects_h
#define	__gator_objects_h  1

/*--------------------------------------------------------------------------------
 * objects.h
 *
 * Constants and data structures defining the basis for a gator object.
 *--------------------------------------------------------------------------------*/

#include <dcedfs/gtxwindows.h>		/*Standard window defs & ops*/

/*Max number of chars in an object name*/
#define	GATOR_OBJNAMELEN 128

/*
 * The onode is the focus of all gator display activity.  There is a unique
 * onode for each gator object.
 */
struct onode {
    int	o_type;				/*Object type*/
    char o_name[GATOR_OBJNAMELEN];	/*Object's string name*/
    int	o_x, o_y;			/*X and Y coordinates*/
    int	o_width, o_height;		/*Width & height in pixels*/
    int	o_changed;			/*Changed since last refresh?*/
    short o_refcount;			/*Reference count*/
    struct gwin	*o_window;		/*Object's associated graphical window*/
    struct onodeops *o_op;		/*Object's operations*/
    struct onode *o_home;		/*Ptr to home object*/
    struct onode *o_help;		/*Ptr to help object, if any*/
    struct onode *o_nextobj;		/*Ptr to next queued object, if any*/
    struct onode *o_upobj;		/*Ptr to parent (up) object, if any*/
    struct onode *o_downobj;		/*Ptr to child (down) object, if any*/
    int	*o_data;			/*Ptr to object's private data region*/
};

/*
 * Operations on individual onodes.  A pointer to this function array is
 * attached to each onode.  In reality, this array is different for each
 * object type, holding additional operations specific to that object.
 * However, every object must implement these functions in these first
 * slots.
 */
struct onodeops {
    int	(*on_destroy)();	/*Destroy an onode*/
    int	(*on_display)();	/*Display an onode*/
    int	(*on_release)();	/*Decrement an onode ref count*/
};

/*
 * Macros facilitating the use of onode functions.
 */
#define	OOP_DESTROY(ONP)	(ONP)->o_op->on_destroy(ONP);
#define	OOP_DISPLAY(ONP)	(ONP)->o_op->on_display(ONP);
#define	OOP_RELEASE(ONP)	(ONP)->o_op->on_release(ONP);

/*
 * Initialization parameters for an onode.
 */
struct onode_initparams {
    int	i_debug;			 /*Turn debugging on?*/
    struct gwin_initparams *i_gwparams;  /*Ptr to window init params*/
};

/*
 * Creation parameters for an onode.
 */
struct onode_createparams {
    int	cr_type;			/*Type of onode*/
    char cr_name[GATOR_OBJNAMELEN];	/*Object name*/
    int	cr_x, cr_y;			/*X and Y coordinates*/
    int	cr_width, cr_height;		/*Width & height in pixels*/
    struct gwin *cr_window;             /*Graphical window to use*/
    struct onode *cr_home_obj;          /*Home object*/
    struct onode *cr_prev_obj;          /*Object having this one as next*/
    struct onode *cr_parent_obj;        /*This object's parent*/
    char *cr_helpstring;		/*Ptr to object's help text*/
};

/*
  * Is debugging output enabled?
  */
extern int objects_debug;

extern int gator_objects_init();
    /*
     * Summary:
     *    Initialize the gator object package.
     *
     * Args:
     *	  struct onode_initparams *params: Initialization parameters.
     *
     * Returns:
     *	  0 on success,
     *	  Error value otherwise.
     */

extern struct onode *gator_objects_create();
    /*
     * Summary:
     *    Create an onode of the given type.
     *
     * Args:
     *	  struct onode_createparams *params: Ptr to creation params.
     *
     * Returns:
     *	  Ptr to newly-created onode if successful,
     *	  Null pointer otherwise.
     */

extern struct onode *gator_objects_lookup();
    /*
     * Summary:
     *    Look up a gator onode by name.
     *
     * Args:
     *	  char *onode_name: Onode string name to find.
     *
     * Returns:
     *	  Ptr to onode matching the given name if one exists,
     *	  Null pointer otherwise.
     */

#endif /* __gator_objects_h */
