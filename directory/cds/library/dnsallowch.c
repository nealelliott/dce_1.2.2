/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsallowch.c,v $
 * Revision 1.1.6.2  1996/02/18  19:32:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:20  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:21:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:08  root]
 * 
 * Revision 1.1.4.4  1994/06/23  18:30:47  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:08:51  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:40:57  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:17  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:52:13  peckham
 * 	Remove VMS conditionals.
 * 	Rename dns*() -> cds*().
 * 	[1994/04/29  14:00:13  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:05:57  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:25:45  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:54:07  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:07:35  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:19:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsallowch.c
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
 *
 * Functional Description:
 *
 *	API routine to allow clearinghouse objects to be added to a directory.
 *      Not allowed in CDS - obsolete function.
 */
  
#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <cdsclerk.h>
#include <uc_parse.h>
#include <uc_clerk.h>     

/* ----------------------------------------------------------------------
 *  Permit a directory to store a clearinghouse object
 * ----------------------------------------------------------------------
 */
int
cdsAllowCH (cds_full_name_t     *DirName_p,
            cdsFlagStat_t       *Flags_p)
{

  /* Clearinghouses must be in the root - function not needed */
  Flags_p->fsLocStat = CDS_NOTSUPPORTED;
  Flags_p->fsCDSStat = 0;
  return(CDS_ERROR);
}
