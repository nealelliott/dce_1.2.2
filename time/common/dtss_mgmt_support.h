/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtss_mgmt_support.h,v $
 * Revision 1.1.8.2  1996/02/18  23:11:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:36:19  marty]
 *
 * Revision 1.1.8.1  1995/12/08  18:06:49  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/09/26  13:00 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:23:48  root]
 * 
 * Revision 1.1.6.2  1994/08/23  20:31:51  cbrooks
 * 	Code Cleanup Checkpoint
 * 	[1994/08/23  13:56:38  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/30  11:56 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.3.4  1993/10/22  12:34:46  pare
 * 	Added check for header file expansion
 * 	[1993/10/22  12:34:05  pare]
 * 	Revision 1.1.6.1  1994/05/12  15:17:07  rhw
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:25  rhw]
 * 
 * Revision 1.1.4.2  1993/07/14  21:26:27  tom
 * 	Change TOO_MANY defines to TIME_PROVIDER_TIMEOUT.
 * 	[1993/07/14  20:41:43  tom]
 * 
 * Revision 1.1.3.3  1993/09/23  17:02:11  pare
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/23  16:55:49  pare]
 * 
 * Revision 1.1.4.3  1993/09/15  17:46:13  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.3.2  1993/07/15  16:08:51  truitt
 * 	OT7973: Remove support for the 'Too Many Server Detected' command.
 * 	        Add support for the 'Time Provider Timeout Detected' command.
 * 	[1993/07/15  16:08:04  truitt]
 * 
 * Revision 1.1.4.2  1993/07/14  21:26:27  tom
 * 	Change TOO_MANY defines to TIME_PROVIDER_TIMEOUT.
 * 	[1993/07/14  20:41:43  tom]
 * 
 * Revision 1.1.2.2  1992/12/30  20:39:07  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:07:32  htf]
 * 
 * Revision 1.1.1.11  1993/05/04  22:37:41  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1  1992/01/19  15:32:23  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/********************************************************************************************************************************/
/* Created 26-NOV-1991 03:09:03 by VAX SDL V3.2-10     Source: 25-NOV-1991 15:27:29 BLD$BASE:[SRC]DTSS_MGMT_SUPPORT.SDL;3 */
/********************************************************************************************************************************/
/* Copyright (c) 1988,1991 by                                               */
/* Digital Equipment Corporation, Maynard, Massachusetts.                   */
/* All rights reserved.                                                     */

/* This software is furnished under a license and may be used and copied    */
/* only  in  accordance  of  the  terms  of  such  license  and with the    */
/* inclusion of the above copyright notice.  This software or any  other    */
/* copies thereof may not be provided or otherwise made available to any    */
/* other person.  No title to and  ownership of the  software is  hereby    */
/* transferred.                                                             */

/* The information in this software is  subject to change without notice    */
/* and  should  not  be  construed  as a commitment by Digital Equipment    */
/* Corporation.                                                             */

/* Digital assumes no responsibility for the use  or  reliability of its    */
/* software on equipment which is not supplied by Digital.                  */

 
#ifndef _DTSS_MGMT_SUPPORT_H
#define _DTSS_MGMT_SUPPORT_H 1
/*** MODULE TransferBuffer ***/

/* Transfer buffer between service_ncl and dts_emaa                         */


#define K_TRANSBUF_LENGTH 1540          /* Structure size                   */
struct TransBuf {
    unsigned short int transbuf_w_size; /* The number of bytes of data      */
    unsigned short int transbuf_w_type; /* Data type                        */
    char transbuf_t_data [1536];        /* The data block                   */
    } ;
#define MAX_TRANSBUF_SIZE 1536
#define K_TYPE_BINRELTIME 19
#define K_TYPE_UNSIGNED32 116
#define K_TYPE_INTEGER32 111
#define K_TYPE_VERSION 33
#define K_TYPE_BOOLEAN 1
#define K_TYPE_SIMPLENAME 26
#define K_TYPE_FULLNAME 27
#define K_TYPE_BINABSTIME 17
#define K_TYPE_UID 28
#define K_TYPE_COUNTER64 121
#define K_TYPE_ENUMERATION 201
#define K_TYPE_TOWERSET 44
#define K_TYPE_NSCTS 52
#define K_TYPE_SET 8
#define K_TYPE_ID802 34
 
