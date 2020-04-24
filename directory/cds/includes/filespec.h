/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: filespec.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:40  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:16:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:11  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:02:23  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:58:05  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:39:09  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:47  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:03:23  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:16:23  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:31:49  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:50:42  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:14:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _FILESPEC_H
#define _FILESPEC_H
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
struct fsblk				/* file spec block */
    {
    int kind_of_structure_code;
    int version;
    struct fsblk *fwd;			/* reserved for ptr to next */
    struct fsblk *bck;			/* reserved for ptr to prev */
    struct fsblk *top;			/* reserved for ptr to head */
    int memcode;			/* 1 if spec is in alloced memory */
    int size;				/* size of spec in bytes */
    int chars;				/* size of spec in chars */
    unsigned char *fsp;			/* ptr to file spec */
    };
#endif  /* #ifndef _FILESPEC_H */


