/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dns_config.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:17  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:15:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:42:51  root]
 * 
 * Revision 1.1.4.4  1994/09/22  18:35:23  mccann
 * 	move defines to cds.mk
 * 	[1994/09/22  18:33:29  mccann]
 * 
 * Revision 1.1.4.3  1994/06/30  19:15:35  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:37:40  mccann]
 * 
 * 	sams cleanup drop 2
 * 
 * Revision 1.1.4.2  1994/06/09  18:38:52  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:25  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:02:42  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:13:31  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:29:20  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:47:17  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:15:30  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _DNS_CONFIG_H
#define _DNS_CONFIG_H
 /***********************************************************************
 * 
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
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
 */

#include <dce/dce.h>

#if defined(DCE_DEBUG) && !defined(DCE_CDS_DEBUG)
# define DCE_CDS_DEBUG 1
#endif

/*
 * Name of the software platform
 */
#define _DNS__UNIX      1
#define _DNS__VMS       2

#ifndef _DNS_OS_
# if defined(unix) || defined(__unix) || defined(__unix__) || defined(_AIX) || defined(__OSF__) || defined(__osf__)
#  define	_DNS_OS_	_DNS__UNIX
# endif
# if defined(vms) || defined(__vms) || defined(VMS) || defined(__VMS) || defined(__vms__)
#  define	_DNS_OS_	_DNS__VMS
# endif
# ifndef _DNS_OS_
   !!!Error: _DNS_OS_ not set
# endif
#endif

/*
 * sizes of various items
 */
#define K_64_BITS       64              /* 64 bit constant */
#define K_32_BITS       32              /* 32 bit constant */
#define K_16_BITS       16              /* 16 bit constant */
#include <limits.h>

/*
 * MIPS C on DEC OSF/1 sets __osf__ but not __OSF__; but gcc on "raw" OSF/1
 * sets __OSF__ but not __osf__. This little ditty provides a bridge.
 */
#if defined (__OSF__) && !defined (__osf__)
# define __osf__
#endif

/*
 * No standard way of getting maximum path length
 */
#define _DNS_MAXPATHLEN 1023


#endif  /* #ifndef _DNS_CONFIG_H */

