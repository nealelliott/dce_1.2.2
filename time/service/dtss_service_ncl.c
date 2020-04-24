/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtss_service_ncl.c,v $
 * Revision 1.1.11.2  1996/02/17  23:35:37  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:50  marty]
 *
 * Revision 1.1.11.1  1995/12/08  18:13:53  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1994/09/26  19:35 UTC  pare
 * 	Merge bl18 changes
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/18  13:54 UTC  pare
 * 	Merge changes from Hpdce02
 * 	[1995/12/08  17:25:53  root]
 * 
 * Revision 1.1.9.10  1994/09/21  22:01:42  tom
 * 	Use new macro in dce64.h in CopyInt64toBuff to help 64 bit platforms.
 * 	[1994/09/21  21:14:24  tom]
 * 
 * Revision 1.1.9.9  1994/09/20  20:28:57  tom
 * 	Bug 12271 - Make sure the bits64 counters get passed
 * 	in correct idl_uhyper_int format.  Modify CopyInt64toBuff
 * 	macro to explicitly copy high and low components.
 * 	[1994/09/20  20:22:01  tom]
 * 
 * Revision 1.1.9.8  1994/08/23  20:32:32  cbrooks
 * 	Code Cleanup Checkpoint
 * 	[1994/08/23  13:57:07  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/19  17:53:06  cbrooks]
 * 
 * Revision 1.1.9.7  1994/08/16  18:13:06  cbrooks
 * 	More 64bit changes for 32bit machines
 * 	[1994/08/15  18:52:21  cbrooks]
 * 
 * 	CR 11494 - 64 bit changes
 * 	[1994/08/09  19:11:10  cbrooks]
 * 
 * Revision 1.1.9.6  1994/08/05  20:47:43  cbrooks
 * 	CR11240 - add dced config functionality
 * 	[1994/08/05  20:23:44  cbrooks]
 * 
 * 	CR11240 - dced config enhancements
 * 	[1994/08/03  14:57:04  cbrooks]
 * 
 * Revision 1.1.9.6  1994/08/05  20:47:43  cbrooks
 * 	CR11240 - add dced config functionality
 * 	[1994/08/05  20:23:44  cbrooks]
 * 
 * 	CR11240 - dced config enhancements
 * 	[1994/08/03  14:57:04  cbrooks]
 * 
 * Revision 1.1.9.5  1994/06/30  19:45:27  cbrooks
 * 	CR11112 - new serviceability messages required
 * 	[1994/06/28  17:48:45  cbrooks]
 * 
 * Revision 1.1.9.4  1994/06/24  19:34:04  cbrooks
 * 	CR10792 - remove prototype macro
 * 	[1994/06/23  16:27:19  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  15:24 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.7.4  1993/09/18  12:39:15  pare
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/18  12:38:41  pare]
 * 
 * Revision 1.1.8.3  1993/09/15  17:48:40  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.7.3  1993/07/15  16:15:58  truitt
 * 	OT7973: Remove support for the 'Too Many Server Detected' command.
 * 	        Add support for the 'Time Provider Timeout Detected' command.
 * 	[1993/07/15  16:15:32  truitt]
 * 
 * Revision 1.1.9.3  1994/05/16  18:51:51  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:24  rhw]
 * 
 * Revision 1.1.9.2  1994/04/21  22:06:06  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  19:59:51  cbrooks]
 * 
 * 	Checkpoint:4.15.94
 * 	[1994/04/15  21:14:59  cbrooks]
 * 
 * Revision 1.1.9.1  1994/01/25  17:16:43  rsalz
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/25  17:15:42  rsalz]
 * 
 * Revision 1.1.7.2  1993/07/14  21:26:48  tom
 * 	Change too many servers counter to time provider timeout counter.
 * 	[1993/07/14  20:44:59  tom]
 * 
 * Revision 1.1.4.6  1993/01/28  22:38:19  tom
 * 	Revision 1.1.7.2  1993/07/08  20:20:08  root
 * 	Initial King Kong branch
 * 	[1993/07/08  20:19:29  root]
 * 
 * Revision 1.1.5.2  1993/04/05  17:13:19  truitt
 * 	Recreate GAMERA branch.
 * 	[1993/04/05  17:12:31  truitt]
 * 
 * Revision 1.1.5.2  1993/03/19  15:22:37  truitt
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/19  15:21:48  truitt]
 * 
 * 	Bug 4806 - show * trace commands returned error, remove code that
 * 	returns error if flags not set.
 * 	Use correct macro to copy uuid instead of doing it manually. (4807)
 * 	[1993/01/28  22:37:33  tom]
 * 
 * Revision 1.1.4.5  1993/01/22  15:48:08  jwade
 * 
 * 	Allow servers to advertise even while synchronizing.  This is not
 * 	harmfull and there is sometimes a large delay in the synch phase
 * 	if you are in a large cell with a large amount of servers.
 * 	[1993/01/19  20:59:42  jwade]
 * 
 * 	Allow a delete even though the type of service has not been
 * 	created yet.
 * 	[1993/01/19  20:33:46  jwade]
 * 
 * 		This fixes an error in the OT5050 fix.  Both calls to the
 * 	CreateSecurityCntxt call were removed, but the one executed during the
 * 	"set server principal name" flow should have stayed.
 * 	[1993/01/07  22:29:14  yankes]
 * 
 * 	This file change goes along with the change to transport_rpc.c to fix OT5050.
 * 	The call to CreateSecurityCntxt was in this file, but was moved to the other.
 * 	[1993/01/04  19:14:43  yankes]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  22:21:13  htf]
 * 
 * Revision 1.1.1.8  1993/03/11  22:35:07  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.3.2  1993/01/08  15:07:53  smythe
 * 	fixed dtsd authentication bug by moving CreateSecurityCntxt call
 * 	from dtss_service_ncl.c to transport_rpc.c (OSF OT 5050 and 6763)
 * 	[1993/01/08  15:04:30  smythe]
 * 
 * Revision 1.1.2.3  1992/05/13  19:43:29  ko
 * 	Fix ot 2818,2815 - service responding to invalid attributes,
 * 	Validate time passed in by change directive, do not set time to wild
 * 	values.
 * 	[1992/05/13  19:31:05  ko]
 * 
 * Revision 1.1.1.2  1992/05/07  15:47:08  ko
 * 	fix ot 2150,3117,2820, use LogDCEevent,support TDF shows,fix advert. int error
 * 
 * Revision 1.1  1992/01/19  15:34:36  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * 	Module DTSS_SERVICE_NCL.c
 *	Version X0.6
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
 * ABSTRACT:	This module provides the necessary agent support for
 *		the NCL interface to DTSS.
 *
 * ENVIRONMENT:	Portable
 *
 * AUTHORS:
 *		Author K.P. Baryiames
 *		Distributed Systems Services (DSS)
 *
 * DATE:	December 15, 1989
 *
 */

#include <dts.h>
#include <dtss_includes.h>
#include <dtss_service.h>
#include <dce/dce.h>
#include <dcedtssvc.h>
#include <dcedtsmsg.h>
#include <dcedtsmac.h>

#include <sys/time.h>



/* internal prototypes */
static unsigned long StartCmd(unsigned long clockSet);
static void SetTrace(unsigned long traceBits,
				  unsigned long defaultValue,
				  unsigned long set );

static int IsValidTime(utc_t *atime);

/*
 * Validation Functions 
 */

int ValidateCheckInterval( void *) ;
int ValidateQueryAttempts( void *);
int ValidateMinServers( void * );
int ValidateCourierRole( void *);
int ValidateTolerance( void *) ;
int ValidateGlobalTimeout( void *);
int ValidateLocalTimeout( void *) ;
int ValidateMaxInaccuracy( void *) ;
int ValidateSyncInterval( void * ) ;

/* Macros */

/*
 * Check provileges.  For ultrix, if UID is 0, then
 * provileges are enabled.
 */
#define PrivedUser( callerID ) \
        ( (callerID) == 0 )

/*
 * Define global variable to be shared between the routines DirectiveDecalre
 * and Synchronize.
 */

int newEpochNoTime = 0;

/*
 * Is the Relative time within the given rage
 */
#define IsInUTCrange( utcPtr, loPtr, hiPtr )	\
	( (IsUTCge( (utcPtr), (loPtr) )) && (IsUTCle( (utcPtr), (hiPtr) )))
#define IsInIntRange(intValue, loInt, hiInt )  \
        ( ((intValue) >= (loInt) ) && ((intValue) <= (hiInt)) )
/*
 * Transfer buffer manipulation macros
 */
#define CopyInt32toBuff( TransBuffAdr, Decimal, Dtype ) 	\
{                                                               \
 int _dec_temp_dec;						\
        (TransBuffAdr)->transbuf_w_size = 4;			\
        (TransBuffAdr)->transbuf_w_type = Dtype;	       	\
	_dec_temp_dec = (int)Decimal;                           \
	(void)memcpy((VOID *)(TransBuffAdr)->transbuf_t_data,   \
		(VOID *)&(_dec_temp_dec), 4);                   \
}

#define CopyInt64toBuff( TransBuffAdr, Decimal, Dtype )         \
{								\
  Bits64 *_u64;							\
  idl_uhyper_int *_h;						\
	(TransBuffAdr)->transbuf_w_size = 8;                    \
	(TransBuffAdr)->transbuf_w_type = Dtype;                \
	_h = (idl_uhyper_int *)((TransBuffAdr)->transbuf_t_data);\
	_u64 = (Bits64 *)(&(Decimal));				\
	U64_TO_HYPER((*_h), (*_u64));				\
}	

#define CopyStatusBlk( statusBlkPtr, statusBlk)                 \
        (statusBlkPtr)->status = statusBlk.status;              \
        (statusBlkPtr)->sysStatus = statusBlk.sysStatus;

#define CopyUTCrelToBuff( TransBuffAdr, CvtUTC, Dtype )		   \
    {								   \
    utc_t TempUTC;                                                 \
                                                                   \
	utc_mkcomptime(&TempUTC, &(CvtUTC), (UTCValue *)NULL, 0L); \
	(TransBuffAdr)->transbuf_w_size = sizeof(utc_t);           \
	(TransBuffAdr)->transbuf_w_type = Dtype;                   \
	memcpy( (VOID *)(TransBuffAdr)->transbuf_t_data,           \
		(VOID *)&TempUTC, sizeof(utc_t) ); 	           \
    }

#define CopyBinAbsTimeToBuff( TransBuffAdr, bintime, Dtype)     \
    (TransBuffAdr)->transbuf_w_size = 16;			\
    (TransBuffAdr)->transbuf_w_type = Dtype;			\
    memcpy( (VOID *)(TransBuffAdr)->transbuf_t_data, 		\
	    (VOID *)&(bintime), sizeof(utc_t) );

