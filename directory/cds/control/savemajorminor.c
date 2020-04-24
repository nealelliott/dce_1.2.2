/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: savemajorminor.c,v $
 * Revision 1.1.6.2  1996/02/18  19:30:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:56  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:11:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:07  root]
 * 
 * Revision 1.1.4.2  1994/06/09  18:37:49  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:52  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:01:05  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:07:10  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:09:54  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:42:05  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:20:23  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
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

/* action routine to support version number major.minor */
#include <cdscp.h>
#include <prsdef.h>

static unsigned char majorminor[2];	/* version, as major.minor */

unsigned char *				/* a pointer to the 2 bytes */
majorminor_pointer (void)
{
    return(&majorminor[0]);
}

int					/* always success, ACT_SUCCESS */
savemajorminor (
    struct prs *pcbp)			/* ptr to parser context block */
{
    majorminor[pcbp->av1] = pcbp->num;	/* save major or minor portion */
    return(ACT_SUCCESS);
}
