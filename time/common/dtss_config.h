/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtss_config.h,v $
 * Revision 1.1.6.2  1996/02/18  23:11:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:36:15  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:06:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:43  root]
 * 
 * Revision 1.1.4.2  1994/08/23  20:31:50  cbrooks
 * 	Code Cleanup Checkpoint
 * 	[1994/08/23  13:56:37  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/19  17:52:24  cbrooks]
 * 
 * Revision 1.1.4.1  1994/05/12  15:17:01  rhw
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:18  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  20:38:45  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:07:10  htf]
 * 
 * Revision 1.1  1992/01/19  15:31:39  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 	Module: dtss_config.h
 */
/*
 * Copyright (c) 1990 by
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
 * ABSTRACT:    Top-level configuration file which contains the common header
 *		CONFIG_* parameters. This header file is meant to be included
 *		by every DTSS module.  It should contain nothing but the
 *		definitions of the CONFIG parameters.
 *
 * ENVIRONMENT:	all
 *
 * AUTHORS:	Joe Comuzzi
 *		Distributed Systems Services (DSS) 
 * 
 * DATE:	July 30, 1990
 *	
 */


#ifndef _DTSS_CONFIG_H
#define _DTSS_CONFIG_H 1

/*
 * Define compile time options for DTSS
 */

/*
 * The following 6 configuration parameters refer to transport mechanisms:
 *	Phase V DECnet
 *	Phase IV DECnet
 *	IEEE 802.3 - Data link interface
 *	UDP/IP
 *	RPC
 */

/*
 * 8/22/94 CBrooks - removed all transport dependent ifdefs...
 * we are RPC only 
 */

/*
 * The following 3 configuration parameters refer to control mechanisms:
 *	NCL/EMAA/CML
 *	Control program
 *	Configuration file
 *  8/23/84 Cbrooks
 *  only CONFIG_CP (control program) is supported in DCE 1.1
 *
 */
#define CONFIG_CP	(1)			/* Include CP */

/*
 * The following configuration parameters control runtime tracing:
 */
#define CONFIG_TRACE	(1)			/* Include Tracing */
#define CONFIG_DEBUG	(1)			/* Include Debugging code */

/*
 * The following configuration parameter includes code for performance
 * logging.
 */
#define CONFIG_LOGPT    (0)

/*
 * The following configuration parameter includes code for pthreads.
 */
#define CONFIG_THREADS  (1)

/*
 * The following configuration parameter includes code for DCE security
 */
#define CONFIG_SECURITY (1)

#endif  /* #ifndef _DTSS_CONFIG_H */
