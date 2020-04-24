/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: replica_actions.c,v $
 * Revision 1.1.6.2  1996/02/18  19:30:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:53  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:11:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:06  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:01:22  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:43:59  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:37:47  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:50  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:01:00  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:06:58  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:09:39  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:41:43  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:22:11  devrcs
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
#include <prsdef.h>

static struct entblk repb;		/* the current replica */
static unsigned char reptyp;		/* the replica type */

/* the replica type code is moved to the above field by the routine below. */
/* there is only one such field.  */
/* higher level routines are required to move the code to a safe place, */
/* if necessary, before it is clobbered. */
/* if replica type is optional, be sure the grammar supplies the default */

/* save_reptyp is called with 1 parameter, the reptyp code, */
/* the parameter is provided in the block */

/* the code for kind of entity and the reptyp code are related */
/* this allows us to use the official reptyp codes when parsing reptypes. */
/* thus, the reptyp parsing routines can be reused. */
/* we add the reptyp code to the entity code for a replica, to form */
/* the kind of entity code for the replica entity on the entity list */

unsigned char *
reptyp_pointer (void)			/* tell caller where to find reptyp */
{
    return(&reptyp);
}

int
save_reptype (				/* remember the reptyp by code */
    struct prs *pcbp)			/* ptr to parser context block */
{
    reptyp = pcbp->av1;
    return(ACT_SUCCESS);
}

struct entblk *
repblk_pointer (void)
{
    return(&repb);
}

int					/* ACT_SUCCESS unless problems */
save_rep (				/* remember the replica info */
    struct prs *pcbp)			/* ptr to parser context block */
{
    struct entblk *rbp;			/* ptr to the replica block */
    unsigned char *rtp;			/* ptr to the replica type */
    dns_opq_fname *rnp;			/* ptr to replica clearinghouse name */
    struct entblk *enthp;		/* ptr to head of entity list */

    int return_code;			/* what we tell the caller */
    int status;				/* what various routines tell us */

    rbp = repblk_pointer();		/* find the replica block */
    rtp = reptyp_pointer();		/* find the replica type */
    rnp = (dns_opq_fname *)name_pointer(); /* find the most recent fullname */
    enthp = entitylist_pointer();	/* find the head of the entity list */

    return_code = ACT_ERROR;		/* assume failure */
    ent_one_onetime(rbp, 0);		/* initialize entity block */
    rbp->kind_of_entity =
	dnsRep + reptyp;		/* remember the kind of replica */
    status =				/* the malloc might fail */
	move_name_to_entblk(		/* save the name in the block */
	    (unsigned char *)rnp,	/* ptr to name block */
	    rbp);			/* ptr to replica entity block */
    if (status)				/* was name saved successfully ? */
	{				/* yes, so save rep on ent list */
	status =
	    copyent_before(
		rbp,			/* ptr to the info we want to save */
		enthp);			/* save it on the entity list */
	if (status)
	    return_code = ACT_SUCCESS;	/* everything worked ok */
	};
    return(return_code);
}
    /* in the above routine, as elsewhere, there is a lie. */
    /* we tell the user he made a syntax error when a malloc failed */
    /* we hope that the cleanup between commands will make all well */
