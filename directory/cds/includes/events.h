/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: events.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:38  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:16:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:10  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:02:23  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:58:03  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:39:07  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:44  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:03:19  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:16:04  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:31:36  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:50:28  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:14:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _EVENTS_H
#define _EVENTS_H
/*
 * Module events.h
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
 *
 */
# include <server.h>

#define FN_TYPE -2
#define SN_TYPE -1

/* ARCHITECTURAL EVENTS */
#define INCOMPATIBLE_REQUEST_EVENT    ((352<<6) | 1)
#define INCOMPATIBLE_RESPONSE_EVENT   ((352<<6) | 2)
#define SECURITY_EVENT                ((352<<6) | 3)
#define TREEBROKEN_EVENT              ((352<<6) | 4)
#define ROOTLOST_EVENT                ((352<<6) | 5)
#define CRUCIALREPLICA_EVENT          ((352<<6) | 7)
#define PARENTPOINTER_EVENT           ((352<<6) | 8)
#define CHCREATED_EVENT               ((352<<6) | 9)
#define DATACORRUPTION_EVENT          ((352<<6) | 10)
#define POSSIBLECYCLE_EVENT           ((352<<6) | 11)
#define SKULKFAILED_EVENT             ((352<<6) | 12)
#define CHENTRYGONE                   ((352<<6) | 13)
#define CHENABLED                     ((352<<6) | 14)
#define CHDISABLED                    ((352<<6) | 15)
#define CHDELETED                     ((352<<6) | 16)

 
/* IMPLEMENTATION EVENTS */
#define RMS_EVENT           ((353<<6) | 1)
#define BACK_COMPLETE_EVENT ((353<<6) | 2)
#define PROPFAILED_EVENT    ((353<<6) | 3)
#define BACKTIME_EVENT	    ((353<<6) | 4)
#define CLERK_NOCOM_EVENT   ((353<<6) | 5)
#define RECOVERY_EVENT	    ((353<<6) | 6)
#define ADV_EVENT           ((353<<6) | 20)
#define BUG_EVENT           ((353<<6) | 21)

typedef int		RegisteredEnumeration_t;

/*
 * *event_lib*.c
 */

void
evl_IncompatibleProtocolError (
    nsgbl_ns_t		*,
    VersionNumber_t	*,
    FullName_t		*);

void
evl_AccessDenied (
    FullName_t		*,
    int			,
    user_descriptor_t	*,
    dns_status_t	);

void
evl_SecurityFailure (
    nsgbl_ns_t		*,
    FullName_t		*);

void
evl_BrokenLookupPaths (
    nsgbl_ns_t		*,
    FullName_t		*);

void
evl_CrucialReplica (
    nsgbl_ns_t		*,
    FullName_t		*,
    FullName_t		*);

void
evl_CannotUpdateChildPointer (
    nsgbl_ns_t		*,
    FullName_t		*,
    DNSException_t	);

void
evl_PossibleCycle (
    nsgbl_ns_t		*,
    FullName_t		*);

void
evl_RootLost (
    nsgbl_ch_t		*);

void
evl_SkulkFailed (
    nsgbl_ch_t		*,
    FullName_t		*,
    DNSException_t	);

void
evl_UpgradeNotPossible (
    nsgbl_ch_t		*,
    FullName_t		*,
    DNSException_t	);

void
evl_AttributeMissing (
    nsgbl_ch_t		*,
    FullName_t		*,
    AttributeName_t	*);

void
evl_RingBroken (
    nsgbl_ch_t		*,
    FullName_t		*);

void
evl_WrongState (
    nsgbl_ch_t		*,
    FullName_t		*,
    int			);

#endif  /* #ifndef _EVENTS_H */
