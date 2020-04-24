/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: parse_command.c,v $
 * Revision 1.1.6.2  1996/02/18  19:30:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:36  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:11:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:56  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:01:14  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:46:08  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:37:35  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:33  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:00:34  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:05:35  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:08:30  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:40:13  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:22:00  devrcs
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
#include <mgmt_com_act_decl.h>

/* this module parses a command after continuation lines have been combined, */
/* comments eliminated, and white space converted to a single blank. */

/* parser action routines are intended by the author of the parser to */
/* use global variables only.  They are called with no parameters. */
/* various tricks are used in this module to reduce the globalness */

/* the following table provides access to parser action routines */
static struct action prs_action[] =
    {
#include <mgmt_com_act_init.h>
    {NULL, NULL}
    };

static struct envblk *ebp;		/* pointer to environment block */
static struct op_blk *obp;		/* pointer to output block */

void
parse_init (				/* call this before using parser */
    struct envblk *p1,			/* ptr to environment      block */
    struct cmdblk *p2,			/* ptr to command(not used)block */
    struct prs    *p3,			/* ptr to parser context   block */
    struct op_blk *p5)			/* ptr to output list head block */
{
    int status;				/* returned by called subroutine */

    ebp = p1;				/* provide access to environment blk */
    p3->act = prs_action;		/* let parser get to action routines */
    obp = p5;				/* let parser get to output list blk */
    status = names_onetime_init();	/* provide space to store names */
    /* need to check status */
}

int
parse_command (
    struct cmdblk *p)
{
    int parse_status;

    parse_status = pr_command(p->prsblk);
    return(parse_status);
}

int
set_func (				/* tell exec_cmd what to do */
    struct prs *p)			/* pointer to parser context block */
{
    get_envblk_ptr()->curcmd->func = p->av1;	/* use value given by parser */
    return(ACT_SUCCESS);
}

/* the following parser action routine is for debugging. */
/* leave it in the shipped product. */
/* to use it, modify the grammar to call qjnop with unique values in av1..3 */
/* set a break point at qjnop */
/* the routine does nothing, thus the nop part of the name. */
/* the qj is unlikely to appear in the .cdt files. */
/* thus, the temporary uses of this routine can easily be removed. */

int
qjnop (					/* qj for easy to find. no operation */
    struct prs *p)			/* ptr to parser context block */
{
    return(ACT_SUCCESS);
}
