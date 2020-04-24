/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscountsimple.c,v $
 * Revision 1.1.6.2  1996/02/18  19:33:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:32  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:22:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:32  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:16:42  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:59:37  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:13  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:26  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:52:40  peckham
 * 	Rename dns*() to cds*()
 * 	[1994/04/29  14:16:56  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:06:12  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:26:50  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:54:52  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:08:50  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:17:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnscountsimple.c
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

/* ----------------------------------------------------------------------
 *  Return a SimpleName count for the given FullName
 * ----------------------------------------------------------------------
 */
int 
cdsCountSimple (cds_full_name_t *Fname_p)
{
    register byte_t *snm_p = ((FullName_t *)Fname_p)->fn_name;
    register int ret = 0;

    while (EXT8(((SimpleName_t *)snm_p)->sn_flag) != SN_null) {
	ret++;
	snm_p = SKIP_SimpleName(snm_p);
    }
    return(ret);
}