#define CopyFullNameToBuff( TransBuffAdr, fullNameStr, Dtype )	\
                                                                \
    (TransBuffAdr)->transbuf_w_size = 				\
	dnsGetOpqFullLen((dns_opq_fname *) (fullNameStr));	\
    (TransBuffAdr)->transbuf_w_type = Dtype;	                \
    if ((TransBuffAdr)->transbuf_w_size >= 511)                 \
    {                                                           \
	(TransBuffAdr)->transbuf_w_size = 0;                    \
    }                                                           \
    else                                                        \
    {                                                           \
	memcpy( (VOID *)(TransBuffAdr)->transbuf_t_data,        \
		(VOID *)(fullNameStr),				\
		(TransBuffAdr)->transbuf_w_size ); 		\
    }

#define CopySimpleNameToBuff( TransBuffAdr, simpleNameStr, Dtype) \
								\
    (TransBuffAdr)->transbuf_w_size = 				\
	dnsGetOpqSimpleLen((dns_opq_sname *) (simpleNameStr));	\
    (TransBuffAdr)->transbuf_w_type = Dtype;	                \
    if ((TransBuffAdr)->transbuf_w_size >= 511)                 \
    {                                                           \
	(TransBuffAdr)->transbuf_w_size = 0;                    \
    }                                                           \
    else                                                        \
    {                                                           \
	memcpy( (VOID *)(TransBuffAdr)->transbuf_t_data,        \
		(VOID *)(simpleNameStr),			\
		(TransBuffAdr)->transbuf_w_size ); 		\
    }

#define CopyVersionToBuff( TransBuffAdr, versionno, Dtype )     \
    {                                           		\
    char retversion[4];						\
                                                                \
    retversion[0] = versionno.dtss_support;                     \
    retversion[1] = versionno.dtss_major;                       \
    retversion[2] = versionno.dtss_minor;                       \
    retversion[3] = versionno.dtss_level;                       \
    (TransBuffAdr)->transbuf_w_size = 4;                        \
    (TransBuffAdr)->transbuf_w_type = Dtype;                    \
    memcpy( (VOID *)(TransBuffAdr)->transbuf_t_data, 		\
	    (VOID *)retversion, 4 );    			\
    }

#define CopyUIDtoBuff( TransBuffAdr, uidbuffer, Dtype )		\
    (TransBuffAdr)->transbuf_w_size = sizeof(uuid_t);           \
    (TransBuffAdr)->transbuf_w_type = Dtype;                    \
    memcpy((VOID *)(TransBuffAdr)->transbuf_t_data,             \
	   (VOID *)uidbuffer, sizeof(uuid_t));

#define CopyStringToBuff( TransBuffAdr, string, Dtype )	        \
{                                                               \
    int size = strlen(string);                                  \
    (TransBuffAdr)->transbuf_w_size = size;                     \
    (TransBuffAdr)->transbuf_w_type = Dtype;                    \
    memcpy((VOID *)(TransBuffAdr)->transbuf_t_data,             \
	   (VOID *)string, (size_t)size);                       \
}

#define CopyBuffToInt( dstinteger, setBuffer, statusVal)\
    if (((int)(setBuffer)->transbuf_w_size) > 4 || 		\
	((int)(setBuffer)->transbuf_w_size) < 1   )            \
	    statusVal = 0;                              \
    else                                                \
    {                                                   \
        dstinteger = 0;                                 \
	memcpy( (VOID *)&(dstinteger),                  \
	        (VOID *)(setBuffer)->transbuf_t_data,   \
	        (setBuffer)->transbuf_w_size);		\
	statusVal = 1;                                  \
    }

#define CopyBuffToBinAbsTime( binTime, setBuffer, statusVal) \
    if ((setBuffer)->transbuf_w_size != sizeof(utc_t))       \
	statusVal = 0;                                       \
    else                                                     \
    {              					     \
	memcpy((VOID *)&binTime,                             \
	       (VOID *)(setBuffer)->transbuf_t_data,         \
	       sizeof(struct utc));                          \
	statusVal = 1;                                       \
    }

#define CopyBuffToSimpleRelTime( binTime, setBuffer, statusVal) \
    {								\
	if ((setBuffer)->transbuf_w_size != sizeof(utc_t)) \
		statusVal = 0;                                  \
	else                                                    \
	{              					     	\
	    (void) utc_comptime(&binTime, (UTCValue *)NULL, 	\
				(long *)NULL,     		\
		  (utc_t *)(setBuffer)->transbuf_t_data);	\
	    statusVal = 1;                                      \
	}							\
    }


/* externs */
extern ShmBuf *shmPtr;
extern Sdb    sdb;
extern RpcBuf *rpcBufPtr;

static int defCourierRole= K_ROLE_BACKUPCOURIER;
static int maxCourierRole= K_ROLE_MAX;
static int minCourierRole= K_ROLE_MIN;

static UTCValue minCheckInt = UTCinit( K_MIN_CHECK_INT_LO,
					K_MIN_CHECK_INT_HI );
static UTCValue maxCheckInt = UTCinit( K_MAX_CHECK_INT_LO,
					K_MAX_CHECK_INT_HI );
static UTCValue defCheckInt = UTCinit( K_CHECK_INT_LO,
					K_CHECK_INT_HI );

static ExtInacc minErrorTol = UTCinit( K_MIN_ERROR_TOL_LO,
					K_MIN_ERROR_TOL_HI );
static ExtInacc maxErrorTol = UTCinit( K_MAX_ERROR_TOL_LO,
					K_MAX_ERROR_TOL_HI );
static ExtInacc defErrorTol = UTCinit( K_ERROR_TOLERANCE_LO,
					K_ERROR_TOLERANCE_HI );

static ExtInacc minMaxInacc = UTCinit( K_MIN_MAX_INACC_LO,
					K_MIN_MAX_INACC_HI );
static ExtInacc maxMaxInacc = UTCinit( K_MAX_MAX_INACC_LO,
					K_MAX_MAX_INACC_HI );
static ExtInacc defMaxInacc = UTCinit( K_MAX_INACC_LO,
					K_MAX_INACC_HI );

static int minMinServers= K_MIN_MIN_SERVERS;
static int maxMinServers= K_MAX_MIN_SERVERS;
static int defMinServers_server= K_MIN_SERVERS_SERVER;
static int defMinServers_clerk = K_MIN_SERVERS_CLERK;

static int minQueryAtt= K_MIN_QUERY_ATTEMPTS;
static int maxQueryAtt= K_MAX_QUERY_ATTEMPTS;
static int defQueryAtt= K_QUERY_ATTEMPTS;

static UTCValue minLANtimeOut= UTCinit( K_MIN_LAN_TIMEOUT_LO,
					K_MIN_LAN_TIMEOUT_HI );
static UTCValue maxLANtimeOut= UTCinit( K_MAX_LAN_TIMEOUT_LO,
					K_MAX_LAN_TIMEOUT_HI );
static UTCValue defLANtimeOut= UTCinit( K_LAN_TIMEOUT_LO,
					K_LAN_TIMEOUT_HI );

static UTCValue minWANtimeOut= UTCinit( K_MIN_WAN_TIMEOUT_LO,
					K_MIN_WAN_TIMEOUT_HI );
static UTCValue maxWANtimeOut= UTCinit( K_MAX_WAN_TIMEOUT_LO,
					K_MAX_WAN_TIMEOUT_HI );
static UTCValue defWANtimeOut= UTCinit( K_WAN_TIMEOUT_LO,
					K_WAN_TIMEOUT_HI );

static UTCValue minsyncHold= UTCinit( K_MIN_SYNC_HOLD_LO,
					K_MIN_SYNC_HOLD_HI );
static UTCValue maxsyncHold= UTCinit( K_MAX_SYNC_HOLD_LO,
					K_MAX_SYNC_HOLD_HI );
static UTCValue defsyncHoldServer= UTCinit( K_SYNC_HOLD_SERVER_LO,
					K_SYNC_HOLD_SERVER_HI );
static UTCValue defsyncHoldClerk = UTCinit( K_SYNC_HOLD_CLERK_LO,
					K_SYNC_HOLD_CLERK_HI );

static ExtInacc defUpdateInacc= UTCinit((K_100NS_PER_SEC*60),0); /* 1 minute */ 


/*
 *++
 *  DirectiveCreate()
 *
 *  Functional Description:
 *
 *  Create the DECdts service as a server or clerk.
 *
 *  Inputs:
 *
 *  serviceType - K_SERVER or K_CLERK, depending upon what service is
 *		  required.
 *  CallerID - the identification of the caller.
 *
 *  Implicit Inputs:
 *
 *  ShmPtr.
 *
 *  Outputs:
 *
 *  statusBlk - status of DTSS directive and any associated status from other
 *              components (like DNS, DNA, RPC, etc.)
 *
 *  Implicit Outputs:
 *
 *  The service state goes from K_STATE_NOT_CREATED to _K_STATE_OFF
 *
 *  Value Returned:
 *
 *	K_NCL_RSP_SUCCESS
 *	K_NCL_RSP_ACCESS_DENIED
 *	K_NCL_RSP_ALREADY_EXISTS
 *	K_NCL_RSP_INV_ARG	! should never happen from NCL call !
 *
 *  Side Effects:
 *
 *--
 */
