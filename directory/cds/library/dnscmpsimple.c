/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscmpsimple.c,v $
 * Revision 1.1.11.1  1996/08/09  11:58:44  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/07/30  15:40 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/2]
 *
 * 	Fix ambiguity in compare simplename function on comparing names where one
 * 	is a prefix of the other.
 * 	[1996/02/18  19:33:03  marty  1.1.9.2]
 *
 * Revision 1.1.9.2  1996/02/18  19:33:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:29  marty]
 * 
 * Revision 1.1.9.1  1995/12/08  15:22:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:27  root]
 * 
 * Revision 1.1.6.4  1994/06/30  19:16:40  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:59:28  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:41:10  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:24  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/29  15:52:34  peckham
 * 	Remove unnecessary conditionals.
 * 	Remove dnstables dependency.
 * 	[1994/04/29  14:16:48  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:06:09  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:26:39  peckham]
 * 
 * Revision 1.1.4.3  1992/12/30  13:54:44  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:08:41  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/09  20:43:25  mfox
 * 	Eliminate assignment in function arg list - order of evaluation assumed
 * 	[1992/12/09  20:42:52  mfox]
 * 
 * Revision 1.1.2.2  1992/03/22  22:18:25  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:31:06  weisman]
 * 
 * Revision 1.1  1992/01/19  15:19:09  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * MODULE: dnscmpsimple.c
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
#include <uc_clerk.h>     
#include <uc_parse.h>
#include <dnsnormal.h>

static int
CDScmp (
    byte_t	*,
    byte_t	*,
    int		,
    int		);

/* ----------------------------------------------------------------------
 *  Compare opaque SimpleNames
 * ----------------------------------------------------------------------
 */
int
cdsCmpSimple (cds_simple_name_t  *Sname1_p,
              cds_simple_name_t  *Sname2_p)
{
    register SimpleName_t *snm1_p = (SimpleName_t *)Sname1_p;
    register SimpleName_t *snm2_p = (SimpleName_t *)Sname2_p;
    const int len1 = EXT8(snm1_p->sn_length);
    const int len2 = EXT8(snm2_p->sn_length);
    register int length = len1;
    register int ret;

    if (length > len2) length = len2; /* set shortest field */

    /*
     * Listed from greatest to least.
     */
    switch (EXT8(snm1_p->sn_flag)) {
    case SN_typed:
        switch (EXT8(snm2_p->sn_flag)) {
	default:
	    return(1);		/* snm1 > snm2 */
        case SN_typed:
            {
            SimpleName_t tsn1;
            SimpleName_t tsn2;
            register int i;

            if (dns_normalize_sname(snm1_p, &tsn1, sizeof(tsn1))
			!= CDS_SUCCESS)
                return(1);
            if (dns_normalize_sname(snm2_p, &tsn2, sizeof(tsn2))
			!= CDS_SUCCESS)
                return(-1);
	    snm1_p = &tsn1;
	    snm2_p = &tsn2;

            length = EXT8(tsn1.sn_length);
            if (length > EXT8(tsn2.sn_length))
	        length = EXT8(tsn2.sn_length);

	    for (i = 0; i < length; i++)
		if (ret = tsn1.sn_name[i] - tsn2.sn_name[i])
		    return(ret);
            break;
            }
	}
	break;
    case SN_quoted:
    case SN_normal:
	switch (EXT8(snm2_p->sn_flag)) {
	case SN_binary:
	case SN_objectid:
	case SN_null:
	    return(1);		/* snm1 > snm2 */
	case SN_quoted:
	case SN_normal:
	    {
	    register byte_t *name1_p = snm1_p->sn_name;
	    register byte_t *name2_p = snm2_p->sn_name;

	    for (; length; name1_p++, name2_p++, --length)
		{
		 /*
		  * Note: the following two lines may look
		  * like they convert the name to all uppercase, but
		  * they don't.  This is an old bug and this comparison,
		  * as a result, is case sensitive.
		  */
		char c1 = islower(*name1_p) ? *name1_p : toupper(*name1_p);
		char c2 = islower(*name2_p) ? *name2_p : toupper(*name2_p);

		if (ret = c1 - c2) return(ret);
		}
	    break;
	    }
	case SN_cds:
	    ret = CDScmp(snm1_p->sn_name, snm2_p->sn_name, len1, len2);
	    return(ret);
	default:
	    return(-1);		/* snm1 < snm2 */
	}
	break;
    case SN_binary:
	switch (EXT8(snm2_p->sn_flag)) {
	case SN_null:
	    return(1);		/* snm1 > snm2 */
	case SN_binary:
	    if (ret = memcmp(snm1_p->sn_name, snm2_p->sn_name, length))
		return(ret);
	    else
		break;
	default:
	    return(-1);		/* snm1 < snm2 */
	}
	break;
    case SN_objectid:
	switch (EXT8(snm2_p->sn_flag)) {
	case SN_binary:
        case SN_null:
	    return(1);		/* snm1 > snm2 */
	case SN_objectid:
	    if (ret = memcmp(snm1_p->sn_name, snm2_p->sn_name, length))
		return(ret);
	    else
		break;
	default:
	    return(-1);		/* snm1 < snm2 */
	}
	break;
    case SN_cds:
	switch (EXT8(snm2_p->sn_flag)) {
	case SN_binary:
	case SN_objectid:
	case SN_null:
	    return(1);		/* snm1 > snm2 */
	case SN_quoted:
	case SN_normal:
	case SN_cds:
	    ret = CDScmp(snm1_p->sn_name, snm2_p->sn_name, len1, len2);
	    return(ret);
	default:
	    return(-1);		/* snm1 < snm2 */
	}
	break;

    case SN_null:
    default:
	return(-1);		/* snm1 < snm2 */
    }

    return (len1 - len2);
}

static int 
CDScmp (byte_t  *name1_p,
        byte_t  *name2_p,
        int     len1,
        int     len2)
{
    int i, j;
    int len;

    len = (len1<len2) ? len1 : len2;

    for ( i=0; i<len; i++ ) {
	/* CDS names may contain escaped wildcards */
	if ( *name2_p == '\\') {
	    name2_p++;
	    --len2;
	}
	if (( j = *name1_p++ - *name2_p++ ) != 0 ) return (j);
    }
    return ( len1 - len2 ); /* strings are equal if adjusted lengths are same */ 
}








