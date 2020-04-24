/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnshandleattrcon.c,v $
 * Revision 1.1.6.2  1996/02/18  19:34:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:14  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:24:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:20  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:14  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:01:31  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:50  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:00  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:54:03  peckham
 * 	Change dns* to cds*
 * 	[1994/04/29  14:22:05  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:07:04  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:30:13  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:57:17  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:12:29  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:19:34  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnshandleattrcon.c
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

#define	DNS_VALUE_P	dns_hcp.hcp_AttrCon.hcp_value_p
#define	DNS_CTS_PP	dns_hcp.hcp_AttrCon.hcp_CTS_pp

static int
cds_parseAttrCon (
    cdsFlagStat_t	*,
    cds_handle_t	 );

/* ----------------------------------------------------------------------
 *  Set up handle context to retrieve generic AttributeContents
 * ----------------------------------------------------------------------
 */
int 
cds_handleAttrCon (cds_handle_t        Handle_p,
                   cds_attrvalue_t     *Value,
                   cds_cts_t           **CTS_pp,
                   cdsFlagStat_t       *Flags_p)
{
    struct dns_ReadHandle *const rh_p = (struct dns_ReadHandle *)Handle_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " >cds_handleAttrCon"));

    if (rh_p->dns_Return != dns_retAttrCon) {
	if (rh_p->dns_Return != dns_retNone)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
                cds_svc_library,
                svc_c_debug2,
                " <cds_handleAttrCon: return(dns_status_clerk(Flags_p, CDS_CLERKBUG))"));

	    return dns_status_clerk(Flags_p, CDS_CLERKBUG);
	}
	

	rh_p->dns_Return = dns_retAttrCon;
	rh_p->dns_Context_p = (unsigned char *)NullTimestamp;
	rh_p->dns_parse = cds_parseAttrCon;
	rh_p->dns_SetIndex = 0;
	rh_p->dns_done = BL_false;
    }

    rh_p->DNS_VALUE_P = Value;
    rh_p->DNS_CTS_PP = CTS_pp;

    if (!rh_p->dns_Context_p)
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_library,
          svc_c_debug2,
          " <cds_handleAttrCon: cds_parseAttrCon(Flags_p, Handle_p)"));

	return cds_parseAttrCon(Flags_p, Handle_p);
    }
    else 
	if (rh_p->dns_done)
	{
	    DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_library,
              svc_c_debug2,
              " <cds_handleAttrCon: return dns_status_clerk(Flags_p, CDS_EMPTY)"));

	    return dns_status_clerk(Flags_p, CDS_EMPTY);
	}
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " <cds_handleAttrCon: return(CDS_PENDING)"));

    return(CDS_PENDING);
}

/* ----------------------------------------------------------------------
 *  Retrieve AttributeContents
 * ----------------------------------------------------------------------
 */
static int 
cds_parseAttrCon (cdsFlagStat_t       *Flags_p,
                  cds_handle_t        Handle_p)
{
    struct dns_ReadHandle *const rh_p = (struct dns_ReadHandle *)Handle_p;
    register byte_t *next_p = rh_p->dns_ReadBuf_p;
    register AtomicValue_t *av_p;
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
	av_p = (AtomicValue_t *)SKIP_MemberHeader(next_p);

	if (rh_p->DNS_VALUE_P) {
	    const int len = EXT16(av_p->av_length);
	    const unsigned char type = EXT8(av_p->av_valuetype);
	    void *const val = av_p->av_value;

	    dns_TLV2AttrVal(type, len, val, rh_p->DNS_VALUE_P);
	}

	if (rh_p->DNS_CTS_PP)
	    *rh_p->DNS_CTS_PP = (cds_cts_t *)((MemberHeader_t *)next_p)->mh_timestamp;

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




