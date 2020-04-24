/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsexpandstrfull.c,v $
 * Revision 1.1.6.2  1996/02/18  19:34:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:04  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:23:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:11  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:04  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:00:59  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:41  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:51  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:53:41  peckham
 * 	Remove unnecessary conditionals.
 * 	Eliminate use of dnstables.
 * 	[1994/04/29  14:18:39  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:06:51  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:29:20  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:56:37  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:11:23  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:18:06  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsexpandstrfull.c
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
#include <uc_clerk.h>

/* ----------------------------------------------------------------------
 *  Return string FullName with all expansions
 * ----------------------------------------------------------------------
 */
int 
cdsExpandStrFull (unsigned char  *InFname_p,
                  unsigned char  *OutFname_p,
                  int            *OutFnameLen_p)
{
/*
 * This will work as long as we don't have local name expansion.
 * Otherwise, parse directly as other code does so bogus local
 * names can be detected.
 */

    int 		 status, len;
    cds_full_name_t	 fname;

    len = sizeof(fname);
    status = cdsCvtFullToOpq(InFname_p, &fname, &len);
    if (status == CDS_SUCCESS)
    	status = cdsCvtFullToStr(&fname, OutFname_p, OutFnameLen_p);
    return(status);
}





