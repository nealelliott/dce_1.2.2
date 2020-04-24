/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: address.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:49  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:14:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:42:01  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:01:52  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:55:21  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:38:26  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:49  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:02:02  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:11:08  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:27:56  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:45:08  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:14:45  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module address.h
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990. ALL RIGHTS RESERVED.
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
#ifndef _ADDRESS_H
#define _ADDRESS_H

#include <dce/dns_record.h>

/*
 * address_lib.c
 */
int
address_build (
    NetworkAddress_t	*);

int
null_address_build (
    NetworkAddress_t	*);

int
address_cmp (
    NetworkAddress_t	*,
    NetworkAddress_t	*);

unsigned char *
address_parse (
    NetworkAddress_t	*);

unsigned int
address_get_iv (
    NetworkAddress_t	*,
    Node_t		*);

int
address_build_input (
    Node_t		*,
    NetworkAddress_t	*);

#endif  /* #ifndef _ADDRESS_H */
