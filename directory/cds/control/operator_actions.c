/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: operator_actions.c,v $
 * Revision 1.1.6.2  1996/02/18  19:29:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:34  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:10:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:55  root]
 * 
 * Revision 1.1.4.2  1994/06/09  18:37:34  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:32  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:00:32  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:05:29  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:08:26  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:40:03  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:20:16  devrcs
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

/* this module, operator_actions.c, contains action routines involved */
/* in parsing operators, eg +, -, <= */

#include <cdscp.h>
#include <prsdef.h>

static int relop;		/* code identifying relational operator*/

/* when the parser detects a relational operator it calls save_relop. */
/* save_relop stores the appropriate code in relop. */
/* the action routines for higher level productions retrieve the value. */
/* they call relop_pointer to find the address of relop. */
/* many other action routines save a record, instead of just an int. */
/* for cultural compatibility with them, we return a pointer, too, to int. */
/* the relop value will be available until the next relop is parsed. */
/* no need to initialize relop for typical parsing. */
/* however, you can not look at relop to tell what happened. */

int *
relop_pointer (void)		/* tell where to find the relational op */
    { return (&relop); }

int
save_relop (
    struct prs *p)		/* ptr to parser control block */
{
    relop = p->av1;		/* keep the relational operator code */
    return (ACT_SUCCESS);	/* tell parser relop was recognized */
}

static int levelop;		/* code identifying level operator*/
				/* level operators are <, <= */
/* when the parser detects a level operator it calls save_levelop. */
/* save_levelop stores the appropriate code in levelop. */
/* the action routines for higher level productions retrieve the value. */
/* they call levelop_pointer to find the address of levelop. */
/* many other action routines save a record, instead of just an int. */
/* for cultural compatibility with them, we return a pointer, too, to int. */
/* the levelop value will be available until the next levelop is parsed. */
/* no need to initialize levelop for typical parsing. */
/* however, you can not look at levelop to tell what happened. */

int *
levelop_pointer (void)		/* tell where to find the level operator */
    { return (&levelop); }

int
save_levelop (
    struct prs *p)		/* ptr to parser control block */
{
    levelop = p->av1;		/* keep the level operator code */
    return (ACT_SUCCESS);	/* tell parser levelop was recognized */
}