unsigned long DirectiveCreate ( 
    unsigned long  callerID,
    int            serviceType,
    StatusBlk     *statusBlkPtr
)
{

    error_status_t	status;
    StatusBlk statusBlk;

    if ( ! PrivedUser( callerID ))
        return( K_NCL_RSP_ACCESS_DENIED );

    if ( !IsState(K_STATE_NOT_CREATED) )
        return( K_NCL_RSP_ALREADY_EXISTS );

    if ( serviceType != K_VARIANT_SERVER &&
         serviceType != K_VARIANT_CLERK )
        return( K_NCL_RSP_INV_ARG );

    if ( serviceType == K_VARIANT_SERVER ) 
    {
        shmPtr->mgtParams.variant = VTserver;
	UTCassign (&(shmPtr->mgtParams.syncHold),K_SYNC_HOLD_SERVER_LO,
			K_SYNC_HOLD_SERVER_HI);
	shmPtr->mgtParams.minServers  = K_MIN_SERVERS_SERVER;
    }
    else
    {
	shmPtr->mgtParams.variant = VTclerk;
    }

    /*
     * Obtain Time Service entry name where bindings will be exported.
     */
    strcpy((char *)rpcBufPtr->TServiceEntryName,
	   (char *)CELL_RELATIVE_PREFIX);

    strcat ((char *)rpcBufPtr->TServiceEntryName,
            (char *)shmPtr->mgtParams.servEntryName);

    if (IsServer())
    {
#if CONFIG_SECURITY

	/*
	 * Set the server principal name to the default value.
	 */
        strcpy ((char *)shmPtr->mgtParams.servPrincName,
                (char *)rpcBufPtr->DefServPrincName);

	/*
	 * Get the server principal's UUID generated by the security services.
	 */
	statusBlk = GetTServicePrincUUID (shmPtr->mgtParams.servPrincName,
					  &rpcBufPtr->TServicePrincUUID);
	/*
	 * If an error occurred return a status error.  However, change the
	 * state to OFF, so that the daemon can be deleted (stopped) in a clean
	 * way, i.e., through a management directive.
	 */

	if (IsError(statusBlk))
	{
	    CopyStatusBlk (statusBlkPtr, statusBlk);
	    StateChange( K_STATE_OFF );
	    return(K_NCL_RSP_SYSTEM_ERROR);
	}

#else
	/*
	 * If dts is not configured with security, use the object uuid
	 * as the principal UUID for this server so that courier role
	 * selection and server import work properly.
	 */
	rpcBufPtr->TServicePrincUUID = rpcBufPtr->TServiceObjUUID;

#endif /* CONFIG_SECURITY */

	shmPtr->mgtParams.uid = rpcBufPtr->TServicePrincUUID;

    }

#if CONFIG_SECURITY

    /*
     * Register and export the acl manager interface
     */
    mgt_register_acl_mgr_if (rpcBufPtr->bindingVector,
			     &rpcBufPtr->objUUIDvector,
			     &status);
    if (status != error_status_ok)
    {
	dce_error_string_t tempString ;
	

	dce_svc_printf( DTS_S_REGISTER_ACL_IF_MSG, 
		       	       dce_dts_error_text( status, tempString ));
	statusBlkPtr->status = K_DCE_ERROR;
	statusBlkPtr->sysStatus = (unsigned long)status;
	StateChange( K_STATE_OFF );
	return(K_NCL_RSP_SYSTEM_ERROR);
    }

    mgt_export_acl_mgr_if (rpcBufPtr->TServiceEntryName,
			   rpcBufPtr->bindingVector,
			   &rpcBufPtr->objUUIDvector,
			   &status);
    if (status != error_status_ok)
    {
	dce_error_string_t tempString ;


	dce_svc_printf( DTS_S_EXPORT_ACL_IF_MSG, 
		       dce_dts_error_text( status, tempString ));

	statusBlkPtr->status = K_DCE_ERROR;
	statusBlkPtr->sysStatus = (unsigned long)status;
	StateChange( K_STATE_OFF );
	return(K_NCL_RSP_SYSTEM_ERROR);
    }
#endif /* CONFIG_SECURITY */

    /*
     * If this is a server, register the time service interface with the
     * endpoint mapper and export bindings to the Name Service.
     */

    if (IsServer())
    {
	/*
	 * This server starts as a local server.
	 */
	shmPtr->mgtParams.servType = K_SERVER_LOCAL;

	/*
	 * Check if this server is a member of the Time Servers group
	 * in the security realm. If so, export bindings to the name
	 * space entry.  Otherwise, attempt to unexport bindings in case
	 * there are stale bidings in databases.
	 */

#if CONFIG_SECURITY
	if ( serverInSecurityGroup( shmPtr->mgtParams.servPrincName ) )
	{
#endif /* CONFIG_SECURITY */

	    statusBlk = ExportTServiceToNS();
	    if (IsError(statusBlk))
	    {
		CopyStatusBlk (statusBlkPtr, statusBlk);
		StateChange( K_STATE_OFF );
		return(K_NCL_RSP_SYSTEM_ERROR);
	    }
#if CONFIG_SECURITY
	}
	else
	{
	    int hostServer = 1;
	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	       "Host Server %s is not a member of Time Servers group\n",
		shmPtr->mgtParams.servPrincName));
	    /*
	     *  Log event ->  <Local Server Not In Group>
	     *  and attempt to unexport bindings.
	     */
	    LogEventServerNotInGroup((char *)shmPtr->mgtParams.servPrincName,
				     (char *)shmPtr->mgtParams.groupName,
				     hostServer);

	    UnexportTimeService();
	    StateChange( K_STATE_OFF );
	    statusBlkPtr->status = K_DCE_ERROR;
	    statusBlkPtr->sysStatus = dts_s_serv_not_in_ts_group;
	    return(K_NCL_RSP_SYSTEM_ERROR);
	}
#endif /* CONFIG_SECURITY */
   }

    StateChange( K_STATE_OFF );

    return( K_NCL_RSP_SUCCESS );
}

/*
 *++
 *  DirectiveDisable()
 *
 *  Functional Description:
 *
 *  Disable the DTSS service by setting the TSstate to K_STATE_OFF.
 *  Cancel any current clock adjustment, cancel all timers, and
 *  reinitialize the state machine.
 *
 *  Inputs:
 *
 *  CallerID - the identification of the caller.
 *
 *  Implicit Inputs:
 *
 *  ShmPtr.
 *
 *  Outputs:
 *
 *  statusBlk - status of DTSS directive and any associated status from other
 *              components (like DNS, DNA, RPC, etc.)
 *
 *
 *  Implicit Outputs:
 *
 *  None.
 *
 *  Value Returned:
 *
 *	K_NCL_RSP_WRONG_STATE
 *	K_NCL_RSP_NOT_CREATED
 *	K_NCL_RSP_SUCCESS
 *
 *  Side Effects:
 *
 *  The clock adjustment is cancelled.
 *
 *--
 */
unsigned long DirectiveDisable ( 
   unsigned long  callerID,
   StatusBlk     *statusBlkPtr
)
{
    StatusBlk statusBlk;

    if ( ! PrivedUser( callerID ))
        return( K_NCL_RSP_ACCESS_DENIED );

    if (IsState(K_STATE_NOT_CREATED))
	return( K_NCL_RSP_NOT_CREATED );

    if (IsState(K_STATE_OFF))
	return( K_NCL_RSP_WRONG_STATE );

    if (!IsState( K_STATE_OFF ) && !StateChange( K_STATE_OFF ))
    {
	BugCheck();
    };

    if (shmPtr->mgtParams.variant == VTserver) {
	statusBlk = UnregisterTServerRPC();
	if (IsError(statusBlk))
	    return(K_NCL_RSP_SYSTEM_ERROR);
    }

    ReInitState();

    LogEventShutdownComplete ();
    return( K_NCL_RSP_SUCCESS );
}

/*
 *++
 *  DirectiveDelete()
 *
 *  Functional Description:
 *
 *  Delete the DECdts service as a server or clerk.
 *
 *  Inputs:
 *
 *  CallerID - the identification of the caller.
 *
 *  Implicit Inputs:
 *
 *  ShmPtr.
 *
 *  Outputs:
 *
 *  statusBlk - status of DTSS directive and any associated status from other
 *              components (like DNS, DNA, RPC, etc.)
 *
 *  Implicit Outputs:
 *
 *  The service state goes from K_STATE_X to _K_STATE_OFF
 *
 *  Value Returned:
 *
 *	K_NCL_RSP_ACCESS_DENIED
 *	K_NCL_RSP_SUCCESS
 *	K_NCL_RSP_NOT_CREATED
 *	K_NCL_RSP_WRONG_STATE
 *
 *  Side Effects:
 *--
 */
unsigned long DirectiveDelete(
    unsigned long  callerID, 
    StatusBlk     *statusBlkPtr
)
{

    if ( ! PrivedUser( callerID ))
        return( K_NCL_RSP_ACCESS_DENIED );

    if ( IsState(K_STATE_OFF) )
    {
	if (!StateChange( K_STATE_NOT_CREATED ))
	{
	    BugCheck();
	}
    }
    else if (!IsState(K_STATE_NOT_CREATED))
	return( K_NCL_RSP_WRONG_STATE );

    GenEvent(ETterminate);

    return( K_NCL_RSP_SUCCESS );
}

/*
 *++
 *  DirectiveEnable()
 *
 *  Functional Description:
 *
 *  Enable the DECdts service.  This includes initializing the transport
 *  modules.
 *
 *  Inputs:
 *
 *  CallerID - the identification of the caller.
 *  ClockSet - A boolean - TRUE if the clock is to be set on the
 *		first synchronization, FALSE if it will adjust.
 *
 *  Implicit Inputs:
 *
 *  ShmPtr.
 *
 *  Outputs:
 *
 *  statusBlk - status of DTSS directive and any associated status from other
 *              components (like DNS, DNA, RPC, etc.)
 *
 *  Implicit Outputs:
 *
 *  The service state goes from K_STATE_OFF to _K_STATE_ON
 *
 *  Value Returned:
 *
 *	K_NCL_RSP_ACCESS_DENIED
 *	K_NCL_RSP_NOT_CREATED
 *	K_NCL_RSP_WRONG_STATE
 *	K_NCL_RSP_FAILURE        ! unknown failure !
 *	K_NCL_RSP_SUCCESS
 *
 *  Side Effects:
 *
 *--
 */
unsigned long DirectiveEnable(
    unsigned long  callerID,
    unsigned long  clockSet,
    StatusBlk     *statusBlkPtr
)
{

    if ( ! PrivedUser( callerID ))
        return( K_NCL_RSP_ACCESS_DENIED );

    if (IsState(K_STATE_NOT_CREATED))
	return( K_NCL_RSP_NOT_CREATED );

    /*
     * If the service is disabled, then enable it, else pass
     * back a success.
     */
    if ( IsState( K_STATE_OFF ) )
    {
        if (!StateChange( K_STATE_ON ))
	    BugCheck();	/* should not happen */

	*statusBlkPtr = InitTransport ();
	if (statusBlkPtr->status != DTSS_SUCCESS)
	    return(K_NCL_RSP_SYSTEM_ERROR);

	/*
	 * If this is a server, register with the RPC rtl.
	 */
        if (shmPtr->mgtParams.variant == VTserver) {
	    *statusBlkPtr = RegisterTServerRPC();
	    if (statusBlkPtr->status != DTSS_SUCCESS)
		return(K_NCL_RSP_SYSTEM_ERROR);
	}

        return ( StartCmd( clockSet ) );
    }
    else
	return( K_NCL_RSP_WRONG_STATE );
}

/*
 *++
 *  StartCMD()
 *
 *  Functional Description:
 *
 *  Create the DECdts service as a server or clerk.
 *
 *  Inputs:
 *
 *    Clock Set - synch by adjust or synch by set.
 *
 *  Implicit Inputs:
 *
 *  ShmPtr.
 *
 *  Outputs:
 *
 *  None.
 *
 *  Implicit Outputs:
 *
 *
 *  Value Returned:
 *
 *
 *
 *  Side Effects:
 *
 *	K_NCL_RSP_SUCCESS
 *
 *--
 */
