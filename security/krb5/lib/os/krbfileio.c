/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: krbfileio.c,v $
 * Revision 1.1.6.2  1996/02/18  00:15:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:45  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:03:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:36  root]
 * 
 * Revision 1.1.3.3  1992/12/29  16:30:17  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:17:51  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  20:42:42  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:34:16  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:50:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  krbfileio.c V=3 11/20/91 //littl/prgy/krb5/lib/os
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
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/krbfileio.c,v $
 * $Author: marty $
 *
 *
 * Copyright (c) Hewlett-Packard Company 1991
 * Released to the Massachusetts Institute of Technology for inclusion
 * in the Kerberos source code distribution.
 *
 * Copyright 1991 by the Massachusetts Institute of Technology.
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
 * krb5_create_secure_file
 * krb5_sync_disk_file
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)krbfileio.c	3 - 11/20/91";
#endif

#include <sys/types.h>

#ifdef SNI_SVR4_POSIX
#include <sys/stat.h>
#include <fcntl.h>
#endif

#include <krb5/krb5.h>
#include <krb5/libos.h>
#include <krb5/los-proto.h>

#include <sys/file.h>

#include <krb5/ext-proto.h>

#ifdef apollo
#   define OPEN_MODE_NOT_TRUSTWORTHY
#endif

krb5_error_code
krb5_create_secure_file(pathname)
    char *pathname;
{
    int fd;

    /*
     * Create the file with access restricted to the owner
     */
    fd = open(pathname, O_RDWR | O_CREAT | O_EXCL, 0600);

#ifdef OPEN_MODE_NOT_TRUSTWORTHY
    /*
     * Some systems that support default acl inheritance do not 
     * apply ownership information from the process - force the file
     * to have the proper info.
     */
    if (fd > -1) {
        uid_t   uid;
        gid_t   gid;

        uid = getuid();
        gid = getgid();

        fchown(fd, uid, gid);

        fchmod(fd, 0600);
    }
#endif /* OPEN_MODE_NOT_TRUSTWORTHY */

    if (fd > -1) {
        close(fd);
        return 0;
    } else {
        return errno;
    }
}

krb5_error_code
krb5_sync_disk_file(fp)
    FILE *fp;
{
    fflush(fp);
    if (fsync(fileno(fp))) {
        return errno;
    }

    return 0;
}

