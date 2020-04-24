/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: entities_actions_cds.c,v $
 * Revision 1.1.8.2  1996/02/18  19:28:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:49  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:08:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:25  root]
 * 
 * Revision 1.1.6.3  1994/08/03  19:00:45  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:43:27  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:36:47  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:31  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  21:58:55  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:00:40  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:04:45  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:35:29  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/30  20:39:20  m_sawyer
 * 	fix 'with' processing
 * 	[1992/04/30  20:29:18  m_sawyer]
 * 
 * Revision 1.1  1992/01/19  15:21:20  devrcs
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

/* parser action routines for entities */

#include <cdscp.h>
#include <prsdef.h>
#include <filespec.h>

#define NOTYET return(ACT_ERROR);

unsigned char *
name_pointer (void);			/* to find where names will be */
struct entblk *
entitylist_pointer (void);		/* to find head of entity list */
struct fsblk *
get_fsblk_ptr(void);

/* there is an entity block for each kind of entity and subentity */
/* there is only one of each kind. */
/* higher level routines have to remove information from the block promptly. */

static struct entblk entb;		/* entity block */
static struct entblk filb;		/* file entity block */
static struct entblk dirb;		/* directory block */

/* these routines assume one location for an opaque full name. */
/* following pointer must be filled in early. */
static unsigned char *np;		/* ptr to opaque full name */

struct entblk *
entblk_pointer (void)
{
    return(&entb);			/* point to the entity block */
}

void
ent_one_onetime (
	struct entblk	*ebp,		/* ptr to entity block */
	int		alloc_code)	/* true if block is in malloced store */
{
    ebp->kind_of_structure_code = ENTITY_BLOCK;
    ebp->version = 1;
    ebp->nxt = ebp->prv = ebp->head = ebp;	/* not linked in yet */
    ebp->blk_alloc = alloc_code;	/* caller tells us if malloced */
    ebp->uid_present = 0;		/* no uid known yet */
    ebp->cts_present = 0;		/* no cts known yet */
    COPY_Timestamp(NullTimestamp, &ebp->cts);	/* look like no cts */
    ebp->kind_of_entity = 0;		/* not known yet */
    ebp->name_len = 0;			/* no name yet */
    ebp->name_p = 0;			/* no name yet */
    ebp->name_alloc = 0;		/* so not allocated */
    ebp->wild = 0;			/* not yet a wildcarded name */
}					/* only name_p and _alloc important */

void
ent_onetime (void)
    {
    np = name_pointer();
    ent_one_onetime(&entb, 0);
    return;				/* no value from void routine */
    }

int
move_name_to_entblk (
	unsigned char	*np,		/* ptr to opaque full name */
	struct entblk	*ebp)		/* ptr to entity block */
{
    if (ebp->name_alloc != 0)		/* is space in use now? */
	{				/* yes */
	free(ebp->name_p);		/* give up old use */
	}				/* in ansi std lib, void free() */
    ebp->name_len =
	dnsGetOpqFullLen((dns_opq_fname *)np);	/* get size of the name */
    ebp->name_p = 
	(unsigned char *)malloc(ebp->name_len);	/* get space to store name */
    if (ebp->name_p != 0)		/* did malloc work ok? */
	{				/* yes */
	ebp->name_alloc = 1;		/* space for name is allocated */
	memcpy(ebp->name_p, np, ebp->name_len);	/* move name */
	}
    return(ebp->name_p ? 1 : 0);	/* let caller know if it worked */
}					/* end of move_name_to_entblk */

int
entity (
	struct prs	*ptp)		/* ptr to parse table */
    /* important action */
{
    struct entblk *entlistp;		/* ptr to head of entity list */
    int status;				/* returned by copyent_before */

    entb.wild = ptp->av1;               /* wildcards allowed or not */
    move_name_to_entblk(np, &entb);

    /* If wildcards were allowed in last simplename, copy it to wildname */
    if (entb.wild)
      COPY_SimpleName(wild_sname_pointer(), &entb.wildname);

    entlistp = entitylist_pointer();	/* find head of entity list */
    status = copyent_before(&entb, entlistp);	/* add this to ent list */
    return(status ? ACT_SUCCESS : ACT_QUIT);
}

int
objent (
	struct prs	*ptp)		/* ptr to parse table */
{
    entb.kind_of_entity = (int)dnsObj;
    return(ACT_SUCCESS);		
}