/*** MODULE DTSSNCLDEFS ***/

/* SERVICE STATES                                                           */

#define K_STATE_OFF 0
#define K_STATE_ON 1
#define K_STATE_SYNCH_ADJUST 2          /* synchronizing by adjust          */
#define K_STATE_UPDATING 3
#define K_STATE_SYNCH_SET 4             /* synch by set (non arch, not in msl) */
#define K_STATE_NOT_CREATED 5           /* service not yet created (non arch, not in msl) */
#define K_STATE_COUNT 6

/* Transport/protocol types                                                 */

#define K_PROT_IEEE802 0
#define K_PROT_DECNET 1
#define K_PROT_UDP_IP 2
#define K_PROT_TCP_IP 3
#define K_PROT_RPC 4

/* Backup Courier Role Types                                                */

#define K_BACKUP_ROLE_COURIER 0
#define K_BACKUP_ROLE_NONCOURIER 1

/* Server Set Types                                                         */

#define K_SERVER_LOCAL 0
#define K_SERVER_GLOBAL 1

/* Supported OS types                                                       */

#define K_OPSYS_VMS 0
#define K_OPSYS_ULTRIX 1

/* VARIANT TYPES                                                            */

#define K_VARIANT_SERVER 0
#define K_VARIANT_CLERK 1

/* SERVER ROLES                                                             */

#define K_ROLE_COURIER 0
#define K_ROLE_NONCOURIER 1
#define K_ROLE_BACKUPCOURIER 2
#define K_ROLE_MIN 0
#define K_ROLE_MAX 3

/* DIRECTIVE CODES                                                          */

#define K_CREATE_DIRECTIVE 1
#define K_DELETE_DIRECTIVE 2
#define K_ENABLE_DIRECTIVE 3
#define K_DISABLE_DIRECTIVE 4
#define K_UPDATE_DIRECTIVE 5
#define K_SYNCHRONIZE_DIRECTIVE 6
#define K_CHANGE_DIRECTIVE 7
#define K_ADVERTISE_DIRECTIVE 8

/* Jacket Routine STATUS VALUES                                             */

#define K_NCL_RSP_SUCCESS 1              /* success                          */
#define K_NCL_RSP_NOT_CREATED 2          /* not yet created	- Map to CMIP no such object instance */
#define K_NCL_RSP_EPOCH_DIFF 4           /* epoch not diff       - Map to EXCEPT Same Epoch */
#define K_NCL_RSP_NO_RESOURCES 6         /* no resources		- Map to CMIP no resources avail */
#define K_NCL_RSP_NOT_CONTAINED 8        /* int. not contained	- Map to EXCEPT Not Contained */
#define K_NCL_RSP_ACCESS_DENIED 10       /* no Privs		- Map to CMIP No Access */
#define K_NCL_RSP_NOT_SUPPORTED 12       /* server op only	- Map to CMIP Not Supported */
#define K_NCL_RSP_INV_ARG 14             /* invalid arg value    - Map to CMIP Inv Arg */
#define K_NCL_RSP_FAILURE 16             /* unknown failure,     - Map to CMIP Process Failure */
#define K_NCL_RSP_NO_SUCH_VALUE 18       /* no arg val to ret    - Map to CMIP no resp ready */
#define K_NCL_RSP_CONSTR_VIOL 20         /* constraint violation - Map to CMIP Constraint */
#define K_NCL_RSP_SYSTEM_ERROR 22        /* local sys err	- Map to EXCEPT System Error */
#define K_NCL_RSP_ALREADY_EXISTS 24      /* Already created	- Map to EXCEPT Already Created */
#define K_NCL_RSP_WRONG_STATE 26         /* wrong state		- Map to EXCEPT Wrong State  */
#define K_NCL_RSP_INV_NS 28              /* invalid name space   - Map to DNS INVALIDNS */
#define K_NCL_RSP_INV_GD 30              /* invalid global dir   - MAP to DNS INVALIDGD */
#define K_NCL_RSP_CREATED 32             /* obsolete                         */
#define K_NCL_RSP_ENABLED 34             /* obsolete                         */
#define K_NCL_RSP_NOT_ENABLED 36         /* obsolete                         */
#define K_NCL_RSP_EPOCH_RANGE 38         /* obsolete                         */
#define K_NCL_RSP_MIN_INACC 40           /* obsolete                         */
#define K_NCL_RSP_SYNCHING 42            /* obsolete                         */
#define K_NCL_RSP_UPDATING 44            /* obsolete                         */
#define K_NCL_RSP_UPDATE_SYNCH 46        /* obsolete                         */
#define K_NCL_RSP_NOT_DISABLED 48        /* obsolete                         */
#define K_NCL_RSP_DISABLED 50            /* obsolete                         */
#define K_NCL_RSP_DNS_ERROR 52           /* obsolete                         */
#define K_NCL_RSP_REQ_ARG 54             /* obsolete                         */

