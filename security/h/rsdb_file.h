/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_file.h,v $
 * Revision 1.1.4.2  1996/02/18  23:00:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:43  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:28:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:49  root]
 * 
 * Revision 1.1.2.3  1992/12/29  13:26:09  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:46:24  zeliff]
 * 
 * Revision 1.1.2.2  1992/11/20  15:51:27  ahop
 * 	Add mkey file i/o operations
 * 	[1992/11/10  23:05:09  ahop]
 * 
 * Revision 1.1  1992/01/19  14:42:14  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      File utilities - external definitions
 * 
 */

#ifndef rsdb_file__included
#define rsdb_file__included

#include <dce/nbase.h>


#include <stdio.h>         /* definitions required for passthru macros below */
#include <un_stat.h>

typedef enum { rsdb_file_position_beginning, 
                rsdb_file_position_end } rsdb_file_position_t;

void rsdb_file_set_base_path (
#ifdef __STDC__
    char    *pname
#endif
);

void rsdb_file_mkey_set_base_path (
#ifdef __STDC__
    char    *pname
#endif
);

int rsdb_file_stat (
#ifdef __STDC__
    char        *pname,
    struct stat *buf
#endif
);

int rsdb_file_mkey_stat (
#ifdef __STDC__
    char        *pname,
    struct stat *buf
#endif
);

int rsdb_file_fstat (
#ifdef __STDC__
    int         fd,
    struct stat *buf
#endif
);

int rsdb_file_open (
#ifdef __STDC__
    char    *pname,
    long    mode,
    long    protection
#endif
);

void rsdb_file_mkey_create (
#ifdef __STDC__
    char            *fname,
    error_status_t  *st_p
#endif
);

int rsdb_file_mkey_open (
#ifdef __STDC__
    char    *pname,
    long    mode,
    long    protection
#endif
);

int rsdb_file_open_fname (
#ifdef __STDC__
    char    *pname,
    long    mode,
    long    protection
#endif
);

void rsdb_file_chmod ( 
#ifdef __STDC__
    char    *pname,
    long    protection
#endif
);

int rsdb_file_mkdir (
#ifdef __STDC__
    char    *pname,
    long    protection
#endif
);

int rsdb_file_rename (
#ifdef __STDC__
    char    *source,
    char    *target
#endif
);

int rsdb_file_mkey_rename (
#ifdef __STDC__
    char    *source,
    char    *target
#endif
);

int rsdb_file_unlink (
#ifdef __STDC__
    char    *fname
#endif
);

int rsdb_file_mkey_unlink (
#ifdef __STDC__
    char    *fname
#endif
);

boolean32 rsdb_file_position (
#ifdef __STDC__
    int                     fd,
    rsdb_file_position_t    position
#endif
);

/*
 * Pass through to stdio macros
 */

#define rsdb_file_close(fd)    close(fd)
#define rsdb_file_fsync(fd)    fsync(fd)

#endif
