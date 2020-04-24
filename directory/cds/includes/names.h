/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: names.h,v $
 * Revision 1.1.6.2  1996/02/18  23:34:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:03  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:17:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:29  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:02:31  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:59:17  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:39:20  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:59  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:03:52  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:17:53  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:32:54  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:52:23  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:15:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _NAMES_H
#define _NAMES_H
/*
 * Module: names.h
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
 */
#include <dce/dns_record.h>

/*
 * names_lib.c
 */
int
names_s_compare (
    SimpleName_t	*,
    SimpleName_t	*);

#ifdef LOWER_BUILD
void
names_lower (
    FullName_t		*,
    FullName_t		*);
#endif

void
names_neutralize_case (
    FullName_t		*,
    FullName_t		*);

void
names_s_neutralize_case (
    SimpleName_t	*,
    SimpleName_t	*);

void
names_simple_casemask (
    SimpleName_t	*,
    SimpleNameMask_t	*,
    SimpleName_t	*);

int
names_fullmatch (
    FullName_t		*,
    FullName_t		*);

void
names_new (
    ObjUID_t		*,
    SimpleName_t	*,
    FullName_t		*);

SimpleName_t *
names_pointlast (
    FullName_t		*);

void
names_stripright (
    FullName_t		*,
    FullName_t		*);

void
names_append (
    SimpleName_t	*,
    FullName_t		*);

int
names_count (
    FullName_t		*);

FullName_t *
names_swap (
    FullName_t		*);

int
names_matchwild (
    FullName_t		*,
    FullName_t		*);

#endif  /* #ifndef _NAMES_H */