/* Execption codes                                                          */

#define K_NCL_EXP_ALREADY_EXISTS 50      /* obj already created              */
#define K_NCL_EXP_WRONG_STATE 51         /* wrong State                      */
#define K_NCL_EXP_SAME_EPOCH 52          /* same epoch                       */
#define K_NCL_EXP_NOT_CONTAINED 53       /* inacc. not contained             */
#define K_NCL_EXP_SYSTEM_ERROR 54        /* OS specific error message        */

/* EVENT NUMBERS                                                            */

#define K_LOCAL_FAULT_EVENT 1
#define K_FAULTY_SERVER_EVENT 2
#define K_DIFFERENT_EPOCH_EVENT 3
#define K_TOO_FEW_EVENT 4
#define K_TIME_PROVIDER_TIMEOUT_EVENT 5
#define K_PROTOCOL_VERSION_EVENT 6
#define K_TIME_REP_EVENT 7
#define K_INVALID_MESSAGE_EVENT 8
#define K_NO_GLOBAL_EVENT 9
#define K_NOT_RESPONDING_EVENT 10
#define K_CLOCK_SET_EVENT 11
#define K_EPOCH_DECLARATION_EVENT 12
#define K_TIME_DIFF_EVENT 13
#define K_SYSTEM_ERROR_EVENT 14
#define K_SYNCH_COMPLETED_EVENT 15
#define K_UPDATE_COMPLETED_EVENT 16
#define K_STARTUP_COMPLETED_EVENT 17
#define K_SHUTDOWN_COMPLETED_EVENT 18
#define K_INSUFFICIENT_EVENT 19
#define K_TIME_PROVIDER_EVENT 20
#define K_LOC_SERV_NOT_TSG_MEMBER_EVENT 21
#define K_SERV_NOT_TSG_MEMBER_EVENT 22

/* The total number of events                                               */

#define K_NUM_EVENTS 22

/* ATTRIBUTES                                                               */

