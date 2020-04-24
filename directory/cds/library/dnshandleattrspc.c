/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnshandleattrspc.c,v $
 * Revision 1.1.8.2  1996/02/18  19:34:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:17  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:24:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:22  root]
 * 
 * Revision 1.1.6.4  1994/06/30  19:17:16  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:01:39  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:41:52  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:02  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/29  15:54:08  peckham
 * 	Change dns* to cds*
 * 	[1994/04/29  14:22:59  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:07:06  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:30:25  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:57:27  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:12:37  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/08  14:01:39  mfox
 * 	Fixed OT 945: Calling cdsEnumAttr() with fsNameType == cdsStrXDS should return
 * 	attribute types as dotted decimal object identifiers.
 * 	[1992/04/08  14:01:13  mfox]
 * 
 * Revision 1.1  1992/01/19  15:19:35  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnshandleattrspc.c
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
#include <string.h>
#include <uc_parse.h>
#include <uc_clerk.h>

#define	CDS_OPQNAME_PP	dns_hcp.hcp_AttrSpc.hcp_OpqName_pp
#define	DNS_ATTRTYPE_P	dns_hcp.hcp_AttrSpc.hcp_AttrType_p
#define	DNS_STRNAME_P	dns_hcp.hcp_AttrSpc.hcp_StrName_p
#define	DNS_STRNAME_LEN_P dns_hcp.hcp_AttrSpc.hcp_StrName_len_p


static int
cds_parseAttrSpc (
    cdsFlagStat_t	*,
    cds_handle_t	);

/* ----------------------------------------------------------------------
 *  Set up handle context to retrieve Set of AttributeSpecifiers
 * ----------------------------------------------------------------------
 */
int 
cds_handleAttrSpc (cds_handle_t        Handle_p,
                   cdsAttrType_t       *AttrType_p,
                   cds_attr_name_t     **AttrName_pp,
                   unsigned char       *AttrNameBuf_p,
                   int                 *AttrNameLen_p,
                   cdsFlagStat_t       *Flags_p)
{
    struct dns_ReadHandle *const rh_p = (struct dns_ReadHandle *)Handle_p;

    if (rh_p->dns_Return != dns_retAttrSpc) {
	if (rh_p->dns_Return != dns_retNone)
	    return dns_status_clerk(Flags_p, CDS_CLERKBUG);

	rh_p->dns_Return = dns_retAttrSpc;
	rh_p->dns_Context_p = (unsigned char *)NullBinarySimpleName;
	rh_p->dns_parse = cds_parseAttrSpc;
	rh_p->dns_SetIndex = 0;
	rh_p->dns_done = BL_false;
    }

    rh_p->CDS_OPQNAME_PP = AttrName_pp;
    rh_p->DNS_ATTRTYPE_P = AttrType_p;
    rh_p->DNS_STRNAME_P = AttrNameBuf_p;
    rh_p->DNS_STRNAME_LEN_P = AttrNameLen_p;

    if (!rh_p->dns_Context_p)
	return cds_parseAttrSpc(Flags_p, Handle_p);
    else if (rh_p->dns_done)
	return dns_status_clerk(Flags_p, CDS_EMPTY);

    return(CDS_PENDING);
}

/* ----------------------------------------------------------------------
 *  Retrieve AttributeSpecifier from Set
 * ----------------------------------------------------------------------
 */
static int 
cds_parseAttrSpc (cdsFlagStat_t       *Flags_p,
                  cds_handle_t        Handle_p)
{
    struct dns_ReadHandle *const rh_p = (struct dns_ReadHandle *)Handle_p;
    register byte_t *next_p = rh_p->dns_ReadBuf_p;
    register AttributeSpecifier_t *as_p;
    register int num_offsets = 0;

    rh_p->dns_Context_p = NULL;
    Flags_p->fsOutLink = rh_p->dns_outLinked;
    Flags_p->fsFromCache = rh_p->dns_fromCache;

    /*
     * Pick up number of entries in the set
     */
    num_offsets = NUM_Set(next_p);

    /*
     * If not present or no entries, it must be empty
     */
    if (FLAG_CLEAR(((Set_t *)next_p)->st_flag, ST_present) ||
	    (num_offsets <= rh_p->dns_SetIndex))
	if (rh_p->dns_done)
	    return dns_status_clerk(Flags_p, CDS_EMPTY);
	else
	    return dns_status_clerk(Flags_p, CDS_CLERKBUG);

    /*
     * index into the set to get the SetMember
     */
    next_p = INDEX_Set(next_p, rh_p->dns_SetIndex);

    /*
     * go to the data portion of the MemberHeader/AtomicData
     */
    as_p = (AttributeSpecifier_t *)((MemberValue_t *)next_p)->mv_value;

    if (rh_p->DNS_ATTRTYPE_P)
	switch (EXT8(as_p->as_type)) {
	case AT_null:
	    *rh_p->DNS_ATTRTYPE_P = cdsEmptySet;
	    break;
	case AT_single:
	    *rh_p->DNS_ATTRTYPE_P = cdsSingle;
	    break;
	case AT_set:
	    *rh_p->DNS_ATTRTYPE_P = cdsSet;
	    break;
	}

    if (rh_p->CDS_OPQNAME_PP)
	*rh_p->CDS_OPQNAME_PP = (cds_attr_name_t *)as_p->as_name;

    if (rh_p->DNS_STRNAME_P || rh_p->DNS_STRNAME_LEN_P) {
      /*
       * Turn the opaque attr type into a string name. If that fails,
       * try to turn it into a dotted decimal object id. However,
       * if the name type is XDS, skip the first step.
       */
      int save_length = *rh_p->DNS_STRNAME_LEN_P;
      int status, no_str;

      if (Flags_p->fsNameType == cdsStrXDS)
	no_str = 1;
      else {
	no_str = 0;
        status = cdsCvtObjIDToStr((cds_attr_name_t *)as_p->as_name,
				  (byte_t *)NULL,
				  rh_p->DNS_STRNAME_P, 
				  rh_p->DNS_STRNAME_LEN_P,
				  (unsigned char *) NULL,
				  (int *) NULL);
      }
      if (no_str ||
         (status == CDS_SUCCESS && strlen((char *)rh_p->DNS_STRNAME_P) == 0)) {
	*rh_p->DNS_STRNAME_LEN_P = save_length;
	status = cdsCvtObjIDToStr((cds_attr_name_t *)as_p->as_name, 
				  (byte_t *)NULL,
				  (unsigned char *) NULL,
				  (int *) NULL,
				  rh_p->DNS_STRNAME_P, 
				  rh_p->DNS_STRNAME_LEN_P);
      }
      if (status != CDS_SUCCESS)
	return dns_status_clerk(Flags_p, status);
    }

    /*
     * The AttributeName may be the next contextVariable
     */
    next_p = as_p->as_name;

    if (num_offsets <= ++rh_p->dns_SetIndex) {
	rh_p->dns_SetIndex = 0;
	rh_p->dns_Context_p = next_p;
    }

    return dns_status_clerk(Flags_p, rh_p->dns_status);
}

