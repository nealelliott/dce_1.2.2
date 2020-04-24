/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: qarith.h,v $
 * Revision 1.1.6.2  1996/02/18  23:34:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:14  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:17:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:35  root]
 * 
 * Revision 1.1.4.4  1994/08/03  19:02:34  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:59:29  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:39:25  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:05  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/07  10:22:24  kevins
 * 	CR 10304 Export some header files for cross component dependency issues
 * 	[1994/04/07  10:18:10  kevins]
 * 
 * Revision 1.1.4.1  1994/03/12  22:03:59  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:18:30  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:33:24  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:53:00  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:15:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _QARITH_H
#define _QARITH_H
/*
 * Module qarith.h
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
#include <dce/sys_time.h>

#if (LONG_BIT < 64)
/*
 * qarith_lib.c
 */
void
dns_qadd ( 
    time_quad_t	       *,
    time_quad_t	       *,
    time_quad_t	       *);

void
dns_qsub (
    time_quad_t	       *,
    time_quad_t        *,
    time_quad_t	       *);

# if _DNS_OS_ == _DNS__VMS

#  define dns_emul(long1_p, long2_p, quad_p) \
    {long ql_tmp = 0; lib$emul((long1_p), (long2_p), &ql_tmp, (quad_p)); }

# else

void
dns_emul (
    int			*,
    int			*,
    time_quad_t		*);

# endif /* VMS */

void
dns_ediv (
    int			*,
    time_quad_t         *,
    int			*,
    int			*);

void
dns_qmul (
    time_quad_t         *,
    time_quad_t         *,
    time_quad_t         *);

#else

# define dns_qadd(add1,add2,sum) { *(sum) = *(add1) + *(add2); }

# define dns_qsub(min,sub,result) { *(result) = *(min) - *(sub); }

# define dns_emul(mult1,mult2,prod) { *(prod) = (long)(*(mult1)) * (long)(*(mult2)); }

# define dns_ediv(dvr,quo,rem,dvd) \
    { *(dvd) = *(quo) / *(dvr); *(rem) = *(quo) % *(dvr); }

# define dns_qmul(mult1,mult2,prod) { *(prod) = *(mult1) * *(mult2); }

#endif

#endif  /* #ifndef _QARITH_H */


