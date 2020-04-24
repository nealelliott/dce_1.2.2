/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmdblk.h,v $
 * Revision 1.1.8.2  1996/02/18  23:33:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:07  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:15:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:42:38  root]
 * 
 * Revision 1.1.6.4  1994/08/03  19:02:07  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:55:44  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:38:45  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:15  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/14  14:39:44  peckham
 * 	Split decnet_types into machine dependent cds_basic_types.h
 * 	Removed VMS, UNIX and DOSorOS2 conditionals.
 * 	[1994/04/11  19:57:27  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:02:30  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:12:42  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:28:51  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:46:29  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  22:02:20  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:25:50  weisman]
 * 
 * Revision 1.1  1992/01/19  15:15:23  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _CMDBLK_H
#define _CMDBLK_H
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
 */

#define NOT_EOF (EOF - 1)		/* guaranteed not EOF */

#define MAX_NCL_CMD	22222		/* how long can an ncl command be ? */
#define MAX_CL_LEN	1111		/* how long on one line ? */

#define BATCH		0
#define INTERACTIVE	1

#define OVERLAYIF	1

#define COMMAND_BLOCK	532

#define MORENEEDED 0
#define NOMORENEEDED 1
#define ENDOFCMD 1
#define UNTERMQ 2
#define UNTERMA 3

struct cmdblk
    {
    int			kind_of_structure_code;
    int			version;
    struct cmdblk	*nxtlvl;	/* next level of command block */
    struct cmdblk	*prvlvl;	/* previous level of cmd blk */
    struct envblk	*envptr;	/* back to environment block */
    struct clre		*rclblk;	/* command recall block */
    struct prs		*prsblk;	/* parser context block */
    char		*cmdbuf;	/* full command */
    int			cmdlen;		/* size of full command */
    char		*cipbuf;	/* one line of command */
    int			ciplen;		/* size of one line of command */
    FILE		*ipfile;	/* FILE ptr  of input */
    char		*ipspec;	/* file spec of input */
    char		*pprompt;	/* primary prompt, first cmd line */
    char		*sprompt;	/* secondary prompt, continuation line*/
    int			num_lines;	/* number of lines in the command */
    int			depth;		/* depth of nesting */
    int			mode;		/* interactive or batch */
    int			func;		/* function code */
    };
#endif  /* #ifndef _CMDBLK_H */







