/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: format_fullname_for_print.c,v $
 * Revision 1.1.6.2  1996/02/18  19:29:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:09  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:09:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:37  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:01:00  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:48:53  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:37:02  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:50  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:59:16  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:02:05  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:06:12  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:37:20  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:21:34  devrcs
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

struct envblk * get_envblk_ptr();	/* to find environment block */

static int
format_fullname_w_nickname(
	char		*bufp,		/* ptr to output buffer */
	unsigned char	*idp)		/* ptr to full name to be formatted */
{
    int valsize = DNS_STR_FNAME_MAX;

    int status =
	dnsCvtCDSFullToStr(
	    (dns_opq_fname *)idp,	/* ptr to the full name to cvt */
	    (unsigned char *)bufp,	/* where to put the string form */
	    &valsize);

    return(status);			/* tell caller result of cvt */
}

void
format_fullname_for_print (
	FullName_t	*idp,		/* ptr to full name of prin/group/etc */
	char		*bufp)		/* ptr to output buffer */
{
    int status = format_fullname_w_nickname(bufp, (unsigned char *)idp);

    if (status != DNS_SUCCESS)
	{				/* use hex, etc, if need be */
	int len = LEN_FullName(idp);	/* measure the name */
	int most = (len > sizeof(FullName_u) ? sizeof(FullName_u) : len);
	int used = (most < sizeof(FullName_overhead)) ?  
		sizeof(FullName_overhead) : most;
	format((unsigned char *)idp, used, bufp);
	}
}
