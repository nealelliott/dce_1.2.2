/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: error_macros.h,v $
 * Revision 1.1.11.1  1996/10/02  17:02:45  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:24  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1991, 1996 Transarc Corporation
 * All Rights Reserved
 */
/*
 * error_macros.h -- macros for error and abort actions
 * 
 * $Header: /u0/rcs_trees/dce/rcs/file/bakserver/error_macros.h,v 1.1.11.1 1996/10/02 17:02:45 damon Exp $
 */

#define ERROR(evalue)                                           \
	{                                                       \
            code = evalue;                                      \
            goto error_exit;                                    \
        }

#define ABORT(evalue)                                           \
	{                                                       \
            code = evalue;                                      \
            goto abort_exit;                                    \
        }

