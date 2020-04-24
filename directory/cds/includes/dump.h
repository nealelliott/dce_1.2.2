/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dump.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:34  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:16:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:07  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:02:21  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:57:59  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:39:05  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:43  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:03:16  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:15:53  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:31:26  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:50:13  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:16:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _DUMP_H
#define _DUMP_H
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

/* Data structure id's */

#define LONGWORD              1
#define WORD                  2
#define BYTE                  3
#define ABS_DATE              4
#define REL_DATE              5
#define FULL_NAME             6
#define SIMPLE_NAME           7
#define CTS                   8
#define TLOG_STRUCTURE        9
#define DIRECTORY_ENTRY      10
#define TLOG_ENTRY_TYPE      11
#define OPERATION            12
#define CHARACTER            13
#define ATTRIBUTE            14
#define ATTRIBUTE_SET        15
#define ATTRIBUTE_VALUE      16
#define VALUE_FLAGS          17
#define CHECKPOINT_HEADER    18
#define CHECKPOINT_STRUCTURE 19
#define DATA_BUCKET_STRUCTURE 20
#define ENTRY_DATA_RECORD    21
#define UPDATE_PACKET        22
#define UPDATE_STRUCTURE     23
#define ATTRIBUTE_TYPE       24
#define ENTITY_TYPE_WIRE     25
#define ENTITY_TYPE_DB       26
#define MESSAGE_LOG_RECORD   27
#define VALUETYPE            28
#define UID                  29
#define CASEMASK	     30

#define MAX_TYPES            30

/*
 * dump_navigator.c
 */

char *
navigate (
    char  *,
    int    ,
    int    );

/*
 * dump_type_interpreters.c
 */

int
interpret (
    int   ,
    int   ,
    char *,
    int  *,
    char *,
    char *);

#endif  /* #ifndef _DUMP_H */
