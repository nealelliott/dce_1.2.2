/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: gdirmain.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:52  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:15  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:50:24  takano
 * 	First Submit
 * 	[1996/02/21  07:50:11  takano]
 * 
 * $EndLog$
 */
#include "gdir.h"

/*********************************************************** EnglishComment **/
/* ID = fnAdminThread                                                        */
/*                                                                           */
/* name = Directory synchronization main routine                             */
/* func = Invokes initialization, and synchronization.                       */
/* io =  void                                                                */
/* return =   void                                                           */
/* common = iAdmin_status = THREAD_ABENDED: failed to initialize             */
/*          iAdmin_status = THREAD_TIMEOUT: failed to initialize all the     */
/*                                          volume (needs to retry)          */
/*          iAdmin_status = THREAD_NACTIVE: termination normally ended       */
/* note = none                                                               */
/* date = December 2, 1994 by Ryuji Shinchi                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
void fnAdminThread( void )
{
	int		iMyStatus;

	int		ilAdmin_status;
	int		iRet;
	int		iAvailableVolume = 0;

    GW_tsh_functrace("fnAdminThread", GW_FUNC_START, 0);

	iMyStatus = DMSTATinit;
	iAdmin_tid = GetThreadID();


	if ( iRet = DMmainSetUp() ) {

		ilAdmin_status = THREAD_ABENDED;
		goto labelQuit;
	}


	if ( bTimeSync ) {
		Timesync( &gComS );
	}


	iMyStatus = DMSTATmakeup;
	iRet = DMinit( &iAvailableVolume );
	switch ( iRet ) {
	case 0:
		break;
	case EDMinitfatal:
		ilAdmin_status = THREAD_ABENDED;
		goto labelQuit;
	case EDMshutdownreq:
		ilAdmin_status = THREAD_ABENDED;
		goto labelDMunload;
	case EDMagentdie:
		ilAdmin_status = THREAD_ABENDED;
		goto labelQuit;
	default:
		break;
	}
	if ( iAvailableVolume == 0 ) {
		switch ( iRet ) {
		case EDManytimeout:
			ilAdmin_status = THREAD_TIMEOUT;
			goto labelQuit;
		default:
			ilAdmin_status = THREAD_ABENDED;
			goto labelQuit;
		}
	}

	iMyStatus = DMSTATinterval;
	WaitOnLocalSemaphore( uiAdminStat_sh );
	if ( iAdmin_status == THREAD_NOW_TERM ) {
		SignalLocalSemaphore( uiAdminStat_sh );
		ilAdmin_status = THREAD_NACTIVE;
		goto labelDMunload;
	} else {
		iAdmin_status = THREAD_WORKING;
		SignalLocalSemaphore( uiAdminStat_sh );
	}

	while ( 1 ) {
		if ( ShutdownReq() ) {
			break;
		}
		delay( DMDefaultShortInterval );
		switch ( iWatch_status ) {
		case THREAD_SLEEP:
			goto labelDMinterval;
		case THREAD_WORKING:
			goto labelDMinterval;
		}
	}


labelDMinterval:
	iMyStatus = DMSTATinterval;
	iRet = DMSleep();
	switch ( iRet ) {
	case EDMshutdownreq:
		ilAdmin_status = THREAD_NACTIVE;
		goto labelDMunload;
		break;
	case EDMagentdie:
		iRet = DMreconnect( DMReconnectWatch );
		if ( iRet == EDMshutdownreq ) {
			ilAdmin_status = THREAD_NACTIVE;
			goto labelDMunload;
		}
		break;
	default:
		break;
	}

labelDMsync:

	if ( bTimeSync ) {
		Timesync( &gComS );
	}

	iMyStatus = DMSTATsync;
	iRet = DMsync();
	switch ( iRet ) {
	case EDMshutdownreq:
		ilAdmin_status = THREAD_NACTIVE;
		goto labelDMunload;
		break;
	case EDMagentdie:
		iRet = DMreconnect( DMReconnectWatch );
		if ( iRet == EDMshutdownreq ) {
			ilAdmin_status = THREAD_NACTIVE;
			goto labelDMunload;
		}
		goto labelDMsync;
	default:
		break;
	}

	goto labelDMinterval;

labelDMunload:
	iMyStatus = DMSTATshutdown;

	while( iAdmin_status != THREAD_NOW_TERM ) {
		delay( DMDefaultShortInterval );
	}
	DMclear( DMDelModeClear );


	DMmainSetdown();

	GW_tsh_functrace("fnAdminThread", GW_FUNC_END, THREAD_NACTIVE);
	WaitOnLocalSemaphore( uiAdminStat_sh );
	iAdmin_status = ilAdmin_status;
	SignalLocalSemaphore( uiAdminStat_sh );
	return;

labelQuit:

	DMmainSetdown();

	GW_tsh_functrace("fnAdminThread", GW_FUNC_END, ilAdmin_status);
	WaitOnLocalSemaphore( uiAdminStat_sh );
	iAdmin_status = ilAdmin_status;
	SignalLocalSemaphore( uiAdminStat_sh );
	return;
}


/*********************************************************** EnglishComment **/
/* ID = DMreconnect                                                          */
/* name = Admin thread reconnection                                          */
/* func = Recommects Admin threads.                                          */
/* io = void                                                                 */
/* return =  0: Admin thread successfully reconnected                        */
/*           EDMshutdownreq: UNLOAD was requested while reconnection is      */
/*                           attempted.                                      */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = April 16, 1995 by Ryuji Shinchi                                    */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DMreconnect( int iStat )
{
	int		iRet;

	if ( bUnload )
		return EDMshutdownreq;

    GW_tsh_functrace("DMreconnect", GW_FUNC_START, 0);
	GW_Message( DFAM_CON, 0, LOG_YES, KDDS11636 );


	while ( iStat == DMReconnectWatch ) {
		if ( ShutdownReq() ) {
			GW_tsh_functrace("DMreconnect", GW_FUNC_END, EDMshutdownreq);
			return EDMshutdownreq;
		}
		if ( iWatch_status == THREAD_SLEEP ) {
			break;
		}
		delay( DMDefaultShortInterval );
	}


	ResumeAllUserThread( BAD_ADMIN_CONN );

	while (1) {

		if ( ShutdownReq() ) {
			GW_tsh_functrace("DMreconnect", GW_FUNC_END, EDMshutdownreq);
			return EDMshutdownreq;
		}



		SetDeathFlag();
		if ( igDCELoginFlag & pAgent_sp->bAdmin_live ) {
			iRet = DMDCELogout();
		}
		igDCELoginFlag = 0;

		if ( igAdmConReqFlag || igAdmSockfdFlag ) {
			iRet = AgentDisConnectReq( &gComS );
			igAdmConReqCount--;
			if ( iRet ) {
			}
		}
		igAdmConReqFlag = 0;
		igAdmSockfdFlag = 0;





		if ( ShutdownReq() ) {
			GW_tsh_functrace("DMreconnect", GW_FUNC_END, EDMshutdownreq);
			return EDMshutdownreq;
		}
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11638 );


		gComS.uiCConn_sh = uigSemHandle;
		gComS.uiConnNum = (LONG)0;
		gComS.pTsh_Trace = pAdminTracep;
		gComS.uiTsh_Size = DMtraceSize;

		igAdmConReqCount++;
		iRet = DMconnect( pAgent_sp, GW_CONTROL_CON, &gComS );
		igAdmConReqFlag = 1;
		switch ( iRet ) {
		case 0:
			igAdmSockfdFlag = 1;
			break;
		case EDMshutdownreq:
			GW_tsh_functrace("DMreconnect", GW_FUNC_END, EDMshutdownreq);
			return EDMshutdownreq;
		default:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11607 );
			break;
		}

		if ( igAdmSockfdFlag ) {
			iRet = DMDCELogin();
			if ( iRet ) {
				if ( ShutdownReq() ) {
					GW_tsh_functrace("DMreconnect", GW_FUNC_END, EDMshutdownreq);
					return EDMshutdownreq;
				}
				GW_Message( DFAM_CON, 0, LOG_YES, KDDS11605 );
			} else {
				ResetDeathFlag();
				igDCELoginFlag = 1;

				if ( ShutdownReq() ) {
					GW_tsh_functrace("DMreconnect", GW_FUNC_END, EDMshutdownreq);
					return EDMshutdownreq;
				}


				if ( iStat == DMReconnectWatch ) {
					WaitOnLocalSemaphore(uiWatchStat_sh);
					iRet = ResumeThread( iWatch_tid );
					SignalLocalSemaphore(uiWatchStat_sh);
					if (iRet != ESUCCESS) {

					} else {
						while ( 1 ) {
							if ( ShutdownReq() ) {
								GW_tsh_functrace("DMreconnect", GW_FUNC_END, EDMshutdownreq);
								return EDMshutdownreq;
							}
							if ( iWatch_status != THREAD_SLEEP ) {
								break;
							}
							delay( DMDefaultShortInterval );
						}
					}
				}
				GW_Message( DFAM_CON, 0, LOG_YES, KDDS11637 );
				GW_tsh_functrace("DMreconnect", GW_FUNC_END, 0);
				return 0;
			}
		}

		delay( DMDefaultReconnectInterval );
	}

}

