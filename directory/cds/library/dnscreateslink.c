/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscreateslink.c,v $
 * Revision 1.1.6.2  1996/02/18  19:33:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:39  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:22:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:44  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:16:47  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:59:56  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:21  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:32  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:52:57  peckham
 * 	Remove unnecessary conditionals.
 * 	Rename dns*() -> cds*().
 * 	[1994/04/29  14:17:20  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:06:20  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:27:24  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:55:18  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:09:25  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:17:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnscreateslink.c
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
#include <dcecdssvc.h>
#include <dce/cdsclerk.h>
#include <uc_parse.h>
#include <uc_clerk.h>

/* ----------------------------------------------------------------------
 *  Create a soft link to an entry in the namespace
 * ----------------------------------------------------------------------
 */
int 
cdsCreateSLink (cds_full_name_t     *Name_p,
                cds_full_name_t     *TargetName_p,
                cds_timeout_t       *Timeout_p,
                cds_cts_t           *CTS_p,
                cdsFlagStat_t       *Flags_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    cds_full_name_t name;
    cds_full_name_t target;
    int status;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" >cdsCreateSLink()"));

    if (dns_validate_NameType(Flags_p->fsNameType) == cdsStrDCE) {
	Name_p = cds_fulltoopq(Name_p, &name, Flags_p);
	TargetName_p = cds_fulltoopq(TargetName_p, &target, Flags_p);

	if (!Name_p || !TargetName_p) {
	    status = CDS_ERROR;
	    goto leave_cdsCreateSLink;
	}
    }

    PRM_build(OP_CreateLink, Flags_p);
    PRM_IN_FullName(Name_p);
    PRM_IN_FullName(TargetName_p);
    PRM_IN_OPT_Timeout(Timeout_p);
    PRM_RET_Timestamp(CTS_p);
    PRM_send(status);

leave_cdsCreateSLink:

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" <cdsCreateSLink() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}
