/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: logevent.c,v $
 * Revision 1.1.2.2  1996/02/17  23:35:45  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:52:00  marty]
 *
 * 	added OSF OT 5465 fix to properly increment show sync comp counter
 * 	[1992/12/19  17:49:44  smythe]
 *
 * Revision 1.1.2.1  1995/12/08  18:14:08  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/5  1995/07/25  19:28 UTC  pare
 * 	Merge files
 * 
 * 	HP revision /main/HPDCE02/pare_MOTHRA/1  1995/07/25  12:30 UTC  pare
 * 	Remove 'ultrix' from source file string -- CHFts15829
 * 
 * 	HP revision /main/HPDCE02/4  1995/04/26  19:55 UTC  pare
 * 	Merge fix for CHFts15011
 * 
 * 	HP revision /main/HPDCE02/pare_mothra/1  1995/04/26  19:53 UTC  pare
 * 	Fix for CHFts15011
 * 
 * 	HP revision /main/HPDCE02/3  1994/09/26  19:53 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:26:06  root]
 * 
 * 	    Loading drop DCE1_0_2B18
 * 	[1993/03/11  22:35:26  osfrcs]
 * 
 * 	    Loading drop Dce1_0_1fnl
 * 	[1992/09/21  18:57:08  osfrcs]
 * 
 * Revision 1.1.10.7  1994/08/16  18:13:10  cbrooks
 * 	CR 11494 - 64 bit changes
 * 	[1994/08/09  19:11:24  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/18  13:59 UTC  pare
 * 	Merge changes from Hpdce02
 * 
 * Revision 1.1.10.6  1994/08/02  19:05:31  cbrooks
 * 	CR11514 assertion failure - fix args to utc_pointtime
 * 	[1994/08/02  17:51:04  cbrooks]
 * 
 * Revision 1.1.10.5  1994/06/30  19:45:28  cbrooks
 * 	CR11112 - new SAMS messages
 * 	[1994/06/28  20:13:40  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  17:35 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.7.4  1993/09/18  12:18:27  pare
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/18  12:16:29  pare]
 * 
 * Revision 1.1.8.3  1993/09/15  17:48:53  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.7.3  1993/07/15  16:13:47  truitt
 * 	OT7973: Remove support for the 'Too Many Server Detected' command.
 * 	        Add support for the 'Time Provider Timeout Detected' command.
 * 	[1993/07/15  16:13:24  truitt]
 * 
 * Revision 1.1.10.4  1994/05/31  21:29:25  cbrooks
 * 	Merged with changes from 1.1.10.3
 * 	[1994/05/31  21:28:23  cbrooks]
 * 
 * 	Merge in DTS code cleanup changes
 * 	[1994/05/31  21:18:45  cbrooks]
 * 
 * 	CR 10611 - DTS needs new ACL manager
 * 	[1994/05/31  21:03:59  cbrooks]
 * 
 * Revision 1.1.10.3  1994/05/16  18:51:57  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:33  rhw]
 * 
 * Revision 1.1.10.2  1994/04/21  22:06:13  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  20:00:01  cbrooks]
 * 
 * 	Checkpoint:4.15.94
 * 	[1994/04/15  21:15:23  cbrooks]
 * 
 * Revision 1.1.10.1  1994/01/25  16:27:22  rsalz
 * 	CR # 9747
 * 	[1994/01/25  16:27:02  rsalz]
 * 
 * Revision 1.1.8.2  1993/07/14  21:26:37  tom
 * 	Increment Time Provider timeout counter, remove too many servers
 * 	event log.
 * 	[1993/07/14  20:46:49  tom]
 * 
 * Revision 1.1.7.2  1993/07/08  20:26:00  root
 * 	Initial King Kong branch
 * 	[1993/07/08  20:25:28  root]
 * 
 * Revision 1.1.5.2  1993/03/24  13:56:04  truitt
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/24  13:54:59  truitt]
 * 
 * Revision 1.1.2.6  1993/02/03  21:14:18  tom
 * 	Add LogEventTPtimeout routine to log time provider timeouts.
 * 	Bug 5204 - Print server principal name instead of network address.
 * 	[1993/02/03  21:11:17  tom]
 * 
 * Revision 1.1.2.5  1993/02/03  16:53:03  tom
 * 	Bug 5204 - Get server address to print if faulty.
 * 	[1993/02/03  16:52:33  tom]
 * 
 * Revision 1.1.2.4  1993/01/18  22:19:11  jwade
 * 	Need to correct the spelling in the word intersecting in syslog output.
 * 	[1993/01/18  17:03:46  jwade]
 * 
 * Revision 1.1.2.3  1992/12/30  16:39:31  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:23:09  htf]
 * 
 * Revision 1.1.2.2  1992/12/15  14:37:39  tom
 * 	Bug 5465 - increment sync-complete counter first thing in
 * 	LogEventSynchCompleted().
 * 	[1992/12/15  00:10:49  tom]
 * 
 * Revision 1.1  1992/01/19  15:34:43  devrcs
 * 	Initial revision
 * 
 * $EndLog$
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
 */

