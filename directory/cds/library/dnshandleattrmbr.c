/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnshandleattrmbr.c,v $
 * Revision 1.1.6.2  1996/02/18  19:34:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:16  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:24:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:21  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:15  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:01:36  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:51  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:01  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:54:06  peckham
 * 	Change dns* to cds*
 * 	[1994/04/29  14:22:29  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:07:05  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:30:19  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:57:22  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:12:33  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:19:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnshandleattrmbr.c
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
#include <dce/assert.h>
#include <dce/cdsclerk.h>
#include <dcecdssvc.h>
#include <uc_parse.h>
#include <uc_clerk.h>     


#define	DNS_MEMBER_PP	dns_hcp.hcp_AttrMbr.hcp_member_pp
#define	DNS_ISAGROUP_P	dns_hcp.hcp_AttrMbr.hcp_isaGroup_p
#define	DNS_MEMBER_P	dns_hcp.hcp_AttrMbr.hcp_member_p
#define	DNS_MEMBER_LEN_P dns_hcp.hcp_AttrMbr.hcp_member_len_p

static int
cds_parseAttrMbr (
    cdsFlagStat_t	*Flags_p,
    cds_handle_t	Handle_p);

/* ----------------------------------------------------------------------
 *  Set up handle context to retrieve Group Members
 * ----------------------------------------------------------------------
 */
int 
cds_handleAttrMbr (cds_handle_t        Handle_p,
                   cds_full_name_t     **MemberName_pp,
                   cdsGrpMbrType_t     *MemberType_p,
                   unsigned char       *MemberBuf_p,
                   int                 *MemberBufLen_p,
                   cdsFlagStat_t       *Flags_p)
{
    struct dns_ReadHandle *const rh_p = (struct dns_ReadHandle *)Handle_p;

    if (rh_p->dns_Return != dns_retAttrMbr) {
	if (rh_p->dns_Return != dns_retNone)
	    return dns_status_clerk(Flags_p, CDS_CLERKBUG);

	rh_p->dns_Return = dns_retAttrMbr;
	rh_p->dns_Context_p = (unsigned char *)NullTimestamp;
	rh_p->dns_parse = cds_parseAttrMbr;
	rh_p->dns_SetIndex = 0;
	rh_p->dns_done = BL_false;
    }

    rh_p->DNS_MEMBER_PP = MemberName_pp;
    rh_p->DNS_ISAGROUP_P = MemberType_p;
    rh_p->DNS_MEMBER_P = MemberBuf_p;
    rh_p->DNS_MEMBER_LEN_P = MemberBufLen_p;

    if (!rh_p->dns_Context_p)
	return cds_parseAttrMbr(Flags_p, Handle_p);
    else if (rh_p->dns_done)
	return dns_status_clerk(Flags_p, CDS_EMPTY);

    return(CDS_PENDING);
}

/* ----------------------------------------------------------------------
 *  Retrieve Group Member
 * ----------------------------------------------------------------------
 */
static int
cds_parseAttrMbr (cdsFlagStat_t       *Flags_p,
                  cds_handle_t        Handle_p)
{
    struct dns_ReadHandle *const rh_p = (struct dns_ReadHandle *)Handle_p;
    byte_t *next_p = rh_p->dns_ReadBuf_p;
    GroupMember_t *gm_p;
    register int num_offsets = 0;

    rh_p->dns_Context_p = NULL;
    Flags_p->fsOutLink = rh_p->dns_outLinked;
    Flags_p->fsFromCache = rh_p->dns_fromCache;

    switch (*next_p++) {	/* check AttributeType */
    case AT_set:
	/*
	 * Insure that attributes are present
	 *
	 * Pick up number of entries in the set
	 * and make sure non-empty
	 */
	if (FLAG_CLEAR(((Set_t *)next_p)->st_flag, ST_present))
	    goto set_empty;
	if ((num_offsets = NUM_Set(next_p)) == 0)
	    goto set_empty;

	/*
	 * index into the set to get the SetMember
	 */
	next_p = INDEX_Set(next_p, rh_p->dns_SetIndex);

	/* fall through with next_p = SetMember */
    case AT_single:
	/*
	 * go to the data portion of the MemberHeader/AtomicData
	 */
	gm_p = (GroupMember_t *)((MemberValue_t *)next_p)->mv_value;

	if (rh_p->DNS_MEMBER_PP)
	    *rh_p->DNS_MEMBER_PP = (cds_full_name_t *)gm_p->gm_member;

	if (rh_p->DNS_ISAGROUP_P)
	    *rh_p->DNS_ISAGROUP_P =
		EXT8(gm_p->gm_isaGroup) ? cdsGroup : cdsPrincipal;

	if (rh_p->DNS_MEMBER_P || rh_p->DNS_MEMBER_LEN_P) {
	    int status;
	    unsigned char STR_temp[CDS_STR_FNAME_MAX];
	    cds_full_name_t resolved;
	    int resolvedLen;

	    switch (Flags_p->fsNameType) {
	    case 0:
	    case cdsStrDCE:
		if ((status = cdsCvtFullToStr(
			(cds_full_name_t *)gm_p->gm_member, rh_p->DNS_MEMBER_P,
			rh_p->DNS_MEMBER_LEN_P)) != CDS_SUCCESS)
		    return dns_status_clerk(Flags_p, status);

		break;
	    case cdsStrXDS:
		resolvedLen = sizeof(STR_temp);
		if ((status = cdsCvtFullToStr(
			(cds_full_name_t *)gm_p->gm_member, STR_temp,
			&resolvedLen)) != CDS_SUCCESS)
		    return dns_status_clerk(Flags_p, status);

		resolvedLen = sizeof(resolved);
		if ((status = dns_CvtCDSFullToGDS(STR_temp, 1, 0,
			&resolved, &resolvedLen)) != CDS_SUCCESS)
		    return dns_status_clerk(Flags_p, status);

		if ((status = cdsCvtFullToStr(&resolved, rh_p->DNS_MEMBER_P,
			rh_p->DNS_MEMBER_LEN_P)) != CDS_SUCCESS)
		    return dns_status_clerk(Flags_p, status);

		break;
	    case cdsStrDNA:
	    default:
		/*
		 * I don't know what to do here 
		 */
		dce_assert(cds__svc_handle, 1 == 2);
	    case cdsOpqDNA:
		resolvedLen = LEN_FullName(gm_p->gm_member);
		if (resolvedLen <= *rh_p->DNS_MEMBER_LEN_P){
		    *rh_p->DNS_MEMBER_LEN_P = resolvedLen;
		    COPY_FullName(gm_p->gm_member, rh_p->DNS_MEMBER_P);
		} else {
		    *rh_p->DNS_MEMBER_LEN_P = resolvedLen;
		    dns_status_clerk(Flags_p, CDS_NOROOM);
		}
		break;
	    }

	    if (status != CDS_SUCCESS)
		return dns_status_clerk(Flags_p, status);
	}

	/*
	 * The MemberHeader Timestamp may be the next contextVariable
	 */
	next_p = ((MemberHeader_t *)next_p)->mh_timestamp;
	break;

    case AT_null:

set_empty:
	if (rh_p->dns_done)
	    return dns_status_clerk(Flags_p, CDS_EMPTY);

    default:
	return dns_status_clerk(Flags_p, CDS_CLERKBUG);
    }

    if (num_offsets <= ++rh_p->dns_SetIndex) {
	rh_p->dns_SetIndex = 0;
	rh_p->dns_Context_p = next_p;
    }

    return dns_status_clerk(Flags_p, rh_p->dns_status);
}