#define K_ADVERTISEMENT_ATTR 0
#define K_CHECK_INTERVAL_ATTR 1
#define K_EPOCH_NUMBER_ATTR 2
#define K_ERROR_TOLLERANCE_ATTR 3
#define K_TIME_DIFF_ATTR 4
#define K_MAXIMUM_INACCURACY_ATTR 5
#define K_SERVERS_REQUIRED_ATTR 6
#define K_QUERY_ATTEMPTS_ATTR 7
#define K_LAN_TIMEOUT_ATTR 8
#define K_WAN_TIMEOUT_ATTR 9
#define K_SYNCH_ATTR 10
#define K_VARIANT_ATTR 11
#define K_SERVER_NAME_ATTR 12
#define K_COURIER_ROLE_ATTR 13
#define K_BACKUP_COURIER_ROLE_ATTR 14   /* 14                               */
#define K_CLOCK_ADJUSTMENT_ATTR 15
#define K_CLOCK_DRIFT_ATTR 16
#define K_DNA_VERSION_ATTR 17
#define K_DECDTS_VERSION_ATTR 18
#define K_TIME_REP_ATTR 19
#define K_SERVICE_TRACE_ATTR 20
#define K_COMM_TRACE_ATTR 21
#define K_SYNCH_TRACE_ATTR 22
#define K_ARITH_TRACE_ATTR 23
#define K_LOCAL_SERVER_ATTR 24          /* 24                               */
#define K_GLOBAL_DIRECTORY_ATTR 25
#define K_TP_PRESENT_ATTR 26
#define K_AUTO_TDF_ATTR 27
#define K_NEXT_TDF_ATTR 28
#define K_CLOCK_RESOLUTION_ATTR 29
#define K_CURRENT_TIME_ATTR 30          /* 30                               */
#define K_UID_ATTR 31
#define K_LAST_SYNCH_ATTR 32
#define K_STATE_ATTR 33
#define K_WAN_SERVICE_ATTR 34
#define K_SERVER_PRINCIPAL_NAME 35
#define K_SERVER_ENTRY_NAME 36
#define K_SERVER_GROUP_NAME 37
#define K_FILL_38_ATTR 38
#define K_FILL_39_ATTR 39
#define K_FILL_40_ATTR 40
#define K_FILL_41_ATTR 41
#define K_FILL_42_ATTR 42
#define K_FILL_43_ATTR 43
#define K_FILL_44_ATTR 44
#define K_FILL_45_ATTR 45
#define K_FILL_46_ATTR 46
#define K_FILL_47_ATTR 47
#define K_FILL_48_ATTR 48               /* COUNTERS                         */
#define K_CREATION_TIME_ATTR 49
#define K_LOCAL_FAULT_ATTR 50
#define K_FAULTY_SERVER_ATTR 51
#define K_DIFFERENT_EPOCH_ATTR 52
#define K_TOO_FEW_ATTR 53
#define K_TIME_PROVIDER_TIMEOUT_ATTR 54
#define K_PROTOCOL_VERSION_ATTR 55
#define K_TIME_REPS_ATTR 56
#define K_INVALID_MESSAGE_ATTR 57
#define K_NO_GLOBAL_ATTR 58
#define K_NOT_RESPONDING_ATTR 59
#define K_CLOCK_SET_ATTR 60
#define K_EPOCH_DECLARATION_ATTR 61
#define K_TIME_DIFFS_ATTR 62
#define K_SYSTEM_ERROR_ATTR 63
#define K_SYNCH_COMPLETED_ATTR 64
#define K_UPDATE_COMPLETED_ATTR 65
#define K_STARTUP_COMPLETED_ATTR 66
#define K_SHUTDOWN_COMPLETED_ATTR 67
#define K_INSUFFICIENT_RESOURCE_ATTR 68
#define K_TIME_PROVIDER_ATTR 69
#define K_LOC_SERV_NOT_TSG_MEMBER_ATTR 70
#define K_SERV_NOT_TSG_MEMBER_ATTR 71
#define K_FIRST_COUNTER 50
#define K_LAST_COUNTER 71
#define K_MAX_ATTR 71
#define K_LOCAL_FAULT_CNTR 0
#define K_FAULTY_SERVER_CNTR 1
#define K_DIFFERENT_EPOCH_CNTR 2
#define K_TOO_FEW_CNTR 3
#define K_TIME_PROVIDER_TIMEOUT_CNTR 4
#define K_PROTOCOL_VERSION_CNTR 5
#define K_TIME_REPS_CNTR 6
#define K_INVALID_MESSAGE_CNTR 7
#define K_NO_GLOBAL_CNTR 8
#define K_NOT_RESPONDING_CNTR 9
#define K_CLOCK_SET_CNTR 10
#define K_EPOCH_DECLARATION_CNTR 11
#define K_TIME_DIFFS_CNTR 12
#define K_SYSTEM_ERROR_CNTR 13
#define K_SYNCH_COMPLETED_CNTR 14
#define K_UPDATE_COMPLETED_CNTR 15
#define K_STARTUP_COMPLETED_CNTR 16
#define K_SHUTDOWN_COMPLETED_CNTR 17
#define K_INSUFFICIENT_RESOURCE_CNTR 18
#define K_TIME_PROVIDER_CNTR 19
#define K_LOC_SERV_NOT_TSG_MEMBER_CNTR 20
#define K_SERV_NOT_TSG_MEMBER_CNTR 21
#define K_MAX_CNTR 21
#define K_NUM_COUNTERS 22

