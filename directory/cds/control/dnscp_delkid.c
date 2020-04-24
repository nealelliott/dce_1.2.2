/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscp_delkid.c,v $
 * Revision 1.1.9.2  1996/02/18  19:28:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:21  marty]
 *
 * Revision 1.1.9.1  1995/12/08  15:07:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:57  root]
 * 
 * Revision 1.1.7.3  1994/08/03  19:00:30  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:39:17  mccann]
 * 
 * Revision 1.1.7.2  1994/06/09  18:36:34  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:15  devsrc]
 * 
 * Revision 1.1.7.1  1994/03/12  21:58:06  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:58:07  peckham]
 * 
 * Revision 1.1.4.3  1993/02/15  21:21:44  keegan
 * 	Always delete the child pointer. It can easily be recreated.
 * 	Previously, this code checked the replica set to see if any
 * 	directories were still reachable.
 * 	[1993/02/15  21:17:33  keegan]
 * 
 * Revision 1.1.4.2  1992/12/30  13:02:43  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:32:45  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:45:57  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:21:09  weisman]
 * 
 * Revision 1.1  1992/01/19  15:22:54  devrcs
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
 *
 */

/* dnscp_delkid.c is used to delete a child pointer. */

#include <cdscp.h>
#include <uc_clerk.h>


int
delkid (
    struct entblk	*entbp,		/* ptr to head of entity list */
    struct attrblk	*atrbp,		/* ptr to head of attribute list */
    struct envblk	*envbp,		/* ptr to environment block */
    struct cmdblk	*cmdbp,		/* ptr to current command block */
    struct op_blk	*outbp)		/* ptr to head of output list */
					/*ARGSUSED*/ /* appease saber */
{
    /* the first entity on the entity list is the child pointer */
    /* for now we do not use the attributes list */

    struct  entblk *ent1p;		/* ptr to the child pointer entity */
    unsigned char *dirp;		/* ptr to full name of directory */
    int status;				/* status used by ALL routines */
    struct dnsFlagStat   delflags;	/* misc parameters for dns */

    ent1p = entbp->nxt;			/* point to child pointer entity */
    dirp  = ent1p->name_p;		/* point to directory name */

    init_flagstatus(
	    &delflags,			/* ptr to the block to init */
	    envbp->curconf,		/* current confidence */
	    &(envbp->curtout));		/* current timeout */
    status = 
	cdsDelChild(			/* delete the child pointer */
	    (dns_opq_fname *)dirp,	/* ptr to directory name */
	    &delflags);			/* ptr to in/out flag/status */
    if ( status != DNS_SUCCESS ) {
	create_err_list_element(	/* report the error */
	    dirp,			/* ptr to name of entity */
	    &delflags,			/* ptr to flags/status info */
	    "cdsDelChild");		/* operation being performed */
	}
    return( status );
}
