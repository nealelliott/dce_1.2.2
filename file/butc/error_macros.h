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
 * Revision 1.1.11.1  1996/10/02  17:06:03  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:05:24  damon]
 *
 * Revision 1.1.5.1  1994/06/09  13:52:56  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:26:55  annie]
 * 
 * Revision 1.1.3.4  1993/01/18  20:48:21  cjd
 * 	embedded copyright notice
 * 	[1993/01/18  17:33:47  cjd]
 * 
 * Revision 1.1.3.3  1993/01/13  16:04:31  shl
 * 	Transarc delta: vijay-ot6381-correct-handling-of-device-queue-lock 1.4
 * 	  Selected comments:
 * 
 * 	    The aim of this delta is to get bak restore to succeed. The problems in restore
 * 	    were some spurious thread exceptions and assertions that made bak dump core.
 * 	    Upon investigation it turned out that there were some locking problems in
 * 	    backup and butc. The delta cleans up locking in backup and butc.
 * 	    The delta is not ready for export. Much more needs to be done to get all the
 * 	    locking issues right. This is just a checkpoint.
 * 	    Second set of changes to get locking issues in order. This time the changes
 * 	    are mostly in bakserver. The changes introduced are
 * 	    1. Establish a global lock so that two RPCs do not interfere with each other.
 * 	    This was introduced because there are some global data structures in
 * 	    backup that might get affected. These global structures now have
 * 	    individual locks to serialize changes. The global lock in temporary, and
 * 	    will go away once we have serialized all accesses to all global data
 * 	    structures.
 * 	    2. Disable cancels in bakserver RPCs. Care is taken to not disable cancels in
 * 	    operations that can be cancelled. There is some more work that needs to be
 * 	    done in this area.
 * 	    3. Accesses to the database are controlled by memoryDB.lock. The accesses are
 * 	    by means of dbread and dbwrite. The hash tables too are protected by this
 * 	    mechanism.
 * 	    4. Changes to the backup database dump code to simplify the dump operation. In
 * 	    particular, the Unix pipe mechanism is used to synchronize reader and
 * 	    writer without need for condition variables.
 * 	    5. Get rid of any pthread_mutex and pthread_cond operations. Use afslk and
 * 	    afsos routines instead.
 * 	    6. Continue the work described by the previous revision of the delta in bak
 * 	    and butc. This should be it for changes in bak and butc.
 * 	    Fix compilation problems on the RIOS.
 * 	    This is hopefully the final revision of this delta. The fixes here are
 * 	    1. Changes to the bakserver and butc RPC interfaces.
 * 	    The RPCs that handle variable size arrays now use conformant arrays.
 * 	    This avoids allocating lots of memory on the client thread stack which
 * 	    was the cause of the exceptions in restoreft. The server allocates
 * 	    memory which is freed by the server stub on the way out. The client thread
 * 	    frees the memory allocated by the client stub.
 * 	    2. get database dump and restore to handle status correctly.
 * 	    3. Get the locking hierarchy right in bakserver, bak and butc.
 * 	    4. Other minor nits.
 * 	    There is still a problem with scantape -dbadd that has to be addressed. With
 * 	    this delta, all backup commands should work when used in a straightforward
 * 	    manner. Some error cases are still not handled properly though. Subsequent
 * 	    deltas would fix those.
 * 	Transarc delta: vijay-ot6663-bak-remove-obsolete-files 1.1
 * 	  Selected comments:
 * 
 * 	    Remove some obsolete files. Reorganize some of the header files which caused
 * 	    some header files to go obsolete. Handle a merge error in budb.idl and also
 * 	    in procs.c.
 * 	[1993/01/12  18:44:16  shl]
 * 
 * Revision 1.1.3.2  1992/09/25  18:02:50  jaffe
 * 	Transarc delta: jaffe-ot5416-cleanup-RCS-log-entries 1.1
 * 	  Selected comments:
 * 	    Cleanup extra RCS log entries.  There should only be one per file, and
 * 	    it should be closed with an EndLog comment.
 * 	[1992/09/23  18:37:36  jaffe]
 * 
 * Revision 1.1  1992/01/19  02:54:28  devrcs
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
 * $Header: /u0/rcs_trees/dce/rcs/file/butc/error_macros.h,v 1.1.11.1 1996/10/02 17:06:03 damon Exp $
 */

#define ERROR_EXIT(evalue)                                      \
	{                                                       \
            code = evalue;                                      \
	    if (debugLevel)					\
	      fprintf(stderr, "ERROR: file %s, line %d, code = %d\n",	\
		      __FILE__, __LINE__, code);		\
            goto error_exit;                                    \
        }

#define ABORT_EXIT(evalue)                                      \
	{                                                       \
            code = evalue;                                      \
	    if (debugLevel)					\
	      fprintf(stderr, "ABORT: file %s, line %d, code = %d\n",	\
		      __FILE__, __LINE__, code);		\
            goto abort_exit;                                    \
        }

