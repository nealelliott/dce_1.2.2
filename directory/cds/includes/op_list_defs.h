/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: op_list_defs.h,v $
 * Revision 1.1.6.2  1996/02/18  23:34:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:07  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:17:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:31  root]
 * 
 * Revision 1.1.4.2  1994/06/09  18:39:22  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:01  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:03:54  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:18:05  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:33:04  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:52:31  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:15:01  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _OP_LIST_DEFS_H
#define _OP_LIST_DEFS_H
/* file op_list_defs.h */
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
/* the op_list allows multiple output files so the work done by */
/* some function can be captured and seen on the screen. */

#ifndef FILE
#include <stdio.h>
#endif

/* kind of structure codes defined in kind_of_structure_codes.h */

/* kind of element codes */
#define HEADER_ELEMENT 0
#define FILE_ELEMENT 1

/* file access codes */
/* assign codes in consecutive order */
/* see valid_access_code() and translate_access() in op_list.c for reasons */
#define MINIMUM_ACCESS_CODE 0
#define UNDEFINED   0
#define SYSTEMFILE  1
#define NEWFILE	    2
#define APPEND	    3
#define MAXIMUM_ACCESS_CODE 3

/* file status codes */
/* assign codes in consecutive order */
/* see valid_status_code() and translate_status() in op_list.c for reasons */
#define MINIMUM_STATUS_CODE 0
/* also uses undefined */
#define FILE_OPEN 1
#define FILE_CLOSED 2
#define MAXIMUM_STATUS_CODE 2

/* file attribute codes */
#define MIN_FILE_ATTR 0
#define UNPAGED 0
#define   PAGED 1
#define MAX_FILE_ATTR 1

#define DEFAULT_WIDTH 80
#define DEFAULT_LINES_PER_PAGE 66
#define DEFAULT_TOP_MARGIN 3
#define DEFAULT_BOTTOM_MARGIN 3
#define DEFAULT_HEADER_ROUTINE simple_header_routine

struct op_blk;

struct f_blk
    {
    int file_attr;			/* uses codes above */
    int width;				/* width of page/screen in chars */
    int lines_per_page;			/* actual, or 0 if unpaged */
    int top_margin;			/* lines to skip on top */
    int bottom_margin;			/* lines to skip on bottom */
    int lines_remaining;		/* counted down by users */
    int page_number;			/* each file can be different */
    void (*header_routine)(struct op_blk *); /* pointer to header routine */
    };

struct op_blk
    {
    int kind_of_structure_code;
    int version;			/* so this can evolve */
    int kind_of_element;		/* uses codes above */
    int memcode;			/* 1 if allocated, 0 if not */
    struct op_blk *fwd_ptr;		/* to next  element */
    struct op_blk *back_ptr;		/* to prior element */
    struct op_blk *head_ptr;		/* to head of list element */
    int file_access_code;		/* uses codes above */
    int file_status;			/* uses codes above */
    FILE *file_ptr;			/* file identity */
    struct f_blk format;		/* formatting info */
    int (*callback)(void *, struct op_blk *, char *);
					/* ptr to function returning int */
    void *callparm;			/* ptr to parm structure */
    int spec_len;			/* length of file name, w/o \0 */
    int spec_alloc;			/* 1 if spec separately malloced */
    char *spec;				/* and spec might immediately follow */
    };

/* end of file op_list_defs.h */
#endif  /* #ifndef _OP_LIST_DEFS_H */
