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
 * $Log: gmaiinit.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:34  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:49  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:05:18  takano
 * 	First Submit
 * 	[1996/02/21  08:05:14  takano]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <process.h>
#include <errno.h>
#include <niterror.h>
#include <nwenvrn.h>
#include <string.h>
#include <fshooks.h>
#include <ncpext.h>
#include <nwbindry.h>
#include <sap.h>
#include <sys/types.h>
#include <netdb.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfammsg.h"
#include "dfamprt.h"
#include "gmai.h"


BOOL bDetail_Output;
extern LONG ulInitCode;

BOOL	bIsHookedPreOpenFile;
BOOL	bIsHookedPostOpenFile;
BOOL	bIsHookedPostCloseFile;
BOOL	bIsHookedPreCreate_OpenFile;
BOOL	bIsHookedPostCreate_OpenFile;
BOOL	bIsHookedPreEraseFile;
BOOL	bIsHookedPostEraseFile;
BOOL	bIsHookedPreRename_or_MoveFile;
BOOL	bIsHookedPostRename_or_MoveFile;
BOOL	bIsHookedPreCreatedir;
BOOL	bIsHookedPostCreatedir;
BOOL	bIsHookedPreDeletedir;
BOOL	bIsHookedPostDeletedir;

LONG uiPreOpenFileHandle;
LONG uiPostOpenFileHandle;
LONG uiPostCloseFileHandle;
LONG uiPreCreate_OpenFileHandle;
LONG uiPostCreate_OpenFileHandle;
LONG uiPreEraseFileHandle;
LONG uiPostEraseFileHandle;
LONG uiPreRename_or_MoveFileHandle;
LONG uiPostRename_or_MoveFileHandle;
LONG uiPreCreateDirHandle;
LONG uiPostCreateDirHandle;
LONG uiPreDeleteDirHandle;
LONG uiPostDeleteDirHandle;

extern BOOL	bUnload;



