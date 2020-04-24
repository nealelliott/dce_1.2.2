/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: err_list_defs.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:36  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:16:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:09  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:02:22  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:58:01  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:39:06  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:44  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:03:17  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:15:59  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:31:31  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:50:22  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:14:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _ERR_LIST_DEFS_H
#define _ERR_LIST_DEFS_H
/* file err_list_defs.h */
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
/* the err_list holds records of errors that happened */

#include <dce/dnsclerk.h>

/* kind of structure codes defined in kind_of_structure_codes.h */

/* kind of element codes */
#define HEADER_ELEMENT 0
#define ERROR_ELEMENT 1

/* function codes */
#define min_f 0
#define max_f 6
#define other_f 7
#define total_f 8

struct err_blk
    {
    int kind_of_structure_code;
    int kind_of_element;		/* uses codes above */
    struct err_blk *fwd_ptr;		/* to next  element */
    struct err_blk *back_ptr;		/* to prior element */
    int size;				/* of block and following data */
    struct dnsFlagStat flags;      	/* API flags structure */
    char *api_function_name;		/* (API) function that failed */
    unsigned char full_name[1];		/* relevant full name, first char */
    /* the rest of the name immediately follows */
    };

/* end of file err_list_defs.h */
#endif  /* #ifndef _ERR_LIST_DEFS_H */
