/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscvtcdssimple.c,v $
 * Revision 1.1.6.2  1996/02/18  19:33:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:42  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:22:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:46  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:16:50  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:00:06  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:23  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:34  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:53:02  peckham
 * 	Remove unnecessary conditionals.
 * 	Remove last reference to dnstables.
 * 	[1994/04/29  14:17:31  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:06:24  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:27:38  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:55:26  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:09:45  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:17:36  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnscvtcdssimple.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
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
#include <dcecdssvc.h>
#include <dce/cdsclerk.h>
#include <uc_clerk.h>     

/* ----------------------------------------------------------------------
 *  Convert SimpleName to opaque
 * ----------------------------------------------------------------------
 */
int 
cdsCvtSimpleToOpq (unsigned char      *StringSname_p,
                   int                IsWildCard,
                   cds_simple_name_t  *OpaqueSname_p,
                   int                *OpaqueSnameLen_p)
{
    dns_ToSimple_t data;
    unsigned int status;

    data.length = 0;
    /*
     * Verify OpaqueSname/OpaqueSnameLen combination
     */
    if (data.snm_p = (SimpleName_t *)OpaqueSname_p) {
	if (!OpaqueSnameLen_p) return(CDS_INVALIDARGUMENT);

	data.length = *OpaqueSnameLen_p;
    } else if (OpaqueSnameLen_p)
	*OpaqueSnameLen_p = 0;

    data.wildcardAllowed = IsWildCard;
    data.noTags = 0;
    data.chr_p = StringSname_p;
    /*
     * Parse the SimpleName
     */
    if ((status = cds_strToSimple(CDS_LOCAL, &data)) != CDS_SUCCESS)
	return(status);

    /*
     * Pass back the computed length
     */
    if (OpaqueSnameLen_p)
	*OpaqueSnameLen_p -= data.length;

    return((OpaqueSname_p && (data.length < 0)) ? CDS_NOROOM : CDS_SUCCESS);
}

/* ----------------------------------------------------------------------
 *  Convert SimpleName to string
 * ----------------------------------------------------------------------
 */
int 
cdsCvtSimpleToStr (cds_simple_name_t  *OpaqueSname_p,
                   unsigned char      *StringSname_p,
                   int                *StringSnameLen_p)
{
    dns_ToSimple_t data;
    unsigned int status;

    data.length = 0;
    /*
     * Verify StringSname/StringSnameLen combination
     */
    if (data.chr_p = StringSname_p) {
	if (!StringSnameLen_p) return(CDS_INVALIDARGUMENT);

	data.length = *StringSnameLen_p;
    } else if (StringSnameLen_p)
	*StringSnameLen_p = 0;

    /*
     * Insert the SimpleName
     */
    data.snm_p = (SimpleName_t *)OpaqueSname_p;
    status = cds_opqToSimple(&data);
    if (status != CDS_SUCCESS) return(status);
    /*
     * Tack on the trailing NULL
     */
    if (0 <= --data.length)
	*data.chr_p = '\0';	/* terminate the string */
    /*
     * Pass back the computed length
     */
    if (StringSnameLen_p)
	*StringSnameLen_p -= data.length;

    return((StringSname_p && (data.length < 0)) ? CDS_NOROOM : CDS_SUCCESS);
}
