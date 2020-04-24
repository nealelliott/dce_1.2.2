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
 * itl_fxConnect.h --
 *    Declarations for using the connection handle code.
 *
 *    This module stores information about a DFS client connection to a DFS
 *    server.  This is the infomation used to issue RPC's to DFS servers.
 *
 *    The initialization routine must be called prior to using this
 *    module.  Connections are added via the Create function.  They  are
 *    never removed.  Once a handle is created it is accessed via a call
 *    to the Get function.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxConnect.h,v 1.1.11.1 1996/10/17 18:26:20 damon Exp $
 */
/*
 * HISTORY
 * $Log: itl_fxConnect.h,v $
 * Revision 1.1.11.1  1996/10/17  18:26:20  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:15  damon]
 *
 * Revision 1.1.6.1  1994/02/04  20:44:19  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:01  devsrc]
 * 
 * Revision 1.1.4.1  1994/01/28  20:44:20  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:45  annie]
 * 
 * Revision 1.1.2.2  1993/01/13  21:09:35  shl
 * 	Transarc delta: fred-ot-6350-add-fx-tests 1.3
 * 	  Selected comments:
 * 	    To place the DFS fole exporter tests into the tree.
 * 	    Migrate the fx exerciser to the new fx interface (OT 6054)
 * 	    Fix compilation problems.
 * 	[1993/01/13  14:20:58  shl]
 * 
 * $EndLog$
 */

#ifndef ITL_FX_CONNECT_H
#define ITL_FX_CONNECT_H

#include <dce/rpc.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_fxFileset.h>

#define ITL_FX_PRIMARY_IF     0
#define ITL_FX_SECONDARY_IF   1

typedef struct {
    handle_t handle;
    unsigned int interface;
    itlFX_fileset_t *filesetP;
} itlFX_connect_t;

/*
 *  The following macros access fields within the itlFX_connect_t data
 *  type.  They expect a pointer to a itlFX_connect_t.
 */
#define ITL_FX_CONNECT_HANDLE(_cp)       (_cp)->handle
#define ITL_FX_CONNECT_INTERFACE(_cp)    (_cp)->interface
#define ITL_FX_CONNECT_FILESET(_cp)      (_cp)->filesetP

/**
 **  Function declarations
 **/

extern void itlFX_ConnectInit _TAKES((
				     void
				     ));

extern boolean_t itlFX_ConnectCreate _TAKES((
					     IN char *filesetName,
					     IN unsigned int interface,
					     IN char *userName,
					     IN char *varName
					     ));

extern itlFX_connect_t *itlFX_ConnectGet _TAKES((
						 void
						 ));

extern var_val_t *itlFX_ConnectNew _TAKES((
					   IN pointer_t newConnectP,
					   IN int newType
					   ));

extern boolean_t itlFX_ConnectFree _TAKES((
					   IN var_val_t *freeConnectP
					   ));

extern var_val_t *itlFX_ConnectCopy _TAKES((
					    IN var_val_t *copyConnectP
					    ));

extern char *itlFX_ConnectDisplay _TAKES((
					  IN var_val_t *displayConnectP,
					  OUT int *lengthP
					  ));

#endif /* ITL_FX_CONNECT_H */