/*********************************************************** EnglishComment **/
/* ID = DMmainSetUp                                                          */
/*                                                                           */
/* name = Directory synchronization main routine (setup part of directory    */
/*        sync module)                                                       */
/* func = (1) Allocates a trace area.                                        */
/*        (2) Gets a semaphore handle for Admin connection.                  */
/*        (3) Establishes Admin connection.                                  */
/*        (4) Logs in to DCE by Admin connection.                            */
/*                                                                           */
/* i/o  = void                                                               */
/* return = 0: normal end                                                    */
/*          EDMmalloc                                                        */
/*          EDMinitfatal                                                     */
/* common = igSemhandleFlag : 1: semaphore obtained                          */
/*                            0: semaphore not obtained                      */
/*          uigSemHandle    : obtained semaphore handle                      */
/*          gComS.uiCConn_sh: setting uigSemHandle                           */
/*          gComS.uiConnNum : setting a connection number                    */
/*          gComS.pTsh_Trace: pointer to a trace area                        */
/*          gComS.uiTsh_Size: size of the trace area                         */
/*          igAdmConReqFlag : 1: issued the Admin connection request         */
/*                            0: not issued the request                      */
/*          igAdmSockfdFlag : 1: Admin connection is established             */
/*                            0: not established                             */
/*          igDCELoginFlag  : 1: successfully logged in to DCE               */
/*                            0: DCE login failed                            */
/* :::                                                                       */
/* note =  initialize DFAMVolumeTable.synctime to 0                          */
/*         get semaphore handle                                              */
/*         get admin connection                                              */
/* date = December 21, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DMmainSetUp( void )
{
	int					iErr = 0;
	int					iRet;
	struct GVolume_t	*pVolTbl;

    GW_tsh_functrace("DMmainSetUP", GW_FUNC_START, 0);


	AccessListRoot.iAccessListMode = DMAccessListModeInterval;
	AccessListRoot.pAccessFile = NULL;


	SetDeathFlag();
	igAdmConReqFlag = 0;
	igAdmSockfdFlag = 0;
	igDCELoginFlag = 0;
	igSemHandleFlag = 0;

	igAdmConReqCount = 0;


	for ( pVolTbl = pVolume_sp; pVolTbl != NULL; pVolTbl = pVolTbl->pUshirop ) {
		pVolTbl->uiSynctime = 0;
	}

	pAdminTracep = GW_malloc(ADM_TR_SIZE);
	if ( pAdminTracep == NULL ) {
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11613 );
		GW_tsh_functrace("DMmainSetUP", GW_FUNC_END, EDMmalloc);
		return EDMmalloc;
	}


	uigSemHandle = OpenLocalSemaphore( (LONG)1 );
	igSemHandleFlag = 1;
	gComS.uiCConn_sh = uigSemHandle;


	gComS.uiConnNum = (LONG)0;
	gComS.pTsh_Trace = pAdminTracep;
	gComS.uiTsh_Size = DMtraceSize;


	GW_Message( DFAM_CON, 0, LOG_YES, KDDS11638 );
	igAdmConReqCount++;
    iRet = DMconnect( pAgent_sp, GW_CONTROL_CON, &gComS );
	igAdmConReqFlag = 1;
	switch( iRet ) {
	case 0:
		igAdmSockfdFlag = 1;
		break;
	case EDMshutdownreq:
		return EDMinitfatal;
	default:
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11607 );
		GW_tsh_functrace("DMmainSetUP", GW_FUNC_END, EDMinitfatal);
		return EDMinitfatal;
	}

	iRet = DMDCELogin();
	if ( iRet ) {
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11605 );
		GW_tsh_functrace("DMmainSetUP", GW_FUNC_END, EDMinitfatal);
		return EDMinitfatal;
	} else {
		ResetDeathFlag();
		igDCELoginFlag = 1;
	}

	GW_tsh_functrace("DMmainSetUP", GW_FUNC_END, iErr);
	return iErr;
}

/*********************************************************** EnglishComment **/
/* ID = DMmainSetDown                                                        */
/*                                                                           */
/* name = Releasing resources                                                */
/* func = Frees the assigned resources                                       */
/* i/o  = void                                                               */
/* return = always 0                                                         */
/* common = Frees the resources with refering to the followings:             */
/*          igSemHandleFlag                                                  */
/*          igAdmConReqFlag                                                  */
/*          igAdmSockfdFlag                                                  */
/*          igDCELoginFlag                                                   */
/*          pAgent_sp->bAdmin_live                                           */
/*          gComS.pTsh_Trace                                                 */
/* :::                                                                       */
/* note = relsase semaphore handle                                           */
/*        relsase admin connection                                           */
/*        relsase semaphore handle                                           */
/* date = December 21, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DMmainSetdown( void )
{
	int					iErr = 0;
	int					iRet;

    GW_tsh_functrace("DMmainSetdown", GW_FUNC_START, 0);


	AccessListRoot.iAccessListMode = DMAccessListModeTerm;
	DMFreeAccessFileList();

	if ( igDCELoginFlag & pAgent_sp->bAdmin_live ) {
		iRet = DMDCELogout();
		if ( iRet ) {

		}
	}

	if ( igAdmConReqFlag || igAdmSockfdFlag ) {
		iRet = AgentDisConnectReq( &gComS );
		igAdmConReqCount--;
		if ( iRet ) {
			iErr = iRet;
		}
	}


	if ( igSemHandleFlag ) {
		CloseLocalSemaphore( uigSemHandle );
		igSemHandleFlag = 0;
	}

	WaitOnLocalSemaphore( uiTrace_sh );
	if ( pAdminTracep != NULL ) {
		DMfree( pAdminTracep, "DMmainSetdown" );
		pAdminTracep = NULL;
	}
	SignalLocalSemaphore( uiTrace_sh );

	GW_tsh_functrace("DMmainSetdown", GW_FUNC_END, iErr);
	return iErr;
}

/*********************************************************** EnglishComment **/
/* ID = DMSleep                                                              */
/*                                                                           */
/* name = Waiting for a designated period                                    */
/* func = Waits for the period of uiDirSyncInterval                          */
/*        If UNLOAD was requested while waiting, returns to the caller       */
/*        immediately.  If an error is detected in the Watchdog connection,  */
/*        returns to the caller immediately, too.                            */
/*                                                                           */
/* i/o  =   void                                                             */
/* return = 0: continue to execute dir sync                                  */
/*	EDMshutdown:                                                             */
/*	EDMagentdie:                                                             */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = December 21, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int	DMSleep( void )
{
	clock_t				uiDirSyncIntervalPer10;
	static clock_t		uiIntervalStart = 0;
	static int			iConErrorAtLastIntervalFlag = 0;

    GW_tsh_functrace("DMSleep", GW_FUNC_START, 0);

	if ( uiDirSyncInterval < 100 ) {
		uiDirSyncIntervalPer10 = 1;
	} else {
		uiDirSyncIntervalPer10 = uiDirSyncInterval / 10;
	}

	if ( !iConErrorAtLastIntervalFlag ) {
		uiIntervalStart = clock();
	}

	while ( clock() < (uiIntervalStart + uiDirSyncIntervalPer10) ) {
		if ( ShutdownReq() ) {
			GW_tsh_functrace("DMSleep", GW_FUNC_END, EDMshutdownreq);
			return EDMshutdownreq;
		}
		if ( AgentDie() ) {
			iConErrorAtLastIntervalFlag = 1;
			GW_tsh_functrace("DMSleep", GW_FUNC_END, EDMagentdie);
			return EDMagentdie;
		}
		delay( DMDefaultShortInterval );
	}

	iConErrorAtLastIntervalFlag = 0;
	GW_tsh_functrace("DMSleep", GW_FUNC_END, 0);
	return 0;
}


