/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscmpfull.c,v $
 * Revision 1.1.6.2  1996/02/18  19:33:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:28  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:22:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:25  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:16:39  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:59:24  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:08  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:23  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:52:32  peckham
 * 	Remove unnecessary conditionals.
 * 	[1994/04/29  14:16:43  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:06:08  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:26:32  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:54:41  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:08:36  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:17:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnscmpfull.c
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

/* ----------------------------------------------------------------------
 *  Compare opaque FullNames
 * ----------------------------------------------------------------------
 */
int
cdsCmpFull (cds_full_name_t  *Fname1_p,
            cds_full_name_t  *Fname2_p)
{
    register FullName_t *fnm1_p = (FullName_t *)Fname1_p;
    register FullName_t *fnm2_p = (FullName_t *)Fname2_p;
    register int diff;

    if (diff = ObjUID_compare(fnm1_p->fn_root, fnm2_p->fn_root))
	return(diff);
    else {
	register SimpleName_t *snm1_p = (SimpleName_t *)fnm1_p->fn_name;
	register SimpleName_t *snm2_p = (SimpleName_t *)fnm2_p->fn_name;
	register int result;

	while (1==1) {
	    if (EXT8(snm1_p->sn_flag) == SN_null)
		if (EXT8(snm2_p->sn_flag) != SN_null)
		    return(-1);
		else
		    return(0);
	    else if (EXT8(snm2_p->sn_flag) == SN_null)
		return(1);

	    if (result = cdsCmpSimple(
		    (cds_simple_name_t *)snm1_p, (cds_simple_name_t *)snm2_p))
		return(result);

	    snm1_p = (SimpleName_t *)SKIP_SimpleName(snm1_p);
	    snm2_p = (SimpleName_t *)SKIP_SimpleName(snm2_p);
	}
    }
}