/* Arguments associated with events                                         */

#define EVT_LOCAL_FLT_LOCAL_TIME 0
#define EVT_LOCAL_FLT_COMPUTED_TIME 1
#define EVT_LOCAL_FLT_CLOCK_SET 2
#define EVT_FLT_SERV_SET 0
#define EVT_DIFE_LOCAL_EPOCH 0
#define EVT_DIFE_SERV_SET 1
#define EVT_TFEW_NUM_DETECTED 0
#define EVT_TFEW_NUM_REQUIRED 1
#define EVT_TMANY_NUM_DETECTED 0
#define EVT_TMANY_NUM_ACCEPTED 1
#define EVT_PROTM_SERV_NAME 0
#define EVT_TIMEM_SERV_NAME 0
#define EVT_INVM_SERV_NAME 0
#define EVT_INVM_MESSAGE 1
#define EVT_SERVNR_SERV_NAME 0
#define EVT_SERVNR_LCL_OS 1
#define EVT_SERVNR_ERROR_CODE 2
#define EVT_SERVNR_ERROR_TEXT 3
#define EVT_CLKSET_OLD_TIME 0
#define EVT_CLKSET_NEW_TIME 1
#define EVT_EPOCHC_OLD_EPOCH 0
#define EVT_EPOCHC_NEW_EPOCH 1
#define EVT_SYSERR_LCL_OS 1
#define EVT_SYSERR_ERROR_CODE 2
#define EVT_SYSERR_ERROR_TEXT 3
#define EVT_SYNCH_COMP_TIME 0
#define EVT_SYNCH_CLOCK_ADJ 1
#define EVT_UPD_CURR_TIME 0
#define EVT_UPD_UPDATE_TIME 1
#define EVT_ENABLE_CLOCK_SET 0

/* DECDTS CLASS                                                             */

#define K_DECDTS_CLASS 10

/* Local/Global Server Subordinate entity                                   */

#define K_DECDTS_GS_CLASS 1
#define K_DECDTS_LS_CLASS 2

/* Error class                                                              */

#define K_DECDTS_ERROR_CLASS 1

/* Known Server Attributes                                                  */

#define K_KS_NAME_ATTR 0                /* server identifier (name-address) */
#define K_KS_FULL_NAME_ATTR 1           /* local server fname               */
#define K_KS_LTP_ATTR 25
#define K_KS_LOT_ATTR 26
#define K_KS_LOS_ATTR 27
#define K_KS_LS_ATTR 28
#define K_KS_TRANSPORT_ATTR 50
#define K_KS_UID_ATTR 51
#define K_KS_TIME_ATTR 52
#define K_KS_MAX_ATTR 52
 
/*** MODULE DNSNCLDEFS ***/
/* Keep this until DTSS changes to using DNS_CLERK_CLASS                    */

#define K_DNSA_CLASS 9
/* Class definitions for DECdns Clerk and its sub-entities ('Known Namespace' */
/* and 'Remote Clearinghouse'                                               */

#define DNS_CLERK_CLASS 9
#define DNS_CLERK_KNOWN_NS_CLASS 1
#define DNS_CLERK_REMOTE_CH_CLASS 2

/* arguments returned as response from clerk directives                     */
#define DNS_CLERK_RESP_SUCCESS 0
#define DNS_CLERK_RESP_ENTITY_EXISTS 1
#define DNS_CLERK_RESP_WRONG_STATE 0

