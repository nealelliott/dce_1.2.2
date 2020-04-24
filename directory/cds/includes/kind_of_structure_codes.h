/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: kind_of_structure_codes.h,v $
 * Revision 1.1.6.2  1996/02/18  23:34:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:56  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:16:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:25  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:02:28  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:58:20  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:39:17  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:56  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:03:42  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:17:18  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:32:29  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:51:52  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:14:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _KIND_OF_STRUCTURE_CODES_H
#define _KIND_OF_STRUCTURE_CODES_H
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
/* begin kind_of_structure_codes.h */
/* this file contains the codes that identify various structures */
/* these codes allow validation of operations and easier/better repairs */
/* do not use low numbers */
/* keep this list sorted by the value of the code */
#define NSNAME_BLOCK_CODE	    513
#define NSNAME_BLOCK_LIST_HEAD_CODE 514
#define OP_LIST_ELEMENT_CODE	    515
#define OP_LIST_HEAD_CODE	    516
#define CLH_LIST_CODE		    521
#define SL_LIST_CODE		    522
#define ERR_LIST_CODE		    523
#define AST_BLOCK_CODE		    531
#define MIN_BLK_CODE		    532
#define DIR_BLK_CODE		    533
#define OBJ_BLK_CODE		    534
#define LNK_BLK_CODE		    535
#define REP_BLK_CODE		    536

/* end of kind_of_structure_codes.h */
#endif  /* #ifndef _KIND_OF_STRUCTURE_CODES_H */
