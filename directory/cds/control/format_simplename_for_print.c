/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: format_simplename_for_print.c,v $
 * Revision 1.1.6.2  1996/02/18  19:29:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:15  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:09:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:40  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:01:03  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:46:44  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:37:05  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:52  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:59:28  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:02:23  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:06:46  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:37:58  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:22:57  devrcs
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

void
format_simplename_for_print (
	char		*bufp,		/* ptr to output buffer */
	dns_opq_sname	*idp)		/* ptr to simple name */
{
    int valsize = DNS_STR_SNAME_MAX;

    int status =
	dnsCvtCDSSimpleToStr(
	    idp,			/* ptr to the simple name to cvt */
	    (unsigned char *)bufp,	/* where to put the string form */
	    &valsize);			/* ptr to in/out length */

    if (status != DNS_SUCCESS)
	{				/* use hex, etc, if need be */
	int len = LEN_SimpleName(idp);
	int most = (len > sizeof(SimpleName_u)) ? sizeof(SimpleName_u) : len;
	int used = ((most < sizeof(SimpleName_overhead)) ? 
	  sizeof(SimpleName_overhead) : most);

	format((unsigned char *)idp, used, bufp); /* try the fallback mechanism */
	}
}