/* Other errors                                                             */
#define DNS_CLERK_ACCESS_DENIED 200     /* no privs                         */
#define DNS_CLERK_NOT_SUPPORTED 202     /* unsupported                      */
#define DNS_CLERK_INV_ARG 204           /* invalid argument value           */
#define DNS_CLERK_INV_STATE 206         /* wrong state                      */
#define DNS_CLERK_NOSUCH_ATTR 208       /* non-existent atribute            */
#define DNS_CLERK_FAILURE 210           /* process failure                  */
#define DNS_CLERK_REQ_ARG 212           /* missing a required argument      */
#define DNS_CLERK_ENTITY_NOEXIST 214    /* no such entity                   */
#define DNS_CLERK_INSUFF_RESOURCES 216

/* Clerk directives                                                         */
#define DNS_CLERK_CREATE_CODE 0
#define DNS_CLERK_DELETE_CODE 1
#define DNS_CLERK_ENABLE_CODE 2
#define DNS_CLERK_DISABLE_CODE 3

/* Clerk attributes                                                         */

#define DNS_CLERK_CREATE_TIME 0
#define DNS_CLERK_VERSION 1
#define DNS_CLERK_TIMEOUT 2
#define DNS_CLERK_SOLICIT_HOLD 3
#define DNS_CLERK_DNA_UID 4
#define DNS_CLERK_STATE 5
/* Clerk counters                                                           */
#define DNS_CLERK_ATTR_6 6
#define DNS_CLERK_ATTR_7 7
#define DNS_CLERK_PROTOCOL_ERROR_COUNT 8
#define DNS_CLERK_AUTHENTICATION_COUNT 9
#define DNS_CLERK_READ_OPS_COUNT 10
#define DNS_CLERK_CACHE_HITS_COUNT 11
#define DNS_CLERK_CACHE_BYPASS_COUNT 12
#define DNS_CLERK_WRITE_OPS_COUNT 13
#define DNS_CLERK_MISC_OPS_COUNT 14
#define DNS_CLERK_MAX_ATTR 14
/* DNS CLERK events                                                         */
#define DNS_CLERK_INCOMP_RESP_EVENT 2
/* Default values for DNS CLERK's settable attributes                       */

/* Default solicit holddown time is 15 seconds                              */
#define DNS_CLERK_DEF_SOLICIT_VAL 15

/* Default timeout value is 60 seconds                                      */
#define DNS_CLERK_DEF_TIMEOUT_VAL 60

/* SUBORDINATE ENTITY KNOWN_NAMESPACE                                       */


/* Known Namespace directives                                               */
#define KNOWN_NS_CREATE_CODE 0
#define KNOWN_NS_DELETE_CODE 1

/* Known namespace CREATE directive argument codes                          */
#define KNOWN_NS_CREATE_ARG_NSCTS 0
#define KNOWN_NS_CREATE_ARG_NICK 1

/* Known namespace attributes                                               */
#define KNOWN_NS_CTS 0
#define KNOWN_NS_NICKNAME 1
#define KNOWN_NS_CREATE_TIME 2
#define KNOWN_NS_DEFAULT 3
#define KNOWN_NS_AMBIGUOUS 4
#define KNOWN_NS_ADDRESSES 5
#define KNOWN_NS_EXPLICIT_CREATION 6
#define KNOWN_NS_MAX_ATTR 6

/* SUB-ENTITY REMOTE CLEARINGHOUSE                                          */


/* Remote clearinghouse attributes                                          */
#define REMOTE_CH_CTS 0
#define REMOTE_CH_NAME 1
#define REMOTE_CH_CREATE_TIME 2
/* Remote clearinghouse counters                                            */
#define REMOTE_CH_DNA_UID 3
#define REMOTE_CH_ATTR_4 4
#define REMOTE_CH_ATTR_5 5
#define REMOTE_CH_ATTR_6 6
#define REMOTE_CH_ATTR_7 7
#define REMOTE_CH_ATTR_8 8
#define REMOTE_CH_ATTR_9 9
#define REMOTE_CH_READ_OPS_COUNT 10
#define REMOTE_CH_ATTR_10 11
#define REMOTE_CH_ATTR_11 12
#define REMOTE_CH_WRITE_OPS_COUNT 13
#define REMOTE_CH_MISC_OPS_COUNT 14
#define REMOTE_CH_MAX_ATTR 14

#endif /* _DTSS_MGMT_SUPPORT_H */
