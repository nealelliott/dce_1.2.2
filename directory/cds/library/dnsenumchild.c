/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsenumchild.c,v $
 * Revision 1.1.6.2  1996/02/18  19:33:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:59  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:23:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:07  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:01  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:00:46  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:35  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:47  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:53:34  peckham
 * 	Remove unnecessary conditionals.
 * 	Rename dns*() -> cds*().
 * 	[1994/04/29  14:18:22  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:06:46  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:28:57  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:56:19  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:11:06  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:18:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsenumchild.c
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

/* ----------------------------------------------------------------------
 *  Return the next child directory for a given directory
 * ----------------------------------------------------------------------
 */
int 
cdsEnumChild (cds_full_name_t     *DirName_p,
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
	" >cdsEnumChild()"));

    if (dns_validate_NameType(Flags_p->fsNameType) == cdsStrDCE) {
	if ((DirName_p = cds_fulltoopq(DirName_p, &name, Flags_p)) == NULL) {
	    status = CDS_ERROR;
	    goto leave_cdsEnumChild;
	}
	if (NameFilter_p
	    && ((NameFilter_p = cds_simpletoopq(NameFilter_p, 1, &filter,
							Flags_p)) == NULL)) {
	    status = CDS_ERROR;
	    goto leave_cdsEnumChild;
	}
    }

    if ((status = cds_handleSetNam(Handle_p, Name_pp, NameBuf_p, NameBufLen_p,
		Flags_p)) != CDS_PENDING)
	goto leave_cdsEnumChild;

    if (!NameFilter_p)
	NameFilter_p = (cds_simple_name_t *)allNames;

    PRM_build(OP_EnumerateChildren, Flags_p);
      PRM_handle(Handle_p);
      PRM_IN_FullName(DirName_p);
      PRM_IN_SimpleName(NameFilter_p);
      PRM_IN_SimpleName(rh_p->dns_Context_p);
      PRM_OUT_Set(rh_p->dns_ReadBuf_p, MAX_MSG_SIZE);
      PRM_RET_BOOLEAN(&rh_p->dns_done);
    PRM_send(status);

leave_cdsEnumChild:

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" <cdsEnumChild() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}