static unsigned long StartCmd ( 
unsigned long clockSet
)
{

    /*
     * To get things started, generate an event requesting a synchronization.
     * Both server and client will discover that they have no server names
     * cached, so they will solicit names to fill the cache. When they have
     * enough names, they will synchronize clocks with the other servers.
     * If there are not yet MINSERVERs servers in the net, they will not
     * synchronize. Note that this means that at least MINSERVERS servers
     * must be up and thus if a faulty server starts first it won't poison
     * all those that start after it.
     */
    GenEvent(ETsynchronize);

    /*
     * If we are to start by setting, rather than the default which is
     * adjustment or drifting, set the flag to tell DoSynch().
     */
    if ( ! StateChange( clockSet ? K_STATE_SYNCH_SET:K_STATE_SYNCH_ADJUST ) )
	BugCheck();

    LogEventStartupCompleted (clockSet);
    return ( K_NCL_RSP_SUCCESS );
}

/*
 *++
 *  DirectiveSynchronize()
 *
 *  Functional Description:
 *
 *  Synchronize the DECdts service
 *
 *  Inputs:
 *
 *  serviceType - K_SERVER or K_CLERK, depending upon what service is
 *		  required.
 *  CallerID - the identification of the caller.
 *
 *  Implicit Inputs:
 *
 *  ShmPtr.
 *
 *  Outputs:
 *
 *  statusBlk - status of DTSS directive and any associated status from other
 *              components (like DNS, DNA, RPC, etc.)
 *
 *  Implicit Outputs:
 *
 *  The service state goes from K_STATE_OFF to _K_STATE_SYNCH_XX
 *
 *  Value Returned:
 *
 *	K_NCL_RSP_ACCESS_DENIED
 *	K_NCL_RSP_NOT_CREATED
 *	K_NCL_RSP_WRONG_STATE
 *	K_NCL_RSP_SUCCESS
 *
 *  Side Effects:
 *--
 */
unsigned long DirectiveSynchronize(
    unsigned long  callerID,
    unsigned long  clockSet,
    StatusBlk     *statusBlkPtr
)
{

    if ( ! PrivedUser( callerID ))
        return( K_NCL_RSP_ACCESS_DENIED );

    if (IsState(K_STATE_NOT_CREATED))
	return( K_NCL_RSP_NOT_CREATED );

    if ( ! IsState(K_STATE_ON) )
	return( K_NCL_RSP_WRONG_STATE );

    /*
     * Cancel the previous synch time and generate an event requesting
     * a synchronization.
     */
    CancelTimer (K_TIMER_NEXT_SYNCH);
    GenEvent(ETsynchronize);

    /*
     * If we are to synching by setting, rather than the default which is
     * adjustment or drifting, set the flag to tell DoSynch().
     */
    if ( !StateChange( clockSet ? K_STATE_SYNCH_SET : K_STATE_SYNCH_ADJUST ))
	BugCheck();

    return ( K_NCL_RSP_SUCCESS );
}

/*
 *++
 *  DirectiveShow()
 *
 *  Functional Description:
 *
 *  Return the value of an attribute
 *
 *  Inputs:
 *
 *  CallerID - the identification of the caller.
 *  attrID   - the Atribute number
 *  showBuf  - the address of a transfer buffer to return the data.
 *
 *  Implicit Inputs:
 *
 *  ShmPtr.
 *
 *  Outputs:
 *
 *  statusBlk - status of DTSS directive and any associated status from other
 *              components (like DNS, DNA, RPC, etc.)
 *
 *  Implicit Outputs:
 *
 *
 *  Value Returned:
 *
 *	K_NCL_RSP_ACCESS_DENIED
 *	K_NCL_RSP_NOT_CREATED
 *	K_NCL_RSP_NOT_SUPPORTED
 *	K_NCL_RSP_INV_ARG
 *	K_NCL_RSP_SUCCESS
 *      K_NCL_RSP_INV_GD   - global directory not set
 *
 *  Side Effects:
 *--
 */
