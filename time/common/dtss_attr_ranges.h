/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtss_attr_ranges.h,v $
 * Revision 1.1.11.2  1996/02/18  23:11:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:36:13  marty]
 *
 * Revision 1.1.11.1  1995/12/08  18:06:11  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1994/09/26  12:51 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:23:42  root]
 * 
 * Revision 1.1.9.5  1994/08/23  20:31:49  cbrooks
 * 	Code Cleanup Checkpoint
 * 	[1994/08/23  13:56:36  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/19  17:52:23  cbrooks]
 * 
 * Revision 1.1.9.4  1994/08/16  18:12:44  cbrooks
 * 	CR 11494
 * 	[1994/08/09  18:59:51  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/18  14:34 UTC  pare
 * 	Merge changes from Hpdce02
 * 
 * Revision 1.1.9.3  1994/07/28  15:37:21  cbrooks
 * 	CR11089 - change error tolerance to be 5 minutes
 * 	[1994/06/27  17:21:50  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  19:23 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.3.3  1993/09/23  16:52:01  pare
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/23  16:50:54  pare]
 * 
 * Revision 1.1.4.3  1993/09/15  17:46:04  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.3.2  1993/07/15  19:17:09  truitt
 * 	OT7357: Allowing large maximum inaccuracy causes timer overflows
 * 	        and continuous syncs.  Limit maximum inaccuracy to 24 hours.
 * 	[1993/07/15  19:16:09  truitt]
 * 
 * Revision 1.1.6.2  1993/07/14  20:43:03  tom
 * 	Bug 7357: allowing large maximum inacuracy causes timer overflows
 * 	and continuous syncs.   Limit it to something reasonable (24 hrs).
 * 	[1993/04/28  21:35:22  tom]
 * 
 * 	Fix OT 3916 - change version number to V1.0.1
 * 	[1992/06/04  16:25:25  ko]
 * 
 * Revision 1.1.1.11  1993/05/04  22:37:32  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.9.2  1994/05/12  15:17:00  rhw
 * 	continued code cleanup & merging
 * 	[1994/05/09  18:46:22  rhw]
 * 
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:17  rhw]
 * 
 * Revision 1.1.9.1  1994/04/21  22:04:58  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  20:05:09  cbrooks]
 * 
 * Revision 1.1.6.2  1993/07/14  20:43:03  tom
 * 	Bug 7357: allowing large maximum inacuracy causes timer overflows
 * 	and continuous syncs.   Limit it to something reasonable (24 hrs).
 * 	[1993/04/28  21:35:22  tom]
 * 
 * Revision 1.1.4.3  1993/01/19  00:40:59  jwade
 * 	Change maximum number of servers required from 32 to 10.
 * 	[1993/01/18  23:31:16  jwade]
 * 
 * Revision 1.1.4.2  1992/12/30  20:38:40  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:07:04  htf]
 * 
 * Revision 1.1  1992/01/19  15:31:43  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 	Module DTSS_ATTR_RANGES.H
 *	Version X0.6
 */
/*
 * Copyright (c) 1989,1990,1991 by
 * Digital Equipment Corporation, Maynard, Mass.
 *
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 *
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:    This is the generic header file for the DTSS_SERVICE module.
 *
 * ENVIRONMENT:	portable
 *
 * AUTHORS:
 *		Ko Baryiames
 *		Distributed Processing Engineering (DPE)
 *
 * DATE:	December 1989
 *
 */
#ifndef _DTSS_ATTR_RANGES_H
#define _DTSS_ATTR_RANGES_H
#include <dtss_config.h>

/* 
 * CHARACTERISTIC ATTRIBUTES
 *
 *  attribute ranges and default values
 */
/*
 * The average period between advertisements. Default = 90 minutes.
 */
#define K_ADVERTISE_INT_LO 0x92A69C00UL
#define K_ADVERTISE_INT_HI 0x0000000CUL
/*
 * Min and max values for the adver interval.
 * Min = 30 sec, Max = infinite.
 */