#include <stdio.h>

#include <dce/dce.h>
#include <dts.h>
#include <dtss_includes.h>
#include <dtss_service.h>
#include <dcedtssvc.h>
#include <dcedtsmsg.h>
#include <dcedtsmac.h>

extern ShmBuf *shmPtr;
extern Sdb    sdb;

UTCValue One = UTCinit(1,0);

#define INCREMENT(i)		\
    UTCadd(&shmPtr->counters[(i)], &One, &shmPtr->counters[(i)])


void LogEventClockSet(
    utc_t	*newTime,
    utc_t	*oldTime
)
{
    char	asctime1[UTC_MAX_STR_LEN+1];
    char	asctime2[UTC_MAX_STR_LEN+1];

    (void)utc_ascanytime(asctime1, UTC_MAX_STR_LEN, oldTime);
    (void)utc_ascanytime(asctime2, UTC_MAX_STR_LEN, newTime);
    dce_svc_printf(DTS_S_CLOCKSET_MSG, asctime1, asctime2);

    INCREMENT(K_CLOCK_SET_CNTR);
}


void LogEventLocalFault(
    utc_t		*computedTime,
    utc_t		*localTime,
    unsigned long	clockset
)
{
    char		asctime1[UTC_MAX_STR_LEN+1];
    char		asctime2[UTC_MAX_STR_LEN+1];

    (void)utc_ascanytime(asctime1, UTC_MAX_STR_LEN, localTime);
    (void)utc_ascanytime(asctime2, UTC_MAX_STR_LEN, computedTime);

    if (clockset)
	dce_svc_printf(DTS_S_ADJUSTLOCAL_MSG, asctime1, asctime2);
    else
	dce_svc_printf(DTS_S_RESETLOCAL_MSG, asctime1, asctime2);

    INCREMENT(K_LOCAL_FAULT_CNTR);
}


