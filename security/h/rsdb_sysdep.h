/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_sysdep.h,v $
 * Revision 1.1.5.2  1996/02/18  23:00:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:02  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:28:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:59  root]
 * 
 * Revision 1.1.3.3  1992/12/29  13:26:38  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:46:54  zeliff]
 * 
 * Revision 1.1.3.2  1992/10/07  20:29:08  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:13:20  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:47:31  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1  1992/01/19  14:42:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - System Dependent  Routines
 *
 */

#ifndef rsdb_sysdep__included
#define rsdb_sysdep__included

#include <dce/uuid.h>
#include <dce/rgynbase.h>

void rsdb_sysdep_site_gen_id (
#ifdef __STDC__
    uuid_t  *(new_uid)
#endif
);

int rsdb_sysdep_fchmod (
#ifdef __STDC__
    int     fd,
    char    *pname,
    long    mode,
    long    protection
#endif
);

void rsdb_sysdep_chmod (
#ifdef __STDC__
    char    *pname,
    long    protection
#endif
);

void rsdb_sysdep_dir_chmod (
#ifdef __STDC__
    char  *pname,
    long  protection
#endif
);

#endif /* rsdb_sysdep__included */

