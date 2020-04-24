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
 * $Log: gdirif.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:48  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:12  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:47:26  takano
 * 	First Submit
 * 	[1996/02/21  07:47:19  takano]
 * 
 * $EndLog$
 */
#include "gdir.h"



/*********************************************************** EnglishComment **/
/* ID = GetNWPath2                                                           */
/*                                                                           */
/* name = Variation of GetNWPath                                             */
/* func = Provides a mutant of GetNWPath.                                    */
/*        *1: In order to reduce the number of requests to Agent, preserve   */
/*            the DFS file directory list made by GetNWPath2, and reuses     */
/*            the list.                                                      */
/*        *2: To flash the preserved lists, call GetNWPath2 with setting     */
/*            NULL to the 1st parameter.                                     */
/*                                                                           */
/* i/o  = pszDFSPath:char *:inp: DFS path name (before renaming)             */
/*        pszVol    :BYTE *:inp: Volume table pointing the target volume     */
/*        pszDFSPath:char *:out: NetWare path name (after renamed)           */
/*                                                                           */
/* return = 0: normal end                                                    */
/*	EDMmalloc                                                                */
/*	EDMwarning                                                               */
/*	(DFSChainFakeDirEnt)                                                     */
/*	EDMshutdownre                                                            */
/*	EDMagentdie                                                              */
/*  EDMlongpath                                                              */
/*  EDMnameconv                                                              */
/*                                                                           */
/* date = June 13, 1994 by Ryuji Shinchi                                     */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int GetNWPath2( BYTE *pszDFSPath, BYTE *pszVol, BYTE *pszNWPath )
{
	int							iRet;
	int							iErr = 0;
	char						*pszDirname = NULL;
	static char					*pszDirnameForGetNWPath = NULL;
	static struct DFSFakeDirEnt	*pDFSDirEntForGetNWPath = NULL;

    GW_tsh_functrace("GetNWPath2", GW_FUNC_START, 0);

	if ( pszDFSPath == NULL ) {
		iErr = 0;
		goto labelGetNWPath2_F;
	}
	if ( (pszDirname=GW_malloc( NW_MAX_PATH + 1 )) == NULL ) {
		iErr = EDMmalloc;
		goto labelGetNWPath2_Q;
	}

	if ( pszDirnameForGetNWPath == NULL ) {
		if ( (pszDirnameForGetNWPath=GW_malloc( NW_MAX_PATH + 1 )) == NULL ) {
			iErr = EDMmalloc;
			goto labelGetNWPath2_Q;
		}
		*pszDirnameForGetNWPath = '\0';
	} else {
		;
	}
	DFSDirname( pszDirname, pszDFSPath );
	if ( strcmp( pszDirnameForGetNWPath, pszDirname ) ) {

		if ( pDFSDirEntForGetNWPath != NULL ) {
			DFSFreeFakeDirEntList( pDFSDirEntForGetNWPath );
			pDFSDirEntForGetNWPath = NULL;
		}
		strcpy( pszDirnameForGetNWPath, pszDirname );


		iRet = DFSChainFakeDirEnt(	pszDirnameForGetNWPath,
				&pDFSDirEntForGetNWPath, 0, NULL );
		switch ( iRet ) {
		case 0:
			break;
		case EDMshutdownreq:
		case EDMagentdie:
			iErr = iRet;
			goto labelGetNWPath2_F;
		default:
			iErr = EDMwarning;
		}
	} else {
		;
	}

	iRet = GetNWPath( pszDFSPath, pszVol, pszNWPath, pDFSDirEntForGetNWPath );
	switch ( iRet ) {
	case GWERR_NORMAL:
		break;
	case GWERR_NOMEM:
		GW_Message( DFAM_CON, 0, LOG_NO, KDDS11613 );
		iErr = EDMmalloc;
		goto labelGetNWPath2_Q;
	case GWERR_INVAL:	case GWERR_NOT_DFAMVOL:
	case GWERR_KEY_IS_FULL:	case GWERR_BUG:
		GW_Message( DFAM_CON, 0, LOG_NO, KDDS11624, iRet );
		iErr = EDMnameconv;
		goto labelGetNWPath2_Q;
	case GWERR_LONG:
		iErr = EDMlongpath;
		goto labelGetNWPath2_Q;
	default:
		GW_Message( DFAM_CON, 0, LOG_NO, KDDS11681, iRet );
		iErr = iRet;
		goto labelGetNWPath2_Q;
	}

labelGetNWPath2_Q:
	if ( pszDirname != NULL ) {
		DMfree( pszDirname, "GetNWPath2" );
		pszDirname = NULL;
	}
	GW_tsh_functrace("GetNWPath2", GW_FUNC_END, iErr);
	return iErr;

labelGetNWPath2_F:
	if ( pszDirname != NULL ) {
		DMfree( pszDirname, "GetNWPath2" );
		pszDirname = NULL;
	}
	if ( pszDirnameForGetNWPath != NULL ) {
		DMfree( pszDirnameForGetNWPath, "GetNWPath2" );
		pszDirnameForGetNWPath = NULL;
	}
	if ( pDFSDirEntForGetNWPath  != NULL ) {
		DFSFreeFakeDirEntList( pDFSDirEntForGetNWPath );
		pDFSDirEntForGetNWPath = NULL;
	}
	GW_tsh_functrace("GetNWPath2", GW_FUNC_END, iErr);
	return iErr;
}

