/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: un_io.h,v $
 * Revision 1.1.9.1  1996/10/03  14:53:00  arvind
 * 	Include <sys/file.h> here to avoid some redefinition errors.
 * 	[1996/09/16  20:37 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.7.2  1996/02/18  23:01:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:59  marty]
 * 
 * Revision 1.1.7.1  1995/12/08  17:30:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:33  root]
 * 
 * Revision 1.1.4.5  1992/12/29  13:28:04  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:47:24  zeliff]
 * 
 * Revision 1.1.4.4  1992/10/20  15:50:30  sommerfeld
 * 	Change _HPUX_SOURCE to __hpux in #ifdef
 * 	[1992/10/05  16:17:18  sommerfeld]
 * 
 * 	Flush superfluous prototypes which should be in unistd.h anyway.
 * 	[1992/09/30  21:43:47  sommerfeld]
 * 
 * Revision 1.1.4.3  1992/09/29  21:17:38  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:21:25  sekhar]
 * 
 * Revision 1.1.4.2  1992/08/31  18:20:48  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  15:08:47  sekhar]
 * 
 * Revision 1.1.2.2  1992/03/31  22:40:39  burati
 * 	CR2419 Make sure POSIX protections are defined for use by sec_lrgy
 * 	[1992/03/30  23:30:37  burati]
 * 
 * Revision 1.1  1992/01/19  14:42:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  un_io.h V=11 02/24/92 //nbs/dds_tools/src
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
**      Prototypes for unix io functions
**
*/

#ifndef __unix_io_h__included__
#define __unix_io_h__included__

#include <sys/types.h>

/* Include pthreads/cma wrappers.  krb5 = //crown/prgy/krb5/include/krb5 */
#ifdef OSF_DCE
#   include <krb5/osf_dce.h>
#endif

#if defined(OSF) || defined(SNI_SVR4_POSIX) || defined(__hpux) || defined(_AIX)
#include <unistd.h>
#endif

#if defined(SNI_SVR4_POSIX) || defined(__hpux)
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>

/*
 * don't have to worry about L_SET because that's defined in <sys/file.h>
 * which is why an L_SET-based test doesn't work correctly -- rcc
 */

#ifndef L_INCR
#define L_INCR          SEEK_CUR       /* relative to current offset */
#endif

#ifndef L_XTND
#define L_XTND          SEEK_END       /* relative to end of file */
#endif

/* turn on posix-style file-locking */

#define DCE_SEC_POSIX_F_LOCK

#endif /* SNI_SVR4_POSIX || HPUX */

/* Definitions for POSIX, in case this system's stat.h doesn't have them */
#ifndef S_IRWXU
#define         S_IRWXU 0000700
#endif

#ifndef S_IRUSR
#define         S_IRUSR 0000400
#endif

#ifndef S_IWUSR
#define         S_IWUSR 0000200
#endif

#ifndef S_IXUSR
#define         S_IXUSR 0000100
#endif

#ifndef S_IRWXG
#define         S_IRWXG	0000070
#endif

#ifndef S_IRGRP
#define         S_IRGRP 0000040
#endif

#ifndef S_IWGRP
#define         S_IWGRP 0000020
#endif

#ifndef S_IXGRP
#define         S_IXGRP 0000010
#endif

#ifndef S_IRWXO
#define         S_IRWXO 0000007
#endif

#ifndef S_IROTH
#define         S_IROTH 0000004
#endif

#ifndef S_IWOTH
#define         S_IWOTH 0000002
#endif

#ifndef S_IXOTH
#define         S_IXOTH 0000001
#endif

#endif /* __unix_io_h__included__ */
