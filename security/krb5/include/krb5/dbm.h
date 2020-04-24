/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dbm.h,v $
 * Revision 1.1.4.2  1996/02/18  23:02:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:42  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:41:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:56:57  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:58:26  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:52:02  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:48:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  dbm.h V=2 10/23/91 //littl/prgy/krb5/include/krb5
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/include/krb5/dbm.h,v $
 * $Author: marty $
 * $Id: dbm.h,v 1.1.4.2 1996/02/18 23:02:16 marty Exp $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America is assumed
 *   to require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 * 
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 *
 * DBM/NDBM include file; deals with converting old-style to new-style.
 */


#ifndef KRB5_DBM_COMPAT__
#define KRB5_DBM_COMPAT__

#include <krb5/osconf.h>

#ifndef ODBM
#include <ndbm.h>
#else /*ODBM*/
#include <dbm.h>
#endif /*ODBM*/

#ifndef ODBM
#define dbm_next(db,key) dbm_nextkey(db)
#else /* OLD DBM */
typedef char DBM;

/* Macros to convert ndbm names to dbm names.
 * Note that dbm_nextkey() cannot be simply converted using a macro, since
 * it is invoked giving the database, and nextkey() needs the previous key.
 *
 * Instead, all routines call "dbm_next" instead.
 */

#define dbm_open(file, flags, mode) ((dbminit(file) == 0)?"":((char *)0))
#define dbm_fetch(db, key) fetch(key)
#define dbm_store(db, key, content, flag) store(key, content)
#define dbm_delete(db, key) delete(key)
#define dbm_firstkey(db) firstkey()
#define dbm_next(db,key) nextkey(key)
#define dbm_close(db) dbmclose()
#endif /* OLD DBM */

#endif /* KRB5_DBM_COMPAT__ */
