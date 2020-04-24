/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: string_lib.h,v $
 * Revision 1.1.6.2  1996/02/18  23:34:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:29  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:18:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:48  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:02:44  mccann
 * 	include file cleanup
 * 	[1994/07/19  18:01:07  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:39:34  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:15  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:04:22  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:19:47  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:34:31  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:54:16  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:16:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _STRING_LIB_H
#define _STRING_LIB_H
/*
 * MODULE: string_lib.h
 * Version: X02.01
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
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
 * Networks & Communications Software Engineering
 */


#define	STRING_LIB_VERSION_MAJOR	2
#define	STRING_LIB_VERSION_MINOR	1

int
dns_bcmp  (
    char		*,
    char		*,
    int			);

int
dns_bcmp_lexical  (
    char		*,
    char		*,
    int			);

void
dns_bcopy  (
    char		*,
    char		*,
    int			);

void
dns_bzero  (
    char		*,
    int			);

void
dns_lower  (
    unsigned char	*,
    unsigned char	*,
    int			);

void
dns_upper  (
    unsigned char	*,
    unsigned char	*,
    int			);

char *
dns_rindex  (
    char		*,
    int			);	/* Must be int to reflect promotion until
				   the function def. is an ANSI prototype */

int
dns_strcasecmp  (
    char		*,
    char		*);

#endif  /* #ifndef _STRING_LIB_H */
