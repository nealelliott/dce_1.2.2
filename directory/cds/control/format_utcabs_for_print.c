/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: format_utcabs_for_print.c,v $
 * Revision 1.1.10.2  1996/02/18  19:29:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:19  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:10:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:41  root]
 * 
 * Revision 1.1.7.3  1994/08/03  19:01:05  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:46:39  mccann]
 * 
 * Revision 1.1.7.2  1994/06/09  18:37:07  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:55  devsrc]
 * 
 * Revision 1.1.7.1  1994/03/12  21:59:37  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:02:35  peckham]
 * 
 * Revision 1.1.5.2  1993/08/12  18:40:15  jd
 * 	Removed the timezone offset from end of timestamp output. Also some cleanup.
 * 	Courtesy Alan Peckhem.
 * 	[1993/08/12  18:35:43  jd]
 * 
 * Revision 1.1.3.4  1992/12/30  13:07:00  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:38:26  zeliff]
 * 
 * Revision 1.1.3.3  1992/12/10  15:38:53  mfox
 * 	Replace usage of obsolete utc functions
 * 	[1992/12/10  15:36:08  mfox]
 * 
 * Revision 1.1.3.2  1992/09/29  19:10:53  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:11:15  weisman]
 * 
 * Revision 1.1  1992/01/19  15:22:37  devrcs
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
 */

#include <cdscp.h>
#include <dce/utc.h>

void
format_utcabs_for_print(
	char		*bufp,		/* ptr to output buffer */
	utc_t		*tp)		/* ptr to the utc absolute time */

    /* caller supplies output buffer, and it better be big enough. */
    /* 28 bytes needed usually. */

{
    char timebuf[99];
    size_t size = sizeof(timebuf);	/* later changed to amount used */

    int status =
	utc_asclocaltime(		/* convert 128 bit utc to string */
	    timebuf,			/* ptr to buffer for results */
	    size,			/* in, size available. */
	    tp);			/* ptr to 128bit utc to be formatted */

    if (status == 0) {			/* goodie, it worked */
	char *p = strchr(timebuf, 'I'); /* If there is an inaccuracy portion */

	if (p)
	    *p = '\0';			/* then eliminate it. */
	p = strrchr(timebuf, '+');	/* If positive timezone is displayed */
	if (p) {
	    *p = '\0';			/* clip it off */
	} else {
	    p = strrchr(timebuf, '-');	/* If timezone is displayed */

	    if (p)
		*p = '\0';		/* clip it off */
	}
	strcpy((char *)bufp, timebuf);	/* move string to user buffer */
    } else {				/* something broke. compensate. */
#ifdef SNI_SVR4
	cds_strcpy(timebuf, CDSCP_NOABSTIME, "<???\?-?\?-?\?-??:??:??.??\?>");
#else
	cds_strcpy(timebuf, CDSCP_NOABSTIME, "<????-??-??-??:??:??.???>");
#endif
	}

    strcpy((char *)bufp, timebuf);	/* move string to user buffer */
    return;				/* void routine has no value */
}
