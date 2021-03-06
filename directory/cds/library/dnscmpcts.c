/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscmpcts.c,v $
 * Revision 1.1.6.2  1996/02/18  19:32:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:27  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:22:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:21  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:16:39  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:59:21  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:07  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:22  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:52:30  peckham
 * 	Remove unnecessary conditionals.
 * 	[1994/04/29  14:16:40  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:06:07  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:26:27  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:54:37  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:08:32  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:17:19  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnscmpcts.c
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
 *  Compare Timestamps
 * ----------------------------------------------------------------------
 */
int 
cdsCmpCTS (cds_cts_t  *cts1_p,
           cds_cts_t  *cts2_p)
{

    register int cnt;
    Timestamp_t *uid1_p,*uid2_p;

    uid1_p = (Timestamp_t *)cts1_p;
    uid2_p = (Timestamp_t *)cts2_p;

    /* compare times first */
    if (GT_Time(uid1_p->ts_time, uid2_p->ts_time))
	return(1);
    if (LT_Time(uid1_p->ts_time, uid2_p->ts_time))
	return(-1);

    /* Break tie by comparing node addresses */
    for (cnt = sizeof(uid1_p->ts_node) - 1; cnt >= 0; --cnt) {
	if (uid1_p->ts_node[cnt] > uid2_p->ts_node[cnt])
	    return(1);
	if (uid1_p->ts_node[cnt] < uid2_p->ts_node[cnt])
	    return(-1);
    }
    return(0);
}