#define K_MIN_ADVERTISE_INT_LO (K_100NS_PER_SEC * 30)
#define K_MIN_ADVERTISE_INT_HI (0)
#define K_MAX_ADVERTISE_INT_LO 0xFFFFFFFFUL
#define K_MAX_ADVERTISE_INT_HI 0x7FFFFFFFUL

/*
 * The average interval between checking for faulty servers.
 * Default = 90 min.
 */
#define K_CHECK_INT_LO 0x92A69C00UL
#define K_CHECK_INT_HI 0x0000000CUL
/*
 * The min and max values for the check interval.
 * min = 30 sec, max = infinite 
 */
#define K_MIN_CHECK_INT_LO (K_100NS_PER_SEC * 30)
#define K_MIN_CHECK_INT_HI (0)
#define K_MAX_CHECK_INT_LO 0xFFFFFFFFUL
#define K_MAX_CHECK_INT_HI 0x7FFFFFFFUL

/*
 * The server's initial epoch number.
 */
#define K_EPOCH_NUMBER 	0
/*
 * The range of valid epoch numbers.
 */
#define K_EPOCH_NUMBER_MIN	0
#define K_EPOCH_NUMBER_MAX	255

/*
 * The maximum seperation between the time of a faulty clock and the
 * computed time before chosing to set the clock rather than adjusting
 * it monotonically.
 */
#define K_ERROR_TOLERANCE_LO 0xB2D05E00UL /* default value is 5 minutes */
#define K_ERROR_TOLERANCE_HI 0x00000000UL

/*
 * min and max values for error tol.
 * min = .5 secongs, max = infinite
 */
#define K_MIN_ERROR_TOL_LO (K_100NS_PER_MS * 500)   /* .5 seconds */
#define K_MIN_ERROR_TOL_HI 0
#define K_MAX_ERROR_TOL_LO 0xFFFFFFFFUL
#define K_MAX_ERROR_TOL_HI 0x7FFFFFFFUL

/*                           
 * Time differential label
 */
#define K_TDL ""

/*
 * The name of the directory in DNS containing names of global servers.
 */
#define K_TS_GLOBALS   ".DTSS_GlobalTimeServers"
#define K_TS_LOCALS    ".DTSS_LocalTimeServers"
/*
 * Default server name
 */
#define K_TS_SERVER_NAME "Unknown"               


/*
 * Maximum time to wait for a response to a given request on a LAN.  Default
 * is 5 seconds.
 */
#define K_LAN_TIMEOUT_LO (5 * K_100NS_PER_SEC)
#define K_LAN_TIMEOUT_HI 0x00000000
/* 
 * LAN timeOut range
 */
#define K_MIN_LAN_TIMEOUT_LO (0)  			 /* 0 second */
#define K_MIN_LAN_TIMEOUT_HI (0)
#define K_MAX_LAN_TIMEOUT_LO (K_100NS_PER_SEC * 60)      /* 1 minute */
#define K_MAX_LAN_TIMEOUT_HI 0

/*
 * The inaccuracy to which the entity attempts to keep the clock. Default
 * is 100,000 microseconds.
 */
#define K_MAX_INACC_LO	(100000 * K_100NS_PER_US)
#define K_MAX_INACC_HI	0x00000000      /* the inaccuracy to which the */
					/* entity attempts to keep the */
					/* clock = 100,000 microseconds */
/*
 * Max inaccuracy min = 0, max = 24 hours 
 */
#define K_MIN_MAX_INACC_LO 0
#define K_MIN_MAX_INACC_HI 0
#define K_MAX_MAX_INACC_LO 0x2A69C000UL
#define K_MAX_MAX_INACC_HI 0xC9UL

/*
 * The number of times an entity queries a server before assuming the server
 * is no longer available.
 */
#define K_REPETITIONS	3
#define K_QUERY_ATTEMPTS (K_REPETITIONS)
/* 
 * Range of query attempts.
 */
#define K_MAX_QUERY_ATTEMPTS 10
#define K_MIN_QUERY_ATTEMPTS 1

/*
 * For Clerks, attempt to contact K_LOCAL_CLERKS_GOAL, otherwise use
 * min_servers.
 * Servers always contact min servers or fail the synch.
 * Clerks attempt to contact at least K_LOCAL_CLERKS_GOAL otherwise
 * use min_servers.
 */
