/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsdelslink.c,v $
 * Revision 1.1.6.2  1996/02/18  19:33:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:54  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:23:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:03  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:16:58  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:00:36  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:31  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:44  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:53:26  peckham
 * 	Remove unnecessary conditionals.
 * 	Rename dns*() -> cds*().
 * 	[1994/04/29  14:18:05  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:06:41  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:28:34  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:56:02  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:10:37  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:18:00  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsdelslink.c
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
#include <dcecdssvc.h>
#include <uc_parse.h>
#include <uc_clerk.h>

/* ----------------------------------------------------------------------
 *  Delete a soft link
 * ----------------------------------------------------------------------
 */
int 
cdsDelSLink (cds_full_name_t     *Name_p,
             cdsFlagStat_t       *Flags_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    cds_full_name_t name;
    int status;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" >cdsDelSLink()"));

    if (dns_validate_NameType(Flags_p->fsNameType) == cdsStrDCE)
	if ((Name_p = cds_fulltoopq(Name_p, &name, Flags_p)) == NULL) {
	    status = CDS_ERROR;
	    goto leave_cdsDelSLink;
	}

    PRM_build(OP_DeleteLink, Flags_p);
      PRM_IN_FullName(Name_p);
    PRM_send(status);

leave_cdsDelSLink:

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" <cdsDelSLink() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}