/*EnglishComment**************************************************************
 *  ID      =   DFAMInit                                                     *
 *  name    =   DFAM Gateway initialization routine                          *
 *  func    =   Initializes DFAM Gateways.  This routine makes a normal      *
 *              return (0) when all the followed processes are normally      *
 *              completed.                                                   *
 *                                                                           *
 *  i/o     =   none                                                         *
 *  return  =    0  :   normal end                                           *
 *              -1  :   abnormal end  (NLM is terminated)                    *
 *              GWERR_UNLOAD : UNLOAD is detected.  DFAMInit is terminated.  *
 *              GWERR_NOMEM  : shortage of memory                            *
 *              GWERR_ABEND  : an error occurred                             *
 *  common  =   none                                                         *
 *  note    =   The version check of CLIB.NLM issues a warning message, and  *
 *              continues its processes.  This is to handle the future       *
 *              upgrades of CLIB.NLM.                                        *
 *  date    =   Nov. 23, 1994  by Ichiyoshi                                  *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
int DFAMInit()
{

	int 	iCcode;
	int		iRtnc = GWERR_NORMAL;
	int		iCnt;
	BOOL	bLoop;

	FILE_SERV_INFO	ServInfo;


	if ( bUnload == TRUE ) {
		iRtnc = GWERR_UNLOAD;
		goto EndOfFunc;
	};



	pGErrFile_p = GW_malloc( sizeof(struct GErrFile_t) );
	if ( pGErrFile_p == NULL ) {
		GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11037 );
		iRtnc = GWERR_NOMEM;
		ulInitCode = INIER_MAIN | INIERC_MEM;
		goto EndOfFunc;
	} else {
		strcpy ( pGErrFile_p->aszErrFileIC, FERR_ICAT );
	}



	pFtrace_p = GW_malloc( FUNC_TR_SIZE);
	if ( pFtrace_p == NULL ) {
		GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11037 );
		iRtnc = GWERR_NOMEM;
		ulInitCode = INIER_MAIN | INIERC_MEM;
		goto EndOfFunc;
	};



	pMainTracep = GW_malloc( COM_TR_SIZE);
	if ( pMainTracep == NULL ) {
		GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11037 );
		iRtnc = GWERR_NOMEM;
		ulInitCode = INIER_MAIN | INIERC_MEM;
		goto EndOfFunc;
	};


	if ( bUnload == TRUE ) {
		iRtnc = GWERR_UNLOAD;
		goto EndOfFunc;
	};


	GW_tsh_functrace( FUNC_DFAMINIT, GW_FUNC_START, NULL);

	if ( bUnload == TRUE ) {
		iRtnc = GWERR_UNLOAD;
		goto EndOfFunc;
	};



	GetServerInformation( SERV_INFO_SIZE, &ServInfo);
	strcpy( aszMyServerName, ServInfo.serverName);
	sprintf( aszOSVer, "%d.%d", ServInfo.netwareVersion, ServInfo.netwareSubVersion );
	if ( strcmp(ALLOWED_OS_VER, aszOSVer) != 0 ){
		GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11001);
		iRtnc = GWERR_ABEND;
		ulInitCode = INIER_MAIN | INIERC_VER;
		goto EndOfFunc;
	};







	if ( ( ServInfo.CLibMajorVersion	<	ALLOWED_CLIB_MAJ_VER )		||

		 (	( ServInfo.CLibMajorVersion	==	ALLOWED_CLIB_MAJ_VER ) &&
			( ServInfo.CLibMinorVersion	<	ALLOWED_CLIB_MIN_VER )	)	||

		 (	( ServInfo.CLibMajorVersion	==	ALLOWED_CLIB_MAJ_VER )	&&
			( ServInfo.CLibMinorVersion	==	ALLOWED_CLIB_MIN_VER )	&&
			( ServInfo.CLibRevision		<	ALLOWED_CLIB_REV)		)		) {
		GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11008 );
	};


	if ( bUnload == TRUE ) {
		iRtnc = GWERR_UNLOAD;
		goto EndOfFunc;
	};



	if ( strcmp( DFAM_NLM_INTERFACE_VER, aszDFALIB_Ver) != 0 ) {
		GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11002, NLM_DFALIB);
		iRtnc = GWERR_ABEND;
		ulInitCode = INIER_MAIN | INIERC_INT;
		goto EndOfFunc;
	};


	if ( bUnload == TRUE ) {
		iRtnc = GWERR_UNLOAD;
		goto EndOfFunc;
	};



	if ( strcmp( DFAM_NLM_INTERFACE_VER, aszDFADCE_Ver) != 0 ) {
		GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11002, NLM_DFADCE);
		iRtnc = GWERR_ABEND;
		ulInitCode = INIER_MAIN | INIERC_INT;
		goto EndOfFunc;
	};



	strcpy(aszDFAGW_Ver,DFAM_NLM_INTERFACE_VER);

	if ( bUnload == TRUE ) {
		iRtnc = GWERR_UNLOAD;
		goto EndOfFunc;
	};



	iCcode = CheckVolumeFile();
	if (iCcode != 0) {
		if (bDetail_Output == FALSE) {
			GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11046 );
		};
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	};

	if ( bUnload == TRUE ) {
		iRtnc = GWERR_UNLOAD;
		goto EndOfFunc;
	};


	iCcode = CheckAgentFile();
	if (iCcode != 0) {
		if (bDetail_Output == FALSE) {
			GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11047 );
		};
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	};

	if ( bUnload == TRUE ) {
		iRtnc = GWERR_UNLOAD;
		goto EndOfFunc;
	};


	iCcode = CheckOptionFile();
	if (iCcode != 0) {
		if (bDetail_Output == FALSE) {
			GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11048 );
		};
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	};




	bLoop = TRUE;


	while(bLoop == TRUE) {


		if ( bUnload == TRUE ) {
			iRtnc = GWERR_UNLOAD;
			goto EndOfFunc;
		};

		iAdmin_status = THREAD_NOW_INIT;


		iCcode = BeginThread ( fnAdminThread, NULL, TG_STACK_SIZE, NULL);
		if (iCcode == EFAILURE) {
			GW_Message( DFAM_CON, NULL, LOG_YES, KDDS11007, NetWareErrno);
			iRtnc = GWERR_ABEND;
			ulInitCode = INIER_MAIN | INIERC_API;
			iAdmin_status = THREAD_NACTIVE;
			goto EndOfFunc;
		};


		while(iAdmin_status == THREAD_NOW_INIT) {
			if ( bUnload == TRUE ) {
				iRtnc = GWERR_UNLOAD;
				goto EndOfFunc;
			};
			delay(SHORTWAITTIME);
			ThreadSwitch();
		}

		switch(iAdmin_status) {
		case THREAD_WORKING:
			bLoop = FALSE;
			break;


		case THREAD_ABENDED:
			GW_Message( DFAM_CON, NULL, LOG_YES, KDDS11004, THREADNAME_ADMIN );
			iAdmin_status = THREAD_NACTIVE;
			iRtnc = GWERR_ABEND;
			ulInitCode = INIER_MAIN | INIERC_ADM;
			goto EndOfFunc;

		case THREAD_TIMEOUT:
			GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11050 );

			for ( iCnt = 0; iCnt <= (TIMEOUT_WAIT_TIME / 100); iCnt++ ) {
				delay(TIMEOUT_WAIT_TIME / 100);
				if ( bUnload == TRUE ) {
					iRtnc = GWERR_UNLOAD;
					goto EndOfFunc;
				};
			}
			break;

		default:
			GW_Message( DFAM_CON, NULL, LOG_YES, KDDS11034);
			iRtnc = GWERR_BUG;
			ulInitCode = INIER_MAIN | INIERC_BUG;
			goto EndOfFunc;
		}
	}


	if ( bUnload == TRUE ) {
		iRtnc = GWERR_UNLOAD;
		goto EndOfFunc;
	};


	iWatch_status = THREAD_NOW_INIT;


	iCcode = BeginThreadGroup ( fnWatchDogThread, NULL, TG_STACK_SIZE, NULL);
	if (iCcode == EFAILURE) {
		GW_Message( DFAM_CON, NULL, LOG_YES, KDDS11007, NetWareErrno);
		iRtnc = GWERR_ABEND;
		iWatch_status = THREAD_NACTIVE;
		ulInitCode = INIER_MAIN | INIERC_API;
		goto EndOfFunc;
	};


	while(iWatch_status == THREAD_NOW_INIT) {
		delay(SHORTWAITTIME);
		ThreadSwitch();
	}

	if (iWatch_status == THREAD_SLEEP){
		while(iWatch_status == THREAD_SLEEP){
			delay(SLEEP_WAIT_TIME );
			if ( bUnload == TRUE ) {
				iRtnc = GWERR_UNLOAD;
				goto EndOfFunc;
			};
		}
	};


	if (iWatch_status == THREAD_ABENDED) {
		GW_Message( DFAM_CON, NULL, LOG_YES, KDDS11004, THREADNAME_WATCHDOG);
		iWatch_status = THREAD_NACTIVE;
		iRtnc = GWERR_ABEND;
		ulInitCode = INIER_MAIN | INIERC_WAT;
		goto EndOfFunc;
	};


	if ( bUnload == TRUE ) {
		iRtnc = GWERR_UNLOAD;
		goto EndOfFunc;
	};


	iCcode = AddHooks();
		if (iCcode != 0) {
		iRtnc = GWERR_ABEND;
		ulInitCode = INIER_MAIN | INIERC_API;
		goto EndOfFunc;
	};


	uiAdHandle = AdvertiseService( OT_DFA, aszMyServerName, NULL);
	if (uiAdHandle == -1) {
		GW_Message( DFAM_CON, NULL, LOG_YES, KDDS11010, NetWareErrno );
		ulInitCode = INIER_MAIN | INIERC_API;
		iRtnc = GWERR_ABEND;
	};


	iCcode = NWRegisterNCPExtension(	NCPEXTNAME,
										fnCommandListen,
										fnConnDown,
										NULL,
										NCP_MAJOR_VERSION,
										NCP_MINOR_VERSION,
										NCP_REVISION,
										&pNCPExt_p);
	if (iCcode != 0) {
		GW_Message( DFAM_CON, NULL, LOG_YES, KDDS11006, NetWareErrno);
		iRtnc = GWERR_ABEND;
		ulInitCode = INIER_MAIN | INIERC_API;
	};



EndOfFunc:

	GW_tsh_functrace( FUNC_DFAMINIT, GW_FUNC_END, iRtnc);

	return (iRtnc);
}




/*EnglishComment**************************************************************
 *  ID      =   AddHooks                                                     *
 *  name    =   FSMonitorHook handler registration                           *
 *  func    =   Registers FSMonitorHook handlers.                            *
 *  i/o     =   none                                                         *
 *  return  =   GWERR_NORMAL : All the registrations normally completed.     *
 *              GWERR_ABEND  : Abnormally ended.                             *
 *  common  =   none                                                         *
 *  note    =   This routine abnormally returns unless all the HOOK          *
 *              registrations normally completed.                            *
 *  date    =   Nov. 24, 1994  by Ichiyoshi                                  *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
int AddHooks(void){
	LONG uiCcode;
	int iRtnc = GWERR_NORMAL;

	GW_tsh_functrace( FUNC_ADDHOOKS, GW_FUNC_START, NULL);

		bIsHookedPreOpenFile			=	FALSE;
		bIsHookedPostOpenFile			=	FALSE;
		bIsHookedPostCloseFile			=	FALSE;
		bIsHookedPreCreate_OpenFile		=	FALSE;
		bIsHookedPostCreate_OpenFile	=	FALSE;
		bIsHookedPreEraseFile			=	FALSE;
		bIsHookedPostEraseFile			=	FALSE;
		bIsHookedPreRename_or_MoveFile	=	FALSE;
		bIsHookedPostRename_or_MoveFile	=	FALSE;
		bIsHookedPreCreatedir			=	FALSE;
		bIsHookedPostCreatedir			=	FALSE;
		bIsHookedPreDeletedir			=	FALSE;
		bIsHookedPostDeletedir			=	FALSE;


	uiCcode = NWAddFSMonitorHook(FSHOOK_PRE_OPENFILE, Pre_Open, &uiPreOpenFileHandle);
	if (uiCcode != 0) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	} else
		bIsHookedPreOpenFile = TRUE;


	uiCcode = NWAddFSMonitorHook(FSHOOK_POST_OPENFILE, Post_Open, &uiPostOpenFileHandle);
	if (uiCcode != 0) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	} else
		bIsHookedPostOpenFile = TRUE;


	uiCcode = NWAddFSMonitorHook(FSHOOK_POST_CLOSEFILE, Post_Close, &uiPostCloseFileHandle);
	if (uiCcode != 0) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	} else
		bIsHookedPostCloseFile = TRUE;


	uiCcode = NWAddFSMonitorHook(FSHOOK_PRE_CREATE_OPENFILE, Pre_CreateOpen, &uiPreCreate_OpenFileHandle);
	if (uiCcode != 0) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	} else
		bIsHookedPreCreate_OpenFile = TRUE;


	uiCcode = NWAddFSMonitorHook(FSHOOK_POST_CREATE_OPENFILE, Post_CreateOpen, &uiPostCreate_OpenFileHandle);
	if (uiCcode != 0) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	} else
		bIsHookedPostCreate_OpenFile = TRUE;


	uiCcode = NWAddFSMonitorHook(FSHOOK_PRE_ERASEFILE, Pre_Erase, &uiPreEraseFileHandle);
	if (uiCcode != 0) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	} else
		bIsHookedPreEraseFile = TRUE;


	uiCcode = NWAddFSMonitorHook(FSHOOK_POST_ERASEFILE, Post_Erase, &uiPostEraseFileHandle);
	if (uiCcode != 0) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	} else
		bIsHookedPostEraseFile = TRUE;


	uiCcode = NWAddFSMonitorHook(FSHOOK_PRE_RENAME_OR_MOVE, Pre_Rename_Move, &uiPreRename_or_MoveFileHandle);
	if (uiCcode != 0) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	} else
		bIsHookedPreRename_or_MoveFile = TRUE;


	uiCcode = NWAddFSMonitorHook(FSHOOK_POST_RENAME_OR_MOVE, Post_Rename_Move, &uiPostRename_or_MoveFileHandle);
	if (uiCcode != 0) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	} else
		bIsHookedPostRename_or_MoveFile = TRUE;


	uiCcode = NWAddFSMonitorHook(FSHOOK_PRE_CREATEDIR, Pre_Createdir, &uiPreCreateDirHandle);
	if (uiCcode != 0) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	} else
		bIsHookedPreCreatedir = TRUE;


	uiCcode = NWAddFSMonitorHook(FSHOOK_POST_CREATEDIR, Post_Createdir, &uiPostCreateDirHandle);
	if (uiCcode != 0) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	} else
		bIsHookedPostCreatedir = TRUE;


	uiCcode = NWAddFSMonitorHook(FSHOOK_PRE_DELETEDIR, Pre_Deletedir, &uiPreDeleteDirHandle);
	if (uiCcode != 0) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	} else
		bIsHookedPreDeletedir = TRUE;


	uiCcode = NWAddFSMonitorHook(FSHOOK_POST_DELETEDIR, Post_Deletedir, &uiPostDeleteDirHandle);
	if (uiCcode != 0) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	} else
		bIsHookedPostDeletedir = TRUE;


EndOfFunc:

	if (uiCcode != 0 ) {
		GW_Message( DFAM_CON, NULL, LOG_YES, KDDS11006, NetWareErrno);
	};
	GW_tsh_functrace( FUNC_ADDHOOKS, GW_FUNC_END, NULL);
	return(iRtnc);
}




/*EnglishComment**************************************************************
 *  ID      =   RemoveHooks                                                  *
 *  name    =   FSMonitorHook handler free routine                           *
 *  func    =   Frees registered FSMonitorHook.                              *
 *  i/o     =   none                                                         *
 *  return  =   none                                                         *
 *  common  =   none                                                         *
 *  note    =   This routine goes through all the processes even an error    *
 *              is detected.                                                 *
 *  date    =   Nov. 24, 1994  by Ichiyoshi                                  *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
void RemoveHooks(void){

	LONG uiCcode;

	GW_tsh_functrace( FUNC_REMOVEHOOKS, GW_FUNC_START, NULL);




	if (bIsHookedPreOpenFile)	{
		uiCcode = NWRemoveFSMonitorHook(FSHOOK_PRE_OPENFILE, uiPreOpenFileHandle);
		if (uiCcode != 0) {
			if ( hErrorLog_fh != NULL) {
				GW_tsh_log( (int)uiCcode , LOG_RH_PREOPEN);
			};
		} else;
	} else ;


	if (bIsHookedPostOpenFile)	{
		uiCcode = NWRemoveFSMonitorHook(FSHOOK_POST_OPENFILE, uiPostOpenFileHandle);
		if (uiCcode != 0) {
			if ( hErrorLog_fh != NULL) {
				GW_tsh_log( (int)uiCcode, LOG_RH_POSOPEN);
			};
		} else ;
	} else ;


	if (bIsHookedPostCloseFile)	{
		uiCcode = NWRemoveFSMonitorHook(FSHOOK_POST_CLOSEFILE, uiPostCloseFileHandle);
		if (uiCcode != 0) {
			if ( hErrorLog_fh != NULL) {
				GW_tsh_log( (int)uiCcode, LOG_RH_POSCLOS);
			};
		} else;
	} else;


	if (bIsHookedPreCreate_OpenFile) {
		uiCcode = NWRemoveFSMonitorHook(FSHOOK_PRE_CREATE_OPENFILE, uiPreCreate_OpenFileHandle);
		if (uiCcode != 0) {
			if ( hErrorLog_fh != NULL) {
				GW_tsh_log( (int)uiCcode, LOG_RH_PRECROP);
			};
		} else;
	} else;


	if (bIsHookedPostCreate_OpenFile) {
		uiCcode = NWRemoveFSMonitorHook(FSHOOK_POST_CREATE_OPENFILE, uiPostCreate_OpenFileHandle);
		if (uiCcode != 0) {
			if ( hErrorLog_fh != NULL) {
				GW_tsh_log( (int)uiCcode, LOG_RH_POSCROP);
			};
		} else;
	} else;


	if (bIsHookedPreEraseFile)	{
		uiCcode = NWRemoveFSMonitorHook(FSHOOK_PRE_ERASEFILE, uiPreEraseFileHandle);
		if (uiCcode != 0) {
			if ( hErrorLog_fh != NULL) {
				GW_tsh_log( (int)uiCcode, LOG_RH_PREERAF);
			};
		} else;
	} else;


	if (bIsHookedPostEraseFile)	{
		uiCcode = NWRemoveFSMonitorHook(FSHOOK_POST_ERASEFILE, uiPostEraseFileHandle);
		if (uiCcode != 0) {
			if ( hErrorLog_fh != NULL) {
				GW_tsh_log( (int)uiCcode, LOG_RH_POSERAF);
			};
		} else;
	} else;


	if (bIsHookedPreRename_or_MoveFile) {
		uiCcode = NWRemoveFSMonitorHook(FSHOOK_PRE_RENAME_OR_MOVE, uiPreRename_or_MoveFileHandle);
		if (uiCcode != 0) {
			if ( hErrorLog_fh != NULL) {
				GW_tsh_log( (int)uiCcode , LOG_RH_PREREN);
			};
		} else;
	} else;


	if (bIsHookedPostRename_or_MoveFile) {
		uiCcode = NWRemoveFSMonitorHook(FSHOOK_POST_RENAME_OR_MOVE, uiPostRename_or_MoveFileHandle);
		if (uiCcode != 0) {
			if ( hErrorLog_fh != NULL) {
				GW_tsh_log( (int)uiCcode , LOG_RH_POSREN);
			};
		} else;
	} else;


	if (bIsHookedPreCreatedir) {
		uiCcode = NWRemoveFSMonitorHook(FSHOOK_PRE_CREATEDIR, uiPreCreateDirHandle);
		if (uiCcode != 0) {
			if ( hErrorLog_fh != NULL) {
				GW_tsh_log( (int)uiCcode, LOG_RH_PRECRD);
			};
		} else;
	} else;


	if (bIsHookedPostCreatedir) {
		uiCcode = NWRemoveFSMonitorHook(FSHOOK_POST_CREATEDIR, uiPostCreateDirHandle);
		if (uiCcode != 0) {
			if ( hErrorLog_fh != NULL) {
				GW_tsh_log( (int)uiCcode, LOG_RH_POSCRD);
			};
		} else;
	} else;


	if (bIsHookedPreDeletedir) {
		uiCcode = NWRemoveFSMonitorHook(FSHOOK_PRE_DELETEDIR, uiPreDeleteDirHandle);
		if (uiCcode != 0) {
			if ( hErrorLog_fh != NULL) {
				GW_tsh_log( (int)uiCcode, LOG_RH_PREDELD);
			};
		} else;
	} else;


	if (bIsHookedPostDeletedir) {
		uiCcode = NWRemoveFSMonitorHook(FSHOOK_POST_DELETEDIR, uiPostDeleteDirHandle);
		if (uiCcode != 0) {
			if ( hErrorLog_fh != NULL) {
				GW_tsh_log( (int)uiCcode, LOG_RH_POSDELD);
			};
		} else;
	} else;

	GW_tsh_functrace( FUNC_REMOVEHOOKS, GW_FUNC_END, NULL);

}


