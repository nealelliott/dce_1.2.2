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
 * $Log: grcvwdog.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:39  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:53  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:07:34  takano
 * 	First Submit
 * 	[1996/02/21  08:07:29  takano]
 * 
 * $EndLog$
 */
#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "dfammsg.h"
#include "dfamer.h"

#define  AGENTTBL_MAX   1

#define  HEALTH_CHECK   0X0012
#define  WD_GW_MESSAGE  0X0013
#define  WD_IAM_HEALTY  0X8012
#define  AGMSG_MAXLEN   256
#define  WAITIME        5
#define  SRECV_BUFSIZE  512
#define  AGTCON_SIZE    16
#define  TR_SIZE        WDG_TR_SIZE
#define  MTYPE          4
#define  MLEN           4
#define  MCONS          4
#define  WDMSGMAXCON    100
#define  MSG_TYPELEN    4
#define  MSG_NUMLEN     4
#define  MSG_LENLEN     4
#define  MSG_MAXLEN     60
#define  MSG_CLNUMLEN   4
#define  MSG_CONNUMLEN  4

/********************************************************** EnglishComment ***/
/*  ID      = WatchdogThread                                                 */
/*  name    = DFAM Gateway Watchdog Thread Group                             */
/*  func    = Communicating with DFAM Agent Watchdog Process.                */
/*  io      = I don't care. Communication Library(DFAMDCE.NLM)               */
/*            does everything. Lucky!                                        */
/*  return  = no return                                                      */
/*  common  =                                                                */
/*  note    =                                                                */
/*                                                                           */
/* date = September 18, 1995 T.Sawahata                                      */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/
void fnWatchDogThread()
{

	int                    icAgtCR;
	int                    iccode;
	LONG                   uiWatchDog_sh;
	unsigned long          uiInfo;
	int                    iErrno;
	unsigned long          MsgGet ( char* );
	int                    iWDSemCnt;
	int                    iADisConCnt;

	int                    iMsgType;
	int                    iTermTime;
	int                    iMsgNum;
	char                   *pcMsg;
	char                   *pcMsgWork;
	char                   *pcMsgWork2;
	int                    iNumConn;
	int                    iwkConnNum;
	int                    iworkCount;
	char                   *ctimer;
	char                   *ctimer2;
	time_t                 timer;
	unsigned short         usConAssort;
	size_t                 uiMsgSize;

	struct	ComTGroup_t    *pComTbl;
	char                   *pcWD_Msg[20];

	int                    iMyStatus;



	iWDSemCnt    =   0;
	iADisConCnt  =   0;
	if ( pAgent_sp == NULL ){
		GW_tsh_log( 3000, "pAgent_sp=0");
		iMyStatus = THREAD_ABENDED;
		goto EndReturn;
	}


	pcWD_Msg[0]  = NULL;

	pcWD_Msg[1]  = KDDS11810;
	pcWD_Msg[2]  = KDDS11811;
	pcWD_Msg[3]  = KDDS11812;
	pcWD_Msg[4]  = KDDS11813;
	pcWD_Msg[5]  = KDDS11814;
	pcWD_Msg[6]  = KDDS11816;

	pcWD_Msg[7] = KDDS11817;
	pcWD_Msg[8] = KDDS11818;
	pcWD_Msg[9] = KDDS11819;

	pcWD_Msg[10]= NULL;
	pcWD_Msg[11]= NULL;
	pcWD_Msg[12]= NULL;
	pcWD_Msg[13]= NULL;
	pcWD_Msg[14]= NULL;
	pcWD_Msg[15]= NULL;
	pcWD_Msg[16]= NULL;
	pcWD_Msg[17]= NULL;
	pcWD_Msg[18]= NULL;
	pcWD_Msg[19]= NULL;


	iWatch_tgid = GetThreadGroupID();
	iWatch_tid  = GetThreadID();
	tmWatchdogMSGBuflen = iMaxUsersNum * 4 + 100;

	GW_Message ( DFAM_CON,0,LOG_YES,KDDS11820 );

GetUp:


	usConAssort        = GW_WATCHDOG_CON;
	pComTbl = (struct ComTGroup_t *)GW_malloc( sizeof(struct ComTGroup_t) );

	if ( pComTbl == NULL ){
		GW_tsh_log( 3001, "pComTbl=0");
		goto ErrSleep;
	}
	uiWatchDog_sh      = OpenLocalSemaphore(1);
	iWDSemCnt = 1;

	pWatchdogTracep     = GW_malloc(TR_SIZE);
	if (pWatchdogTracep == NULL ) {
		GW_tsh_log( 3002, "pTrace=0");
		goto ErrSleep;
	}
	pComTbl->uiCConn_sh = uiWatchDog_sh;
	pComTbl->pTsh_Trace = pWatchdogTracep;
	pComTbl->uiConnNum  = 0;
	pComTbl->uiTsh_Size = TR_SIZE;


	icAgtCR = AgtConnectReq( pAgent_sp, usConAssort, pComTbl );
	switch ( icAgtCR ) {
	case 0:
		break;
	case DEDFAMTERM:
		GW_tsh_log( 3003, "DEDFAMTERM");
		goto ErrSleep;
	default:
		GW_tsh_log( 3003, "icAgtCR!=0");
		GW_Message( DFAM_CON,0,LOG_YES,KDDS11802 );
		goto ErrSleep;

    }
	iADisConCnt = 1;

	GW_Message ( DFAM_CON,0,LOG_YES,KDDS11821 );

	WaitOnLocalSemaphore( uiWatchStat_sh );
	if ( iWatch_status == THREAD_NOW_TERM ){
		SignalLocalSemaphore( uiWatchStat_sh );
		iMyStatus = THREAD_NACTIVE;
		goto EndReturn;
	}
    iWatch_status = THREAD_WORKING;
	SignalLocalSemaphore( uiWatchStat_sh );

	pAgent_sp->bWatch_live = TRUE;
	pcMsg = GW_malloc( tmWatchdogMSGBuflen );
	if ( pcMsg == NULL ){
		GW_tsh_log( 3004, "pcMsg=0");
		goto ErrSleep;
	}

	while(1){
		if ( bIsConnection_live( pAgent_sp ) == FALSE ) {
			GW_tsh_log( 3005, "bIsCon=FLS");
			goto ErrSleep;
		}
		if ( iWatch_status == THREAD_NOW_TERM ){
			GW_tsh_log( 3006, "iW=NOW_TERM");
			iMyStatus = THREAD_NACTIVE;
			goto EndReturn;
		}
		iccode = WatchDogStatRecvReq( &uiInfo, &uiMsgSize, pcMsg, &iErrno, ( void* )pComTbl );

		switch(iccode){
			case -1:


				switch ( iErrno ) {
				case DEDFAMTERM:
					GW_tsh_log( 3007, "DEDFAMTEERM");
					goto ErrSleep;
				default:
					GW_tsh_log( 3007, "WDSRR ic=-1");
					GW_Message( DFAM_CON,0,LOG_YES,KDDS11803);

					goto ErrSleep;
				}

			case 0:

				continue;

			case 1:


				if ( uiInfo == HEALTHCHK_REQ ) {
					continue;
				}
				else if ( uiInfo == MESSAGE_REQ ) {
					iMsgType = (int)MsgGet( pcMsg );
					switch(iMsgType){
						case 1:
							iTermTime = (int)MsgGet( pcMsg + MSG_TYPELEN );

							pAgent_sp->bLogin_Stop = TRUE;
                            GW_Message( (DFAM_NWC_ALL | DFAM_CON),0,LOG_YES
                                                        , KDDS11808, iTermTime);
                            break;

						case 2:
							GW_Message( (DFAM_NWC_ALL | DFAM_CON),0,LOG_YES,KDDS11809 );
							pAgent_sp->bLogin_Stop = TRUE;
							break;

						case 3:

							iMsgNum = (int)MsgGet( pcMsg + MSG_TYPELEN );

							if( iMsgNum == 0 ){
								pcMsgWork = pcMsg + MSG_TYPELEN + MSG_NUMLEN + MSG_LENLEN;
								strcpy( (pcMsgWork + MSG_MAXLEN), "\0" );

								GW_Message( DFAM_CON, 0, LOG_YES, pcMsgWork );
							} else {
								GW_Message( DFAM_CON, 0, LOG_YES, KDDS11828 );
							}
							break;

						case 4:

							iMsgNum = (int)MsgGet( pcMsg + MSG_TYPELEN );

							iNumConn = (int)MsgGet( pcMsg + MSG_TYPELEN + MSG_NUMLEN );

							pcMsgWork = pcMsg + MSG_TYPELEN + MSG_NUMLEN + MSG_CLNUMLEN
                                            + ( iNumConn * MSG_CONNUMLEN ) + MSG_LENLEN;

							pcMsgWork2 = pcMsg + MSG_TYPELEN + MSG_NUMLEN + MSG_CLNUMLEN;

							if( iMsgNum == 0 ){
								*(pcMsgWork+MSG_MAXLEN) = '\0';
								iworkCount = 1;
								while ( iworkCount <= iNumConn ){
									iwkConnNum = ( int )MsgGet( pcMsgWork2 );
									GW_Message( DFAM_NWC, &iwkConnNum, LOG_YES, pcMsgWork );
									++iworkCount;
									pcMsgWork2 += MSG_CONNUMLEN;
								}
								break;
							} else if ( iMsgNum == 2 ) {
								timer = (time_t )MsgGet( pcMsgWork2 + ( MSG_CLNUMLEN * iNumConn ) );
								ctimer = ctime( &timer );
								ctimer2 = GW_malloc(30);
								if ( ctimer2 == NULL ) {
									GW_tsh_log( 3009, "ctimer2=0");
									goto ErrSleep;
								}
								strcpy( ctimer2, ctimer );
								memset ( (ctimer2 + 24) , '\0', 1 );
								iworkCount = 1;
								while ( iworkCount <= iNumConn ){
									iwkConnNum = (  int )MsgGet( pcMsgWork2 );
									GW_Message( DFAM_NWC, &iwkConnNum, LOG_YES, pcWD_Msg[iMsgNum], ctimer2 );
									++iworkCount;
									pcMsgWork2 += MSG_CONNUMLEN;
									break;
								}
								GW_free( ctimer2, 0 );
							} else if ( ( iMsgNum >= 1 ) && ( iMsgNum <= 9) ) {
								iwkConnNum = ( int )MsgGet( pcMsgWork2 );
								iworkCount = 1;
								while ( iworkCount <= iNumConn ){
									iwkConnNum = ( int )MsgGet( pcMsgWork2 );
									GW_Message( DFAM_NWC, &iwkConnNum, LOG_YES, pcWD_Msg[ iMsgNum ] );
									++iworkCount;
									pcMsgWork2 += MSG_CONNUMLEN;
								}
								break;
							} else {
								iworkCount = 1;
								while ( iworkCount <= iNumConn ){
									iwkConnNum = ( int )MsgGet( pcMsgWork2 );
									GW_Message( DFAM_NWC, &iwkConnNum, LOG_YES, KDDS11828 );
									++iworkCount;
									pcMsgWork2 += MSG_CONNUMLEN;
								}
							}
							break;
						default :
							GW_Message( DFAM_CON, 0, LOG_YES, KDDS11828 );
							break;
						}
					}
		}
	}

ErrSleep:
	if ( !bUnload )
		GW_Message ( DFAM_CON,0,LOG_YES,KDDS11822 );
	pAgent_sp->bWatch_live = FALSE;
	if ( iADisConCnt == 1 ) {
		iccode = AgentDisConnectReq( (void*)pComTbl );
		iADisConCnt = 0;
		if (  iccode != 0  ) {
			GW_tsh_log( 3008, "ADCR ic!=0");
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11803 );
		}
	}
	WaitOnLocalSemaphore( uiTrace_sh );
	if ( pWatchdogTracep != NULL ) {
		GW_free( pWatchdogTracep, 0 );
		pWatchdogTracep = NULL;
	}
	SignalLocalSemaphore( uiTrace_sh );
	GW_free( NULL, iWatch_tid );
	if ( iWDSemCnt == 1 ) {
		CloseLocalSemaphore( uiWatchDog_sh );
		iWDSemCnt = 0;
	}

	WaitOnLocalSemaphore( uiWatchStat_sh );
	if ( iWatch_status == THREAD_NOW_TERM ) {
		SignalLocalSemaphore( uiWatchStat_sh );
		iMyStatus = THREAD_NACTIVE;
		goto EndReturn;
	} else {
		iWatch_status = THREAD_SLEEP;
		SignalLocalSemaphore( uiWatchStat_sh );
		SuspendThread( iWatch_tid );
	}


	WaitOnLocalSemaphore( uiWatchStat_sh );
	if ( iWatch_status == THREAD_NOW_TERM ) {
		SignalLocalSemaphore( uiWatchStat_sh );
		iMyStatus = THREAD_NACTIVE;
		goto EndReturn;
	}
	SignalLocalSemaphore( uiWatchStat_sh );

	pAgent_sp->bLogin_Stop = FALSE;
	GW_Message ( DFAM_CON,0,LOG_YES,KDDS11823 );
	goto GetUp;

