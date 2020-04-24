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
 * Revision 1.1.11.1  1996/10/02  16:58:45  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:03:53  damon]
 *
 * Revision 1.1.5.1  1994/06/09  13:50:29  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:23:52  annie]
 * 
 * Revision 1.1.3.4  1993/01/18  19:51:17  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:25:36  cjd]
 * 
 * Revision 1.1.3.3  1993/01/13  15:23:51  shl
 * 	Transarc delta: vijay-ot1104-bak-exit-status-not-set-correctly 1.1
 * 	  Selected comments:
 * 
 * 	    bak lsftfam was not setting the exit status correctly on failure. I made a
 * 	    pass through the bak command functions to make sure exit status was set
 * 	    properly for other commands. There were a couple of other error paths where
 * 	    the exit status was not set correctly. Two other minor things fixed as part
 * 	    of this delta
 * 	    1. error messages now don't print file/line numbers of errors. Less clutter.
 * 	    2. bak <subcommand> -h takes us into the bak interactive mode. It should
 * 	    just print usage for this subcommand and exit.
 * 	[1993/01/12  17:32:35  shl]
 * 
 * Revision 1.1.3.2  1992/09/25  17:48:23  jaffe
 * 	Transarc delta: jaffe-ot5416-cleanup-RCS-log-entries 1.1
 * 	  Selected comments:
 * 	    Cleanup extra RCS log entries.  There should only be one per file, and
 * 	    it should be closed with an EndLog comment.
 * 	[1992/09/23  18:10:01  jaffe]
 * 
 * Revision 1.1  1992/01/19  02:56:49  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Copyright (C) 1991 Transarc Corporation
 * All Rights Reserved
 */
/*
 * error_macros.h -- error and abort macros
 * 
 * $Header: /u0/rcs_trees/dce/rcs/file/bak/error_macros.h,v 1.1.11.1 1996/10/02 16:58:45 damon Exp $
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

