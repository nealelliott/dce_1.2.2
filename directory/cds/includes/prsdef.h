/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: prsdef.h,v $
 * Revision 1.1.6.2  1996/02/18  23:34:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:10  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:17:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:33  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:02:33  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:59:23  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:39:24  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:03  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:03:56  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:18:17  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:33:13  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:52:49  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:15:04  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _PRSDEF_H
#define _PRSDEF_H
/*
 * Library PRS, Module PRSDEF - Prefix file.
 *
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
 * MODULE DESCRIPTION:
 *
 * Library PRS,  Module PRSDEF
 *
 * Prefix definitions for calling PRS functions.
 *
 *
 * Networks & Communications Software Engineering
 *
 */

/*
 * Constants.
 */
#define ACT_SUCCESS  1			/* Action routine succeeded */
#define ACT_ERROR    0			/* Action routine discovered error */
#define ACT_QUIT    -1			/* Action routine had fatal error */
#define ACT_FILE    -2			/* No such action routine */

#define RET_SUCCESS    1		/* Parse completed successfully */
#define RET_ERROR      0		/* Parse found syntax error */
#define RET_EOF       -1		/* PROMPT input returned EOF */
#define RET_QUIT      -2		/* Action routine had fatal error */
#define RET_OPENERROR -3		/* Parse table file open error */
#define RET_IOERROR   -4		/* Parse file I/O error */
#define RET_VERSION   -5		/* Parse file version error */
#define RET_FILE      -6		/* Parse file had illegal contents */
#define RET_SIZE      -7		/* Parse table buffer too small */
/*
 * Structure for defining an action routine array entry.
 */

struct prs;

struct	action
	{
	int (*adr)(struct prs *);	/* Pointer to routine to call */
	char *nam;			/* Pointer to routine name */
	};

/*
 * Structure for calling "pr_command".
 */
struct prs
    {
	char   *tbl;			/* Pointer to parse table buffer */
	unsigned int tbs;		/* Size of parse table buffer */
	unsigned int tbu;		/* Amount of buffer actually used */
	char   *cmd;			/* Pointer to command buffer */
	int	cms;			/* Size of command buffer */
	char   *cur;			/* Pointer to current element in cmd */
	char   *end;			/* Pointer to next char after element */
	char   *nxt;			/* Pointer to next element to check */
	long    num;			/* Value of OCT/DEC/HEX numeric */
	struct  action *act;		/* Pointer to action routine array */
	int	av1;			/* Action routine value 1 */
	int	av2;			/* Action routine value 2 */
	int	av3;			/* Action routine value 3 */
	char *maxcur;			/* the highest address seen */
    };


/*
 * Structure for calling "pf_command".
 */
struct prf
    {
	int     fpt;			/* Index for parse table file */
	char   *cmd;			/* Pointer to command buffer */
	int	cms;			/* Size of command buffer */
	char   *cur;			/* Pointer to current element in cmd */
	char   *end;			/* Pointer to next char after element */
	char   *nxt;			/* Pointer to next element to check */
	long    num;			/* Value of OCT/DEC/HEX numeric */
	struct  action *act;		/* Pointer to action routine array */
	int	av1;			/* Action routine value 1 */
	int	av2;			/* Action routine value 2 */
	int	av3;			/* Action routine value 3 */
    };
#endif  /* #ifndef _PRSDEF_H */
