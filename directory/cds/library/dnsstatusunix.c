/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsstatusunix.c,v $
 * Revision 1.1.6.2  1996/02/18  19:34:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:51  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:26:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:54  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:39  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:03:12  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:42:19  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:29  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:55:06  peckham
 * 	Change dns* to cds*
 * 	[1994/04/29  14:27:56  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:07:51  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:33:01  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:16:01  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:15:12  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:18:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsstatusunix.c
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
  
#include <pthread.h>
#include <dce/dce.h>
#include <dce/cdsclerk.h>
#include <dcecdssvc.h>
#include <errno.h>
#include <uc_clerk.h>     

/* ----------------------------------------------------------------------
 *  Set local status code for UNIX error
 * ----------------------------------------------------------------------
 */
int 
dns_status_unix (cdsFlagStat_t       *Flags_p,
                 int                 status)
{
    if (Flags_p) {
	Flags_p->fsCDSStat = CDS_NOCOMMUNICATION - CDS_NAMESERVERERRORS;

	if (status < 0)
	    Flags_p->fsLocStat = CDS_UNIXERRORS + errno;
	else
	    Flags_p->fsLocStat = CDS_READ_SHORT;
    }
    return(CDS_ERROR);
}
