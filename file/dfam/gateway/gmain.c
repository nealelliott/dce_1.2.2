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
 * $Log: gmain.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:35  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:50  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:05:43  takano
 * 	First Submit
 * 	[1996/02/21  08:05:37  takano]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <process.h>
#include <errno.h>
#include <niterror.h>
#include <signal.h>
#include <nwsemaph.h>
#include <ncpext.h>
#include <sap.h>
#include <string.h>
#include <conio.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "dfammsg.h"
#include "gmai.h"

LONG	ulInitCode;
BOOL	bInit_processing;
BOOL	bResource_freed;
BOOL	bTerm;

extern  BOOL    bUnload;


/*EnglishComment**************************************************************
 *  ID      =   main                                                         *
 *  name    =   DFAM Gateway main routine                                    *
 *  func    =   initializes global variables, and starts AdminThread and     *
 *              Watchdog thread.  "Main" suspends when the initialization is *
 *              completed.  If UNLOAD or fatal error occurred, "main" frees  *
 *              global resources, and terminates NLM.                        *
 *  i/o     =   mone                                                         *
 *  return  =   none                                                         *
 *  common  =   none                                                         *
 *  note    =   none                                                         *
 *  date    =   Nov. 22, 1994  by Ichiyoshi                                  *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/

void main(void)
{


	int 	iCcode;
	BYTE	aszMsgBuf[80];


#if defined (USER5)
		iMaxUsersNum	=	5;
#elif defined (USER10)
		iMaxUsersNum	=	10;
#elif defined (USER25)
		iMaxUsersNum	=	25;
#elif defined (USER50)
		iMaxUsersNum	=	50;
#elif defined (USER100)
		iMaxUsersNum	=	100;
#elif defined (USER250)
		iMaxUsersNum	=	250;
#else
		iMaxUsersNum	=	5;
#endif

	iSystem_ccode			=	GWERR_NORMAL;
	uiAdHandle				=	NULL;
	bGet_Dump				=	FALSE;
	hErrorLog_fh			=	NULL;
    hMessageLog_fh			=	NULL;
	aszOSVer[0]				=   '\0';
    aszNLMVer[0]			=	'\0';
	aszDFAGW_Ver[0]			=	'\0';
	pMemManage_p			=	NULL;
	pVolume_sp				=	NULL;
	iDFANLM_ID				=	GetNLMID();
	iMain_tgid				=	GetThreadGroupID();
	iMain_tid				=	GetThreadID();
	iWatch_tgid				=	NULL;
	iWatch_tid				=	NULL;
	iWatch_status			=	THREAD_NACTIVE;
	iAdmin_tgid				=	NULL;
	iAdmin_tid				=	NULL;
	iAdmin_status			=	THREAD_NACTIVE;
	iLoginUsersNum			=	0;
	iResumeAll				=	0;
	iScreenHandle			=	GetCurrentScreen();
	uiMainConn_sh			=	(LONG)0;
	uiCopyFile_sh			=	(LONG)0;
	uiErrLog_sh				=	(LONG)0;
	uiMalloc_sh				=	(LONG)0;
	uiTrace_sh				=	(LONG)0;
	uiFTrace_sh				=	(LONG)0;
	uiKeyFile_sh			=	(LONG)0;
	uiWatchStat_sh			=	(LONG)0;
	uiAdminStat_sh			=	(LONG)0;
	uiMsg_sh				=	(LONG)0;
	uiDump_sh				=	(LONG)0;
	uiRAS_sh				=	(LONG)0;


	pMemManage_p				=	NULL;
	pFtrace_p					=	NULL;
	pVolume_sp					=	NULL;
	pCopyFile_sp				=	NULL;
	pTGroup_sp					=	NULL;
	pName_sp					=	NULL;
	pAgent_sp					=	NULL;
	pFHandle_sp					=	NULL;
	pNCPExt_p					=	NULL;
	pCopyBack_sp				=	NULL;
	pGErrFile_p					=	NULL;


	uiDirSyncInterval			=	NULL;
	iTraceFlushCount		=	TRACEFLUSHCOUNT_DFL;
	iFuncFlushCount			=	TRACEFLUSHCOUNT_DFL;
	usCopyBackDelayTime			=	NULL;
	bTimeSync					=	FALSE;
	bMountWarning				=	FALSE;
	aszCopyBackDirName[0]		=	'\0';
	aszMessageLogFileName1[0]	=	'\0';
	aszMessageLogFileName2[0]	=	'\0';
	aszErrorLogFileName1[0]		=	'\0';
	aszErrorLogFileName2[0]		=	'\0';
	pszCur_Msg_Fname			=	NULL;
	pszCur_Err_Fname			=	NULL;


	bUnload						=	FALSE;
	bInit_processing			=	FALSE;
	bResource_freed				=	FALSE;
	bTerm						=	FALSE;


	signal( SIGTERM, fnDFAMTerm);
		SetCtrlCharCheckMode( FALSE );


	bInit_processing = TRUE;


	uiMainConn_sh	=	OpenLocalSemaphore( SEMAPHORE_INIVALUE );
	uiCopyFile_sh	=	OpenLocalSemaphore( SEMAPHORE_INIVALUE );
	uiErrLog_sh		=	OpenLocalSemaphore( SEMAPHORE_INIVALUE );
	uiMalloc_sh		=	OpenLocalSemaphore( SEMAPHORE_INIVALUE );
	uiTrace_sh		=	OpenLocalSemaphore( SEMAPHORE_INIVALUE );
	uiFTrace_sh		=	OpenLocalSemaphore( SEMAPHORE_INIVALUE );
	uiKeyFile_sh	=	OpenLocalSemaphore( SEMAPHORE_INIVALUE );
	uiWatchStat_sh	=	OpenLocalSemaphore( SEMAPHORE_INIVALUE );
	uiAdminStat_sh	=	OpenLocalSemaphore( SEMAPHORE_INIVALUE );
	uiMsg_sh		=	OpenLocalSemaphore( SEMAPHORE_INIVALUE );
	uiDump_sh		=	OpenLocalSemaphore( SEMAPHORE_INIVALUE );
	uiRAS_sh		=	OpenLocalSemaphore( SEMAPHORE_INIVALUE );



	GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11003 );
	ConsolePrintf( "%s\r", KDDS11003);

	if (bUnload == FALSE) {
		iCcode = DFAMInit();
		if ( (iCcode != 0) && (iCcode != GWERR_UNLOAD) ) {
			sprintf(aszMsgBuf, "%s%04X.\n", KDDS11012, ulInitCode );
			GW_Message( DFAM_CON, NULL, LOG_YES, aszMsgBuf );
			ConsolePrintf( "%s\r", aszMsgBuf );
			bTerm = TRUE;
			bInit_processing = FALSE;
			FreeGlobalResource();
			goto EndOfFunc;
		};

		if (iCcode != GWERR_UNLOAD) {
			GW_Message( DFAM_CON, NULL, LOG_YES, KDDS11049 );
			ConsolePrintf( "%s\r", KDDS11049);
		};

	};
	bInit_processing = FALSE;


	iCcode = SuspendThread(iMain_tid);
	if (iCcode == EFAILURE) {
		if ( uiMsg_sh != (LONG)0 ) {
			WaitOnLocalSemaphore( uiMsg_sh );
		};
		printf(KDDS11011);
		if ( uiMsg_sh != (LONG)0 ) {
			SignalLocalSemaphore( uiMsg_sh );
		};
	};

	if ( bResource_freed == FALSE ) {
		FreeGlobalResource();
	}

EndOfFunc:
	ExitThread( EXIT_NLM, NULL);
}




/*EnglishComment**************************************************************
 *  ID      =   FreeGlobalResource                                           *
 *  name    =   Free global resources                                        *
 *  func    =   Frees global resources as follows:                           *
 *                - issues a termination request to a live thread group,     *
 *                  and waits for its termination.                           *
 *                - frees the registration of NCPExtension and FSMonitorHook *
 *                  handler.                                                 *
 *                - closes semaphore handles.                                *
 *  i/o     =   none                                                         *
 *  return  =   none                                                         *
 *  common  =   none                                                         *
 *  note    =   none                                                         *
 *  date    =   Nov. 24, 1994  by Ichiyoshi                                  *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/

void FreeGlobalResource()
{
	int iCcode;

	GW_Message( DFAM_CON, NULL, LOG_YES, KDDS11053 );

	if ( pFtrace_p ) {
		GW_tsh_functrace( FUNC_FREEGLOBAL, GW_FUNC_START, NULL);
	};



	GW_tsh_dump( DUMP_REGULAR );


	if ( pGErrFile_p != NULL ) {
		GW_free( pGErrFile_p, NULL );
		pGErrFile_p = NULL;
	};




	ResumeAllUserThread( UNLOAD_DFA );



	if( pNCPExt_p != NULL ) {
		NWDeRegisterNCPExtension( pNCPExt_p);
	};


	if (uiAdHandle != NULL) {
		ShutdownAdvertising( uiAdHandle );
	};


	RemoveHooks();








	WaitOnLocalSemaphore(uiWatchStat_sh);

	switch(iWatch_status) {
	case THREAD_WORKING:
		iWatch_status = THREAD_NOW_TERM;
		break;

	case THREAD_SLEEP:
		iWatch_status = THREAD_NOW_TERM;
		iCcode = ResumeThread( iWatch_tid );
		if (iCcode != ESUCCESS) {
			GW_Message( DFAM_CON, NULL, LOG_YES, KDDS11036, NetWareErrno );
		};
		break;

	case THREAD_NACTIVE:
	case THREAD_ABENDED:
		break;

	default:
		GW_Message( DFAM_CON, NULL, LOG_YES, KDDS11034 );
	}

	SignalLocalSemaphore(uiWatchStat_sh);


	if (iWatch_status == THREAD_NOW_TERM) {


		while(1) {
			WaitOnLocalSemaphore(uiWatchStat_sh);
			if (iWatch_status != THREAD_NOW_TERM) {
				SignalLocalSemaphore(uiWatchStat_sh);
				break;
			};
			SignalLocalSemaphore(uiWatchStat_sh);
			delay(SHORTWAITTIME);
			ThreadSwitch();
		}
	};


	WaitOnLocalSemaphore( uiAdminStat_sh );

	if ( (iAdmin_status == THREAD_WORKING) || (iAdmin_status == THREAD_NOW_INIT) ) {


		iAdmin_status = THREAD_NOW_TERM;
		SignalLocalSemaphore( uiAdminStat_sh );
		while(1) {
			WaitOnLocalSemaphore( uiAdminStat_sh );
			if ( iAdmin_status != THREAD_NOW_TERM ) {
				SignalLocalSemaphore( uiAdminStat_sh );
				break;
			};
			SignalLocalSemaphore( uiAdminStat_sh );
			delay(SHORTWAITTIME);
			ThreadSwitch();
		}
	} else {
		SignalLocalSemaphore( uiAdminStat_sh );
	}


	if ( uiRAS_sh ) {
		MainCloseSemaphore( &uiRAS_sh );
	};


	if ( pCopyFile_sp != NULL ) {

		FAR_Terminate();

	};


	while(pVolume_sp != NULL){
		DeleteFromGVolume_t(pVolume_sp);
	}


	if ( pAgent_sp != NULL ) {
		DeleteFromGAgent_t(pAgent_sp);
	};


	if ( hMessageLog_fh != NULL) {
		fclose(hMessageLog_fh);
		hMessageLog_fh = NULL;
	};

	if ( pMainTracep ) {
		GW_free(pMainTracep, NULL);
		pMainTracep = NULL;
	};

	if ( pFtrace_p ) {
		GW_free(pFtrace_p, NULL);
		pFtrace_p = NULL;
	};


	if ( hErrorLog_fh != NULL) {
		fclose(hErrorLog_fh);
		hErrorLog_fh = NULL;
	};


	GW_free_all();


	if ( uiMainConn_sh ) {
		MainCloseSemaphore( &uiMainConn_sh );
	};

	if ( uiCopyFile_sh ) {
		MainCloseSemaphore( &uiCopyFile_sh );
	};

	if ( uiErrLog_sh ) {
		MainCloseSemaphore( &uiErrLog_sh );
	};

	if ( uiMalloc_sh ) {
		MainCloseSemaphore( &uiMalloc_sh );
	};

	if ( uiTrace_sh ) {
		MainCloseSemaphore( &uiTrace_sh );
	};

	if ( uiFTrace_sh ) {
		MainCloseSemaphore( &uiFTrace_sh );
	};

	if ( uiKeyFile_sh ) {
		MainCloseSemaphore( &uiKeyFile_sh );
	};

	if ( uiWatchStat_sh ) {
		MainCloseSemaphore( &uiWatchStat_sh );
	};

	if ( uiAdminStat_sh ) {
		MainCloseSemaphore( &uiAdminStat_sh );
	};

	if ( uiMsg_sh ) {
		MainCloseSemaphore( &uiMsg_sh );
	};

	if ( uiDump_sh ) {
		MainCloseSemaphore( &uiDump_sh );
	};


	printf( KDDS11052 );
	ConsolePrintf( "%s\r", KDDS11052 );

	bResource_freed = TRUE;

}




/*EnglishComment**************************************************************
 *  ID      =   fnDFAMterm                                                   *
 *  name    =   Unload handler                                               *
 *  func    =   This is started by "unload."  This invokes "Free global      *
 *              resources," then starts the main thread.                     *
 *  i/o     =   none                                                         *
 *  return  =   none                                                         *
 *  common  =   none                                                         *
 *  note    =   none                                                         *
 *  date    =   Dec. 13, 1994  by Ichiyoshi                                  *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
void fnDFAMTerm(iSignal)
int iSignal;
{


	iSignal = iSignal;

	SetThreadGroupID(iMain_tgid);

	if (bTerm == FALSE) {
		GW_Message( DFAM_CON, NULL, LOG_YES, KDDS11051);
		ConsolePrintf( "%s\r", KDDS11051);
	};

	bUnload = TRUE;

	while ( bInit_processing == TRUE ) {
		delay(SHORTWAITTIME);
		ThreadSwitch();
	}

	if (bTerm == FALSE) {
		FreeGlobalResource();

		if (ESUCCESS != ResumeThread(iMain_tid) ) {
			printf( KDDS11036, NetWareErrno);
			ExitThread(EXIT_NLM, 0);
		} else;
	} else {
		while( bResource_freed == FALSE ) {
			delay(SHORTWAITTIME);
			ThreadSwitch();
		}
	}
}




/*EnglishComment**************************************************************
 *  ID   = MainCloseSemaphore                                                *
 *  name = Closing a semaphore                                               *
 *  func = Waits until the target semaphore is not in use, then closes the   *
 *         semaphore.                                                        *
 *                                                                           *
 *  i/o    = uiSemhandle : LONG * : pointer to the semaphore handle          *
 *  return = none                                                            *
 *  common = none                                                            *
 *                                                                           *
 *  note   = (1) uiSemhandle will be 0 after the semaphore is freed.         *
 *           (2) DFA.NLM will never access the closed semaphore handle, but  *
 *               DFARAS.NLM may go to access the being-closed semaphore.     *
 *               This function is provided for such case.                    *
 *                                                                           *
 *  date   = Oct. 18, 1995 by Isao Ichiyoshi                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
void MainCloseSemaphore( uiSemhandle )
LONG * uiSemhandle;
{
	while(*uiSemhandle){

	if ( ExamineLocalSemaphore(*uiSemhandle) == SEMAPHORE_INIVALUE ) {
		CloseLocalSemaphore(*uiSemhandle);
		*uiSemhandle = (LONG)0;
	} else {
		ThreadSwitch();
		delay(SHORTWAITTIME);
		}
	}
	return;
}

