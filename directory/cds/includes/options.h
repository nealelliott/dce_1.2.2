/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: options.h,v $
 * Revision 1.1.6.2  1996/02/18  23:34:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:08  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:17:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:32  root]
 * 
 * Revision 1.1.4.2  1994/06/09  18:39:23  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:02  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:03:55  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:18:11  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:33:09  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:52:38  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:15:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _OPTIONS_H
#define _OPTIONS_H
/* this is file options.h */
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
struct opt_blk
    {
    int full;			    /* true if word FULL seen */
    int formatted;		    /* true if word FORMATTED seen */
    /* expect to add others */
    };
/* end of file options.h */
#endif  /* #ifndef _OPTIONS_H */
