/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: krbfileio.c,v $
 * Revision 1.1.2.1  1996/06/05  21:16:56  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:07:11  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/os/krbfileio.c
 *
 * Copyright (c) Hewlett-Packard Company 1991
 * Released to the Massachusetts Institute of Technology for inclusion
 * in the Kerberos source code distribution.
 *
 * Copyright 1991 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
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
static char *VersionID = "@(#)krbfileio.c	2 - 08/22/91";
#endif

#define NEED_LOWLEVEL_IO                        /* Need open(), etc. */

#include "k5-int.h"
#ifdef HAVE_SYS_FILE_H
#include <sys/file.h>
#endif
#ifdef NEED_SYS_FCNTL_H
#include <sys/fcntl.h>
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifdef apollo
#   define OPEN_MODE_NOT_TRUSTWORTHY
#endif

krb5_error_code
krb5_create_secure_file(context, pathname)
    krb5_context context;
    const char * pathname;
{
    int fd;

    /*
     * Create the file with access restricted to the owner
     */
    fd = open(pathname, O_RDWR | O_CREAT | O_EXCL | O_BINARY, 0600);

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
krb5_sync_disk_file(context, fp)
    krb5_context context;
    FILE *fp;
{
    fflush(fp);
#ifndef MSDOS_FILESYSTEM
    if (fsync(fileno(fp))) {
        return errno;
    }
#endif

    return 0;
}