unsigned long DirectiveShow(
    unsigned long    callerID,
    unsigned long    attrID,
    struct TransBuf *showBuff,
    StatusBlk       *statusBlkPtr
)
{
    if (IsState(K_STATE_NOT_CREATED))
	return( K_NCL_RSP_NOT_CREATED );

    statusBlkPtr->status = DTSS_SUCCESS;

    switch  ( (int)attrID )
    {

	case K_CHECK_INTERVAL_ATTR:
		if ( ! IsServer())
		{
		    return( K_NCL_RSP_NOT_SUPPORTED );
		}
		else
		{
                    CopyUTCrelToBuff( showBuff, shmPtr->mgtParams.checkInt,
				     K_TYPE_BINRELTIME);
		}
	break;

	case K_BACKUP_COURIER_ROLE_ATTR:
	    {
	        int backupRole;

		if ( ! IsServer())
		{
		    return( K_NCL_RSP_NOT_SUPPORTED );
		}
		else
		{
		    backupRole = shmPtr->mgtParams.myCourierRole;
                    CopyInt32toBuff( showBuff, backupRole, K_TYPE_ENUMERATION);
		}
		break;
	    }

	case K_COURIER_ROLE_ATTR:
		if ( ! IsServer())
		{
		    return( K_NCL_RSP_NOT_SUPPORTED );
		}
		else
		{
                    CopyInt32toBuff( showBuff, shmPtr->mgtParams.courierRole,
				    K_TYPE_ENUMERATION);

		}
		break;

	case K_EPOCH_NUMBER_ATTR:
		if ( ! IsServer())
		{
		    return( K_NCL_RSP_NOT_SUPPORTED );
		}
		else
		{
                    CopyInt32toBuff( showBuff, shmPtr->mgtParams.epochNumber,
				    K_TYPE_UNSIGNED32);
		}
		break;

	case K_ERROR_TOLLERANCE_ATTR:
                CopyUTCrelToBuff( showBuff, shmPtr->mgtParams.errorTolerance,
				 K_TYPE_BINRELTIME);
		break;

	case K_TIME_DIFF_ATTR:
		{
		UTCValue   tdfValue;
		long seconds;

		/*
	 	 * Convert the tdf into a UTC relative time
		 */

		(void) utc_bintime((timespec_t *)NULL,
				   (timespec_t *)NULL, &seconds,
				   (utc_t *)NULL);
		emul(seconds, K_100NS_PER_SEC, &tdfValue);
                CopyUTCrelToBuff( showBuff, tdfValue, K_TYPE_BINRELTIME);
		}
		break;

	case K_MAXIMUM_INACCURACY_ATTR:
                CopyUTCrelToBuff( showBuff,shmPtr->mgtParams.maxInacc,
				 K_TYPE_BINRELTIME);
		break;

	case K_SERVERS_REQUIRED_ATTR:
		CopyInt32toBuff( showBuff,shmPtr->mgtParams.minServers,
				K_TYPE_INTEGER32);
		break;

	case K_QUERY_ATTEMPTS_ATTR:
		CopyInt32toBuff( showBuff, shmPtr->mgtParams.repetitions,
				K_TYPE_INTEGER32);
		break;

	case K_LAN_TIMEOUT_ATTR:
		CopyUTCrelToBuff(showBuff,shmPtr->mgtParams.LANtimeOut,
				 K_TYPE_BINRELTIME);
		break;

	case K_WAN_TIMEOUT_ATTR:
		CopyUTCrelToBuff(showBuff,shmPtr->mgtParams.WANtimeOut,
				 K_TYPE_BINRELTIME);
		break;

	case K_VARIANT_ATTR:
		CopyInt32toBuff( showBuff,shmPtr->mgtParams.variant,
				K_TYPE_ENUMERATION);
		break;

	case K_SYNCH_ATTR:
		CopyUTCrelToBuff( showBuff,shmPtr->mgtParams.syncHold,
				 K_TYPE_BINRELTIME);
		break;

	case K_SERVICE_TRACE_ATTR:
		{
		    int flagSet;

		    flagSet = (M_TRACE_STATES & shmPtr->mgtParams.traceFlags) ?
			      1 : 0;
		    CopyInt32toBuff( showBuff, flagSet, K_TYPE_BOOLEAN);
		    break;
                }
	case K_COMM_TRACE_ATTR:
		{
		    int flagSet;

		    flagSet = (M_TRACE_MESSAGES & shmPtr->mgtParams.traceFlags) ?
			      1 : 0;
		    CopyInt32toBuff( showBuff, flagSet, K_TYPE_BOOLEAN );
		    break;
		}

	case K_SYNCH_TRACE_ATTR:
		{
		    int flagSet;

		    flagSet = (M_TRACE_GRAPH & shmPtr->mgtParams.traceFlags) ?
			      1 : 0;
		    CopyInt32toBuff( showBuff, flagSet, K_TYPE_BOOLEAN);
		}
		break;

	case K_ARITH_TRACE_ATTR:
		{
		    int flagSet;

		    flagSet = (M_TRACE_ARITH & shmPtr->mgtParams.traceFlags) ?
			      1 : 0;
		    CopyInt32toBuff( showBuff, flagSet, K_TYPE_BOOLEAN );
		}
	  	break;

	case K_CLOCK_ADJUSTMENT_ATTR:

		CopyInt32toBuff(showBuff,
				(K_100NS_PER_SEC*100)/K_TICKADJ_RATE,
				K_TYPE_INTEGER32);
		break;

	case K_CLOCK_DRIFT_ATTR:
		CopyInt32toBuff(showBuff,
				shmPtr->clockParams.clockDrift,
				K_TYPE_INTEGER32);
		break;

	case K_CLOCK_RESOLUTION_ATTR:
		CopyInt32toBuff(showBuff,
				shmPtr->clockParams.clockResolution,
				K_TYPE_INTEGER32);
		break;

	case K_CURRENT_TIME_ATTR:
                {
		utc_t utcNow;

		utc_gettime( &utcNow );
		CopyBinAbsTimeToBuff( showBuff, utcNow, K_TYPE_BINABSTIME );
		break;
		}

	case K_UID_ATTR:

		if ( ! IsServer())
		{
		    return( K_NCL_RSP_NOT_SUPPORTED );
		}
		else
		CopyUIDtoBuff( showBuff, &shmPtr->mgtParams.uid, K_TYPE_UID );
		break;

	case K_LAST_SYNCH_ATTR:
		{
		long TDFseconds;
		utc_t temp;

		(void) utc_bintime((timespec_t *)NULL,
				   (timespec_t *)NULL,
				   &TDFseconds,
				   (utc_t *)NULL);
		(void) utc_mkcomptime(&temp, &shmPtr->baseTimes.Tbase,
				      &shmPtr->baseTimes.Ibase, TDFseconds);

		CopyBinAbsTimeToBuff( showBuff, temp, K_TYPE_BINABSTIME );

		}
		break;

	case K_STATE_ATTR:
		{
		    int tempState;

                    /*
		     * Map the current state to the correct NCL display
		     * state.
		     */
		    switch (shmPtr->mgtParams.TSstate) {
		        /*
			 * The display does not distinguish between
			 * synch set and synch adjust, only synchronizing
			 * is displayed.  Map synch_set to synch_adjust
			 * since synch_adjust displays properly.
			 */
			case K_STATE_SYNCH_SET:
				tempState = K_STATE_SYNCH_ADJUST;
				break;

			/*
			 * Return the rest of the states.  If a state
			 * which can not be displayed is returned (default:),
			 * then EMA/CML will display the integer.
			 */
			case K_STATE_OFF:              /* off */
			case K_STATE_ON:               /* on  */
			case K_STATE_SYNCH_ADJUST:     /* synchronizing */
			case K_STATE_UPDATING:         /* updating      */
			default:                       /* bad state???  */
				tempState = shmPtr->mgtParams.TSstate;
				break;
                        }

                CopyInt32toBuff( showBuff, tempState, K_TYPE_ENUMERATION);

		break;
		}
	case K_TP_PRESENT_ATTR:
            if ( IsServer())
	    {
		CopyInt32toBuff(showBuff,shmPtr->mgtParams.hasProvider,
				K_TYPE_BOOLEAN);
		break;
	    }
	    else
		return( K_NCL_RSP_NOT_SUPPORTED);

	case K_AUTO_TDF_ATTR:

		CopyInt32toBuff(showBuff,shmPtr->mgtParams.autoTDF,
				K_TYPE_BOOLEAN);

		break;

	case K_NEXT_TDF_ATTR:
	    {
		utc_t nextChange;
		utc_t currTime;

		utc_gettime(&currTime);
		if (utc_getnextzone (&nextChange, &currTime))
		    return(K_NCL_RSP_INV_ARG);

		CopyBinAbsTimeToBuff(showBuff, nextChange,
				     K_TYPE_BINABSTIME);
	    }
	    break;

	case K_DECDTS_VERSION_ATTR:
		CopyVersionToBuff( showBuff, shmPtr->mgtParams.DECdts,
				  K_TYPE_VERSION);
		break;

	case K_TIME_REP_ATTR:
		CopyVersionToBuff( showBuff, shmPtr->mgtParams.timeVersion,
				  K_TYPE_VERSION);
		break;

/* counters */

	case K_CREATION_TIME_ATTR:
		CopyBinAbsTimeToBuff( showBuff, shmPtr->mgtParams.creationTime,
				     K_TYPE_BINABSTIME);
		break;

	case K_LOCAL_FAULT_ATTR:
		CopyInt64toBuff( showBuff,shmPtr->counters[K_LOCAL_FAULT_CNTR],
				K_TYPE_COUNTER64);
		break;

	case K_FAULTY_SERVER_ATTR:
		if ( ! IsServer())
		{
		    return( K_NCL_RSP_NOT_SUPPORTED );
		}
		else
		{
		    CopyInt64toBuff( showBuff,
				    shmPtr->counters[K_FAULTY_SERVER_CNTR],
				    K_TYPE_COUNTER64);
		}
		break;

	case K_DIFFERENT_EPOCH_ATTR:
		if ( ! IsServer())
		{
		    return( K_NCL_RSP_NOT_SUPPORTED );
		}
		else
		{
		    CopyInt64toBuff( showBuff,
				    shmPtr->counters[K_DIFFERENT_EPOCH_CNTR],
				    K_TYPE_COUNTER64);
		}
		break;

	case K_TOO_FEW_ATTR:
		CopyInt64toBuff( showBuff,shmPtr->counters[K_TOO_FEW_CNTR],
				K_TYPE_COUNTER64);
		break;

	case K_TIME_PROVIDER_TIMEOUT_ATTR:
		CopyInt64toBuff( showBuff,shmPtr->counters[K_TIME_PROVIDER_TIMEOUT_CNTR],
				K_TYPE_COUNTER64);
		break;

	case K_TIME_REPS_ATTR:
		CopyInt64toBuff( showBuff,shmPtr->counters[K_TIME_REPS_CNTR],
				K_TYPE_COUNTER64);
		break;

	case K_NO_GLOBAL_ATTR:
		if ( ! IsServer())
		{
		    return( K_NCL_RSP_NOT_SUPPORTED );
		}
		else
		{
		    CopyInt64toBuff( showBuff,
				    shmPtr->counters[K_NO_GLOBAL_CNTR],
				    K_TYPE_COUNTER64);
		}
		break;

	case K_NOT_RESPONDING_ATTR:
		if ( ! IsServer())
		{
		    return( K_NCL_RSP_NOT_SUPPORTED );
		}
		else
		{
		    CopyInt64toBuff( showBuff,
				    shmPtr->counters[K_NOT_RESPONDING_CNTR],
				    K_TYPE_COUNTER64);
		}
		break;

	case K_CLOCK_SET_ATTR:
		CopyInt64toBuff( showBuff,shmPtr->counters[K_CLOCK_SET_CNTR],
				K_TYPE_COUNTER64);
		break;

	case K_EPOCH_DECLARATION_ATTR:
		if ( ! IsServer())
		{
		    return( K_NCL_RSP_NOT_SUPPORTED );
		}
		else
		{
		    CopyInt64toBuff( showBuff,
				    shmPtr->counters[K_EPOCH_DECLARATION_CNTR],
				    K_TYPE_COUNTER64);
		}
		break;

	case K_TIME_DIFFS_ATTR:
		CopyInt64toBuff( showBuff,shmPtr->counters[K_TIME_DIFFS_CNTR],
				K_TYPE_COUNTER64);
		break;

	case K_SYSTEM_ERROR_ATTR:
		CopyInt64toBuff( showBuff,shmPtr->counters[K_SYSTEM_ERROR_CNTR],
				K_TYPE_COUNTER64);
		break;

	case K_SYNCH_COMPLETED_ATTR:
		CopyInt64toBuff( showBuff,
				shmPtr->counters[K_SYNCH_COMPLETED_CNTR],
				K_TYPE_COUNTER64);
		break;

	case K_UPDATE_COMPLETED_ATTR:
		if ( ! IsServer())
		{
		    return( K_NCL_RSP_NOT_SUPPORTED );
		}
		else
		{
		    CopyInt64toBuff( showBuff,
				    shmPtr->counters[K_UPDATE_COMPLETED_CNTR],
				    K_TYPE_COUNTER64);
		}
		break;

	case K_STARTUP_COMPLETED_ATTR:
		CopyInt64toBuff( showBuff,
				shmPtr->counters[K_STARTUP_COMPLETED_CNTR],
				K_TYPE_COUNTER64);
		break;

	case K_SHUTDOWN_COMPLETED_ATTR:
		CopyInt64toBuff( showBuff,
				shmPtr->counters[K_SHUTDOWN_COMPLETED_CNTR],
				K_TYPE_COUNTER64);
		break;

	case K_INSUFFICIENT_RESOURCE_ATTR:
		CopyInt64toBuff( showBuff,shmPtr->counters[K_INSUFFICIENT_RESOURCE_CNTR],
				K_TYPE_COUNTER64);
		break;

	case K_TIME_PROVIDER_ATTR :
		if ( ! IsServer())
		{
		    return( K_NCL_RSP_NOT_SUPPORTED );
		}
		else
		{
		    CopyInt64toBuff(showBuff,
				    shmPtr->counters[K_TIME_PROVIDER_CNTR],
				    K_TYPE_COUNTER64);
		}
		break;
#if CONFIG_SECURITY

	case K_LOC_SERV_NOT_TSG_MEMBER_ATTR :
		if ( ! IsServer())
		{
		    return( K_NCL_RSP_NOT_SUPPORTED );
		}
		else
		{
		    CopyInt64toBuff(showBuff,
			       shmPtr->counters[K_LOC_SERV_NOT_TSG_MEMBER_CNTR],
			       K_TYPE_COUNTER64);
		}
		break;

	case K_SERV_NOT_TSG_MEMBER_ATTR :

	        CopyInt64toBuff(showBuff,
				shmPtr->counters[K_SERV_NOT_TSG_MEMBER_CNTR],
				K_TYPE_COUNTER64);
		break;

	case K_SERVER_PRINCIPAL_NAME :
		if ( ! IsServer())
		    return( K_NCL_RSP_NOT_SUPPORTED );
		else
		    CopyStringToBuff(showBuff,
				     (char *)shmPtr->mgtParams.servPrincName,
				     K_TYPE_STRING);
		break;

	case K_SERVER_ENTRY_NAME :
		if ( ! IsServer())
		    return( K_NCL_RSP_NOT_SUPPORTED );
		else
		    CopyStringToBuff(showBuff,
				     (char *)shmPtr->mgtParams.servEntryName,
				     K_TYPE_STRING);
		break;

	case K_SERVER_GROUP_NAME :
		if ( ! IsServer())
		    return( K_NCL_RSP_NOT_SUPPORTED );
		else
		    CopyStringToBuff(showBuff,
				     (char *)shmPtr->mgtParams.groupName,
				     K_TYPE_STRING);
		break;

#endif	 /* CONFIG_SECURITY */

	default:
		return( K_NCL_RSP_INV_ARG );
	}

	return( K_NCL_RSP_SUCCESS );
}


/*
 *++
 *  DirectiveSet()
 *
 *  Functional Description:
 *
 *  Set the specified DECdts attribute.
 *
 *  Inputs:
 *
 *  CallerID - the identification of the caller.
 *  AttrID   - the ID of an attribute
 *  setBuffAdr - the address of a buffer in which the datum value is tranfered.
 *  defaultValue- flag, TRUE if the default value for the attribute should
 *		  be set, false if the value in setBuffer should be used.
 *
 *  Implicit Inputs:
 *
 *  ShmPtr.
 *
 *  Outputs:
 *
 *  statusBlk - status of DTSS directive and any associated status from other
 *              components (like DNS, DNA, RPC, etc.)
 *
 *  Implicit Outputs:
 *
 * 	If the default value is set to true, then setBuffAdr is written.
 *	It is set to the default value of the attribute.
 *
 *  Value Returned:
 *
 *
 *
 *
 *	K_NCL_RSP_ACCESS_DENIED
 *	K_NCL_RSP_NOT_CREATED
 *	K_NCL_RSP_NOT_SUPPORTED
 *	K_NCL_RSP_INV_ARG
 *	K_NCL_RSP_SUCCESS
 *  Side Effects:
 *
 *--
 */
