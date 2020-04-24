/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsgather.c,v $
 * Revision 1.1.6.2  1996/02/18  19:34:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:06  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:24:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:13  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:07  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:01:06  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:43  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:54  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:53:45  peckham
 * 	Change dns* to cds*.
 * 	[1994/04/29  14:19:49  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:06:55  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:29:31  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:56:50  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:11:45  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:19:48  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsgather.c
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

/*
 *++
 *  FUNCTIONAL DESCRIPTION:
 *  	Concatenate FullName.SimpleName.FullName
 *
 *
 *  FORMAL PARAMETERS:
 *	Fname1_p	- In  - Address of first fullname to be combined
 *	Sname2_p	- In  - Address of simplename to be combined
 *	Fname3_p	- In  - Address of Second fullname to be combined
 *	OutFname_p	- Out - Address of buffer to combine 
 *				fullname.simplename.fullname into
 *	OutFnameLen_p	- In  - Size of buffer
 *			  Out - Actual size of combined name
 *
 *  COMPLETION CODES:
 * 	CDS_INVALIDNAME
 *	CDS_INVALIDARGUMENT
 *	CDS_NOROOM
 *	CDS_SUCCESS
 *
 */
int 
cdsGather (cds_full_name_t    *Fname1_p,
           cds_simple_name_t  *Sname2_p,
           cds_full_name_t    *Fname3_p,
           cds_full_name_t    *OutFname_p,
           int                *OutFnameLen_p)
{
    FullName_t 			*to_p;
    register SimpleName_t 	*last_sn_p;
    register byte_t 		*end_p;
    register int 		length = 0;
    unsigned char		tmpStringBuf[CDS_STR_FNAME_MAX];
    int				tmpStringLen = sizeof(tmpStringBuf);
    cds_full_name_t		tmpStringFname;
    int				tmpStringFnameLen = sizeof(tmpStringFname);
    int				status = CDS_SUCCESS;

    /*
     * Parameter validation:
     *
     * If an input parameter is not supplied, supply a null name.
     * If an output buffer is supplied, then there must be a length.
     * If any input buffer is being used as the output buffer
     * then declare an error for now.
     */
    if (!Fname1_p) Fname1_p = (cds_full_name_t *)NullFullName;
    if (!Sname2_p) Sname2_p = (cds_simple_name_t *)NullSimpleName;
    if (!Fname3_p) Fname3_p = (cds_full_name_t *)NullFullName;

    /* Make sure resultant name won't be to big */
    if (CDS_OPQ_FNAME_MAX < (LEN_FullName(Fname1_p) + 
			     LEN_SimpleName(Sname2_p) +
			     LEN_FullName(Fname3_p) - 
			     LEN_FullName(NullFullName)))
	return(CDS_INVALIDNAME);

    /* 
     * Make certain no repeat arguments, ie, one argument cannot be the same
     * as another.
     */
    if (to_p = (FullName_t *)OutFname_p) {
	if (!OutFnameLen_p) return(CDS_INVALIDARGUMENT);
	if (Fname1_p == OutFname_p) return(CDS_INVALIDARGUMENT);
	if ((cds_full_name_t *)Sname2_p == OutFname_p) 
	    return(CDS_INVALIDARGUMENT);
	if (Fname3_p == OutFname_p) return(CDS_INVALIDARGUMENT);

	length = *OutFnameLen_p;
    } else if (OutFnameLen_p)
	*OutFnameLen_p = 0;

    /* 
     * Copy first fullname into final location 
     */
    if (0 <= (length -= LEN_FullName(Fname1_p))) {
	COPY_FullName(Fname1_p, to_p);
	end_p = SKIP_FullName(OutFname_p) - LEN_SimpleName(NullSimpleName);
      };

    /* 
     * Copy simplename into final location 
     */
    if (EXT8(((SimpleName_t *)Sname2_p)->sn_flag) != SN_null) {
      if (0 <= (length -= LEN_SimpleName(Sname2_p))) {
	COPY_SimpleName(Sname2_p, end_p);
	end_p = SKIP_SimpleName(end_p);
      }
    };

    /*
     * Copy second fullname into final location.
     */
    last_sn_p = (SimpleName_t *)((FullName_t *)Fname3_p)->fn_name;
    while (1==1) {
	if (0 <= (length -= LEN_SimpleName(last_sn_p))) {
	    COPY_SimpleName(last_sn_p, end_p);
	    end_p = SKIP_SimpleName(end_p);
	}
	if (EXT8(last_sn_p->sn_flag) == SN_null) /* Copied last name, done.  */
	    break;

	last_sn_p = (SimpleName_t *)SKIP_SimpleName(last_sn_p);
    }

    if (OutFnameLen_p)				 /* Get final length   */
	*OutFnameLen_p -= length;

    if (OutFname_p) {				 /* Make certain that buffer  */
	if (length < 0)				 /* was big enough for name.  */
	    return(CDS_NOROOM);
	INS16(to_p->fn_length, (byte_t *)end_p - to_p->fn_name);
    }

    /* 
     * We must check that we have a valid name before returning.  Rather than
     * do the checking here and duplicate the efforts of cdsCvtStrFullToOpq
     * since we know the precise checking as defined by BNF in the Architecture
     * is subject to change... we will be converting the opaque name to a
     * string and back to an opaque name and have those routines do our checking
     * for us.
     */
    status = cdsCvtFullToStr(OutFname_p, tmpStringBuf, &tmpStringLen);
    if (status == CDS_SUCCESS)
	status = cdsCvtFullToOpq(tmpStringBuf,
				&tmpStringFname, &tmpStringFnameLen);
    if (status != CDS_SUCCESS) {		 /* If not a valid name  */
	*OutFnameLen_p = 0;			 /* zero the length returned */
	}

    return(status);
}
