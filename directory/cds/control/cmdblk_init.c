/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmdblk_init.c,v $
 * Revision 1.1.6.2  1996/02/18  19:27:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:58  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:06:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:27  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:00:15  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:36:23  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  16:08:26  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:53:12  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:57:38  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:56:25  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:01:27  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:31:06  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:20:43  devrcs
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
 *
 *
 */

#include <cdscp.h>
#include <cmdblk.h>

	void
cmdblk_init (
	struct cmdblk	*nbp,		/* ptr to cmd block to be filled in */
	struct cmdblk	*obp)		/* ptr to another one, for hints */

    /* mnemonic aids: nbp = new block pointer; obp = old blk pointer */
    {
    if (obp)				/* any hints available ? */
	{				/* copy from the block with hints */
	*nbp = *obp;			/* take every field, then fix up */
	nbp->prvlvl = obp;		/* point to previous block */
	nbp->depth = obp->depth + 1;	/* depth of command file nesting */
	}
    else
	{				/* make reasonable guesses */
	nbp->prvlvl   = nbp;		/* this is the previous block */
	nbp->depth    = 0;		/* depth of command file nesting */
	nbp->envptr   = 0;		/* no environment pointer yet */
	nbp->rclblk   = 0;		/* no cmd recall block yet */
	nbp->prsblk   = 0;		/* no parser block yet */
	nbp->cmdbuf   = 0;		/* no command buffer yet */
	nbp->cmdlen   = 0;		/* no command buffer length yet */
	nbp->cipbuf   = 0;		/* no command input buffer yet */
	nbp->ciplen   = 0;		/* no command input length yet */
	nbp->ipfile   = 0;		/* no command input file yet */
	nbp->ipspec   = 0;		/* no command input spec yet */
	nbp->pprompt  = "NCL> ";	/* primary prompt */
	nbp->sprompt  = "_";		/* secondary prompt */
	nbp->num_lines= 0;		/* no lines of command */
	nbp->depth    = 0;		/* depth of nesting */
	nbp->mode     = INTERACTIVE;	/* not batch */
#ifdef vms
	nbp->keybd_id = 0;
#endif
	};

    nbp->kind_of_structure_code = COMMAND_BLOCK;	/* it is */
    nbp->version = 1;
    nbp->nxtlvl = nbp;			/* this is the next level, 'til then */
    nbp->func  = 0;			/* nature of command not known yet */
    return;				/* no value. this is a void routine */
    }
