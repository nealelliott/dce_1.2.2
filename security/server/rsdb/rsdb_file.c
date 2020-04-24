/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_file.c,v $
 * Revision 1.1.4.2  1996/02/18  00:21:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:09:20  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:59:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:22:20  root]
 * 
 * Revision 1.1.2.5  1992/12/29  16:38:53  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:28:31  zeliff]
 * 
 * Revision 1.1.2.4  1992/11/20  15:54:51  ahop
 * 	fix typo in prototye declaration
 * 	[1992/11/16  16:42:18  ahop]
 * 
 * 	remove problematic base_path initialization
 * 	[1992/11/13  21:00:51  ahop]
 * 
 * 	Add rsdb_file_mkey_* for mkey file i/o
 * 	[1992/11/10  23:15:30  ahop]
 * 
 * Revision 1.1.2.3  1992/09/29  20:14:07  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:40:24  sekhar]
 * 
 * Revision 1.1.2.2  1992/08/31  18:28:23  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  15:12:56  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:44:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 *      File utilities
 */

#ifdef MODULE_VERSION_ID
static char *VersionID =
    "$Id: rsdb_file.c,v 1.1.4.2 1996/02/18 00:21:30 marty Exp $";
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/param.h>
#include <errno.h>

#include <rs_base.h>
#include <rsdb_file.h>
#include <rsdb_sysdep.h>

#include <un_io.h>
#include <un_strng.h>

#include <krb5/krb5.h>
#include <krb5/kdb.h>
#include <krb5/los-proto.h>


PRIVATE char base_path[MAXPATHLEN];
PRIVATE int  base_path_len;

PRIVATE char mkey_base_path[MAXPATHLEN];
PRIVATE int  mkey_base_path_len;

PUBLIC void rsdb_file_set_base_path
#ifndef __STDC__
    ( pname )
    char    *pname;
#else
  (
    char    *pname
  )
#endif
{
    strcpy(base_path, pname);
    base_path_len = strlen(base_path);
}

PUBLIC void rsdb_file_mkey_set_base_path
#ifndef __STDC__
    ( pname )
    char    *pname;
#else
  (
    char    *pname
  )
#endif
{
    strcpy(mkey_base_path, pname);
    mkey_base_path_len = strlen(mkey_base_path);
}

PUBLIC int rsdb_file_stat
#ifndef __STDC__
    ( pname, buf )
    char        *pname;
    struct stat *buf;
#else
  (
    char        *pname,
    struct stat *buf
  )
#endif
{
    strcpy(&base_path[base_path_len], pname);
    return stat(base_path, buf);
}

PUBLIC int rsdb_file_mkey_stat
#ifndef __STDC__
    ( fname, buf )
    char        *fname;
    struct stat *buf;
#else
  (
    char        *fname,
    struct stat *buf
  )
#endif
{
    strcpy(&mkey_base_path[mkey_base_path_len], fname);
    return stat(mkey_base_path, buf);
}

PUBLIC int rsdb_file_fstat
#ifndef __STDC__
    ( fd, buf )
    int         fd;
    struct stat *buf;
#else
  (
    int         fd,
    struct stat *buf
  )
#endif
{
    return fstat(fd, buf);
}

PUBLIC int rsdb_file_open
#ifndef __STDC__
    ( pname, mode, protection )
    char    *pname;
    long    mode;
    long    protection;
#else
  (
    char    *pname,
    long    mode,
    long    protection
  )
#endif
{
    int fd;

    strcpy(&base_path[base_path_len], pname);
    fd = open(base_path, mode, protection);
    fd = rsdb_sysdep_fchmod(fd, base_path, mode, protection);
    return fd;
}

PUBLIC void rsdb_file_mkey_create
#ifndef __STDC__
    ( fname, st_p )
    char            *fname;
    error_status_t  *st_p;   
#else
  (
    char            *fname,
    error_status_t  *st_p
  )
#endif
{
    CLEAR_STATUS(st_p);

    strcpy(&mkey_base_path[mkey_base_path_len], fname);
    *st_p = krb5_create_secure_file(mkey_base_path);
}

PUBLIC int rsdb_file_mkey_open
#ifndef __STDC__
    ( pname, mode, protection )
    char    *pname;
    long    mode;
    long    protection;
#else
  (
    char    *pname,
    long    mode,
    long    protection
  )
#endif
{
    int fd;

    strcpy(&mkey_base_path[mkey_base_path_len], pname);
    fd = open(mkey_base_path, mode, protection);
    fd = rsdb_sysdep_fchmod(fd, mkey_base_path, mode, protection);
    return fd;
}

