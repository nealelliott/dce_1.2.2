/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsgetopqsimplelen.c,v $
 * Revision 1.1.6.2  1996/02/18  19:34:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:13  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:24:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:19  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:13  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:01:28  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:49  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:00  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:54:00  peckham
 * 	Change dns*() to cds*().
 * 	[1994/04/29  14:21:26  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:07:03  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:30:07  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:57:13  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:12:25  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:17:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsgetopqsimplelen.c
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
 *  Get the length of an opaque SimpleName
 * ----------------------------------------------------------------------
 */
int
cdsGetOpqSimpleLen (cds_simple_name_t *Sname_p)
{
    return(LEN_SimpleName(Sname_p));
}



