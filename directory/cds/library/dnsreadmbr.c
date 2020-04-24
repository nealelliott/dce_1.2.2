/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsreadmbr.c,v $
 * Revision 1.1.8.2  1996/02/18  19:34:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:35  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:25:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:37  root]
 * 
 * Revision 1.1.6.4  1994/06/30  19:17:28  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:02:33  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:42:04  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:17  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/29  15:54:40  peckham
 * 	Remove unnecessary conditionals.
 * 	Rename dns*() -> cds*().
 * 	[1994/04/29  14:25:51  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:07:27  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:31:50  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:58:33  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:14:05  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  22:22:24  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:32:13  weisman]
 * 
 * Revision 1.1  1992/01/19  15:18:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsreadmbr.c
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

/* OID for group member */
static byte_t attribute[] = {"1.3.22.1.3.10"};


/* ----------------------------------------------------------------------
 *  Read member from group
 * ----------------------------------------------------------------------
 */
int 
cdsReadMbr (cds_full_name_t     *Name_p,
            cds_handle_t        Handle_p,
            cds_full_name_t     **MemberName_pp,
            cdsGrpMbrType_t     *MemberType_p,
            unsigned char       *MemberBuf_p,
            int                 *MemberBufLen_p,
            cdsFlagStat_t       *Flags_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    struct dns_ReadHandle *const rh_p = (struct dns_ReadHandle *)Handle_p;
    cds_full_name_t name;
    cds_attr_name_t opq_attribute;
    int opq_attribute_len = sizeof(opq_attribute);
    int status;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" >cdsReadMbr()"));

    if (dns_validate_NameType(Flags_p->fsNameType) == cdsStrDCE)
	if ((Name_p = cds_fulltoopq(Name_p, &name, Flags_p)) == NULL) {
	    status = CDS_ERROR;
	    goto leave_cdsReadMbr;
	}

    if ((status = cds_handleAttrMbr(Handle_p, MemberName_pp, MemberType_p,
		MemberBuf_p, MemberBufLen_p, Flags_p)) != CDS_PENDING)
        goto leave_cdsReadMbr;

    status = cdsCvtStrToObjID(attribute, NULL, NULL,
                              NULL, &opq_attribute,
                              &opq_attribute_len, NULL);
    if (status != CDS_SUCCESS)
        goto leave_cdsReadMbr;

    PRM_build(OP_ReadAttribute, Flags_p);
      PRM_handle(Handle_p);
      PRM_IN_FullName(Name_p);
      PRM_IN_EntryType(cdsObj);
      PRM_IN_SimpleName(&opq_attribute);
      PRM_IN_fsMaybeMore(Flags_p->fsMaybeMore);
      PRM_IN_Timestamp(rh_p->dns_Context_p);
      PRM_IN_FullName_Null(NullFullName);
      PRM_OUT_Set(rh_p->dns_ReadBuf_p, MAX_MSG_SIZE);
      PRM_RET_BOOLEAN(&rh_p->dns_done);
    PRM_send(status);

leave_cdsReadMbr:

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" <cdsReadMbr() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}

