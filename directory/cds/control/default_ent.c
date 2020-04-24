/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: default_ent.c,v $
 * Revision 1.1.9.2  1996/02/18  19:27:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:12  marty]
 *
 * Revision 1.1.9.1  1995/12/08  15:07:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:47  root]
 * 
 * Revision 1.1.6.3  1994/08/03  19:00:24  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:38:40  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:36:26  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:08  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  21:57:54  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:57:30  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:02:11  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:32:02  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:44:43  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:20:48  weisman]
 * 
 * Revision 1.1  1992/01/19  15:20:49  devrcs
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
#include <cmdblk.h>
#include <uc_clerk.h>

static dns_opq_fname	def_ent;	/* the full name of the default ent */

static struct entblk    cur_def_ent;	/* the current default entity */

void
init_default_entity(void)
{
    ent_one_onetime(&cur_def_ent, 0);	/* init ent block. not malloced */
    cur_def_ent.name_p = 
	(unsigned char *)&def_ent;	/* hook it to name of ent */
    cur_def_ent.nxt = &cur_def_ent;	/* next is self */
    cur_def_ent.prv = &cur_def_ent;	/* prev is self */
    cur_def_ent.head= &cur_def_ent;	/* head is self */
    return;				/* no value from void routine */
}					/* end of init_default_entity */

int					/*ARGSUSED*/	/* appease saber */
reset_default_entity(
    struct envblk   *envbp,		/* ptr to environment block */
    struct cmdblk   *cmdbp,		/* ptr to current command block */
    struct op_blk   *outbp)		/* ptr to head of output list */
{
    int status;
    int len;				/* convert length, in/out */
    struct dnsFlagStat flags;		/* to pretend this is used */
    static ignore_conf = 0;		/* confidence is not used */
					/*SUPPRESS594*/	/* appease saber */
    struct dnsFlagStat *ignore_flagsp;	/* returned pointer not used */
    static int ignore_timeout = 0;	/* timeout is not used */

    cur_def_ent.kind_of_entity = 
	(int)dnsDir;			/* default is . of current namespace */
    len = sizeof(def_ent);		/* how much room is available */
    status =
      dnsCvtCDSFullToOpq (
	   (unsigned char *)"/.:",          /* root for cds */
	    &def_ent,
	    &len);
    if (status == DNS_SUCCESS)
	{
	cur_def_ent.name_len = len;
	}
    else
	{				/* provide some name, a null one */
	COPY_FullName(NullFullName, &def_ent);
	cur_def_ent.name_len = LEN_FullName(&def_ent);
	ignore_flagsp =			/* create some little white lies */
	    init_flagstatus(
		&flags,
		ignore_conf,
		&ignore_timeout);
	dns_status_clerk(&flags, status);
	create_err_list_element(	/* tell the little white lie */
	    (unsigned char *)&def_ent,	/* name of entity */
	    &flags,			/* where to get more info */
	    "dnsCvtStrFullToOpq");	/* operation that failed */
	};
    return(DNS_SUCCESS);		/* always success */
}

static void
use_default_entity(
    struct entblk *entbp)		/* ptr to head of entity list */
{
    struct entblk *p;			/* new copy of default ent blk */

    p = copyent_new(&cur_def_ent);	/* make a copy of the default entity */
    linkent_after(p, entbp);		/* put the copy on the entity list */
    return;				/* no value from void routine */
}

void
allow_default_entity(
    struct entblk *entbp)		/* ptr to head of entity list */
{
    if (entbp->nxt == entbp)		/* is the list empty ? */
	{				/* yes */
	use_default_entity(entbp);	/* so add the default entity to list */
	}
    return;				/* no value from void routine */
}