/*********************************************************** EnglishComment **/
/* ID = DMDCELogin                                                           */
/*                                                                           */
/* name = DCE login for Admin connection                                     */
/* func = Handles DCE login for Admin connection.                            */
/* i/o  = void                                                               */
/*                                                                           */
/* return = 0: normal end                                                    */
/*          EDMadminid                                                       */
/*          EDMmalloc                                                        */
/*          EDMbindery                                                       */
/*          EDMbug                                                           */
/*          EDMmalloc                                                        */
/*          EDMagentdie                                                      */
/*          EDMtcpip                                                         */
/*          EDMlogin                                                         */
/*          <-DCE_Login                                                      */
/*          errcode :                                                        */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = December 21, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int	DMDCELogin( void )
{
	int		iErr = 0;
	int		iRet;
	int		iLinErr;
	char	acDCEuid[MAX_DCEUSERNAME+1];
	char	acDCEpwd[MAX_DCEPASSWORD+1];

    GW_tsh_functrace("DMDCELogin", GW_FUNC_START, 0);


	iRet = GetDCEUserName( DFAMADMIN, acDCEuid );
	switch ( iRet ) {
	case 0:
		break;
	case -1:
	case -2:
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11604 );
		GW_tsh_functrace("DMDCELogin", GW_FUNC_END, EDMadminid);
		return EDMadminid;
	case -3:
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11613 );
		GW_tsh_functrace("DMDCELogin", GW_FUNC_END, EDMmalloc);
		return EDMmalloc;
	case -4:
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11215, EDMbindery );
		GW_tsh_functrace("DMDCELogin", GW_FUNC_END, EDMbindery);
		return EDMbindery;
	default:
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11624, iRet );
		GW_tsh_functrace("DMDCELogin", GW_FUNC_END, EDMbug);
		return EDMbug;
	}


	iRet = GetDCEPassword( DFAMADMIN, acDCEpwd );
	switch ( iRet ) {
	case 0:
		break;
	case -1:
	case -2:
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11604 );
		GW_tsh_functrace("DMDCELogin", GW_FUNC_END, EDMadminid);
		return EDMadminid;
	case -3:
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11613 );
		GW_tsh_functrace("DMDCELogin", GW_FUNC_END, EDMmalloc);
		return EDMmalloc;
	case -4:
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11215, EDMbindery );
		GW_tsh_functrace("DMDCELogin", GW_FUNC_END, EDMbindery);
		return EDMbindery;
	default:
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11624, iRet );
		GW_tsh_functrace("DMDCELogin", GW_FUNC_END, EDMbug);
		return EDMbug;
	}


	iRet = DCE_Login( &gComS, acDCEuid, acDCEpwd, &iLinErr );
	if ( iRet ) {

		memset( acDCEuid, 0, MAX_DCEUSERNAME+1 );
		memset( acDCEpwd, 0, MAX_DCEPASSWORD+1 );

		switch ( iLinErr ) {

		case DENETWORKENV:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11651 );
			iErr = EDMunknown;
			break;
		case DEREFUSEAGT:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653 );
			iErr = EDMagentdie;
			break;
		case DETIMEOUTAGT:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11654 );
			iErr = EDMagentdie;
			break;
		case DECOMNOMEM:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11613 );
			iErr = EDMmalloc;
			break;
		case DEDFAMDCESYSTEM:	case DEINVALSOCKET: case DERESPONCEDATA:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11624, iLinErr );
			iErr = EDMbug;
			break;
		case DEUNLOADTCPIP:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11204 );
			iErr = EDMtcpip;
			break;
		case DEDFAMTONOVELL:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11681, DEDFAMTONOVELL);
			iErr = EDMunknown;
			break;

		case DENOKFILE:	case DENOKEY:	case DEKEYSIZE:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11206 );
			iErr = EDMlogin;
			break;
		case DEUPDATEKEY:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11207 );
			iErr = EDMlogin;
			break;
		case DENOAGTKF:	case DEAGTKSIZE:	case DENOGWENT:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11208 );
			iErr = EDMlogin;
			break;
		case DEINVALACNT:	case DELOCAL:	case DENOACNT:
		case DEINVALPSWD:	case DENULLPSWD:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11661 );
			iErr = EDMlogin;
			break;
		case DEINVALAGT:	case DEINVALGTWY:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11210 );
			iErr = EDMlogin;
			break;
		case DEUNAVAILSRV:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11211 );
			iErr = EDMlogin;
			break;
		case DEPERM:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11662 );
			iErr = EDMlogin;
			break;
		case DENOMEM:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11663, DENOMEM );
			iErr = EDMmalloc;
			break;
		case DEDFAMTERM:
			iErr = EDMshutdownreq;
			break;
		default:
			if ( iLinErr < 1200 ) {
				GW_Message( DFAM_CON, 0, LOG_YES, KDDS11663, iLinErr );
				iErr = iLinErr;
			} else {
				GW_Message( DFAM_CON, 0, LOG_YES, KDDS11624, iLinErr );
				iErr = EDMbug;
			}
			break;
		}
	}

	GW_tsh_functrace("DMDCELogin", GW_FUNC_END, iErr);
	return iErr;
}