void LogEventFaultyServer(
    utc_t		*computedTime
)
{
    unsigned char	*serverName;
    char		asctime1[UTC_MAX_STR_LEN+1];
    char		asctime2[UTC_MAX_STR_LEN+1];
    error_status_t	status;
    ResponseElement	*rp;
    utc_t		serverTime;
    UTCValue		Tcomp;
    ExtInacc		Icomp;

    INCREMENT(K_FAULTY_SERVER_CNTR);

    if (utc_comptime(&Tcomp, &Icomp, (long *)NULL, computedTime))
	return;

    (void)utc_ascanytime(asctime1, UTC_MAX_STR_LEN, computedTime);
    serverName = NULL;

    /* Loop around the *LOCAL* set and for each server entry,
     * obtain its name and reported time. */
    QloopBegin(&sdb.localSet.respQ, &rp)
    {
	if (!IsIntervalIntersection(&Tcomp,      /* Log only faulty servers */
			    &Icomp, &rp->Tserv, &rp->Iserv)) {
	    serverTime = *(utc_t *)&rp->serverTime;
	    (void)utc_ascanytime(asctime2, UTC_MAX_STR_LEN, &serverTime);

	    /* Get server name */
	    rpc_mgmt_inq_server_princ_name(rp->serverPtr->bindPtr->handle,
		  rpc_c_authn_default, &serverName, &status);

	    dce_svc_printf(DTS_S_LOCAL_NOINTERSECT_MSG,
		serverName, asctime2, asctime1);

	    if (serverName != NULL)
		rpc_string_free(&serverName, &status);

	}
    }
    QloopEnd;

    /* Loop around the *GLOBAL* set and for each server entry,
     * obtain its name and reported time. */
    QloopBegin(&sdb.globalSet.respQ, &rp)
    {
	if (!IsIntervalIntersection(&Tcomp,      /* Log only faulty servers */
		&Icomp, &rp->Tserv, &rp->Iserv)) {
	    serverTime = *(utc_t *)&rp->serverTime;
	    (void)utc_ascanytime(asctime2, UTC_MAX_STR_LEN, &serverTime);

	    /* Get server name */
	    rpc_mgmt_inq_server_princ_name(rp->serverPtr->bindPtr->handle,
		  rpc_c_authn_default, &serverName, &status);

	    dce_svc_printf(DTS_S_GLOBAL_NOINTERSECT_MSG,
		serverName, asctime2, asctime1);
	    if (serverName != NULL)
		rpc_string_free(&serverName, &status);
	}
    }
    QloopEnd;
}


void LogEventSynchCompleted(
    utc_t	*computedTime,
    utc_t	*syncTime,
    int		numRecvd
)
{
    utc_t	Adjustment;
    utc_t       jT1, jT2;
    char	asctime1[UTC_MAX_STR_LEN+1];
    char	asctime2[UTC_MAX_STR_LEN+1];

    INCREMENT(K_SYNCH_COMPLETED_CNTR);

    /* compute the adjustment, use a zero inacc in the adjustment time. */
    if (utc_subtime(&Adjustment, computedTime, syncTime) || 
	utc_pointtime((utc_t *)&jT1, &Adjustment, (utc_t *)&jT2, 
		      &Adjustment))
	return;

    (void)utc_ascanytime(asctime1, UTC_MAX_STR_LEN, computedTime);
    (void)utc_ascreltime(asctime2, UTC_MAX_STR_LEN, &Adjustment);
    dce_svc_printf(DTS_S_SYNC_COMPLETE_MSG, asctime1, asctime2);
}

void LogEventUpdateTime(
    utc_t	*newTime,
    utc_t	*oldTime
)
{
    char	asctime1[UTC_MAX_STR_LEN+1];
    char	asctime2[UTC_MAX_STR_LEN+1];

    INCREMENT(K_UPDATE_COMPLETED_CNTR);

    (void)utc_ascanytime(asctime1, UTC_MAX_STR_LEN, oldTime);
    (void)utc_ascanytime(asctime2, UTC_MAX_STR_LEN, newTime);
    dce_svc_printf(DTS_S_UPDATE_COMPLETE_MSG, asctime1, asctime2);
}


void LogEventTooFewServers(
    unsigned long	serversAvail,
    unsigned long	serversExpected
)
{
#ifdef __hpux /* Fix for CHFts15011 */
    if (DCE_SVC_DEBUG_ATLEAST(dts_svc_handle, dts_s_states, 1)) 
#endif
    dce_svc_printf(DTS_S_TOO_FEW_SERVERS_MSG, serversAvail, serversExpected);
    INCREMENT(K_TOO_FEW_CNTR);
}

void LogEventStartupCompleted(
    unsigned long	clockSet
)
{
    if (clockSet)
	dce_svc_printf(DTS_S_STARTUP_DONE_SET_MSG);
    else
	dce_svc_printf(DTS_S_STARTUP_DONE_MSG);
    INCREMENT(K_STARTUP_COMPLETED_CNTR);
}

