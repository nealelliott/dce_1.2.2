/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtssmsg_ultrix.h,v $
 * Revision 1.1.7.2  1996/02/18  23:11:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:36:35  marty]
 *
 * Revision 1.1.7.1  1995/12/08  18:07:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:54  root]
 * 
 * Revision 1.1.5.2  1994/05/12  15:17:15  rhw
 * 	continued code cleanup & merging
 * 	[1994/05/09  18:46:26  rhw]
 * 
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:32  rhw]
 * 
 * Revision 1.1.5.1  1994/04/21  22:05:11  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  20:05:18  cbrooks]
 * 
 * Revision 1.1.2.2  1992/12/30  20:39:34  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:08:09  htf]
 * 
 * Revision 1.1  1992/01/19  15:31:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/************************************************************************** */
/**                        COPYRIGHT (C) 1990 BY                          * */
/**             DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.             * */
/**                                                                       **/
/** THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED **/
/** ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE **/
/** INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER **/
/** COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY **/
/** OTHER PERSON.  NO TITLE TO OR  OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY **/
/** TRANSFERRED.                                                           **/
/**                                                                        **/
/** THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE **/
/** AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT **/
/** CORPORATION.                                                           **/
/**                                                                        **/
/** DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS **/
/** SOFTWARE ON EQUIPMENT WHICH IS NOT SUPPLIED BY DIGITAL.                **/
/**                                                                        **/
/****************************************************************************/
/*++                                                                        
 * FACILITY:                                                                
 *                                                                          
 *   DISTRIBUTED TIME SYNCHRONIZATION SERVICE - DTSS                        
 *                                                                          
 * ABSTRACT:                                                                
 *                                                                          
 *   Ultrix Message Facility.                                                
 *   Header file for DTSS Error message numbers as described in             
 *   the Message Text Source File.                                          
 *                                                                          
 * AUTHOR:                                                                  
 *                                                                          
 *   Cesar Cortes                                                           
 *                                                                          
 * CREATION DATE: Jan-1990                                                  
 *                                                                          
 */

#ifndef _DTSSMSG_ULTRIX_H
#define _DTSSMSG_ULTRIX_H
/*
 * Start error message numbers high enough so they don't clash with Ultrix
 * error messages (the global value sys_nerr is currently < 500)
 */
                                                                        
#define DTSS_MESSAGES             1

#define DTSS_NORMAL               501
#define DTSS_SYNCHING             502
#define DTSS_UPDATINGTIME         503
#define DTSS_EPOCHRANGE           504
#define DTSS_HASPROVIDER          505
#define DTSS_ISSTARTED            506
#define DTSS_MINSERVERSRANGE      507
#define DTSS_NOPRIV               508
#define DTSS_NOTAFTERSTART        509
#define DTSS_NOTIMEUPDATE         510
#define DTSS_NOTSERVER            511
#define DTSS_NOTSTARTED           512
#define DTSS_SYNCHOLDRANGE        513
#define DTSS_SYNTAX               514
#define DTSS_INITIPC              515
#define DTSS_ANOTHERDTSS          516
#define DTSS_UNKPROTOCOL          517
#define DTSS_ALLOCVIRT		  518
#define DTSS_ERRCREATE		  519
#define DTSS_ALREADYDIS		  520
#define DTSS_NOTCREATED		  521
#define DTSS_OPENETHER		  522
#define DTSS_OPENDECNET		  523
#define DTSS_SETERROR             524
#define DTSS_FAILED               525
#define DTSS_WRONGSTATE		  526
#define DTSS_RPC_COMFAILURE	  527
#define DTSS_TS_SHUTDOWN	  528
#define DTSS_NOTGLOBALSERVER	  529

#endif /* ifndef _DTSSMSG_ULTRIX_H */

