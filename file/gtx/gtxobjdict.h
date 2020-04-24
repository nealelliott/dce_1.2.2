/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gtxobjdict.h,v $
 * Revision 1.1.11.1  1996/10/02  17:51:09  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:40:02  damon]
 *
 * Revision 1.1.6.2  1994/07/13  22:28:28  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:38:09  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:31  mbs]
 * 
 * Revision 1.1.6.1  1994/06/09  14:10:05  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:37  annie]
 * 
 * Revision 1.1.2.2  1993/01/19  16:03:15  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:12:57  cjd]
 * 
 * Revision 1.1  1992/01/19  02:41:40  devrcs
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

#ifndef __gator_objdict_h
#define	__gator_objdict_h  1

/*--------------------------------------------------------------------------------
 * objdict.h
 *
 * Definitions for the gator object dictionary.
 *--------------------------------------------------------------------------------*/

#include <gtxobjects.h>		/*Standard gator object defns*/

extern int gator_objdict_init();
    /*
     * Summary:
     *    Initialize the gator object dictionary package.
     *
     * Args:
     *	  int adebug: Is debugging output turned on?
     *
     * Returns:
     *	  0 on success,
     *	  Error value otherwise.
     */

extern int gator_objdict_add();
    /*
     * Summary:
     *    Add an entry to the gator object dictionary.
     *
     * Args:
     *	  struct onode *objtoadd: Ptr to object to add.
     *
     * Returns:
     *	  0 on success,
     *	  Error value otherwise.
     */

extern int gator_objdict_delete();
    /*
     * Summary:
     *    Delete an entry from the gator object dictionary.
     *
     * Args:
     *	  struct onode *objtodelete: Ptr to object to delete.
     *
     * Returns:
     *	  0 on success,
     *	  Error value otherwise.
     */

extern struct onode *gator_objdict_lookup();
    /*
     * Summary:
     *    Look up a gator object by name.
     *
     * Args:
     *	  char *nametofind: String name of desired onode.
     *
     * Returns:
     *	  Ptr to desired onode if successful,
     *	  Null pointer otherwise.
     */

#endif /* __gator_objdict_h */
