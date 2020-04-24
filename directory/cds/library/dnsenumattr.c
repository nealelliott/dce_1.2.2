/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsenumattr.c,v $
 * Revision 1.1.6.2  1996/02/18  19:33:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:58  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:23:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:07  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:00  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:00:42  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:34  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:47  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:53:32  peckham
 * 	Remove unnecessary conditionals.
 * 	Rename dns*() -> cds*().
 * 	[1994/04/29  14:18:18  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:06:45  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:28:51  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:56:15  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:10:51  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:18:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsenumattr.c
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
 */

#include <dce/dce.h>
#include <dce/cdsclerk.h>
#include <dcecdssvc.h>
#include <uc_parse.h>
#include <uc_clerk.h>



/* ----------------------------------------------------------------------
 *  Return the next attribute for a given object
 * ----------------------------------------------------------------------
 */
int 
cdsEnumAttr (cds_full_name_t     *Name_p,
             cds_handle_t        Handle_p,
             cdsEntries_t        EntryType,
             cdsAttrType_t       *AttrType_p,
             cds_attr_name_t     **AttrName_pp,
             unsigned char       *AttrNameBuf_p,
             int                 *AttrNameLen_p,
             cdsFlagStat_t       *Flags_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    struct dns_ReadHandle *const rh_p = (struct dns_ReadHandle *)Handle_p;
    cds_full_name_t name;
    int status;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" >cdsEnumAttr()"));

    if (dns_validate_NameType(Flags_p->fsNameType) == cdsStrDCE)
	if ((Name_p = cds_fulltoopq(Name_p, &name, Flags_p)) == NULL) {
	    status = CDS_ERROR;
	    goto leave_cdsEnumAttr;
	}

    if ((status = cds_handleAttrSpc(Handle_p, AttrType_p, AttrName_pp,
		AttrNameBuf_p, AttrNameLen_p, Flags_p)) != CDS_PENDING)
	goto leave_cdsEnumAttr;

    PRM_build(OP_EnumerateAttributes, Flags_p);
      PRM_handle(Handle_p);
      PRM_IN_FullName(Name_p);
      PRM_IN_EntryType(EntryType);
      PRM_IN_AttributeName(rh_p->dns_Context_p);
      PRM_OUT_Set(rh_p->dns_ReadBuf_p, MAX_MSG_SIZE);
      PRM_RET_BOOLEAN(&rh_p->dns_done);
    PRM_send(status);

leave_cdsEnumAttr:

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" <cdsEnumAttr() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}
