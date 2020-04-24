/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscvttimeouttoutc.c,v $
 * Revision 1.1.6.2  1996/02/18  19:33:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:45  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:23:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:50  root]
 * 
 * Revision 1.1.4.5  1994/09/06  17:37:04  proulx
 * 	fix timestamps
 * 	[1994/09/06  17:29:52  proulx]
 * 
 * Revision 1.1.4.4  1994/08/03  19:03:06  mccann
 * 	includes file cleanup
 * 	[1994/07/19  18:09:44  mccann]
 * 
 * Revision 1.1.4.3  1994/06/30  19:16:52  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:00:17  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:41:25  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:37  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:06:29  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:27:53  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:55:34  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:09:58  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:18:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnscvttimeouttoutc.c
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
 *
 */

#include <dce/dce.h>
#include <dce/cdsclerk.h>
#include <dce/utc.h>
#include <uc_clerk.h>
#include <dce/sys_time.h>

/*
 *  DTSS UTC base time is October 15, 1582 (Gregorian).
 *  DNS base time is November 17, 1858 (Smithsonian).
 */
#define BASE_TIME_DIFF_LO   0xC7960000
#define BASE_TIME_DIFF_HI   0x0135886A
#define BASE_TIME_DIFF      0x0135886AC7960000

/* Difference between smithsonian zero and gregorian zero */
#if LONG_BIT == 64
static time_quad_t base = BASE_TIME_DIFF;
#else
static time_quad_t base = {BASE_TIME_DIFF_LO, BASE_TIME_DIFF_HI};
#endif



/* ----------------------------------------------------------------------
 *  Convert DNS Timeout to DTSS utc/utc
 * ----------------------------------------------------------------------
 */
void 
cdsCvtTimeoutToUTC (unsigned char  *timeout,
                    struct utc     *expire,
                    struct utc     *extend)
{
  Timeout_t *time_p = (Timeout_t *)timeout;
  time_quad_t time;
#if LONG_BIT == 64
  static time_quad_t nulltime=0;
#else
  static time_quad_t nulltime={0,0};
#endif


  /* Now adjust from timeout smithsonian to utc gregorian */
  COPY_Time(time_p->to_expiration, &time);

  if (!ZERO_time_quad(&time))
    ADD_time_quad(&time, &base, &time);

  utc_mkcomptime(expire, &time, &nulltime, 0);

  COPY_Time(time_p->to_extension, &time);

  utc_mkcomptime(extend, &time, &nulltime, 0);
}
