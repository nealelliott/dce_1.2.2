/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: autoconf.h,v $
 * Revision 1.1.2.1  1996/06/04  21:50:11  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:32 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 	[1996/05/03  19:58 UTC  mullan_s  /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/1]
 *
 * 	Add OSF/HP copyrights & RCS header.
 * 	[1996/05/03  19:58 UTC  mullan_s  /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/1]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/

/* autoconf.h.  Generated automatically by configure.  */
/* autoconf.h.in.  Generated automatically from configure.in by autoheader.  */

/* Define to empty if the keyword does not work.  */
/* #undef const */

/* Define if you can safely include both <sys/time.h> and <time.h>.  */
#define TIME_WITH_SYS_TIME 1

#define ANSI_STDIO 1
#define HAS_SETVBUF 1
/* #undef HAS_ANSI_CONST */
/* #undef HAS_ANSI_VOLATILE */
#define HAS_STDLIB_H 1
#define HAS_STRDUP 1
#define HAS_LABS 1
/* #undef HAS_SYS_FILE_H */
/* #undef HAS_SYS_PARAM_H */
#define HAS_VOID_TYPE 1
#define KRB5_PROVIDE_PROTOTYPES 1
/* #undef NEED_SYS_FCNTL_H */
/* #undef NO_NESTED_PROTOTYPES */
/* #undef NO_STDLIB_H */
#define NO_YYLINENO 1
#define POSIX_FILE_LOCKS 1
#define POSIX_SIGTYPE 1
#define POSIX_TERMIOS 1
#define POSIX_TYPES 1
#define USE_DIRENT_H 1
#define USE_STRING_H 1
/* #undef WAIT_USES_INT */
#define krb5_sigtype void
#define HAS_UNISTD_H 1
#define KRB5_USE_INET 1
/* #undef ODBM */
/* #undef TM_IN_SYS_TIME */

/* Define if MIT Project Athena default configuration should be used */
/* #undef KRB5_ATHENA_COMPAT */

/* Define if Kerberos V4 backwards compatibility should be supported */
/* #undef KRB5_KRB4_COMPAT */

/* The number of bytes in a int.  */
#define SIZEOF_INT 4

/* The number of bytes in a long.  */
#define SIZEOF_LONG 4

/* The number of bytes in a short.  */
#define SIZEOF_SHORT 2

/* Define if you have the <macsock.h> header file.  */
/* #undef HAVE_MACSOCK_H */

/* Define if you have the <stdarg.h> header file.  */
#define HAVE_STDARG_H 1

/* Define if you have the <sys/file.h> header file.  */
#define HAVE_SYS_FILE_H 1

/* Define if you have the <sys/param.h> header file.  */
#define HAVE_SYS_PARAM_H 1

/* Define if you have the <sys/stat.h> header file.  */
#define HAVE_SYS_STAT_H 1

/* Define if you have the <sys/time.h> header file.  */
#define HAVE_SYS_TIME_H 1

/* Define if you have the <sys/types.h> header file.  */
#define HAVE_SYS_TYPES_H 1

/* Define if you have the dbm library (-ldbm).  */
#define HAVE_LIBDBM 1

/* Define if you have the ndbm library (-lndbm).  */
#define HAVE_LIBNDBM 1