#define K_MIN_SERVERS_SERVER  3
#define K_MIN_SERVERS_CLERK   1
#define K_LOCAL_CLERKS_GOAL   3

/* 
 * Range of valid min servers values
 */
#define K_MIN_MIN_SERVERS 1
#define K_MAX_MIN_SERVERS 10
#define K_MIN_SERVERS_MIN (K_MIN_MIN_SERVERS)
#define K_MIN_SERVERS_MAX (K_MAX_MIN_SERVERS)

#define K_MIN_TDF_LO  0x0906B800UL	/* - 13 hours */
#define K_MIN_TDF_HI  0xFFFFFF93UL
#define K_MAX_TDF_LO  0xF6F94800UL        /* + 13 hours */
#define K_MAX_TDF_HI  0x0000006CUL

/*
 * Maximum time to wait for a response to a given request on a WAN.  Default
 * is 15 seconds.
 */
#define K_WAN_TIMEOUT_LO (15 * K_100NS_PER_SEC)
#define K_WAN_TIMEOUT_HI 0x00000000
/*
 * WAN TIMEOUT RANGE 
 */
#define K_MIN_WAN_TIMEOUT_LO (0)
#define K_MIN_WAN_TIMEOUT_HI (0)
#define K_MAX_WAN_TIMEOUT_LO 0x65A0BC00UL		      /* 10 min */
#define K_MAX_WAN_TIMEOUT_HI 0x00000001UL

/*
 * Hold down timer to ensure that the entity does not synch too frequently
 * when it cannot satisfy the inaccuracy constraint.  Default is 2 minutes
 * for a server.
 */
#define K_SYNC_HOLD_SERVER_LO (2 * 60 * K_100NS_PER_SEC)
#define K_SYNC_HOLD_SERVER_HI 0x00000000

/*
 * Hold down timer to ensure that the entity does not synch too frequently
 * when it cannot satisfy the inaccuracy constraint.  Default is 10 minutes
 * for a clerk.
 */
#define K_SYNC_HOLD_CLERK_LO 0x65A0BC00UL
#define K_SYNC_HOLD_CLERK_HI 0x00000001UL

/*
 * Valid range for the hold down timer.
 */
#define K_MIN_SYNC_HOLD_LO ( 30 * K_100NS_PER_SEC )   /* 30 sec */
#define K_MIN_SYNC_HOLD_HI 0
#define K_MAX_SYNC_HOLD_LO 0x2A69C000UL                 /* 1 day */
#define K_MAX_SYNC_HOLD_HI 0x000000C9UL
#define K_SYNC_HOLD_MIN_LO  (K_MIN_SYNC_HOLD_LO)
#define K_SYNC_HOLD_MIN_HI  (K_MIN_SYNC_HOLD_HI)
#define K_SYNC_HOLD_MAX_LO  (K_MAX_SYNC_HOLD_LO)
#define K_SYNC_HOLD_MAX_HI  (K_MAX_SYNC_HOLD_HI)
/*
 * Global server name is initially null.
 */
#define K_SERVERNAME	""

/*
 * Entity is either a server or a clerk.
 */
#define K_VARIANT	VTclerk
/*
 * DNA version  T1.0-3
 */
#define K_DNA_VERSION_SUPPORT 'V'
#define K_DNA_VERSION_MINOR 0
#define K_DNA_VERSION_MAJOR 1
#define K_DNA_VERSION_LEVEL 0

/*
 * DEC DTS Version 1.0.1
 */
#define K_DECDTS_VERSION_SUPPORT 'V'
#define K_DECDTS_VERSION_MINOR 0
#define K_DECDTS_VERSION_MAJOR 1
#define K_DECDTS_VERSION_LEVEL 1

/*
 * Time Representation version V1.0-0
 */
#define K_TIME_VERSION_SUPPORT 'V'
#define K_TIME_VERSION_MINOR 0
#define K_TIME_VERSION_MAJOR 1
#define K_TIME_VERSION_LEVEL 0

#endif	/* #ifndef _DTSS_ATTR_RANGES_H */

