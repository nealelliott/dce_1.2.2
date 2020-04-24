/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsaddmbr.c,v $
 * Revision 1.1.8.2  1996/02/18  19:32:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:17  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:21:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:03  root]
 * 
 * Revision 1.1.6.4  1994/06/30  19:16:32  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:58:56  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:40:54  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:15  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/29  15:52:08  peckham
 * 	Remove VMS conditionals.
 * 	Rename dns*() -> cds*().
 * 	[1994/04/29  14:00:03  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:05:54  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:25:31  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:53:58  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:07:26  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  22:16:24  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:30:28  weisman]
 * 
 * Revision 1.1  1992/01/19  15:17:43  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsaddmbr.c
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
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <dce/cdsclerk.h>
#include <uc_parse.h>
#include <uc_clerk.h>     

/* OID for CDS_Members */
static byte_t attribute[] = {"1.3.22.1.3.10"};


/* ----------------------------------------------------------------------
 *  Add a user or a group to an existing group
 * ----------------------------------------------------------------------
 */
int
cdsAddMbr (cds_full_name_t     *GroupName_p,
           cds_full_name_t     *MemberName_p,
           cdsGrpMbrType_t     MemberType,
           cdsFlagStat_t       *Flags_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    cds_full_name_t	groupname;
    cds_full_name_t	membername;
    cds_attr_name_t     opq_attribute;
    int opq_attribute_len = sizeof(opq_attribute);
    int status;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" >cdsAddMbr()"));

    if (dns_validate_NameType(Flags_p->fsNameType) == cdsStrDCE) {
	GroupName_p = cds_fulltoopq(GroupName_p, &groupname, Flags_p);
	MemberName_p = cds_fulltoopq(MemberName_p, &membername, Flags_p);

	if (!GroupName_p || !MemberName_p) {
	    status = CDS_ERROR;
	    goto leave_cdsAddMbr;
	}
    }

    status = cdsCvtStrToObjID(attribute, NULL, NULL,
			      NULL, &opq_attribute,
			      &opq_attribute_len, NULL);

    if (status != CDS_SUCCESS)
	goto leave_cdsAddMbr;

    PRM_build(OP_ModifyAttribute, Flags_p);
      PRM_IN_FullName(GroupName_p);
      PRM_IN_EntryType(cdsObj);
      /* Update record */
      PRM_begin();
	PRM_byte(UD_present);	/* present */
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

leave_cdsAddMbr:

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" <cdsAddMbr() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}
