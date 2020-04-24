/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsrmmbr.c,v $
 * Revision 1.1.8.2  1996/02/18  19:34:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:42  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:25:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:41  root]
 * 
 * Revision 1.1.6.4  1994/06/30  19:17:32  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:02:49  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:42:12  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:22  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/29  15:54:49  peckham
 * 	Remove unnecessary conditionals.
 * 	Rename dns*() -> cds*().
 * 	[1994/04/29  14:26:13  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:07:34  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:32:19  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:58:56  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:14:33  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  22:22:54  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:32:23  weisman]
 * 
 * Revision 1.1  1992/01/19  15:18:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsrmmbr.c
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

/* OID for CDS_Members */
static byte_t attribute[] = {"1.3.22.1.3.10"};


/* ----------------------------------------------------------------------
 *  Remove a user or a group from an existing group
 * ----------------------------------------------------------------------
 */
int 
cdsRmMbr (cds_full_name_t     *GroupName_p,
          cds_full_name_t     *MemberName_p,
          cdsGrpMbrType_t     MemberType,
          cdsFlagStat_t       *Flags_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    cds_full_name_t name;
    cds_full_name_t member;
    cds_attr_name_t opq_attribute;
    int opq_attribute_len = sizeof(opq_attribute);
    int status;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" >cdsRmMbr()"));

    if (dns_validate_NameType(Flags_p->fsNameType) == cdsStrDCE) {
	if ((GroupName_p = cds_fulltoopq(GroupName_p, &name,
					 Flags_p)) == NULL) {
	    status = CDS_ERROR;
	    goto leave_cdsRmMbr;
	}
	if ((MemberName_p = cds_fulltoopq(MemberName_p, &member,
					  Flags_p)) == NULL) {
	    status = CDS_ERROR;
	    goto leave_cdsRmMbr;
	}
    }

    status = cdsCvtStrToObjID(attribute, NULL, NULL,
                              NULL, &opq_attribute,
                              &opq_attribute_len, NULL);
    if (status != CDS_SUCCESS)
	goto leave_cdsRmMbr;

    PRM_build(OP_ModifyAttribute, Flags_p);
      PRM_IN_FullName(GroupName_p);
      PRM_IN_EntryType(cdsObj);
      /* Update record */
      PRM_begin();
	PRM_byte(UD_absent);	/* absent */
	PRM_Timestamp(NullTimestamp);
	PRM_AttributeSpecifier(cdsSet, &opq_attribute);
	PRM_BOOLEAN(BL_true);
	/* AtomicValue record */
	PRM_word(1 + LEN_FullName(MemberName_p));
        PRM_byte(VT_GroupMember);
	  /* GroupMember record */
	  PRM_isaGroup(MemberType);
	  PRM_FullName(MemberName_p);
	PRM_end();
    PRM_send(status);

leave_cdsRmMbr:

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" <cdsRmMbr() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}
