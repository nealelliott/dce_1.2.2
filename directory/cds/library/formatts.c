/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: formatts.c,v $
 * Revision 1.1.10.2  1996/02/18  19:34:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:05  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:27:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:22  root]
 * 
 * Revision 1.1.7.7  1994/09/06  17:37:05  proulx
 * 	fix timestamps
 * 	[1994/09/06  17:29:57  proulx]
 * 
 * Revision 1.1.7.6  1994/08/03  19:03:10  mccann
 * 	includes file cleanup
 * 	[1994/07/19  18:10:07  mccann]
 * 
 * Revision 1.1.7.5  1994/06/30  19:17:49  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:04:07  mccann]
 * 
 * Revision 1.1.7.4  1994/06/09  18:42:25  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:37  devsrc]
 * 
 * Revision 1.1.7.3  1994/04/29  15:55:25  peckham
 * 	Remove utc_comptime() prototype.
 * 	[1994/04/29  14:33:44  peckham]
 * 
 * Revision 1.1.7.1  1994/03/12  22:08:32  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:34:23  peckham]
 * 
 * Revision 1.1.5.3  1992/12/30  14:16:58  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:16:36  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  18:53:05  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:16:02  weisman]
 * 
 * Revision 1.1.2.2  1992/04/29  14:37:03  m_sawyer
 * 	remove unnecessary prototype and #include<>
 * 	[1992/04/29  14:32:05  m_sawyer]
 * 
 * Revision 1.1  1992/01/19  15:18:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
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
#include <limits.h>
#include <dce/dce64.h>
#include <dce/dcecdsmsg.h>
#include <dce/cdsclerk.h>
#include <dce/dns_record.h>
#include <string.h>
#include <stdio.h>
#include <dce/utc.h>
#include <dce/sys_time.h>
#include <dce/dtss_ctrl.h>

int utc_settdf (
    utc_t      *,
    utc_t      *,
    long       );

static int dns_asclocaltime (
    char         *,
    int          ,
    utc_t        *);

static int dns_ascanytime (
    char         *,
    int          ,
    utc_t        *);

void 
format_nodeid_for_print (unsigned char  *np,
                         unsigned char  *bufp)
    {
    unsigned int a,b,c,d,e,f;
    static char node_fmt[] = "%02.2x-%02.2x-%02.2x-%02.2x-%02.2x-%02.2x";

    a = *(np+0);
    b = *(np+1);
    c = *(np+2);
    d = *(np+3);
    e = *(np+4);
    f = *(np+5);

    sprintf((char *)bufp, node_fmt, a,b,c,d,e,f);

    }

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

void 
format_ts (Timestamp_t    *tsp,
           unsigned char  *bufp)
    {
    unsigned char nodebuf[100];
    char timebuf[100];
    time_quad_t time;
    struct utc  exputc;			/* expiration time as utc */
    unsigned64 utc64, nulltime;

    if (EQ_Timestamp(tsp, NullTimestamp)) /* is it null */
	{				/* yes */
	strcpy((char *)bufp, "0");	/* use this short, special form */
	}
    else
	{		         	/* the normal case */
	/* In v3api a timeout and a timestamp.ts_time are not the same,
         * so cannot use old conversion
	 */

	COPY_Time((Timestamp_t *)tsp->ts_time, &time);

	if (!ZERO_time_quad(&time))
	  ADD_time_quad(&time, &base, &time);

	U64ZERO(nulltime);
#if (LONG_BIT == 64)
	utc64 = time;
#else
	U64SET64(utc64, time.tq_hi, time.tq_lo);
#endif
	utc_mkcomptime(&exputc, &utc64, &nulltime, 0);
	format_nodeid_for_print(tsp->ts_node, nodebuf);
	if (dns_asclocaltime(timebuf, sizeof(timebuf), &exputc) != 0)
#if defined(SNI_SVR4)
	    strcpy(timebuf, "<???\?-?\?-?\?-??:??:??.??\?>");
#else
	    strcpy(timebuf, "<????-??-??-??:??:??.???>");
#endif

	sprintf((char *)bufp, "%s/%s", timebuf, nodebuf);
	}
    return;				/* void routine has no value */
    }


/* Routine proved by dtss - KO - to get cts displayed so that no
 * bits of inaccuracy are lost.  Without this cannot use this display
 * to test equality
 */
static int 
dns_asclocaltime (char   *cp,
                  int    stringlen,
                  utc_t  *utcTime)
{
    utc_t utcTimeInt1, utcTimeInt2;
    long tdf;
    int status;

    if (NULL == utcTime)
    {
        if (utc_gettime(&utcTimeInt1))
            return(-1);
    }
    else
        utcTimeInt1 = *utcTime;

    if (utc_localzone ((char *)0, 0, &tdf, (int *)0, &utcTimeInt1))
        return(-1);

    if (utc_settdf(&utcTimeInt2, &utcTimeInt1, 0))
        return(-1);

    status = dns_ascanytime(cp,stringlen,&utcTimeInt2);
    return(status);

}
#define K_100NS_PER_SEC 10000000
#define MAXISTR 50
#define MAXTSTR 50
static int 
dns_ascanytime (char   *cp,
                int    stringlen,
                utc_t  *utcTime)
{
    struct tm tmTime;
    struct tm tmInacc;
    long tns;
    long ins;
    long tdf;
    char timestr[MAXTSTR];


    /*
     * print a local string
     */
    if (utc_anytime(&tmTime,
                    &tns,
                    &tmInacc,
                    &ins,
                    &tdf,
                    utcTime))

        return(-1);

    (void) sprintf(timestr,
                   "%04d-%02d-%02d-%02d:%02d:%02d.%09d",
                   tmTime.tm_year+1900,
                   ++tmTime.tm_mon,
                   tmTime.tm_mday,
                   tmTime.tm_hour,
                   tmTime.tm_min,
                   tmTime.tm_sec,
                   tns);

    if (strlen(timestr) > stringlen)
        return(-1);

    strcpy(cp, timestr);

    return (0);
}