EndReturn:
	if ( iADisConCnt == 1 ) {
		GW_Message ( DFAM_CON,0,LOG_YES,KDDS11824 );
		iccode = AgentDisConnectReq( (void*)pComTbl );
		iADisConCnt = 0;
		if ( ( iccode != 0 ) && ( icAgtCR ==0 ) ) {
			GW_tsh_log( 3008, "ADCR ic!=0");
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11803 );
		}
	}
	if ( iWDSemCnt == 1 ){
		CloseLocalSemaphore( uiWatchDog_sh );
		iWDSemCnt = 0;
	}
	WaitOnLocalSemaphore( uiTrace_sh );
	if ( pWatchdogTracep != NULL ) {
		GW_free( pWatchdogTracep, 0 );
		pWatchdogTracep = NULL;
	}
	SignalLocalSemaphore( uiTrace_sh );
	GW_free( NULL, iWatch_tid );
	iWatch_tgid = 0;
	iWatch_tid = 0;
	GW_Message ( DFAM_CON,0,LOG_YES,KDDS11825 );

	WaitOnLocalSemaphore( uiWatchStat_sh );
	if ( iWatch_status == THREAD_NOW_TERM )
		iMyStatus = THREAD_NACTIVE;
	iWatch_status = iMyStatus;
	SignalLocalSemaphore( uiWatchStat_sh );

	ExitThread(EXIT_THREAD,0);
}

/*EnglishComment**************************************************************/
/*  ID   =  MsgGet                                                           */
/*  name =  Message Get Function                                             */
/*  func =  Gets 4-byte unsigned message from the character string.          */
/*  i/o  =  pwdMsg : char * : i : character string                           */
/*                                                                           */
/*  return = unsigned long value                                             */
/*  common = :::                                                             */
/*  note   = none                                                            */
/*                                                                           */
/*  date   = Feb. 02, 1996  Tomoharu Sawahata                                */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
unsigned long MsgGet ( char* pwdMsg ) {
	return( ntohl(*(( unsigned long * )pwdMsg)) );
}