unsigned long DirectiveSet( 
    unsigned long    callerID,
    unsigned long    attrID,
    struct TransBuf *setBuffAdr,
    int              defaultValue,
    StatusBlk       *statusBlkPtr
)
{
    unsigned long retValue;
    unsigned long status;
    UTCValue setSimpleRelTime;
    unsigned long setInt32;
    unsigned long setBoolean;

    if ( ! PrivedUser( callerID ))
        return( K_NCL_RSP_ACCESS_DENIED );

    retValue = K_NCL_RSP_INV_ARG;
    statusBlkPtr->status = DTSS_SUCCESS;

        /*
	 * Do size checks on set data
	 */
    if (IsState(K_STATE_NOT_CREATED))
	return( K_NCL_RSP_NOT_CREATED );

	switch ( (int)attrID )
	{
	    case K_CHECK_INTERVAL_ATTR:
                if ( !IsServer() )	/* server only */
		{
		    retValue = K_NCL_RSP_NOT_SUPPORTED;
		    break;
		}
                else if ( defaultValue )
	        {
		    shmPtr->mgtParams.checkInt = defCheckInt;
		    retValue = DirectiveShow(callerID, attrID,
					     setBuffAdr, statusBlkPtr);
		    break;
		}

		CopyBuffToSimpleRelTime( setSimpleRelTime, setBuffAdr,status);

		if ( status && IsInUTCrange( &setSimpleRelTime,
					     &minCheckInt, &maxCheckInt) )
		{
		    shmPtr->mgtParams.checkInt = setSimpleRelTime;
		    retValue = K_NCL_RSP_SUCCESS;
		}
		else
		    retValue = K_NCL_RSP_INV_ARG;
		break;

           case K_COURIER_ROLE_ATTR:
		if ( !IsServer() )
		{
		    retValue = K_NCL_RSP_NOT_SUPPORTED;
		    break;
		}

		if ( defaultValue )
		{
		    shmPtr->mgtParams.courierRole = defCourierRole;
		    retValue = DirectiveShow(callerID, attrID, setBuffAdr,
                                             statusBlkPtr);
		}
		else {

		    CopyBuffToInt( setInt32, setBuffAdr, status);

		    if ( status && IsInIntRange( setInt32, minCourierRole,
						maxCourierRole ) )
		    {
			shmPtr->mgtParams.courierRole = setInt32;
			retValue = K_NCL_RSP_SUCCESS;
		    }
		    else
			retValue = K_NCL_RSP_INV_ARG;
		}

		/*
		 * If the courier role is being set to non-courier/courier,
		 * make the acting courier role non-courier as well!
		 */
		if (shmPtr->mgtParams.courierRole == K_ROLE_NONCOURIER){
		    shmPtr->mgtParams.myCourierRole=
			K_BACKUP_ROLE_NONCOURIER;
		}
		else if (shmPtr->mgtParams.courierRole == K_ROLE_COURIER &&
			 shmPtr->mgtParams.hasProvider == FALSE) {
		    shmPtr->mgtParams.myCourierRole=
			K_BACKUP_ROLE_COURIER;
		}
		break;

	    case K_ERROR_TOLLERANCE_ATTR:
                if ( defaultValue )
	        {
		    shmPtr->mgtParams.errorTolerance = defErrorTol;
		    retValue = DirectiveShow(callerID, attrID, setBuffAdr,
                                             statusBlkPtr);
		    break;
		}

                CopyBuffToSimpleRelTime( setSimpleRelTime, setBuffAdr ,status);

		if ( status && IsInUTCrange( &setSimpleRelTime,
					     &minErrorTol, &maxErrorTol))
		{
		    shmPtr->mgtParams.errorTolerance = setSimpleRelTime;
		    retValue = K_NCL_RSP_SUCCESS;
		}
		else
		    retValue = K_NCL_RSP_INV_ARG;
		break;

	    case K_TIME_DIFF_ATTR:
		retValue = K_NCL_RSP_NOT_SUPPORTED;
		break;

	    case K_MAXIMUM_INACCURACY_ATTR:
                if ( defaultValue )
	        {
		    shmPtr->mgtParams.maxInacc = defMaxInacc;
		    retValue = DirectiveShow(callerID, attrID, setBuffAdr,
                                             statusBlkPtr);
		    break;
		}

		CopyBuffToSimpleRelTime( setSimpleRelTime, setBuffAdr, status);
		if (status && IsInUTCrange( (ExtInacc *)&setSimpleRelTime,
					    &minMaxInacc, &maxMaxInacc) )
		{
		    shmPtr->mgtParams.maxInacc = *(ExtInacc*)&setSimpleRelTime;
		    retValue = K_NCL_RSP_SUCCESS;
		}
		else
		    retValue = K_NCL_RSP_INV_ARG;
		break;

	    case K_SERVERS_REQUIRED_ATTR:
                if ( defaultValue )
	        {
		    if (IsServer())
		    {
			shmPtr->mgtParams.minServers = defMinServers_server;
		    }
		    else
			shmPtr->mgtParams.minServers = defMinServers_clerk;

		    retValue = DirectiveShow(callerID, attrID, setBuffAdr,
                                             statusBlkPtr);
		    break;

		}

		CopyBuffToInt( setInt32, setBuffAdr ,status);
		if (status && IsInIntRange(setInt32,
				minMinServers, maxMinServers))
		{
		    shmPtr->mgtParams.minServers = setInt32;
		    retValue = K_NCL_RSP_SUCCESS;
		}
		else
		    retValue = K_NCL_RSP_INV_ARG;

		break;

	    case K_QUERY_ATTEMPTS_ATTR:
                if ( defaultValue )
	        {
		    shmPtr->mgtParams.repetitions = defQueryAtt;
		    retValue = DirectiveShow(callerID, attrID, setBuffAdr,
                                             statusBlkPtr);
		    break;
		}

		CopyBuffToInt( setInt32, setBuffAdr ,status);
		if (status && IsInIntRange(setInt32,
				minQueryAtt, maxQueryAtt))
		{
		    shmPtr->mgtParams.repetitions = setInt32;
		    retValue = K_NCL_RSP_SUCCESS;
		}
		else
		    retValue = K_NCL_RSP_INV_ARG;
		break;

	    case K_LAN_TIMEOUT_ATTR:
                if ( defaultValue )
	        {
		    shmPtr->mgtParams.LANtimeOut = defLANtimeOut;
		    retValue = DirectiveShow(callerID, attrID, setBuffAdr,
                                             statusBlkPtr);
		    break;
		}

		CopyBuffToSimpleRelTime( setSimpleRelTime, setBuffAdr ,status);
		if ( status && IsInUTCrange( &setSimpleRelTime,
					     &minLANtimeOut, &maxLANtimeOut) )
		{
		    shmPtr->mgtParams.LANtimeOut = setSimpleRelTime;
		    retValue = K_NCL_RSP_SUCCESS;
		}
		else
		    retValue = K_NCL_RSP_INV_ARG;
		break;

	    case K_WAN_TIMEOUT_ATTR:
                if ( defaultValue )
	        {
		    shmPtr->mgtParams.WANtimeOut = defWANtimeOut;
		    retValue = DirectiveShow(callerID, attrID, setBuffAdr,
                                             statusBlkPtr);
		    break;
		}

		CopyBuffToSimpleRelTime( setSimpleRelTime, setBuffAdr ,status);
		if ( status && IsInUTCrange( &setSimpleRelTime,
					     &minWANtimeOut, &maxWANtimeOut) )
		{
		    shmPtr->mgtParams.WANtimeOut = setSimpleRelTime;
		    retValue = K_NCL_RSP_SUCCESS;
		}
		else
		    retValue = K_NCL_RSP_INV_ARG;
		break;

	    case K_SYNCH_ATTR:
                if ( defaultValue )
	        {
		    if ( IsServer() )
		        shmPtr->mgtParams.syncHold = defsyncHoldServer;
		    else
			shmPtr->mgtParams.syncHold = defsyncHoldClerk;

		    retValue = DirectiveShow(callerID, attrID, setBuffAdr,
                                             statusBlkPtr);
		    break;
		}

		CopyBuffToSimpleRelTime( setSimpleRelTime, setBuffAdr ,status);

		if ( status && IsInUTCrange( &setSimpleRelTime,
						&minsyncHold, &maxsyncHold) )
		{
		    shmPtr->mgtParams.syncHold = setSimpleRelTime;
		    retValue = K_NCL_RSP_SUCCESS;
		}
		else
		    retValue = K_NCL_RSP_INV_ARG;
		break;

	    case K_ARITH_TRACE_ATTR:
		if (defaultValue)
		{
		    SetTrace( M_TRACE_ARITH, (unsigned long)defaultValue,
                              (unsigned long)FALSE);
		    retValue = DirectiveShow(callerID, attrID, setBuffAdr,
                                             statusBlkPtr);
                    break;
		}

		CopyBuffToInt( setBoolean, setBuffAdr, status);
		if ( !status )
		{
		    retValue = K_NCL_RSP_INV_ARG;
		    break;
		}

		SetTrace( M_TRACE_ARITH, (unsigned long)defaultValue,
                          setBoolean);

		retValue = K_NCL_RSP_SUCCESS;

	    case K_SERVICE_TRACE_ATTR:

		if (defaultValue)
		{
		    SetTrace( M_TRACE_STATES | M_TRACE_THREADS,
                              (unsigned long)defaultValue,
			      (unsigned long)FALSE);
		    retValue = DirectiveShow(callerID, attrID, setBuffAdr,
                                             statusBlkPtr);
                    break;
		}

		CopyBuffToInt( setBoolean, setBuffAdr, status);
		if ( !status )
		{
		    retValue = K_NCL_RSP_INV_ARG;
		    break;
		}

		SetTrace( M_TRACE_STATES | M_TRACE_THREADS,
                          (unsigned long)defaultValue, setBoolean);
		retValue = K_NCL_RSP_SUCCESS;
		break;

	    case K_COMM_TRACE_ATTR:

		if (defaultValue)
		{
		    SetTrace( M_TRACE_MESSAGES, (unsigned long)defaultValue,
                              (unsigned long)FALSE );
		    retValue = DirectiveShow(callerID, attrID, setBuffAdr,
                                             statusBlkPtr);
                    break;
		}

		CopyBuffToInt( setBoolean, setBuffAdr, status);
		if ( !status )
		{
		    retValue = K_NCL_RSP_INV_ARG;
		    break;
		}

		SetTrace( M_TRACE_MESSAGES, (unsigned long)defaultValue,
                          setBoolean);
		retValue = K_NCL_RSP_SUCCESS;
		break;

	    case K_SYNCH_TRACE_ATTR:

		if (defaultValue)
		{
		    shmPtr->mgtParams.newTraceFile = TRUE;
		    SetTrace( M_TRACE_GRAPH, (unsigned long)defaultValue,
                              (unsigned long)FALSE);
	            retValue = DirectiveShow(callerID, attrID, setBuffAdr,
                                             statusBlkPtr);
                    break;
		}

		CopyBuffToInt( setBoolean, setBuffAdr, status);
		if ( !status )
		{
		    retValue = K_NCL_RSP_INV_ARG;
		    break;
		}

		shmPtr->mgtParams.newTraceFile = TRUE;
		SetTrace( M_TRACE_GRAPH, (unsigned long)defaultValue,
                          setBoolean);

		retValue = K_NCL_RSP_SUCCESS;
		break;

#if 0
	    case K_SERVER_NAME_ATTR:
		if (defaultValue) {
		    unsigned char tempString[512];
		    int serverNameLen;

		    (void)strcpy((char *)tempString, K_TS_SERVER_NAME);
		    serverNameLen = sizeof(dns_opq_sname);
		    dnsCvtStrSimpleToOpq(tempString,
					 0,
					 &shmPtr->mgtParams.serverName,
					 &serverNameLen,
					 (unsigned char **)0);
	            retValue = DirectiveShow(callerID, attrID, setBuffAdr,
                                             statusBlkPtr);
		    break;
		}

		if (IsServer()) {

		    if (setBuffAdr->transbuf_w_size > sizeof(dns_opq_sname) ||
			setBuffAdr->transbuf_w_size <= 0 )
			retValue = K_NCL_RSP_INV_ARG;
		    else {
			int dnsStatus;
			int stringLength;

			dnsStatus = dnsCvtOpqSimpleToStr
			    ((dns_opq_sname *)setBuffAdr->transbuf_t_data,
			     (unsigned char *)0,
			     &stringLength);

			if (dnsStatus == DNS_SUCCESS) {
			    (void)memcpy
				((VOID *)&shmPtr->mgtParams.serverName,
				 (VOID *)setBuffAdr->transbuf_t_data,
				 (size_t)setBuffAdr->transbuf_w_size);
			    retValue = K_NCL_RSP_SUCCESS;
			}
			else
			    retValue = K_NCL_RSP_INV_ARG;
		    }
		}

		else
		    retValue = K_NCL_RSP_NOT_SUPPORTED;
		break;
#endif

	    case K_AUTO_TDF_ATTR:
		/*
		 * Ultrix systems have automatic TDF change imbedded,
		 * so this is a non-settable attribute.
		 */
		retValue = K_NCL_RSP_NOT_SUPPORTED;

		break;

#if CONFIG_SECURITY

	case K_SERVER_PRINCIPAL_NAME :
	{

#if DTS_ENTITY_PRINCIPAL

	    StatusBlk statusBlk;

	    if ( !IsServer() )
	    {
		retValue = K_NCL_RSP_NOT_SUPPORTED;
		break;
	    }

	    if (defaultValue)
	    {
		strcpy ((char *)shmPtr->mgtParams.servPrincName,
			(char *)rpcBufPtr->DefServPrincName);
	    }
	    else
	    {
		(void)memcpy((VOID *)shmPtr->mgtParams.servPrincName,
			     (VOID *)setBuffAdr->transbuf_t_data,
			     (size_t)setBuffAdr->transbuf_w_size);

	    }

	    statusBlk = GetTServicePrincUUID(shmPtr->mgtParams.servPrincName,
					     &rpcBufPtr->TServicePrincUUID);
	    if(IsError(statusBlk))
	    {
		CopyStatusBlk (statusBlkPtr, statusBlk);
		return(K_NCL_RSP_SYSTEM_ERROR);
	    }

            statusBlk = CreateSecurityCntxt ();
            if (IsError(statusBlk))
            {
                CopyStatusBlk (statusBlkPtr, statusBlk);
                StateChange( K_STATE_OFF );
                return(K_NCL_RSP_SYSTEM_ERROR);
            }

	    retValue = K_NCL_RSP_SUCCESS;

#else
	    statusBlkPtr->status = K_DCE_ERROR;
	    statusBlkPtr->sysStatus = dts_s_read_only;
	    return(K_NCL_RSP_SYSTEM_ERROR);

#endif /* DTS_ENTITY_PRINCIPAL */

	    break;
	}

	case K_SERVER_ENTRY_NAME :
	{
	    StatusBlk statusBlk;

	    if ( !IsServer() )
	       return(K_NCL_RSP_NOT_SUPPORTED);

	    /*
	     * Unexport bindings and remove the old entry from the name
	     * space.
	     */
	    UnexportTimeService();

	    memset((VOID *)shmPtr->mgtParams.servEntryName, 0,
		   sizeof(shmPtr->mgtParams.servEntryName));

	    if (defaultValue)
		strcpy((char *)shmPtr->mgtParams.servEntryName,
		       DEF_SERVER_ENTRY_NAME );
	    else
		(void)memcpy((VOID *)shmPtr->mgtParams.servEntryName,
			     (VOID *)setBuffAdr->transbuf_t_data,
			     (size_t)setBuffAdr->transbuf_w_size);

	    /*
	     * Set the new server entry name where bindings are exported.
	     */

	    strcpy ((char *)rpcBufPtr->TServiceEntryName,
		    (char *)CELL_RELATIVE_PREFIX);
	    strcat ((char *)rpcBufPtr->TServiceEntryName,
		    (char *)shmPtr->mgtParams.servEntryName);

	    if ( serverInSecurityGroup( shmPtr->mgtParams.servPrincName))
	    {
		statusBlk = ExportTServiceToNS();
		if (IsError(statusBlk))
		{
		    CopyStatusBlk (statusBlkPtr, statusBlk);
		    return(K_NCL_RSP_SYSTEM_ERROR);
		}
	    }
	    else
	    {
		statusBlkPtr->status = K_DCE_ERROR;
		statusBlkPtr->sysStatus = dts_s_serv_not_in_ts_group;
		return(K_NCL_RSP_SYSTEM_ERROR);
	    }

	    retValue = K_NCL_RSP_SUCCESS;
	    break;
	}

	case K_SERVER_GROUP_NAME :

	    if ( !IsServer() )
	    {
		retValue = K_NCL_RSP_NOT_SUPPORTED;
		break;
	    }

	    if (defaultValue)
		strcpy((char *)shmPtr->mgtParams.groupName,
		       DEF_TSERV_GROUP_NAME );
	    else
		(void)memcpy((VOID *)shmPtr->mgtParams.groupName,
			     (VOID *)setBuffAdr->transbuf_t_data,
			     (size_t)setBuffAdr->transbuf_w_size);

		retValue = K_NCL_RSP_SUCCESS;
		break;

#endif /* CONFIG_SECURITY */

	 default:    retValue = K_NCL_RSP_NOT_SUPPORTED;
		     break;
	}

	return( retValue );
}
/*
 *++
 *  SetTrace()
 *
 *  Functional Description:
 *
 *  Enable or Disable the tracing functions, set the trace attributes.
 *
 *
 *
 *  Inputs:
 *
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *
 *  Implicit Outputs:
 *
 *
 *  Value Returned:
 *
 *
 *  Side Effects:
 *
 *
 *--
 */
