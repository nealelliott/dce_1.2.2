/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscp_delobj.c,v $
 * Revision 1.1.7.2  1996/02/18  19:28:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:23  marty]
 *
 * Revision 1.1.7.1  1995/12/08  15:07:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:59  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:00:30  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:40:44  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:36:35  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:16  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:58:08  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:58:12  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:02:48  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:32:54  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:20:54  devrcs
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

#include <cdscp.h>
#include <assert.h>

struct dnsFlagStat *init_flagstatus ();

int
delobj (
	struct entblk	*entbp,		/* ptr to head of entity list */
	struct attrblk	*atrbp,		/* ptr to head of attribute list */
	struct envblk	*envbp,		/* ptr to environment block */
	struct cmdblk	*cmdbp,		/* ptr to current command block */
	struct op_blk	*outbp)		/* ptr to head of output list */
{
    /* the first entity on the entity list is the object */
    /* for now we do not use the attributes list */

    struct  entblk *ent1p;		/* ptr to the object entity */
    unsigned char *objp;		/* ptr to full name of object */
    int status;				/* the portable interface tells us */
    struct dnsFlagStat flags;		/* misc parameters for dns */

    ent1p = entbp->nxt;			/* point to object ent */
    objp  = ent1p->name_p;		/* point to object name */

    init_flagstatus(
	    &flags,			/* ptr to the block to init */
	    envbp->curconf,		/* current confidence */
	    &(envbp->curtout));		/* current timeout */
    status = 
	dnsDelObj(			/* delete the specified object */
	    (dns_opq_fname *)objp,	/* ptr to object name */
	    &flags);			/* ptr to in/out flag/status */
    if (status != DNS_SUCCESS) {
      create_err_list_element (objp,
			       &flags,
			       "dnsDelObj");

    }
    return(status);			/* tell caller what happened */    
}
