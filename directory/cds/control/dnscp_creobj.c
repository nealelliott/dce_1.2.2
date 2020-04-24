/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscp_creobj.c,v $
 * Revision 1.1.6.2  1996/02/18  19:28:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:18  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:07:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:54  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:00:28  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:39:07  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:36:32  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:13  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:58:02  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:57:55  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:02:33  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:32:32  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:20:51  devrcs
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
creobj (
	struct entblk	*entbp,		/* ptr to head of entity list */
	struct attrblk	*atrbp,		/* ptr to head of attribute list */
	struct envblk	*envbp,		/* ptr to environment block */
	struct cmdblk	*cmdbp,		/* ptr to current command block */
	struct op_blk	*outbp)		/* ptr to head of output list */

{
    /* the first entity on the entity list is the object */
    struct  entblk *ent1p;		/* ptr to the object entity */
    struct attrblk  *cnabp;		/* ptr to classname     attr blk */
    struct attrblk  *cvabp;		/* ptr to classversion  attr blk */
    unsigned char  *objp;		/* ptr to full name of object */
    int status;				/* the portable interface tells us */
    struct dnsFlagStat flags;		/* misc parameters for dns */

    ent1p = entbp->nxt;			/* point to object ent */
    objp  = ent1p->name_p;		/* point to object name */
    /* we need the dns$classname and dns$classversion attributes */
                                        /* point to class name    attr block */
    cnabp = find_attr(atrbp, (dns_opq_sname *)dns_AttributeName(DNS_CLASS));
                                        /* point to class version attr block */
    cvabp = find_attr(atrbp, (dns_opq_sname *)dns_AttributeName(DNS_CLASSVERSION));

    /* we know what these attribute values look like. */
    /* no need to look at the atomic value structure */    

     init_flagstatus(
	    &flags,			/* ptr to the block to init */
	    envbp->curconf,		/* current confidence */
	    &(envbp->curtout));		/* current timeout */
    if ((!cvabp) || (!cnabp))
    status = 
	dnsCreateObj(			/* create the new object */
	    (dns_full_name_t *)objp,	/* ptr to object name */
	    NULL_DNS_OPQ_SNAME,
	    (dnsVersion_t *)NULL,
	    NULL_DNS_ID,
	    (dns_cts *)NULL,
	    &flags);			/* ptr to in/out flag/status */
    else
    status = 
	dnsCreateObj(			/* create the new object */
	    (dns_full_name_t *)objp,	/* ptr to object name */
	    (dns_simple_name_t *)cnabp->av->vp->av_value,
	    (dnsVersion_t *)cvabp->av->vp->av_value,
	    NULL_DNS_ID,
	    (dns_cts *)NULL,
	    &flags);			/* ptr to in/out flag/status */

    if (status != DNS_SUCCESS) {
      create_err_list_element (objp,
			       &flags,
			       "dnsCreateObj");

    }
    return(status);			/* tell caller what happened */    
}
