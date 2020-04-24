/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsenumgrp.c,v $
 * Revision 1.1.8.2  1996/02/18  19:33:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:00  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:23:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:08  root]
 * 
 * Revision 1.1.6.4  1994/06/30  19:17:02  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:00:49  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:41:38  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:48  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/29  15:53:36  peckham
 * 	Remove unnecessary conditionals.
 * 	Rename dns*() -> cds*().
 * 	[1994/04/29  14:18:26  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:06:48  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:29:02  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:56:23  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:11:10  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  22:20:45  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:31:45  weisman]
 * 
 * Revision 1.1  1992/01/19  15:18:04  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsenumgrp.c
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

static byte_t allNames[] = {"\11\1*"};
static byte_t classFilter[] = {"\10\11CDS_Group"};

/* ----------------------------------------------------------------------
 *  Return the next member for a given group
 * ----------------------------------------------------------------------
 */
int 
cdsEnumGrp (cds_full_name_t     *DirName_p,
            cds_handle_t        Handle_p,
            cds_simple_name_t   *NameFilter_p,
            cds_simple_name_t   **Name_pp,
            unsigned char       *NameBuf_p,
            int                 *NameBufLen_p,
            cdsFlagStat_t       *Flags_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    struct dns_ReadHandle *const rh_p = (struct dns_ReadHandle *)Handle_p;
    cds_full_name_t name;
    cds_simple_name_t filter;
    int status;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" >cdsEnumGrp()"));

    if (dns_validate_NameType(Flags_p->fsNameType) == cdsStrDCE) {
	if ((DirName_p = cds_fulltoopq(DirName_p, &name, Flags_p)) == NULL) {
	    status = CDS_ERROR;
	    goto leave_cdsEnumGrp;
	}
	if (NameFilter_p
	    && ((NameFilter_p = cds_simpletoopq(NameFilter_p, 1, &filter,
							Flags_p)) == NULL)) {
	    status = CDS_ERROR;
	    goto leave_cdsEnumGrp;
	}
    }

    if ((status = cds_handleSetNam(Handle_p, Name_pp, NameBuf_p, NameBufLen_p,
		Flags_p)) != CDS_PENDING)
	goto leave_cdsEnumGrp;

    if (!NameFilter_p)
	NameFilter_p = (cds_simple_name_t *)allNames;

    PRM_build(OP_EnumerateObjects, Flags_p);
      PRM_handle(Handle_p);
      PRM_IN_FullName(DirName_p);
      PRM_IN_SimpleName(NameFilter_p);
      PRM_IN_SimpleName(classFilter);
      PRM_IN_SimpleName(rh_p->dns_Context_p);
      PRM_INOUT_BOOLEAN(BL_false, 0);	/* don't return class */
      PRM_OUT_Set(rh_p->dns_ReadBuf_p, MAX_MSG_SIZE);
      PRM_RET_BOOLEAN(&rh_p->dns_done);
    PRM_send(status);

leave_cdsEnumGrp:

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" <cdsEnumGrp() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}