static void SetTrace( 
    unsigned long traceBits,
    unsigned long defaultValue,
    unsigned long set
)
{
    if ( defaultValue || !set )
    {
	shmPtr->mgtParams.traceFlags &= ~traceBits;
    }
    else
    {
	shmPtr->mgtParams.traceFlags |= traceBits;
    }
}

/*
 *++
 *  DirectiveUpdate()
 *
 *  Functional Description:
 *
 *   Service an update request.
 *
 *
 *
 *  Inputs:
 *
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *  statusBlk - status of DTSS directive and any associated status from other
 *              components (like DNS, DNA, RPC, etc.)
 *
 *  Implicit Outputs:
 *
 *
 *  Value Returned:
 *	K_NCL_RSP_NOT_SUPPORTED:	  Server operation only.
 *	K_NCL_RSP_FAILURE:         Process Failure, can not complete operation.
 *	K_NCL_RSP_INV_ARG:         Time value is not within interval.
 *	K_NCL_RSP_SUCCESS:         Update begun.
 *
 *
 *  Side Effects:
 *
 *
 *--
 */
unsigned long DirectiveUpdate(
    unsigned long  callerID,
    utc_t          *newTimePtr,
    unsigned long  defaultValue,
    StatusBlk      *statusBlkPtr
)
{
    UTCValue newTime;
    ExtInacc newInacc;
    ExtInacc timeInacc;
    UTCValue now;
    utc_t curTime;

    if (IsState(K_STATE_NOT_CREATED))
	return( K_NCL_RSP_NOT_CREATED );

    /*
     *  Server only operation
     */
    if (IsClerk())
	return( K_NCL_RSP_NOT_SUPPORTED );
    /*
     * Make sure we are allowed to do this
     */

    if ( ! PrivedUser( callerID ))
        return( K_NCL_RSP_ACCESS_DENIED );

    /*
     * Probe to see if we are in the correct state.
     */
    if ( !(IsState(K_STATE_ON) || IsState(K_STATE_UPDATING)) )
    {
	return ( K_NCL_RSP_WRONG_STATE );
    }

    (void) utc_comptime(&newTime, &newInacc, (long *)NULL, newTimePtr);

    /*
     * What we really should do is use the time that the user entered
     * the command.  But since the command can be generated remotely
     * we must use a local time.  Should we grab the time that the
     * command arrived at the service?
     */

    utc_gettime (&curTime);             /* call dtss system routine */
    (void) utc_comptime(&now, &timeInacc, (long *)NULL, &curTime);

    if ( defaultValue )
	newInacc = defUpdateInacc;

    /*
     * Check to see if the specified interval is contained within
     * the current time interval.
     */
    if ( IsInterval1InInterval2( &newTime, &newInacc,
				 &now, &timeInacc ) )
    {
	UTCValue        adjustTime;
	UTCValue        relTime;

        /*
	 * Change the state, stop the clock adjustment,
	 * calculate the new time correction.
  	 */

	 if ( !StateChange( K_STATE_UPDATING ) )
	    return( K_NCL_RSP_FAILURE );		/* should not happen */

	 AdjustClkEnd();
	 CancelTimer (K_TIMER_NEXT_SYNCH);

	 AdjustClock
		(&now, &timeInacc, &newTime, &newInacc);

	 /*
	  * Figure out when the adjustment will complete and set a
	  * timer to go off then so we can schedule the next synch,
	  * change the state, and log an event.
	  */
	UTCsub( &now, &newTime, &relTime);
	UTCabsVal( &relTime, &relTime );
	UTCiMul(&relTime, K_TICKADJ_RATE, &adjustTime);
	InitiateTimer (K_TIMER_NEXT_SYNCH, &adjustTime);

	/*
	 * Log an update event
	 */
        LogEventUpdateTime( newTimePtr, &curTime );

	return( K_NCL_RSP_SUCCESS );
    }

    else
	return( K_NCL_RSP_NOT_CONTAINED );
}

/*
 *++
 *  DirectiveDeclare()
 *
 *  Functional Description:
 *	Used to recover for a catastrophic failure.  This is the only way
 *	to reset the time interval of a server.  This command is only
 *	valid for servers and the service does not have to be on to
 *	execute this command.
 *
 *  Inputs:
 *	callerID		- the process attempting the operation
 *	newTime 		- pointer to the new time interval
 *	epochNum		- new epoch number
 *
 *  Implicit Inputs:
 *	shmPtr 		- ptr to shmBuf
 *
 *  Outputs:
 *
 *  statusBlk - status of DTSS directive and any associated status from other
 *              components (like DNS, DNA, RPC, etc.)
 *
 *  Implicit Outputs:
 *	the clock will be reset and the BaseTimes will be updated
 *
 *  Value Returned:
 *	K_NCL_RSP_NOT_SUPPORTED - Server Support only - this is a clerk
 *	K_NCL_RSP_INV_ARG -	 The epoch number is invalid
 *	K_NCL_RSP_FAILURE -	 Can not complete the operation,process failure
 *	K_NCL_RSP_SUCCESS -	 Successfully completed operation
 *      K_NCL_RSP_NOT_CREATED -   Service not yet created
 *      K_NCL_RSP_ACCESS_DENIED - no privs
 *      K_NCL_RSP_WRONG_STATE -   Bad state
 *      K_NCL_RSP_EPOCH_DIFF -    Epoch must be different
 *--
 */

