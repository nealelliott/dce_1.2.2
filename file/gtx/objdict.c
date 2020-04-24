/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: objdict.c,v $
 * Revision 1.1.71.1  1996/10/02  17:51:29  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:40:18  damon]
 *
 * Revision 1.1.66.3  1994/08/03  16:14:25  ruby
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  16:59:05  ruby]
 * 
 * Revision 1.1.66.2  1994/06/09  14:10:33  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:49  annie]
 * 
 * Revision 1.1.66.1  1994/02/04  20:20:31  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:13:58  devsrc]
 * 
 * Revision 1.1.64.1  1993/12/07  17:26:29  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:33:13  jaffe]
 * 
 * Revision 1.1.5.3  1993/01/19  16:03:55  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:13:44  cjd]
 * 
 * Revision 1.1.5.2  1992/11/24  17:52:26  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  18:05:59  bolinger]
 * 
 * Revision 1.1.3.3  1992/01/24  03:45:43  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:13:50  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  19:50:12  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:34:38  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:17:32  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
*/
/*
 * (C) Copyright Transarc Corporation 1989
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

/*------------------------------------------------------------------------
 * objdict.c
 *
 * Description:
 *	Implementation of the gator object dictionary.
 *
 * Author:
 *	Ed. Zayas
 *	Transarc Corporation
 *------------------------------------------------------------------------*/

#include <dcedfs/gtxobjdict.h>		/*Interface for this module*/
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

static char mn[] = "gator_objdict";	/*Module name*/
static int objdict_debug;		/*Is debugging turned on?*/

/*------------------------------------------------------------------------
 * gator_objdict_init
 *
 * Description:
 *	Initialize the gator object dictionary package.
 *
 * Arguments:
 *	int adebug: Is debugging turned on?
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
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_objdict_init(adebug)
    int adebug;

{ /*gator_objdict_init*/

    static char	rn[] = "gator_objdict_init";	/*Routine name*/

    /*
      * Remember what our debugging setting is.
      */
    objdict_debug = adebug;

    if (objdict_debug)
      dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    /*
     * Finally, return the good news.
     */
    return(0);

} /*gator_objdict_init*/

/*------------------------------------------------------------------------
 * gator_objdict_add
 *
 * Description:
 *	Add an entry to the gator object dictionary.
 *
 * Arguments:
 *	struct onode *objtoadd: Ptr to object to add.
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

int gator_objdict_add(objtoadd)
    struct onode *objtoadd;

{ /*gator_objdict_add*/

    static char	rn[] = "gator_objdict_add"; /*Routine name*/

    if (objdict_debug)
      dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    /*
     * Finally, return the good news.
     */
    return(0);

} /*gator_objdict_add*/

/*------------------------------------------------------------------------
 * gator_objdict_delete
 *
 * Description:
 *	Delete an entry from the gator object dictionary.
 *
 * Arguments:
 *	struct onode *objtodelete: Ptr to object to delete.
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

int gator_objdict_delete(objtodelete)
    struct onode *objtodelete;

{ /*gator_objdict_delete*/

    static char	rn[] = "gator_objdict_delete";	/*Routine name*/

    if (objdict_debug)
      dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    /*
     * Finally, return the good news.
     */
    return(0);

} /*gator_objdict_delete*/

/*------------------------------------------------------------------------
 * gator_objdict_lookup
 *
 * Description:
 *	Look up a gator object by name.
 *
 * Arguments:
 *	char *nametofind: String name of desired onode.
 *
 * Returns:
 *	Ptr to desired onode if successful,
 *	Null pointer otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

struct onode *gator_objdict_lookup(nametofind)
    char *nametofind;

{ /*gator_objdict_lookup*/

    static char	rn[] = "gator_objdict_lookup";	/*Routine name*/

    if (objdict_debug)
      dce_svc_printf(GTX_S_ENTERING_ROUTINE_MSG, mn, rn);

    /*
     * Finally, return the good news.
     */
    return((struct onode *)0);

} /*gator_objdict_lookup*/