/*********************************************************** EnglishComment **/
/* ID = DMYesNo                                                              */
/*                                                                           */
/* name = Displaying message and receiving reply                             */
/* func = Displays a message on a console, and receives Y/N.                 */
/* i/o  = pszMessage  :char *:inp: message to be printed                     */
/*                                                                           */
/* return = EDMshutdownreq: UNLOAD was detected while waiting for key input  */
/*	    EDMyes        : Y was inputted                                       */
/*          EDMno         : N was inputted                                   */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/*                                                                           */
/* date = December 20, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DMYesNo( char *pszNWPath )
{
	int		iMyRet;
	int		iAns;

    GW_tsh_functrace("DMYesNo", GW_FUNC_START, 0);

	DisplayScreen( iScreenHandle );
	GW_Message( DFAM_CON, 0, LOG_NO, KDDS11625, pszNWPath );

	while ( 1 ) {
		if ( ShutdownReq() ) {
			GW_tsh_functrace("DMYesNo", GW_FUNC_END, iMyRet);
			return EDMshutdownreq;
		}
		if ( kbhit() ) {
			int breakflag = 0;
			iAns = getch();
			switch ( iAns ) {
			case 'Y':
			case 'y':
				GW_Message( DFAM_CON, 0, LOG_NO, "Y\n" );
				iMyRet = EDMyes;
				breakflag = 1;
				break;
			case 'N':
			case 'n':
				GW_Message( DFAM_CON, 0, LOG_NO, "N\n" );
				iMyRet = EDMno;
				breakflag = 1;
				break;
			case 0:
				iAns = getch();
				breakflag = 0;
				break;
			default:
				breakflag = 0;
				break;
			}
			if ( breakflag ) {
				break;
			}
		}
	}

	GW_tsh_functrace("DMYesNo", GW_FUNC_END, iMyRet);
	return iMyRet;
}

/*********************************************************** EnglishComment **/
/* ID = ShutdownReq                                                          */
/* name = UNLOAD checking                                                    */
/* func = Checks if UNLOAD is requested.                                     */
/* i/o  = void                                                               */
/* return = 0: UNLOAD is requested                                           */
/*          1: UNLOAD is not requested                                       */
/* common = iAdmin_status                                                    */
/*          bUnload                                                          */
/* note = none                                                               */
/*                                                                           */
/* date = April 17, 1995 by Ryuji Shinchi                                    */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int ShutdownReq( void )
{
	ThreadSwitch();

	WaitOnLocalSemaphore( uiAdminStat_sh );
	if ( iAdmin_status == THREAD_NOW_TERM ) {
		SignalLocalSemaphore( uiAdminStat_sh );
		return 1;
	} else {
		SignalLocalSemaphore( uiAdminStat_sh );
		if ( bUnload )
			return 1;
		else
			return 0;
	}
}

