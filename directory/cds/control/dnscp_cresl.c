/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscp_cresl.c,v $
 * Revision 1.1.6.2  1996/02/18  19:28:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:19  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:07:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:56  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:00:29  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:39:11  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:36:33  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:14  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:58:04  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:58:01  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:02:38  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:32:38  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:20:52  devrcs
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

int
cresl (
	struct entblk	*entbp,		/* ptr to head of entity list */
	struct attrblk	*atrbp,		/* ptr to head of attribute list */
	struct envblk	*envbp,		/* ptr to environment block */
	struct cmdblk	*cmdbp,		/* ptr to current command block */
	struct op_blk	*outbp)		/* ptr to head of output list */
{
    /* the first entity on the entity list is the softlink */

    struct  entblk *ent1p;		/* ptr to the softlink entity */
    struct attrblk *tgtbp;		/* ptr to target  attr blk */
    struct attrblk *tobp;		/* ptr to timeout attr blk */
    unsigned char  *slp;		/* ptr to full name of softlink */
    int status;				/* the portable interface tells us */
    struct dnsFlagStat flags;		/* misc parameters for dns */
    dns_timeout_t *to_p;

    ent1p = entbp->nxt;			/* point to softlink ent */
    slp  = ent1p->name_p;		/* point to softlink name */

    /* we need the dns$linktarget and dns$linktimeout attributes */
    /* both are on the list of attribute blocks after parsing */
                                        /* point to target  attr block */
    tgtbp = find_attr(atrbp, (dns_opq_sname *)dns_AttributeName(DNS_LINKTARGET));
                                        /* point to timeout attr block */
    tobp  = find_attr(atrbp, (dns_opq_sname *)dns_AttributeName(DNS_LINKTIMEOUT));
    assert(tgtbp != 0);
    if (tobp) 
      to_p= (dns_timeout_t *)tobp->av->vp->av_value;
    else
      to_p = NULL_DNS_TIMEOUT;

    /* we know what these attribute values look like. */
    init_flagstatus(
	    &flags,			/* ptr to the block to init */
	    envbp->curconf,		/* current confidence */
	    &(envbp->curtout));		/* current timeout */
    status = 
	dnsCreateSLink(			/* create the new softlink */
	    (dns_opq_fname *)slp,	/* ptr to softlink name */
	    (dns_opq_fname *)tgtbp->av->vp->av_value,
	    to_p,
	    (dns_cts *)NULL,
	    &flags);			/* ptr to in/out flag/status */
    if (status != DNS_SUCCESS) {
      create_err_list_element (slp,
			       &flags,
			       "dnsCreateSLink");

    }
    return(status);			/* tell caller what happened */    
}
