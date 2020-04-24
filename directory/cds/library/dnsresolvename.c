/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsresolvename.c,v $
 * Revision 1.1.6.2  1996/02/18  19:34:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:38  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:25:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:38  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:30  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:02:39  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:42:06  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:19  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:54:43  peckham
 * 	Remove unnecessary conditionals.
 * 	Rename dns*() -> cds*().
 * 	[1994/04/29  14:25:59  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:07:29  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:32:02  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:58:42  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:14:21  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:19:17  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsresolvename.c
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

/* ----------------------------------------------------------------------
 *  Follow soft links to produce translated name
 * ----------------------------------------------------------------------
 */
int 
cdsResolveName (cds_full_name_t     *Name_p,
                cds_full_name_t     *ResolvedName_p,
                int                 *ResolvedNameLen_p,
                cdsFlagStat_t       *Flags_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    cds_full_name_t name;
    cds_full_name_t rname;
    cds_full_name_t *orname;
    cds_full_name_t temp_opq_name;
    int	*orlen;
    int	len;
    int status;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" >cdsResolveName()"));

    switch (Flags_p->fsNameType) {
    case cdsStrXDS:
    case cdsStrDCE:
    case 0:
	if ((Name_p = cds_fulltoopq(Name_p, &name, Flags_p)) == NULL) {
	    status = CDS_ERROR;
	    goto leave_cdsResolveName;
	}
	/*
	 * Sidetrack the opaque resolved name - but remember the user's buffer
	 */
	orname = ResolvedName_p;
	ResolvedName_p = &rname;
	orlen = ResolvedNameLen_p;
	len = sizeof(rname);
	ResolvedNameLen_p = &len;
    case cdsOpqDNA:
    case cdsStrDNA:
	break;
    default:
	status = cds_status_clerk(Flags_p, CDS_INVALIDARGUMENT);
	goto leave_cdsResolveName;
    }

    PRM_build(OP_ResolveName, Flags_p);
      PRM_IN_FullName(Name_p);
      PRM_OUT_bytes(ResolvedNameLen_p, sizeof(int));
      PRM_RET_FullName(ResolvedName_p, *ResolvedNameLen_p);
    PRM_send(status);

    if (status == CDS_SUCCESS) {
	cds_full_name_t tmp_str_name;

	switch (Flags_p->fsNameType) {
	case cdsStrDCE:
	case 0:
	    status = cdsCvtFullToStr(ResolvedName_p,
				(unsigned char *)orname, orlen);
	    break;
	case cdsStrXDS:
	    len = sizeof(tmp_str_name);
	    status = cdsCvtFullToStr(ResolvedName_p,
				(unsigned char *)&tmp_str_name, &len);
	    len = sizeof(temp_opq_name);
	    status = dns_CvtCDSFullToGDS((unsigned char *)&tmp_str_name,
		1, 0, &temp_opq_name, &len);
	    if (status == CDS_SUCCESS)
	        status = cdsCvtFullToStr(&temp_opq_name,
		    (unsigned char *)orname, orlen);
        default:
            break;
	}
    }

leave_cdsResolveName:

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" <cdsResolveName() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}
