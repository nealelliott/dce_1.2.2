/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: msgblk.h,v $
 * Revision 1.1.8.2  1996/02/18  23:34:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:01  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:17:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:27  root]
 * 
 * Revision 1.1.6.4  1994/08/03  19:02:30  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:58:25  mccann]
 * 
 * Revision 1.1.6.3  1994/06/23  18:30:39  mccann
 * 	sams cleanup drop1
 * 	[1994/06/23  18:08:34  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:39:19  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:58  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  22:03:48  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:17:35  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:32:40  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:52:07  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  22:09:55  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:28:28  weisman]
 * 
 * Revision 1.1  1992/01/19  15:17:01  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _MSGBLK_H
#define _MSGBLK_H
/*
 * msgblk.h
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
 *   Note: msgblk.h contains extensions to the the portable message utility
 *
 */

#include <stdio.h>
#include <dce/rpc.h>
/*
 * Constants for loading into MSG structure.
 */
#define MS_NEXT		NULL	/* In xxx.key, get next record */

#define MS_PRINT	NULL	/* In xxx.buf, print message */

#define MS_EXACT	0	/* In xxx.match, exact match */
#define MS_BEST		1	/* In xxx.match, best match */

#define MS_CONTINUE	0	/* In xxx.rewind, continue in file */
#define MS_REWIND	1	/* In xxx.rewind, rewind the file */

/*
 * Structure for calling "ms_get".
 */
struct msg
    {
	FILE   *fpnt;			/* File pointer for message file */
	char   *buf;			/* Pointer to caller's buffer */
	int	siz;			/* Size of buffer */
	int	match;			/* Match type flag */
	int	rewind;			/* File rewind flag */
	int     flag;			/* Returned flag value */
    };

/*
 * Structure for calling "ms_find".
 */
struct msf
    {
	char   *msgb;			/* Pointer to message file buffer */
	int	size;			/* Buffer size */
	int     used;			/* Returned amount of buffer used */
	int	match;			/* Match type flag */
	char   *buf;			/* Returned pointer to string */
	int     flag;			/* Returned flag value */
    };

#define MESSAGE_BLOCK 534

struct msgblk
    {
    int			kind_of_structure_code;
    int			version;
    int			longest_msg;	/* longest msg in file */
    char		*msgspecp;	/* file spec of msg file */
    struct msf		blk;		/* context block of portable msg util*/
    };


#endif  /* #ifndef _MSGBLK_H */
