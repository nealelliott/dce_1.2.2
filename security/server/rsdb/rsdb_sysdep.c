/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_sysdep.c,v $
 * Revision 1.1.7.2  1996/02/18  00:21:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:09:43  marty]
 *
 * Revision 1.1.7.1  1995/12/08  18:00:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:22:32  root]
 * 
 * Revision 1.1.3.4  1992/12/29  16:39:33  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:29:10  zeliff]
 * 
 * Revision 1.1.3.3  1992/10/07  20:38:15  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:20:34  sekhar]
 * 
 * Revision 1.1.4.2  1992/10/06  16:04:35  burati
 * 	Second replication drop: bl5
 * 
 * Revision 1.1.3.2  1992/09/29  20:14:23  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:41:12  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:44:35  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  rsdb_sysdep.c V=13 04/29/91 //littl/prgy/src/server/rsdb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 *
 *      Registry Server - miscellaneous system dependent routines
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rsdb_sysdep.c	13 - 04/29/91";
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <un_strng.h>
#include <un_io.h>

#include <rsdb_sysdep.h>
#include <rs_reserved.h>  /* unix numbers formerly in rgyc_list.ins.c */

#include <rs_base.h>
#include <rs_util.h>

PUBLIC int rsdb_sysdep_fchmod
#ifndef __STDC__
    ( fd, pname, mode, protection )
    int     fd;
    char    *pname;
    long    mode;
    long    protection;
#else
  (
    int     fd,
    char    *pname,
    long    mode,
    long    protection
  )
#endif
{
    if (fd == -1 || !(mode & O_CREAT)) {
        return fd;
    }

    fchmod(fd, protection);

    return fd;

}

PUBLIC void rsdb_sysdep_chmod
#ifndef __STDC__
    ( pname, protection )
    char    *pname;
    long    protection;
#else
  (
    char    *pname,
    long    protection
  )
#endif
{
    chmod(pname, protection);
}

PUBLIC void rsdb_sysdep_dir_chmod
#ifndef __STDC__
    ( pname, protection )
    char  *pname;
    long  protection;
#else
  (
    char  *pname,
    long  protection
  )
#endif
{
    chmod(pname, protection);
}
