/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sets.h,v $
 * Revision 1.1.6.2  1996/02/18  23:34:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:27  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:18:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:46  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:02:42  mccann
 * 	include file cleanup
 * 	[1994/07/19  18:01:05  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:39:32  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:13  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:04:16  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:19:30  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:34:18  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:54:02  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:16:01  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _SETS_H
#define _SETS_H
/*
 * Module sets.h
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
#include <dce/dns_record.h>
#include <dce/attributes.h>
#include <dbdef.h>

/*
 * sets_lib.c
 */

#ifdef DCE_CDS_DEBUG
# define SET_CHECK
#endif /* DCE_CDS_DEBUG */

#ifdef SET_CHECK

void
att_set_check (
    const DBSet_t *const );

#else
# define att_set_check(x)
#endif

dns_status_t
set_timestamp_set (
    db_data_t		   *,
    Timestamp_t		   *,
    attribute_descriptor_t *,
    Update_t		   *,
    int			   *);

dns_status_t
set_member (
    DBSet_t		**,
    AttributeName_t	*,
    unsigned int	,
    Timestamp_t		*,
    byte_t		*,
    int			,
    int			,
    int			,
    int			,
    int			);

DBSetMember_t *
set_valuetest (
    DBSet_t		   *,
    int			   ,
    byte_t		   *,
    int			   ,
    int			   *,
    attribute_descriptor_t *);

int
set_dnstest (
    data_record_t	   *,
    int                    ,
    byte_t                 *,
    attribute_descriptor_t *);

int
set_test (
    data_record_t	*,
    AttributeName_t	*,
    AtomicValue_t       *);

dns_status_t
set_replace_value (
    db_data_t		   *,
    Update_t		   *,
    attribute_descriptor_t *);

dns_status_t
set_insert (
    db_data_t		   *,
    Update_t		   *,
    attribute_descriptor_t *,
    int			   );

#ifdef DCE_CDS_DEBUG

byte_t *
prn_DBSetMember (
    int		,
    char	*,
    byte_t	*);

byte_t *
prn_DBSet (
    int		,
    char	*,
    byte_t	*);

byte_t *
prn_DBSetA (
    int		,
    char	*,
    byte_t	*);

void
pr_DBSetMember (
    long	);

void
pr_DBSet (
    long	);

void
pr_DBSetA (
    long	);

#endif /* DCE_CDS_DEBUG */

#endif  /* #ifndef _SETS_H */