PUBLIC int rsdb_file_open_fname
#ifndef __STDC__
    ( pname, mode, protection )
    char    *pname;
    long    mode;
    long    protection;
#else
  (
    char    *pname,
    long    mode,
    long    protection
  )
#endif
{
    int fd;

    fd = open(pname, mode, protection);
    fd = rsdb_sysdep_fchmod(fd, pname, mode, protection);
    return fd;
}

PUBLIC void rsdb_file_chmod
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
    strcpy(&base_path[base_path_len], pname);
    rsdb_sysdep_chmod(base_path, protection);
}

PUBLIC int rsdb_file_mkdir
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
    char    c;
    char    *p;
    boolean32 retval = true;
    int     mkdir_result;

    /*
    * Make sure that the last two components of the base path exist.
    * in general the basepath will be .../registry/rgy_data{.new} and
    * it is possible that neither the registry nor the rgy_data{.new}
    * directories pre-exist.
    */
    if (base_path_len > 1) {
        for (p = &base_path[base_path_len - 2];
                *p != '/' && p != base_path; p--);
        if (p != base_path) {
            c = *p;
            *p = '\0';
            mkdir(base_path, protection);
            *p = c;
            base_path[base_path_len] = '\0';

            /* Sun's mkdir will not accept pathnames that
             *end in '/'.
             */
            /* Could be ifdef'd ??? */
            c = '\0';
            if ( base_path[base_path_len-1] == '/' ) {
                c = base_path[base_path_len-1];
                base_path[base_path_len-1] ='\0';
            }
            mkdir_result = mkdir(base_path, protection);

            /* Hold off on restoring trailing '/' */

        }
    }


    /* Create the named directory if need be */
    if (pname[0] != '\0' ) {
        strcat(base_path, "/");
        strcat(base_path, pname);
        mkdir_result = mkdir(base_path, protection);
    }
    else
        base_path[base_path_len-1] = c;   /* Restore trailing '/' */

    if (mkdir_result == -1) {
        if (errno != EEXIST) {
            retval = false;
        }
    }


    if (retval) {
        rsdb_sysdep_dir_chmod(base_path, protection);
    }

    return retval;
}

PUBLIC int rsdb_file_rename
#ifndef __STDC__
    ( source, target )
    char    *source;
    char    *target;
#else
  (
    char    *source,
    char    *target
  )
#endif
{
    char src_path[MAXPATHLEN];
    char dst_path[MAXPATHLEN];

    base_path[base_path_len] = '\0';

    strcpy(src_path, base_path);
    strcpy(dst_path, base_path);

    strcpy(&src_path[base_path_len], source);
    strcpy(&dst_path[base_path_len], target);

    return rename(src_path, dst_path);
}

PUBLIC int rsdb_file_mkey_rename
#ifndef __STDC__
    ( source, target )
    char    *source;
    char    *target;
#else
  (
    char    *source,
    char    *target
  )
#endif
{
    char src_path[MAXPATHLEN];
    char dst_path[MAXPATHLEN];

    mkey_base_path[mkey_base_path_len] = '\0';

    strcpy(src_path, mkey_base_path);
    strcpy(dst_path, mkey_base_path);

    strcpy(&src_path[mkey_base_path_len], source);
    strcpy(&dst_path[mkey_base_path_len], target);

    return rename(src_path, dst_path);
}

PUBLIC int rsdb_file_unlink
#ifndef __STDC__
    ( fname )
    char    *fname;
#else
  (
    char    *fname
  )
#endif
{
    strcpy(&base_path[base_path_len], fname);

    return unlink(base_path);
}

PUBLIC int rsdb_file_mkey_unlink
#ifndef __STDC__
    ( fname )
    char    *fname;
#else
  (
    char    *fname
  )
#endif
{
    strcpy(&mkey_base_path[mkey_base_path_len], fname);

    return unlink(mkey_base_path);
}

PUBLIC boolean32 rsdb_file_position
#ifndef __STDC__
    ( fd, position )
    int                     fd;
    rsdb_file_position_t    position;
#else
  (
    int                     fd,
    rsdb_file_position_t    position
  )
#endif
{
    boolean32   status;

#ifdef BSD
    if (position == rsdb_file_position_beginning) {
        status = (lseek(fd, 0, L_SET) != -1);
    } else {
        status = (lseek(fd, 0, L_XTND) != -1);
    }
#else
    if (position == rsdb_file_position_beginning) {
        status = (lseek(fd, 0, SEEK_SET) != -1);
    } else {
        status = (lseek(fd, 0, SEEK_END) != -1);
    }
#endif /* BSD */

    return status;
}



