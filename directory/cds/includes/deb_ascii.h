/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: deb_ascii.h,v $
 * Revision 1.1.4.2  1996/02/18  23:33:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:14  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:15:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:42:46  root]
 * 
 * Revision 1.1.2.3  1994/08/03  19:02:10  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:56:00  mccann]
 * 
 * Revision 1.1.2.2  1994/06/09  18:38:49  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:21  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/29  15:50:07  peckham
 * 	New file split off from dns_debug.h
 * 	[1994/04/29  13:57:02  peckham]
 * 
 * $EndLog$
 */
#if !defined(_DEB_ASCII_H)
#define _DEB_ASCII_H
/*
 * Module: deb_ascii.h
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 * This file contains the parameter interpreters for debug statements.
 */
#include <dce/dns_config.h>

#if defined(DCE_CDS_DEBUG)
# define DEB_ASCII_BUF(nam,len) char nam[len+1];
#else
# define DEB_ASCII_BUF(nam,len)
#endif

#define LOG_ASCII_BUF(nam,len) char nam[len+1];

/*
 * Debugging event buffer declarators.   These are used to store ASCII
 * versions of data structures inline with the DEBUG_EVENT call.
 *
 * All lengths exclude terminating NULL.
 *
 * DEB_xxx structures are sensitive to the instrumentation build settings
 * while LOG_xxx structures are not.
 */

/* numbers correct for sizeof() <= 8 */
#define DEB_ASCII_LEN_s_8	4
#define DEB_ASCII_LEN_u_8	3
#define DEB_ASCII_LEN_s_16	6
#define DEB_ASCII_LEN_u_16	5
#define DEB_ASCII_LEN_s_32	11
#define DEB_ASCII_LEN_u_32	10
#define DEB_ASCII_LEN_ptr	(2*sizeof(int *)+2)
#define DEB_ASCII_LEN_ptr_TO	(2*sizeof(int *)+4)

#define DEB_ASCII_LEN_char	4
#define DEB_ASCII_LEN_u_char	3
#define DEB_ASCII_LEN_short	(2*sizeof(short)+3)
#define DEB_ASCII_LEN_u_short	(2*sizeof(short)+3)
#define DEB_ASCII_LEN_int	(2*sizeof(int)+3)
#define DEB_ASCII_LEN_u_int	(2*sizeof(int)+3)
#define DEB_ASCII_LEN_long	(2*sizeof(long)+3)
#define DEB_ASCII_LEN_u_long	(2*sizeof(long)+3)

/*
 * Format strings for pointers
 */
#define DEB_ASCII_FMT_ptr	"%p"
#define DEB_ASCII_FMT_ptr_TO	"%p->"

/*
 * formatting aids
 */
char *
deb_ascii_ptr_to_buf (
    char       *,
    const long *);

/*
 * unix
 */

char *
deb_ascii_errno (
    char *const ,
    const int   ,
    const int   );
#define	DEB_ASCII_LEN_errno (DEB_ASCII_LEN_int+15)
#define	DEB_ASCII_BUF_errno(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_errno)
#define	LOG_ASCII_BUF_errno(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_errno)


/*
 * dnsmessage.h
 */
#if !defined(DNS_SUCCESS)
# include <dce/dcecdsmsg.h>
# include <dce/dnsmessage.h>
#endif

  /* temporary */
#if !defined(_DNS_STATUS_T_)
# define _DNS_STATUS_T_ 1
  typedef unsigned int dns_status_t;
#endif

char *
deb_ascii_dns_status (
    char *const        ,
    const dns_status_t );
#define	DEB_ASCII_LEN_dns_status DEB_ASCII_LEN_long
#define	DEB_ASCII_BUF_dns_status(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_dns_status)
#define	LOG_ASCII_BUF_dns_status(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_dns_status)

/*
 * sys_time.h
 */

char *
deb_ascii_time_quad (
    char *const       ,
    const void *const );
#define deb_ascii_time(b,p) deb_ascii_time_quad(b,p)
#define	DEB_ASCII_LEN_time_quad 50
#define	DEB_ASCII_BUF_time_quad(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_time_quad)
#define	LOG_ASCII_BUF_time_quad(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_time_quad)

char *
deb_ascii_time_local (
    char *const       ,
    const void *const );
#define	DEB_ASCII_LEN_time_local 50
#define	DEB_ASCII_BUF_time_local(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_time_local)
#define	LOG_ASCII_BUF_time_local(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_time_local)

char *
deb_ascii_uuid (
    char *const       ,
    const void *const );

#define DEB_ASCII_LEN_uuid 50
#define DEB_ASCII_BUF_uuid(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_uuid)
#define LOG_ASCII_BUF_uuid(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_uuid)

#if defined(DCE_SEC)

char *
deb_ascii_seclogin (
    char *const       ,
    const void *const );

# define DEB_ASCII_LEN_seclogin 256 /* should be FILENAME_MAX, but this ok*/
# define DEB_ASCII_BUF_seclogin(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_seclogin)
# define LOG_ASCII_BUF_seclogin(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_seclogin)

#endif /* DCE_SEC */

#endif  /* #ifndef _DEB_ASCII_H */
