/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsstatusclerk.c,v $
 * Revision 1.1.6.2  1996/02/18  19:34:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:50  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:26:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:51  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:38  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:03:09  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:42:17  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:28  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:55:04  peckham
 * 	Fix DCE_SVC_DEBUG() call to designate library, not server.
 * 	[1994/04/29  14:27:33  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:07:47  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:32:56  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:15:56  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:15:08  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:17:35  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsstatusclerk.c
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
  
/*
 *	The arch. spec. requires that for non-architected errors we
 *	return CDS_CLERKBUG for implementation specific errors.
 *	Note that the name CDS_CLERKBUG is an unfortunate choice since
 *	these are not necessarily bugs but errors!!!
 *
 */

#include <dce/dce.h>
#include <dce/cdsclerk.h>
#include <dcecdssvc.h>
#include <uc_clerk.h>     

/* ----------------------------------------------------------------------
 *  Set the appropriate status codes in the Flags
 * ----------------------------------------------------------------------
 */
int 
dns_status_clerk (cdsFlagStat_t       *Flags_p,
                  int                 status)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " >dns_status_clerk: status=%d",
	status));

    if (Flags_p) {
	Flags_p->fsLocStat = status;
	Flags_p->fsCDSStat = 0;	/* assume a benign status	*/
    }

    switch (status) {
    case CDS_SUCCESS:
    case CDS_EMPTY:
    case CDS_NOTFND:
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug2,
            " <dns_status_clerk: return(status=%d)",
	    status));
	return(status);
    case CDS_SUCCESS_STALE:
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug2,
            " <dns_status_clerk: return(CDS_SUCCESS)"));
        return(CDS_SUCCESS);
    case CDS_NOTFND_STALE:
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug2,
            " <dns_status_clerk: return(CDS_NOTFND)"));
	return(CDS_NOTFND);
    default:
	if (Flags_p && (0 < (status -= CDS_NAMESERVERERRORS)))
	    if ( ( status < CDS_ACCESSVIOLATION - CDS_NAMESERVERERRORS )
	      || ( status == CDS_CLERKBUG - CDS_NAMESERVERERRORS )
	      || ( status == CDS_NAMESERVERBUG - CDS_NAMESERVERERRORS ) )
		Flags_p->fsCDSStat = status;
	    else Flags_p->fsCDSStat = CDS_CLERKBUG - CDS_NAMESERVERERRORS;

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug2,
            " <dns_status_clerk: return(CDS_ERROR)"));

	return(CDS_ERROR);
    }
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " <dns_status_clerk: @ end"));
}
