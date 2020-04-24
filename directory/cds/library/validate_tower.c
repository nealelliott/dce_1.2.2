/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: validate_tower.c,v $
 * Revision 1.1.6.2  1996/02/18  19:35:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:26  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:27:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:50  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:18:04  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:05:21  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:42:39  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:53  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/14  14:42:24  peckham
 * 	Use cds_basic_types.h instead of decnet_types.h
 * 	[1994/04/11  19:14:42  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:09:27  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:36:40  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:18:08  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:18:19  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:17:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
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
 */

/* the following includes should be in quotes instead of in <> */

#include <dce/dce.h>
#include <dce/cds_basic_types.h>	/* to use GETL16 */


int 
validate_tower (char  *p,
                int   size)
    {
    char *cp;				/* to scan across the tower */
    int pieces;				/* count pieces here. 2 per floor */
    int floors;				/* number of floors in tower */

    cp = p;
    pieces = 0;
    floors = (int)GETL16(cp);		/* save number of floors. advance ptr */
    while ( cp < p+size )
	{
	    pieces++;			/* count these counted strings */
	    cp += GETL16(cp);		/* now advance the pointer */
	};
    /* check that we had the right number of pieces and just used up all input*/
    return ((size > 1) && (cp == p+size) && (2 * floors == pieces));
    }