void LogEventDiffEpoch (
    BadEpochSet		*badEpochSet
)
{
    BadEpochElement	*badEpochPtr;

    /* If the epoch queue is empty (there are no servers with a different
     * epoch) then return immediately.  Otherwise log the event. */
    if (IsQempty(badEpochSet->serverQ))
	return;

    /* Loop around the set of Different Epoch Servers and append
     * them to the NMVL list.  Then delete the entry from the queue
     * since it will no longer be needed. */
    QloopBegin(badEpochSet, &badEpochPtr)
    {
	dce_svc_printf(DTS_S_DIFFERENT_EPOCH_MSG,
	       shmPtr->mgtParams.epochNumber, badEpochPtr->epochNumber,
	       badEpochPtr->server.serverName.serverTextName);

	/* remove the entry from the queue and free memory. */
	Qremove(badEpochPtr);
	DTSSfree((VOID *)badEpochPtr);
    }
    QloopEnd;

    INCREMENT(K_DIFFERENT_EPOCH_CNTR);
}


void LogEventShutdownComplete(void)
{
    dce_svc_printf(DTS_S_SHUTDOWN_COMPLETE_MSG);
    INCREMENT(K_SHUTDOWN_COMPLETED_CNTR);
}


void LogEventTimeRep(
    ServerName	*serverName
)
{
    if (serverName->selector == SNtext)
	dce_svc_printf(DTS_S_BAD_TIMESTAMP_MSG,
	    serverName->serverName.serverTextName);
    INCREMENT(K_TIME_REPS_CNTR);
}

void LogEventNoGlobalServers(void)
{
    dce_svc_printf(DTS_S_NO_GLOBAL_SERVERS_MSG);
    INCREMENT(K_NO_GLOBAL_CNTR);
}

void LogEventServerNotResp(
    ServerName		*serverPtr,
    unsigned int	errCode,
    char		*errMessage
)
{
    /* only log this event for global servers */
    if (serverPtr->selector != SNdnsFull)
	return;

    if (serverPtr->selector == SNtext) {
	dce_svc_printf(DTS_SERVER_NOT_RESPONDING_MSG,
	    serverPtr->serverName.serverTextName, strerror((int)errCode));
    }

    INCREMENT(K_NOT_RESPONDING_CNTR);
}

void LogEventEpochDeclared(
    epoch_t	oldEpoch,
    epoch_t	newEpoch,
    utc_t	*oldTime,
    utc_t	*newTime
)
{
    dce_svc_printf(DTS_SERVER_EPOCH_CHANGED_MSG, oldEpoch, newEpoch);
    INCREMENT(K_EPOCH_DECLARATION_CNTR);
}

void LogEventTPfailure(void)
{
    dce_svc_printf(DTS_S_PROVIDER_FAILED_MSG);
    INCREMENT(K_TIME_PROVIDER_CNTR);
}

void LogEventTPtimeout(void)
{
    dce_svc_printf(DTS_S_PROVIDER_TIMEOUT_MSG);
    INCREMENT(K_TIME_PROVIDER_TIMEOUT_CNTR);
}

void LogEventSystemError(
    int		errorNo
)
{
    dce_svc_printf(DTS_S_SYSTEM_ERROR_MSG, strerror(errorNo));
    INCREMENT(K_SYSTEM_ERROR_CNTR);
}


static char big_buff[ 512 ] ;

void LogEventDCEerror(
    error_status_t	dceError,
    char		*message
)
{
    unsigned char	buff[dce_c_error_string_len];
    int			status;

#ifdef DCE_DEBUG
    sprintf(big_buff, "%s: ", message) ;
#endif
    dce_error_inq_text(dceError, buff, &status);
    strcat(big_buff, (char *)buff) ;
    dce_svc_printf(DTS_S_DCE_ERROR_MSG, big_buff);
    big_buff[0] = '\0';
}

void LogEventNoLANProfile(void)
{
    dce_svc_printf(DTS_S_NO_PROFILE_MSG);
}

void LogEventServerNotInGroup(
    char	*serverName,
    char	*groupName,
    int		localServer
)
{
    dce_svc_printf(DTS_S_NOT_IN_GROUP_MSG, serverName, groupName);
    if (localServer) {
	INCREMENT(K_LOC_SERV_NOT_TSG_MEMBER_CNTR);
    }
    else {
	INCREMENT(K_SERV_NOT_TSG_MEMBER_CNTR);
    }
}