/*********************************************************** EnglishComment **/
/* ID = AgentDie                                                             */
/*                                                                           */
/* name = Watchdog checking                                                  */
/* func = Checks if Watchdog detected an error in Agent                      */
/*                                                                           */
/* i/o  = none                                                               */
/* return = 0: normal end                                                    */
/*          1: abnormal end                                                  */
/* common = none                                                             */
/* note = none                                                               */
/*                                                                           */
/* date = April 17, 1995 by Ryuji Shinchi                                    */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int AgentDie( void )
{
	ThreadSwitch();

	if ( iWatch_status == THREAD_SLEEP ) {
		return 1;
	} else {
		return 0;
	}
}


/*********************************************************** EnglishComment **/
/* ID = SetDeathFlag                                                         */
/* name = Admin connection error reporting                                   */
/* func = Reports an Admin connection failure to other threads.              */
/* i/o  = none                                                               */
/* return = none                                                             */
/* common = none                                                             */
/* note = none                                                               */
/*                                                                           */
/* date = January 9, 1995 by Ryuji Shinchi                                   */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
void SetDeathFlag( void )
{
    GW_tsh_functrace("SetDeathFlag", GW_FUNC_START, 0);
	pAgent_sp->bAdmin_live = FALSE;
	GW_tsh_functrace("SetDeathFlag", GW_FUNC_END, 0);
}

/*********************************************************** EnglishComment **/
/* ID = ResetDeathFlag                                                       */
/*                                                                           */
/* name = Admin connection normal reporting                                  */
/* func = Reports other threads that an Admin connection is normal.          */
/* i/o  = none                                                               */
/* return = none                                                             */
/* common = none                                                             */
/* note = none                                                               */
/*                                                                           */
/* date = January 9, 1995 by Ryuji Shinchi                                   */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
void ResetDeathFlag( void )
{
    GW_tsh_functrace("ResetDeathFlag", GW_FUNC_START, 0);
	pAgent_sp->bAdmin_live = TRUE;
	GW_tsh_functrace("ResetDeathFlag", GW_FUNC_END, 0);

}

/*EnglishComment**************************************************************/
/*  ID   =  NoticeDMInitSuccess                                              */
/*  name =  Send success message function                                    */
/*  func =  Notifies the caller that, with using a global variable, the      */
/*          initialization of the directory synchronization has been         */
/*          successful.                                                      */
/*  i/o  =  :::                                                              */
/*  return = :                                                               */
/*  common = iAdmin_status :int : o : AdminThread control flag               */
/*  note   = none                                                            */
/*                                                                           */
/*  date   = Jan. 09, 1996  Ryuji Shinchi                                    */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
void NoticeDMInitSuccess()
{

	iAdmin_status = THREAD_WORKING;
}

/*EnglishComment**************************************************************/
/*  ID   =  NoticeDMInitAbort                                                */
/*  name =  Send failure message function                                    */
/*  func =  Notifies the caller that, with using a global variable, the      */
/*          initialization of the directory synchronization has been         */
/*          unsuccessful.                                                    */
/*  i/o  =  :::                                                              */
/*  return = :                                                               */
/*  common = iAdmin_status :int : o : AdminThread control flag               */
/*  note   = none                                                            */
/*                                                                           */
/*  date   = Jan. 09, 1996  Ryuji Shinchi                                    */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
void NoticeDMInitAbort()
{

	iAdmin_status = THREAD_ABENDED;
}

/*********************************************************** EnglishComment **/
/* ID = DMfree( char *pszDFSPath, char *pszVolName, pszNWPath )              */
/* name = Invoking GW_free()                                                 */
/* func = function to invoke GW_free()                                       */
/* i/o  = none                                                               */
/* return = none                                                             */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = January 9, 1995 by Ryuji Shinchi                                   */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
void DMfree( void *pPtr, char *pszLoc )
{
	if ( GW_free( pPtr, 0 ) != 1 ) {
		GW_tsh_log( GWLOGDMFREE, pszLoc );
		return;
	}
	uigAllocatedMem--;
}