int
grpent (
	struct prs	*ptp)		/* ptr to parse table */
{
    entb.kind_of_entity = (int)refhdr_grpent;	
    return(ACT_SUCCESS);		
}

int
slent (
	struct prs	*ptp)		/* ptr to parse table */
{
    entb.kind_of_entity = (int)dnsSLink;
    return(ACT_SUCCESS);	
}

int
dirent (
	struct prs	*ptp)		/* ptr to parse table */
{
    entb.kind_of_entity = (int)dnsDir;
    return(ACT_SUCCESS);		
}

int
cellent (
	struct prs	*ptp)		/* ptr to parse table */
{
    entb.kind_of_entity = (int)cellentity;
    return(ACT_SUCCESS);		
}

int
clhent (
	struct prs	*ptp)		/* ptr to parse table */
{
    entb.kind_of_entity = (int)dnsCH;	
    return(ACT_SUCCESS);		
}

int
kident (
	struct prs	*ptp)		/* ptr to parse table */
{
    entb.kind_of_entity = (int)dnsChildPtr;
    return(ACT_SUCCESS);	
}

int
repent (
	struct prs	*ptp)		/* ptr to parse table */
{
    entb.kind_of_entity = (int)dnsRep;
    return(ACT_SUCCESS);		
}

int				/* always ACT_SUCCESS */
clear_ent_list(
    struct prs *ptp)			/* ptr to parse table */
{	/* get rid of entities that might be there from failed subparses */
    delete_ent_list(entitylist_pointer());
    return(ACT_SUCCESS);
}

/************************************************************************/

int
exclude(
    struct prs *ptp)			/* ptr to parse table */
{					/*SUPPRESS594*/	/* appease saber */
    int status;				/* ignored status */
    unsigned char *np;			/* ptr to opaque full name */

    np = (unsigned char *)name_pointer(); /* find hidey-hole for opaque name */
    ZERO_FullName(np);
    move_name_to_entblk(np, &filb);
    status = 
	move_entblk_to_entblk(		/* copy file block to entity block */
	    &filb,			/* ptr to the file entity blk */
	    &entb);			/* ptr to the entity block */
    entb.wild = filb.wild;		/* copy the wildcard info */
    entb.kind_of_entity = ptp->av1;	/* remember what kind entity it is */
    entity(ptp);			/* attach it to the entity list */
    return(ACT_SUCCESS);		/* tell parser all is ok */ 
}

/************************************************************************/
/* User specified a file name on one of the 'subtree' commands.
 * There is no clean way to save a unix filename in cds opaque format
 * for insertion on the "entity list", so just save the file name in
 * a global buffer, for later
*/

char subtree_filename1[_DNS_MAXPATHLEN+1];
char subtree_filename2[_DNS_MAXPATHLEN+1];

int
filename1(
    struct prs *ptp)			/* ptr to parse table */
{
    strcpy(subtree_filename1, (char *)get_fsblk_ptr()->fsp);
    return(ACT_SUCCESS);		/* tell parser all is ok */ 
}

int
filename2(
    struct prs *ptp)			/* ptr to parse table */
{
    strcpy(subtree_filename2, (char *)get_fsblk_ptr()->fsp);
    return(ACT_SUCCESS);		/* tell parser all is ok */ 
}

/************************************************************************/

int				/*ARGSUSED*/	/* appease saber */
wilddir1ent (
	struct prs	*ptp)		/* ptr to parse table */
    /* DIRECTORY <nsnwildfullname> */
{
    move_name_to_entblk(np, &dirb);
    dirb.wild = 1;			/* wildcard in name */
    dirb.kind_of_entity = (int)dnsDir;	/* remember it is a directory */
    return(ACT_SUCCESS);
}

int
move_entblk_to_entblk(
    struct entblk *from,		/* ptr to from entity block */
    struct entblk *to)			/* ptr to  to  entity block */
    /* directory, object, or other kind of entity to general entity */
{
    int status;

    status = 
	move_name_to_entblk(		/* copy name, len, alloc info */
	    from->name_p,		/* the ptr to name in the from blk */
	    to);			/* ptr to the to entity block */
    to->wild = from->wild;		/* copy the wildcard info */

    return(status ? ACT_SUCCESS : 0);	/* all ok, or malloc failed */ 
}
