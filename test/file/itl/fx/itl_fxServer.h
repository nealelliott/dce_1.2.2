/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_fxPrincipal.h --
 *    Declarations for using the server name hash table.
 *
 *    This module stores information about a DFS server that is needed
 *    to issue DFS cache manager RPC's.
 *    The initialization routine must be called prior to using this
 *    module.  Filesets are added via the AddName function, and
 *    are removed with the RemoveName function.  Data is accessed via
 *    the appropriately named functions.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxServer.h,v 1.1.11.1 1996/10/17 18:26:42 damon Exp $
 */
/*
 * HISTORY
 * $Log: itl_fxServer.h,v $
 * Revision 1.1.11.1  1996/10/17  18:26:42  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:25  damon]
 *
 * Revision 1.1.6.1  1994/02/04  20:44:45  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:15  devsrc]
 * 
 * Revision 1.1.4.1  1994/01/28  20:44:37  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:51  annie]
 * 
 * Revision 1.1.2.2  1993/01/13  21:34:31  shl
 * 	Transarc delta: fred-ot-6350-add-fx-tests 1.3
 * 	  Selected comments:
 * 	    To place the DFS fole exporter tests into the tree.
 * 	    Migrate the fx exerciser to the new fx interface (OT 6054)
 * 	    Fix compilation problems.
 * 	[1993/01/13  14:36:36  shl]
 * 
 * $EndLog$
 */

#ifndef ITL_FX_SERVER_H
#define ITL_FX_SERVER_H

#include <netinet/in.h>
#include <dcedfs/afsvl_data.h>
#include <utils/itl/itl_stds.h>
#include <utils/cht_hash.h>


#define ITL_FX_MAX_SERVER_NAME     64

#define ITL_FX_NUM_INTERFACES      2
#define ITL_FX_PRIMARY_IF          0
#define ITL_FX_SECONDARY_IF        1

/*
 *  The normal usage for this data will be that it will be accessed via
 *  the fileset info.  That data type returns a pointer to a itlFX_server_t
 *  so the following macros are provided.  All expect a pointer to a 
 *  itlFX_server_t as an argument, and return the approriate data.
 */
#define ITL_FX_SERVER_NAME(_sp)        (_sp)->principalName
#define ITL_FX_SERVER_OBJ_ID(_sp)      (_sp)->objId
#define ITL_FX_SERVER_PRIMARY(_sp)     (_sp)->primaryUUID
#define ITL_FX_SERVER_SECONDARY(_sp)   (_sp)->secondaryUUID
#define ITL_FX_SERVER_ADDRESS(_sp)     (_sp)->address

/**
 **  Data definitions
 **/

/*
 *  itlFX_server_t --
 *    Defines all data associated with a server.  The principal name
 *    (also assumed to be the server name) is kept here for error reporting.
 */
typedef struct itlFX_server {
    char *principalName;          /* name to use when talking to server */
    afsUUID objId;		  /* object-ID for ifspec being used */
    afsUUID primaryUUID;	  /* uuid to primary FX server interface */
    afsUUID secondaryUUID;	  /* uuid to secondary FX server interface */
    struct sockaddr_in address;   /* server's network address */
} itlFX_server_t;


/**
 **  Function declarations
 **/

extern void itlFX_ServerInit _TAKES((
				     void
				     ));

extern boolean_t itlFX_ServerAddName _TAKES((
					      IN struct vldbentry *vldbInfoP
					      ));

extern void itlFX_ServerRemoveName _TAKES((
					    IN char *nameP
					    ));

extern itlFX_server_t *itlFX_ServerLookup _TAKES((
						  IN char *nameP
						  ));

#endif /* ITL_FX_SERVER_H */

