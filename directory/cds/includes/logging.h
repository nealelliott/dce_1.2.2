/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: logging.h,v $
 * Revision 1.1.6.2  1996/02/18  23:34:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:00  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:17:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:26  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:02:29  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:58:22  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:39:18  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:57  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:03:46  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:17:29  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:32:35  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:51:57  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:14:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _LOGGING_H
#define _LOGGING_H
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

#define LOGGING_DISABLED 0
#define LOGGING_ENABLED  1

struct logblk
    {
    int			kind_of_structure_code;
    int			version;
    int			mode;		/* logging enabled or disabled */
    int			format;		/* code: prepend, overlay, etc */
    int			numcmd;		/* number of commands logged */
    int			numcl;		/* number of command lines logged */
    int			numopl;		/* number of output lines logged */
    int			numnot;		/* number of commands not logged */
    unsigned char	*defspec;	/* ptr to default log file */
    int			alloc_code;	/* 1 if block is malloced. or 0 */
    int			change;		/* 1..4=change in process. 0=not. */
    struct envblk	*envptr;	/* access to the environment block */
    struct op_blk	*pres;		/* present output info */
    struct op_blk	*next;		/* soon to be true info */
    int			next_mode;	/* mode on next command */
    };

struct logparmblk
    {
    struct logblk *lp;			/* ptr to logging control block */
    struct msgblk *mbp;			/* ptr to message control block */
    struct cmdblk *cbp;			/* ptr to command block */
    };
#endif  /* #ifndef _LOGGING_H */




