/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsscatter.c,v $
 * Revision 1.1.6.2  1996/02/18  19:34:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:45  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:25:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:43  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:34  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:02:57  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:42:14  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:24  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:54:53  peckham
 * 	change dns* to cds*
 * 	[1994/04/29  14:26:44  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:07:38  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:32:31  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:59:05  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:14:43  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:18:27  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsscatter.c
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

static SimpleName_t *
scatterToFullName (
    ObjUID_t		*,
    SimpleName_t	*,
    int			,
    FullName_t		**,
    int			*);

/* ----------------------------------------------------------------------
 *  Scatter FullName to FullName, SimpleName, FullName
 * ----------------------------------------------------------------------
 */
int 
cdsScatter (cds_full_name_t    *Fname1_p,
            int                count,
            cds_full_name_t    *Fname2_p,
            int                *Fname2Len_p,
            cds_simple_name_t  *Sname3_p,
            int                *Sname3Len_p,
            cds_full_name_t    *Fname4_p,
            int                *Fname4Len_p)
{
    register SimpleName_t *snm_p;
    register int length = 0;

    /*
     * Parameter validation:
     *
     * If an input parameter is not supplied, supply a null name.
     * If an output buffer is supplied, then there must be a length.
     * If the input buffer is being used as any output buffer
     * then declare an error for now.
     */

    if (!Fname1_p) Fname1_p = (cds_full_name_t *)NullFullName;

    if (Fname2_p) {
	if (!Fname2Len_p) return(CDS_INVALIDARGUMENT);
	if (Fname2_p == Fname1_p) return(CDS_INVALIDARGUMENT);
    } else if (Fname2Len_p)
	*Fname2Len_p = 0;

    if (Sname3_p) {
	if (!Sname3Len_p) return(CDS_INVALIDARGUMENT);
	if ((cds_full_name_t *)Sname3_p == Fname1_p) return(CDS_INVALIDARGUMENT);
    } else if (Sname3Len_p)
	*Sname3Len_p = 0;

    if (Fname4_p) {
	if (!Fname4Len_p) return(CDS_INVALIDARGUMENT);
	if (Fname4_p == Fname1_p) return(CDS_INVALIDARGUMENT);
    } else if (Fname4Len_p)
	*Fname4Len_p = 0;

    snm_p = (SimpleName_t *)((FullName_t *)Fname1_p)->fn_name;
    snm_p = scatterToFullName((ObjUID_t *)Fname1_p, snm_p, count,
	(FullName_t **)&Fname2_p, Fname2Len_p);

    if (Sname3Len_p) {
	length = *Sname3Len_p;

	if (0 <= (length -= LEN_SimpleName(snm_p)))
	    COPY_SimpleName(snm_p, Sname3_p);

	*Sname3Len_p -= length;
	if (!Sname3_p)
	  length = 0;
      }

    if (EXT8(snm_p->sn_flag) != SN_null)
	snm_p = (SimpleName_t *)SKIP_SimpleName(snm_p);

    scatterToFullName((ObjUID_t *)Fname1_p, snm_p, 16000,
	(FullName_t **)&Fname4_p, Fname4Len_p);

    if (Fname2_p) length = -1;
    if (Fname4_p) length = -1;

    return ((length < 0) ? CDS_NOROOM : CDS_SUCCESS);
}

static SimpleName_t *
scatterToFullName (ObjUID_t      *root_p,
                    SimpleName_t  *snm_p,
                    int           count,
                    FullName_t    **fnm_pp,
                    int           *fnmLen_p)
{
    register FullName_t *fnm_p;
    register SimpleName_t *fsnm_p;
    register int length = 0;

    if (fnm_p = *fnm_pp)
	length = *fnmLen_p;

    if (0 <= (length -= sizeof(NULLFULLNAME_u))) {
	COPY_ObjUID(root_p, fnm_p);
	fsnm_p = (SimpleName_t *)fnm_p->fn_name;
    }

    for (; 0 < count; --count) {
	if (EXT8(snm_p->sn_flag) == SN_null)
	    break;

	if (0 <= (length -= LEN_SimpleName(snm_p))) {
	    COPY_SimpleName(snm_p, fsnm_p);
	    fsnm_p = (SimpleName_t *)SKIP_SimpleName(fsnm_p);
	}
	snm_p = (SimpleName_t *)SKIP_SimpleName(snm_p);
    }
    if (0 <= length) {
	COPY_SimpleName(NullSimpleName, fsnm_p);
	fsnm_p = (SimpleName_t *)SKIP_SimpleName(fsnm_p);
	INS16(fnm_p->fn_length, (byte_t *)fsnm_p - fnm_p->fn_name);
	*fnm_pp = NULL;
    }

    if (fnmLen_p)
	*fnmLen_p -= length;

    return (snm_p);
}