unsigned long DirectiveDeclare(
    unsigned long callerID,
    utc_t         *newTime,
    epoch_t       newEpoch,
    StatusBlk     *statusBlkPtr
)
{
    epoch_t	oldEpoch;
    utc_t       now, setTime;
    UTCValue    nowTime;
    ExtInacc    newInacc;
    UTCValue    setvalue;
    StatusBlk   statusBlk;

    if (IsState(K_STATE_NOT_CREATED))
	return( K_NCL_RSP_NOT_CREATED );

    /*
     * Make sure we are allowed to do this
     */

    if ( ! PrivedUser( callerID ))
        return( K_NCL_RSP_ACCESS_DENIED );

    /*
     * Make sure we are a server.
     */
    if (IsClerk())
	return( K_NCL_RSP_NOT_SUPPORTED );
    /*
     * Make sure we are in the right state
     */
    if ( !(IsState(K_STATE_ON) || IsState(K_STATE_UPDATING)))
	return( K_NCL_RSP_WRONG_STATE );

    /*
     * Be sure the new time is in valid range
     */
    if ((newTime != NULL) && !IsValidTime(newTime))
      return(K_NCL_RSP_INV_ARG);

    /*
     * Declare a new epoch.  There is not a restriction that epoch
     * numbers always increase although this is the wise thing to do.
     * Do not allow the epoch to be set to the current epoch.
     */
    oldEpoch = shmPtr->mgtParams.epochNumber;
    if ( newEpoch == oldEpoch )
	return( K_NCL_RSP_EPOCH_DIFF );

    if (newEpoch > K_EPOCH_NUMBER_MAX)
	return(K_NCL_RSP_EPOCH_DIFF);

    shmPtr->mgtParams.epochNumber = newEpoch;

    /*
     * get the current time and inaccuracy
     */
    utc_gettime( &now );

    /*
     * If a time was specified, stop any outstanding clock adjustment and
     * set the clock.
     */
    if (newTime != (utc_t *)0)
    {
        setTime = *newTime;
	(void) utc_comptime(&setvalue, &newInacc, (long *)NULL,
			    &setTime);
	(void) utc_comptime(&nowTime, (UTCValue *)NULL, (long *)NULL,
			    &now);

	AdjustClkEnd();
	statusBlk = SetClock (&setvalue, &newInacc, &nowTime, (int *)NULL);
	if ( IsError(statusBlk) )
	    BugCheck();
	/*
	 * Set the new epoch in the synchronization database so that
	 * servers requesting time before this node synchronizes will
	 * get the correct its new epoch number.
	 */
	sdb.mgtParams.epochNumber = newEpoch;
    }
    else
    {
	/*
	 * If only the new epoch is declared, set the inaccuracy to
	 * infinity.  Moving a server to a new epoch without specifying a
	 * time assumes that another server (in the new epoch) was given the
	 * correct time.  Setting this server's inaccuracy to infinite prevents
	 * it from providing the "bad" time to clerks, which ignore epochs.
	 */

	setTime = now;
	(void) utc_comptime(&setvalue, (UTCValue *)NULL, (long *)NULL,
			    &now);
	nowTime = setvalue;
	UTCassign (&newInacc,K_INFINITE_INACC_LO,K_INFINITE_INACC_HI);

	AdjustClkEnd();
	statusBlk = SetClock (&setvalue, &newInacc, &nowTime, (int *)NULL);
	if ( IsError(statusBlk) )
	    BugCheck();

	/*
	 * Set this flag so that when the server starts its synchronization, it
	 * will set minservers = 2. This will protect clerks in the case
	 * where there are only 2 servers in the LAN.  Presumibly the other
	 * server "knows" the correct time.  This server has its inaccuracy
	 * set to infinity. The default minserver is 3, therefore this server
	 * will not synchronize successfully until its minservers value
	 * gets decreased.
	 */
	newEpochNoTime = 1;

	/*
	 * Force a synchronization to obtain the time from the new
	 * time from the server in the new epoch that has the correct
	 * value of time.
	 */
	CancelTimer (K_TIMER_NEXT_SYNCH);
	GenEvent(ETsynchronize);

	if ( !StateChange( K_STATE_SYNCH_SET))
	    BugCheck();
    }

    LogEventEpochDeclared( oldEpoch, newEpoch, &now, &setTime );

    return( K_NCL_RSP_SUCCESS );
}
/* End of routine UpdateDirective */


/*
 *++
 *  DirectiveAdvertise()
 *
 *  Functional Description:
 *
 *  Advertise a global server
 *
 *  Inputs:
 *
 *  CallerID   - ID of requesting user
 *  ServerName - The name of the server we want to advertise.
 *  ServType   - Server type to advertise (Global or Local)
 *
 *
 *  Implicit Inputs:
 *
 *  ShmPtr.
 *
 *  Outputs:
 *
 *  statusBlk - status of DTSS directive and any associated status from other
 *              components (like DNS, DNA, RPC, etc.)
 *
 *  Implicit Outputs:
 *
 *
 *  Value Returned:
 *
 *
 *
 *  Side Effects:
 *
 *	K_NCL_RSP_ACCESS_DENIED
 *	K_NCL_RSP_NOT_CREATED
 *	K_NCL_RSP_NOT_SUPPORTED
 *	K_NCL_RSP_INV_ARG
 *	K_NCL_RSP_SUCCESS
 *	K_NCL_RSP_WRONG_STATE
 *
 *--
 */

unsigned long DirectiveAdvertise (
    unsigned long callerID,
    VOID          *ServerNamePtr,
    unsigned long ServType,
    StatusBlk     *statusBlkPtr
)
{

    if (IsState(K_STATE_NOT_CREATED))
	return( K_NCL_RSP_NOT_CREATED );

    /*
     * Make sure the requesting user is allowed to advertise a server
     */
    if ( ! PrivedUser( callerID ))
        return( K_NCL_RSP_ACCESS_DENIED );

    /*
     * Make sure we are a server.

     */
    if (IsClerk())
	return( K_NCL_RSP_NOT_SUPPORTED );

    /*
     * Check the server type to advertise.  It is LOCAL
     * or GLOBAL.  Return an error if neither.
     */
    if (ServType != K_SERVER_LOCAL &&
	ServType != K_SERVER_GLOBAL)
	return ( K_NCL_RSP_INV_ARG );

    return (ExportGblTServiceToNS (statusBlkPtr) );
}


/*
 *++
 *  DirectivUnAdvertise()
 *
 *  Functional Description:
 *
 *  Advertise a global server
 *
 *  Inputs:
 *
 *  CallerID   - ID of requesting user
 *  ServerName - The name of the server we want to unadvertise
 *  ServType   - Server type to advertise (Global or Local)
 *
 *
 *  Implicit Inputs:
 *
 *  ShmPtr.
 *
 *  Outputs:
 *
 *  statusBlk - status of DTSS directive and any associated status from other
 *              components (like DNS, DNA, RPC, etc.)
 *
 *  Implicit Outputs:
 *
 *
 *  Value Returned:
 *
 *
 *
 *  Side Effects:
 *
 *	K_NCL_RSP_ACCESS_DENIED
 *	K_NCL_RSP_NOT_CREATED
 *	K_NCL_RSP_NOT_SUPPORTED
 *	K_NCL_RSP_INV_ARG
 *	K_NCL_RSP_SUCCESS
 *	K_NCL_RSP_WRONG_STATE
 *
 *--
 */

unsigned long DirectiveUnadvertise (
    StatusBlk     *statusBlkPtr
)
{

    if (IsState(K_STATE_NOT_CREATED))
	return( K_NCL_RSP_NOT_CREATED );

    if ( !(IsState(K_STATE_ON)))
	return( K_NCL_RSP_WRONG_STATE );

    /*
     * Make sure we are a server.
     */
    if (IsClerk())
	return( K_NCL_RSP_NOT_SUPPORTED );

    return (UnexportGblTServiceFromNS (statusBlkPtr) );
}


/*
 *++
 *  IsValidTime()
 *
 *  Functional Description:
 *      Checks to see if a utc to see if it is within the range
 *      of times which can be represented by this operating system.
 *      Note that only the year field is checked, so it is possible
 *      that a small number of valid dates may be rejected.
 *
 *  Inputs:
 *	newTime 		- pointer to the new time interval
 *
 *  Implicit Inputs:
 *
 *  Value Returned:
 *
 *      1 (TRUE) - the time is valid
 *      0 (FALSE)- the time is Not valid.
 *
 *--
 */
static int IsValidTime( 
    utc_t *atime
)
{
timespec_t timesp;
timespec_t inaccsp;
long       tdf;

    return((utc_bintime(&timesp,&inaccsp,&tdf,atime)==0)?1:0);
}




int ValidateCheckInterval( void * v)
{
    UTCValue * uvp = (UTCValue *)v;
    return IsInUTCrange( uvp, &minCheckInt, &maxCheckInt);
}

int ValidateQueryAttempts( void * v)
{
    unsigned32 uv = *((unsigned32 *)v);
    return IsInIntRange( uv, minQueryAtt, maxQueryAtt) ;
}

int ValidateMinServers( void * v)
{
    unsigned32 uv = *((unsigned32 *)v);
    return IsInIntRange( uv, minMinServers, maxMinServers) ;
}

int ValidateCourierRole( void * v)
{
    unsigned32  uv = *((unsigned32 *)v);
    return IsInIntRange( uv, minCourierRole, maxCourierRole) ;
}

int ValidateTolerance( void * v)
{
    UTCValue * uvp = (UTCValue *)v;
    return IsInUTCrange( uvp, &minErrorTol, &maxErrorTol);
}

int ValidateGlobalTimeout( void * v)
{
    UTCValue * uvp = (UTCValue *)v;
    return IsInUTCrange( uvp, &minWANtimeOut, &maxWANtimeOut);
}

int ValidateLocalTimeout( void * v)
{
    UTCValue * uvp = (UTCValue *)v;
    return IsInUTCrange( uvp, &minLANtimeOut, &maxLANtimeOut);
}

int ValidateMaxInaccuracy( void * v)
{
    UTCValue * uvp = (UTCValue *)v;
    return IsInUTCrange( uvp, &minMaxInacc, &maxMaxInacc);
}

int ValidateSyncInterval( void * v)
{
    UTCValue * uvp = (UTCValue *)v;
    return IsInUTCrange( uvp, &minsyncHold, &maxsyncHold);
}
