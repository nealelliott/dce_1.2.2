/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnshandlesetnam.c,v $
 * Revision 1.1.6.2  1996/02/18  19:34:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:19  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:24:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:25  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:18  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:01:47  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:54  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:04  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:54:13  peckham
 * 	Change dns* to cds*
 * 	[1994/04/29  14:24:03  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:07:09  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:30:38  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:57:38  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:12:47  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:19:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnshandlesetnam.c
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
 */
  
#include <dce/dce.h>
#include <dce/cdsclerk.h>
#include <dcecdssvc.h>
#include <uc_parse.h>
#include <uc_clerk.h>     

#define	CDS_OPQNAME_PP		dns_hcp.hcp_SetNam.hcp_OpqName_pp
#define	DNS_STRNAME_P		dns_hcp.hcp_SetNam.hcp_StrName_p
#define	DNS_STRNAME_LEN_P 	dns_hcp.hcp_SetNam.hcp_StrName_len_p

static int
cds_parseSetNam (
    cdsFlagStat_t	*,
    cds_handle_t	 );


/* ----------------------------------------------------------------------
 *  Set up handle context to retrieve Set of SimpleNames
 * ----------------------------------------------------------------------
 */
int 
cds_handleSetNam (cds_handle_t        Handle_p,
                  cds_simple_name_t   **Name_pp,
                  unsigned char       *NameBuf_p,
                  int                 *NameBufLen_p,
                  cdsFlagStat_t       *Flags_p)
{
    struct dns_ReadHandle *const rh_p = (struct dns_ReadHandle *)Handle_p;

    if (rh_p->dns_Return != dns_retSetNam) {
	if (rh_p->dns_Return != dns_retNone)
	    return dns_status_clerk(Flags_p, CDS_CLERKBUG);

	rh_p->dns_Return = dns_retSetNam;
	rh_p->dns_Context_p = (unsigned char *)NullSimpleName;
	rh_p->dns_parse = cds_parseSetNam;
	rh_p->dns_SetIndex = 0;
	rh_p->dns_done = BL_false;
    }

    rh_p->CDS_OPQNAME_PP = Name_pp;
    rh_p->DNS_STRNAME_P = NameBuf_p;
    rh_p->DNS_STRNAME_LEN_P = NameBufLen_p;

    if (!rh_p->dns_Context_p)
	return cds_parseSetNam(Flags_p, Handle_p);
    else if (rh_p->dns_done)
	return dns_status_clerk(Flags_p, CDS_EMPTY);

    return(CDS_PENDING);
}

/* ----------------------------------------------------------------------
 *  Retrieve SimpleName from Set
 * ----------------------------------------------------------------------
 */
static int 
cds_parseSetNam (cdsFlagStat_t       *Flags_p,
                 cds_handle_t        Handle_p)
{
    struct dns_ReadHandle *const rh_p = (struct dns_ReadHandle *)Handle_p;
    register byte_t *next_p = rh_p->dns_ReadBuf_p;
    register SimpleName_t *sn_p;
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
     * go to the data portion of the SetMember/AtomicData
     */
    sn_p = (SimpleName_t *)((MemberValue_t *)next_p)->mv_value;

    if (rh_p->CDS_OPQNAME_PP)
	*rh_p->CDS_OPQNAME_PP = (cds_simple_name_t *)sn_p;

    if (rh_p->DNS_STRNAME_P || rh_p->DNS_STRNAME_LEN_P) {
	int status = cdsCvtSimpleToStr((cds_simple_name_t *)sn_p,
	    rh_p->DNS_STRNAME_P, rh_p->DNS_STRNAME_LEN_P);

	if (status != CDS_SUCCESS)
	    return dns_status_clerk(Flags_p, status);
    }
    /*
     * The SimpleName may be the next contextVariable
     */
    next_p = (byte_t *)sn_p;

    if (num_offsets <= ++rh_p->dns_SetIndex) {
	rh_p->dns_SetIndex = 0;
	rh_p->dns_Context_p = next_p;
    }

    return dns_status_clerk(Flags_p, rh_p->dns_status);
}

#undef CDS_OPQNAME_PP
#undef DNS_STRNAME_P	
#undef DNS_STRNAME_LEN_P

#define	CDS_OPQNAME_PP		dns_hcp.hcp_SetNamCls.hcp_OpqName_pp
#define	DNS_STRNAME_P		dns_hcp.hcp_SetNamCls.hcp_StrName_p
#define	DNS_STRNAME_LEN_P	dns_hcp.hcp_SetNamCls.hcp_StrName_len_p
#define	CDS_OPQCLASS_PP		dns_hcp.hcp_SetNamCls.hcp_OpqClass_pp
#define	DNS_STRCLASS_P		dns_hcp.hcp_SetNamCls.hcp_StrClass_p
#define	DNS_STRCLASS_LEN_P	dns_hcp.hcp_SetNamCls.hcp_StrClass_len_p
#define	DNS_RETURNCLASS_P	dns_hcp.hcp_SetNamCls.hcp_ReturnClass_p

static int
cds_parseSetNamCls (
    cdsFlagStat_t	*Flags_p,
    cds_handle_t	Handle_p);
/* ----------------------------------------------------------------------
 *  Set up handle context to retrieve Set of SimpleName/ClassNames
 * ----------------------------------------------------------------------
 */
int
cds_handleSetNamCls (cds_handle_t        Handle_p,
                     cds_simple_name_t   **Name_pp,
                     unsigned char       *NameBuf_p,
                     int                 *NameBufLen_p,
                     int                 *ReturnClass_p,
                     cds_simple_name_t   **ClassName_pp,
                     unsigned char       *ClassNameBuf_p,
                     int                 *ClassNameBufLen_p,
                     cdsFlagStat_t       *Flags_p)
{
    struct dns_ReadHandle *const rh_p = (struct dns_ReadHandle *)Handle_p;
    cds_simple_name_t *oname;
    cds_simple_name_t *oclass;

    if (rh_p->dns_Return != dns_retSetNamCls) {
	if (rh_p->dns_Return != dns_retNone)
	    return dns_status_clerk(Flags_p, CDS_CLERKBUG);

	rh_p->dns_Return = dns_retSetNamCls;
	rh_p->dns_Context_p = (unsigned char *)NullSimpleName;
	rh_p->dns_parse = cds_parseSetNamCls;
	rh_p->dns_SetIndex = 0;
	rh_p->dns_done = BL_false;
    }

    rh_p->CDS_OPQNAME_PP = (Name_pp) ? Name_pp : &oname;
    rh_p->CDS_OPQCLASS_PP = (ClassName_pp) ? ClassName_pp : &oclass;
    rh_p->DNS_STRNAME_P = NameBuf_p;
    rh_p->DNS_STRNAME_LEN_P = NameBufLen_p;
    rh_p->DNS_STRCLASS_P = ClassNameBuf_p;
    rh_p->DNS_STRCLASS_LEN_P = ClassNameBufLen_p;
    rh_p->DNS_RETURNCLASS_P = ReturnClass_p;

    if (!rh_p->dns_Context_p)
	return cds_parseSetNamCls(Flags_p, Handle_p);
    else if (rh_p->dns_done)
	return dns_status_clerk(Flags_p, CDS_EMPTY);

    return(CDS_PENDING);
}

/* ----------------------------------------------------------------------
 *  Retrieve SimpleName/ClassName from Set
 * ----------------------------------------------------------------------
 */
static int 
cds_parseSetNamCls (cdsFlagStat_t       *Flags_p,
                    cds_handle_t        Handle_p)
{
    struct dns_ReadHandle *const rh_p = (struct dns_ReadHandle *)Handle_p;
    register byte_t *next_p = rh_p->dns_ReadBuf_p;
    register byte_t *nc_p;
    register int num_offsets = 0;
    int status = DNS_SUCCESS;

    rh_p->dns_Context_p = NULL;
    Flags_p->fsOutLink = rh_p->dns_outLinked;
    Flags_p->fsFromCache = rh_p->dns_fromCache;

    if (rh_p->DNS_RETURNCLASS_P)
	*rh_p->DNS_RETURNCLASS_P = rh_p->dns_ReturnClass;

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
    nc_p = ((MemberValue_t *)next_p)->mv_value;

    /*
     * This SimpleName may be the next contextVariable
     */
    next_p = nc_p;

    if (rh_p->CDS_OPQNAME_PP)
	*rh_p->CDS_OPQNAME_PP = (cds_simple_name_t *)nc_p;

    if (rh_p->DNS_STRNAME_P || rh_p->DNS_STRNAME_LEN_P) {
	int status_n = cdsCvtSimpleToStr((cds_simple_name_t *)nc_p,
	    rh_p->DNS_STRNAME_P, rh_p->DNS_STRNAME_LEN_P);

	if (status_n != DNS_SUCCESS) status = status_n;
    }

    /*
     * If there is a ClassName, then give to user
     */
    if (rh_p->dns_ReturnClass) {
	nc_p = SKIP_SimpleName(nc_p);

	if (rh_p->CDS_OPQCLASS_PP)
	    *rh_p->CDS_OPQCLASS_PP = (cds_simple_name_t *)nc_p;

	if (rh_p->DNS_STRCLASS_P || rh_p->DNS_STRCLASS_LEN_P) {
	    int status_c = cdsCvtSimpleToStr((cds_simple_name_t *)nc_p,
		rh_p->DNS_STRCLASS_P, rh_p->DNS_STRCLASS_LEN_P);

	    if (status_c != CDS_SUCCESS) status = status_c;
	}
    }

    if (status != CDS_SUCCESS)
	return dns_status_clerk(Flags_p, status);

    if (num_offsets <= ++rh_p->dns_SetIndex) {
	rh_p->dns_SetIndex = 0;
	rh_p->dns_Context_p = next_p;
    }

    return dns_status_clerk(Flags_p, rh_p->dns_status);
}