/*********************************************************** EnglishComment **/
/* ID = DMDCELogout                                                          */
/*                                                                           */
/* name = DCE logout for Admin connection                                    */
/* func = Handles DCE logout for Admin connection.                           */
/* i/o  = void                                                               */
/*                                                                           */
/* return = 0: normal end                                                    */
/*          EDMlogout                                                        */
/*          <-DCE_Login                                                      */
/*          errcode :                                                        */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = December 21, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DMDCELogout( void )
{
	int		iDFSErrno;
	int		iRet;

    GW_tsh_functrace("DMDCELogout", GW_FUNC_START, 0);

	if ( iRet = DFSLoutReq( &iDFSErrno, &gComS ) ) {
		GW_tsh_functrace("DMDCELogout", GW_FUNC_END, EDMlogout);
		return EDMlogout;
	}

	GW_tsh_functrace("DMDCELogout", GW_FUNC_END, 0);
	return 0;
}


/*********************************************************** EnglishComment **/
/* ID = DMconnect                                                            */
/*                                                                           */
/* name = Establishing Admin connection                                      */
/* func = Establishes the Admin connection                                   */
/*                                                                           */
/* i/o  = void                                                               */
/* return = 0: normal end                                                    */
/*          EDMhosts                                                         */
/*          EDMservices                                                      */
/*          EDMagentdie                                                      */
/*          EDMmalloc                                                        */
/*          EDMtcpip                                                         */
/*          EDMunknown                                                       */
/*          EDMbug                                                           */
/*           errcode :                                                       */
/*                                                                           */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = May 13, 1995 by Ryuji Shinchi                                      */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DMconnect(	struct GAgent_t *pAgent,
				unsigned short ushConAssort,
				void *pComTbl)
{
	int		iRet;
	int		iErr = 0;

    GW_tsh_functrace("DMconnect", GW_FUNC_START, 0);

	iRet = AgtConnectReq( pAgent, ushConAssort, pComTbl );
	switch ( iRet ) {
	case 0:
		iErr = 0;
		break;

	case DEHOSTFILE:
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11202 );
		iErr = EDMhosts;
		break;
	case DESERVICESFILE:
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11203 );
		iErr = EDMservices;
		break;
	case DENETWORKENV:
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11651 );
		iErr = EDMagentdie;
		break;
	case DEREFUSEAGT:
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11652 );
		iErr = EDMagentdie;
		break;
	case DETIMEOUTAGT:
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11654 );
		iErr = EDMagentdie;
		break;
	case DERESPONCEDATA:
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11652 );
		iErr = EDMagentdie;
		break;
	case DECOMNOMEM:
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11613 );
		iErr = EDMmalloc;
		break;
	case DEDFAMDCESYSTEM:	case DENONBKIO:
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11624, iRet );
		iErr = EDMbug;
		break;
	case DEUNLOADTCPIP:
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11204 );
		iErr = EDMtcpip;
		break;
	case DEDFAMTONOVELL:
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11681, DEDFAMTONOVELL);
		iErr = EDMunknown;
		break;
	case DECIPHER:
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11667 );
		iErr = EDMcipher;
		break;
	case DENOSUPPORT:
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11668 );
		iErr = EDMtrans;
		break;
	case DEDFAMTERM:
		iErr = EDMshutdownreq;
		break;
	default:
		if ( iRet < 1200 ) {
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11663, iRet );
			iErr = iRet;
		} else {
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11624, iRet );
			iErr = EDMbug;
		}
		break;
	}
	GW_tsh_functrace("DMconnect", GW_FUNC_END, iErr);
	return iErr;
}
