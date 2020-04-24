/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: utl_table_mgr.h,v $
 * Revision 1.1.6.2  1996/02/18  23:34:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:56  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:18:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:44:08  root]
 * 
 * Revision 1.1.4.3  1994/06/09  18:39:48  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:30  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/14  14:39:54  peckham
 * 	Split decnet_types into machine dependent cds_basic_types.h
 * 	Removed VMS, UNIX and DOSorOS2 conditionals.
 * 	[1994/04/11  19:58:18  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:04:54  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:21:43  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:52:01  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:56:10  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:15:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _UTL_TABLE_MGR_H
#define _UTL_TABLE_MGR_H
/*
 * Module utl_table_mgr.h
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
 */

typedef void *Hashed_Table;

Hashed_Table 
declare_table(
    int);

int lookup_table(
    Hashed_Table	,
    char		*,
    void		**);

int enter_table(
    Hashed_Table          ,
    char                  *,
    void		*);

int remove_table(
    Hashed_Table          ,
    char                  *,
    void		**);

#endif  /* #ifndef _UTL_TABLE_MGR_H */
