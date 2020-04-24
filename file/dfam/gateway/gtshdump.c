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
 * $Log: gtshdump.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:59  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:10  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:16:48  takano
 * 	First Submit
 * 	[1996/02/21  08:16:43  takano]
 * 
 * $EndLog$
 */
#include <stdlib.h>
#include <nwdir.h>
#include <ctype.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "dfammsg.h"
#include "dfamer.h"

#define RW_OK      0
#define DUMPFILE1  "DFADUMP.001"
#define DUMPFILE2  "DFAABND.001"
#define UNKNOWNFILE  "?"
#define EXTENSION   8
#define FPRINTTBLS 59
#define FPRINTAREA 61

/*********************************************************** EnglishComment **/
/*  ID      = Gateway Dump Output Function                                   */
/*  name    = DFAM Gateway Dump                                              */
/*  func    = Output the GW_malloc ed Area                                   */
/*  i/o     = iMode:int:inp: output file selection                           */
/*                   DUMP_NORMAL: directed by iSystem_ccode                  */
/*                   DUMP_DFARAS: DFADUMP.001... are going to be written     */
/*  return  = 0:            Success                                          */
/*            DRFAILURE:    File i/o Error                                   */
/*            DROPENERR:    Error in opening dump file                       */
/*            DRWRITEERR:   Error in writing dump file                       */
/*            DRNOMEM:      Memory shortage                                  */
/*            DRNORESOURCE: Resources not ready                              */
/*            DRNODATA:     No data                                          */
/*            DRNOGETMODE:  No file                                          */
/*  common  = GMemManage_t                                                   */
/*  note    =                                                                */
/* date = April 16, 1995  by T.Sawahata                                      */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*********************************************************** EnglishComment **/

int  GW_tsh_dump( int iMode ) {


	int						iMyRet = 0;

	struct GMem_AllcInfo	*pGMemAllcEnd_t;
	struct GMem_HeadFoot	*pGMemHF_t;
	struct GMem_AllcInfo	*pGMemAllc_t;
	struct GMemManage_t 	*pGMemw_t;
	struct stat				*pstatblock;
	FILE					*GW_dmp_fchg( FILE*, char* );

	void					*pMalcArea;
	int						icCode2;
	size_t					uicCode;
	size_t					uicCode2;

	int		icCode;
	FILE	*pnewFILE;
	size_t	uiFileSize;
	char	cCurFILE[15];
	LONG    lbuflen;
	size_t  uiMalcSize;
	int     ifiletop;

	if ( uiDump_sh != 0L ) {
		WaitOnLocalSemaphore( uiDump_sh );
	} else {
		return DRNORESOURCE;
	}

	GW_tsh_functrace( "GW_tsh_dump",GW_FUNC_START, NULL );
	if ( pMemManage_p == NULL ) {
		GW_tsh_functrace( "GW_tsh_dump",GW_FUNC_END, -1 );
		SignalLocalSemaphore( uiDump_sh );
		return DRNORESOURCE;
	}
	ifiletop = 1;


	pstatblock = GW_malloc(sizeof(struct stat));
	if (pstatblock == NULL) {
		GW_Message( DFAM_CON,0,LOG_YES,KDDS11801);
		GW_tsh_functrace( "GW_tsh_dump",GW_FUNC_END, -1 );
		SignalLocalSemaphore( uiDump_sh );
		return DRNOMEM;
	}
	icCode = stat( DUMPFILE1,pstatblock );
	GW_free( pstatblock, 0 );
	if ( (icCode != 0 ) &&
			( (iSystem_ccode == GWERR_NORMAL) || (iMode == DUMP_DFARAS) ) ) {
		SignalLocalSemaphore( uiDump_sh );
		return DRNOGETMODE;
	}
	if ( ( icCode == 0 ) &&
			( (iSystem_ccode == GWERR_NORMAL) || (iMode == DUMP_DFARAS) ) ) {

		DeleteAllNumberedFiles( DUMPFILE1 );
		if ( (pnewFILE = fopen( DUMPFILE1,"w" )) == NULL ){
			GW_Message( DFAM_CON,0,LOG_YES,KDDS11801);
			GW_tsh_functrace( "GW_tsh_dump",GW_FUNC_END, -1 );
			SignalLocalSemaphore( uiDump_sh );
			return DROPENERR;
		}
		strcpy( cCurFILE,DUMPFILE1 );
	} else {
		DeleteAllNumberedFiles( DUMPFILE2 );
		if ( (pnewFILE = fopen( DUMPFILE2,"w" )) == NULL ){
			GW_Message( DFAM_CON,0,LOG_YES,KDDS11801);
			GW_tsh_functrace( "GW_tsh_dump",GW_FUNC_END, -1 );
			SignalLocalSemaphore( uiDump_sh );
			return DROPENERR;
		}
		strcpy( cCurFILE,DUMPFILE2 );
	}
	uiFileSize = 0;
	uicCode    = 0;
	uicCode2   = 0;
	lbuflen = sizeof( struct GMem_HInfo ) + ( MAXARRNUM * sizeof( struct GMem_AllcInfo ) );

	GW_Message( DFAM_CON,0,LOG_YES,KDDS11826);


	uicCode2 += fprintf ( pnewFILE , "Global Variables\n\n" );


	uicCode2 += fprintf ( pnewFILE , " iSystem_ccode          = %0X \n", iSystem_ccode );
	uicCode2 += fprintf ( pnewFILE , " iUnloadTcpipFlag       = %0X \n", iUnloadTcpipFlag );


	uicCode2 += fprintf ( pnewFILE , " bGet_Dump              = %0X \n", bGet_Dump );


	uicCode2 += fprintf ( pnewFILE , " hErrorLog_fh           = %0X \n", hErrorLog_fh );
	uicCode2 += fprintf ( pnewFILE , " hMessageLog_fh         = %0X \n", hMessageLog_fh );


	uicCode2 += fprintf ( pnewFILE , " aszMyServerName        = %s \n" , aszMyServerName );
	uicCode2 += fprintf ( pnewFILE , " uiAdHandle             = %0X \n", uiAdHandle );
	uicCode2 += fprintf ( pnewFILE , " aszDFAGW_Ver           = %s \n" , aszDFAGW_Ver );
	uicCode2 += fprintf ( pnewFILE , " aszDFALIB_Ver          = %s \n" , aszDFALIB_Ver );
	uicCode2 += fprintf ( pnewFILE , " aszDFADCE_Ver          = %s \n" , aszDFADCE_Ver );
	uicCode2 += fprintf ( pnewFILE , " aszOSVer               = %s \n" , aszOSVer );
	uicCode2 += fprintf ( pnewFILE , " aszNLMVer              = %s \n" , aszNLMVer );


	uicCode2 += fprintf ( pnewFILE , " iMain_tgid             = %0X \n", iMain_tgid );
	uicCode2 += fprintf ( pnewFILE , " iMain_tid              = %0X \n", iMain_tid );
	uicCode2 += fprintf ( pnewFILE , " iWatch_tgid            = %0X \n", iWatch_tgid );
	uicCode2 += fprintf ( pnewFILE , " iWatch_tid             = %0X \n", iWatch_tid );
	uicCode2 += fprintf ( pnewFILE , " iWatch_status          = %0X \n", iWatch_status );
	uicCode2 += fprintf ( pnewFILE , " iAdmin_tgid            = %0X \n", iAdmin_tgid );
	uicCode2 += fprintf ( pnewFILE , " iAdmin_tid             = %0X \n", iAdmin_tid );
	uicCode2 += fprintf ( pnewFILE , " iAdmin_status          = %0X \n", iAdmin_status );


	uicCode2 += fprintf ( pnewFILE , " pFtrace_p              = %0X \n", pFtrace_p );


	uicCode2 += fprintf ( pnewFILE , " pFtrace_p              = %0X \n", pFtrace_p );
	uicCode2 += fprintf ( pnewFILE , " pMemManage_p           = %0X \n", pMemManage_p );
	uicCode2 += fprintf ( pnewFILE , " pVolume_sp             = %0X \n", pVolume_sp );
	uicCode2 += fprintf ( pnewFILE , " pCopyFile_sp           = %0X \n", pCopyFile_sp );
	uicCode2 += fprintf ( pnewFILE , " pTGroup_sp             = %0X \n", pTGroup_sp );
	uicCode2 += fprintf ( pnewFILE , " pName_sp               = %0X \n", pName_sp );
	uicCode2 += fprintf ( pnewFILE , " pAgent_sp              = %0X \n", pAgent_sp );
	uicCode2 += fprintf ( pnewFILE , " pFHandle_sp            = %0X \n", pFHandle_sp );
	uicCode2 += fprintf ( pnewFILE , " pNCPExt_p              = %0X \n", pNCPExt_p );
	uicCode2 += fprintf ( pnewFILE , " pCopyBack_sp           = %0X \n", pCopyBack_sp );
	uicCode2 += fprintf ( pnewFILE , " GNCPExt_p              = %0X \n", GNCPExt_p );


	uicCode2 += fprintf ( pnewFILE , " uiDirSyncInterval      = %0X \n", uiDirSyncInterval );
	uicCode2 += fprintf ( pnewFILE , " iTraceFlushCount       = %0X \n", iTraceFlushCount );
	uicCode2 += fprintf ( pnewFILE , " iFuncFlushCount        = %0X \n", iFuncFlushCount );
	uicCode2 += fprintf ( pnewFILE , " usCopyBackDelayTime    = %0X \n", usCopyBackDelayTime );
	uicCode2 += fprintf ( pnewFILE , " bTimeSync              = %0X \n", bTimeSync );
	uicCode2 += fprintf ( pnewFILE , " bMountWarning          = %0X \n", bMountWarning );
	uicCode2 += fprintf ( pnewFILE , " aszCopyBackDirName     = %s \n", aszCopyBackDirName );
	uicCode2 += fprintf ( pnewFILE , " aszMessageLogFileName1 = %s \n", aszMessageLogFileName1 );
	uicCode2 += fprintf ( pnewFILE , " aszMessageLogFileName2 = %s \n", aszMessageLogFileName2 );
	uicCode2 += fprintf ( pnewFILE , " aszErrorLogFileName1   = %s \n", aszErrorLogFileName1 );
	uicCode2 += fprintf ( pnewFILE , " aszErrorLogFileName2   = %s \n", aszErrorLogFileName2 );
	if ( pszCur_Msg_Fname != NULL ) {
		uicCode2 += fprintf ( pnewFILE , " pszCur_Msg_Fname       = %s \n", pszCur_Msg_Fname );
	} else {
		uicCode2 += fprintf ( pnewFILE , " pszCur_Msg_Fname       = %s \n", UNKNOWNFILE );
	}
	if ( pszCur_Err_Fname != NULL ) {
		uicCode2 += fprintf ( pnewFILE , " pszCur_Err_Fname       = %s \n", pszCur_Err_Fname );
	} else {
		uicCode2 += fprintf ( pnewFILE , " pszCur_Err_Fname       = %s \n", UNKNOWNFILE );
	}
	uicCode2 += fprintf ( pnewFILE , " tmWatchdogMSGBuflen    = %0X \n", tmWatchdogMSGBuflen );
	uicCode2 += fprintf ( pnewFILE , " iMaxUsersNum           = %0X \n", iMaxUsersNum );
	uicCode2 += fprintf ( pnewFILE , " iLoginUsersNum         = %0X \n", iLoginUsersNum );


	uicCode2 += fprintf ( pnewFILE , " iMaxUsersNum           = %0X \n", iMaxUsersNum );
	uicCode2 += fprintf ( pnewFILE , " iWatchSockfd           = %0X \n", iWatchSockfd );


	uicCode2 += fprintf ( pnewFILE , " uiMainConn_sh          = %0X \n", uiMainConn_sh );
	uicCode2 += fprintf ( pnewFILE , " uiCopyFile_sh          = %0X \n", uiCopyFile_sh );
	uicCode2 += fprintf ( pnewFILE , " uiErrLog_sh            = %0X \n", uiErrLog_sh );
	uicCode2 += fprintf ( pnewFILE , " uiMalloc_sh            = %0X \n", uiMalloc_sh );
	uicCode2 += fprintf ( pnewFILE , " uiTrace_sh             = %0X \n", uiTrace_sh );
	uicCode2 += fprintf ( pnewFILE , " uiFTrace_sh            = %0X \n", uiFTrace_sh );
	uicCode2 += fprintf ( pnewFILE , " uiKeyFile_sh           = %0X \n", uiKeyFile_sh );
	uicCode2 += fprintf ( pnewFILE , " uiWatchStat_sh         = %0X \n", uiWatchStat_sh );
	uicCode2 += fprintf ( pnewFILE , " uiMsg_sh               = %0X \n", uiMsg_sh );


	uicCode2 += fprintf ( pnewFILE , "\n pMainTracep            = %0X \n", pMainTracep );
	uiFileSize = uicCode2;


	WaitOnLocalSemaphore( uiMalloc_sh );


	pGMemw_t = pMemManage_p;
	while ( pGMemw_t != NULL ) {
		uiFileSize += lbuflen + FPRINTTBLS;
		if ( uiFileSize >= MAXDFSIZE ) {
			pnewFILE = GW_dmp_fchg( pnewFILE, cCurFILE );
			if ( pnewFILE == NULL) {
				iMyRet = DROPENERR;
				goto ErrReturn;
			};
			ifiletop = 1;
			uiFileSize = lbuflen + FPRINTTBLS;
		}
		if ( ifiletop == 1 ) {
			icCode2 = fprintf( pnewFILE,"GW_malloc Management Table No.%d  Address = %X \n\n",
								pGMemw_t->SGMem_HInfo.iGMemTblCnt,pGMemw_t );

			ifiletop = 0;
		} else {
			icCode2 = fprintf( pnewFILE,"\n\nGW_malloc Management Table No.%d  Address = %X\n\n",
								pGMemw_t->SGMem_HInfo.iGMemTblCnt,pGMemw_t );

		}
		if ( icCode2 <= 0 ) {
			iMyRet = DRWRITEERR;
			goto ErrReturn;
		}
		uicCode = fwrite( pGMemw_t,lbuflen,1,pnewFILE );
		if ( uicCode < 1 ) {
			fclose( pnewFILE );
			iMyRet = DRWRITEERR;
			goto ErrReturn;
		}
		pGMemw_t = pGMemw_t->SGMem_HInfo.pNextGMemTbl;
	}

	pGMemw_t = pMemManage_p;
	while ( pGMemw_t != NULL ) {

		pGMemAllc_t = pGMemw_t->SGMem_AllcInfo;


		pGMemAllcEnd_t = pGMemw_t->SGMem_HInfo.pGMemTblEnd;
		while ( pGMemAllc_t <= pGMemAllcEnd_t ) {
			pMalcArea = ( pGMemAllc_t->pGMem_malc );
			if ( pMalcArea == NULL ){
				++pGMemAllc_t;
				continue;
			}
			pGMemHF_t = ( struct GMem_HeadFoot *)pMalcArea;
			uiMalcSize = ( pGMemHF_t->uiSizeM );
			uiFileSize +=  uiMalcSize + FPRINTAREA;
			if ( uiFileSize >= MAXDFSIZE ) {
				pnewFILE = GW_dmp_fchg( pnewFILE, cCurFILE );
				if ( pnewFILE == NULL) {
					iMyRet = DROPENERR;
					goto ErrReturn;
				};
				ifiletop   = 1;
				uiFileSize =  uiMalcSize + FPRINTAREA;
			}
			if ( ifiletop == 1 ) {
				icCode2 = fprintf( pnewFILE,"Memory Contents taken by GW_malloc. Address = %X\n\n",
								pGMemAllc_t->pGMem_malc );
				ifiletop = 0;
			} else {
				icCode2 = fprintf( pnewFILE, "\n\nMemory Contents . Address = %0X Thread ID = %0X \n\n"
                                  , pGMemAllc_t->pGMem_malc, pGMemAllc_t->iGMem_thID );

			}
			if ( icCode2 <= 0 ) {
				fclose( pnewFILE );
				iMyRet = DRWRITEERR;
				goto ErrReturn;
			}
			uicCode = fwrite( pGMemAllc_t->pGMem_malc,uiMalcSize,1,pnewFILE );

			if ( uicCode < 1 ) {
				fclose( pnewFILE );
				iMyRet = DRWRITEERR;
				goto ErrReturn;
			}
			++pGMemAllc_t;
		}
		pGMemw_t = pGMemw_t->SGMem_HInfo.pNextGMemTbl;
	}
	SignalLocalSemaphore(uiMalloc_sh);
	icCode2 = fprintf( pnewFILE,"\n\nEnd of Dump OutPut\n");
		if ( icCode2 <= 0 ) {
			fclose( pnewFILE );
			iMyRet = DRWRITEERR;
			goto ErrReturn;
		}
	icCode2 = fclose( pnewFILE );
		if ( icCode2 !=0 ) {
			fclose( pnewFILE );
			iMyRet = DRWRITEERR;
			goto ErrReturn;
		}
	GW_tsh_functrace( "GW_tsh_dump",GW_FUNC_END, 0 );
	GW_Message( DFAM_CON,0,LOG_YES,KDDS11827);
	SignalLocalSemaphore( uiDump_sh );
	return 0;
ErrReturn:
	SignalLocalSemaphore(uiMalloc_sh);
	GW_Message( DFAM_CON,0,LOG_YES,KDDS11801);
	GW_tsh_functrace( "GW_tsh_dump",GW_FUNC_END, -1 );
	SignalLocalSemaphore( uiDump_sh );
	return iMyRet;
}

/*********************************************************** EnglishComment **/
/*  ID      = GW_dmp_fchg                                                    */
/*  name    = Close currently opend outputfile and open next                 */
/*  func    = Close currently opend outputfile and open next                 */
/*              (orderd by its extension as decimal serial number)           */
/*  io      =                                                                */
/*          pnewFile : FILE *:i: Currentry opened file handle                */
/*          cCurFile : char *:i/o: File name corresponds pnewFile            */
/*  return  =                                                                */
/*          NULL    : Error in opening new file                              */
/*          non-NULL: pointing new output file                               */
/*  common  =                                                                */
/*  note    =                                                                */
/* date = April 16, 1995 by Tomoharu Sawahata                                */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*********************************************************** EnglishComment **/
FILE *GW_dmp_fchg ( FILE *pnewFILE, char *cCurFILE ){
	char	*pcExt;
	int		iExt;

	pcExt = cCurFILE + EXTENSION;
	iExt = atoi( pcExt );
	++iExt;
	strncpy( pcExt, "000", 3 );

	if ( iExt < 10 ) {
		itoa( iExt, pcExt+2, 10 );
	} else if ( (iExt > 9) && (iExt < 100) ) {
		itoa( iExt, pcExt+1, 10 );
	} else if ( (iExt > 99) && (iExt < 1000) ){
		itoa( iExt, pcExt, 10 );
	} else;

	fclose(pnewFILE);
	pnewFILE = fopen( cCurFILE, "w" );
	return( pnewFILE );
}


/*********************************************************** EnglishComment **/
/*  ID      = DeleteAllNumberedFiles                                         */
/*  name    = Remove specified series of files numberd by their extention    */
/*  func    = Remove specified series of files numberd by their extention    */
/*  io      =                                                                */
/*			pszPath	:	char *	:i:	Basename of series of files              */
/*  return  =  0: Success                                                    */
/*            -1: File i/o Error                                             */
/*            -2: Memory shortage                                            */
/*  common  =                                                                */
/*  note    =                                                                */
/* date = September 6, 1995  by Ryuji Shinchi                                */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*********************************************************** EnglishComment **/
DeleteAllNumberedFiles( char *pszPath )
{
	int				iErr = 0;
	static char		aszWorkPath[_MAX_PATH+1];
	char			*pLoc;
    DIR             *pDIRopen;
    DIR             *pDIRread;

	strncpy( aszWorkPath, pszPath, _MAX_PATH );
	aszWorkPath[ _MAX_PATH - 4 ] = '\0';


	pLoc = strrchr( aszWorkPath, '.' );
	if ( pLoc == NULL ) {
		strcat( aszWorkPath, ".*" );
	} else {
		sprintf( pLoc, ".*" );
	}


    if ( (pDIRopen = opendir( aszWorkPath )) == NULL ) {
        if ( errno == ENOENT ) {
            iErr = 0;
			goto labelDeleteAllNumberedFiles_quit;
        } else {
            iErr = -1;
			goto labelDeleteAllNumberedFiles_quit;
        }
    }


    while( (pDIRread = readdir( pDIRopen )) != NULL ) {
		int		i;
        strcpy( aszWorkPath, pDIRread->d_name );
		i = strlen( aszWorkPath );
		if ( (aszWorkPath[i-4] == '.') &&
			 (isdigit(aszWorkPath[i-3])) &&
			 (isdigit(aszWorkPath[i-2])) &&
			 (isdigit(aszWorkPath[i-1])) ) {
			if ( unlink( aszWorkPath ) )
				iErr = -2;
		}
    }
    closedir( pDIRopen );

labelDeleteAllNumberedFiles_quit:
	return iErr;
}

/*********************************************************** EnglishComment **/
/*  ID      = DFARASdump                                                     */
/*                                                                           */
/* name = Invoking a dump function                                           */
/* func = Switches the context to DFA.NLM, and invokes the dump outputting   */
/*        function.                                                          */
/*                                                                           */
/* i/o  = none                                                               */
/*                                                                           */
/*  return  = 0:            Success                                          */
/*            DRFAILURE:    File i/o Error                                   */
/*            DROPENERR:    Error in opening dump file                       */
/*            DRWRITEERR:   Error in writing dump file                       */
/*            DRNOMEM:      Memory shortage                                  */
/*            DRNOSESOURCE: Resources not ready                              */
/*            DRNODATA:     No data                                          */
/*            DRNOGETMODE:  No file                                          */
/*                                                                           */
/* common = iDFANLM_ID                                                       */
/* note   = none                                                             */
/*                                                                           */
/* date = September 18, 1995  by Ryuji Shinchi                               */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*********************************************************** EnglishComment **/

int DFARASdump( void )
{
	int				iRet;
	int				iErr = DRNOERR;
	int				iCurTG_ID;
	char			*pszPath = NULL;

	if ( iMain_tgid != 0 ) {
		iCurTG_ID = SetThreadGroupID( iMain_tgid );
	} else {
		return DRNORESOURCE;
	}

	pszPath = (char *)GW_malloc( _MAX_PATH + 1 );
	if ( pszPath == NULL ) {
		iErr = DRNOMEM;
		goto labelDFARASdump_quit;
	}


	sprintf( pszPath, "%s\\%s", "SYS:", DUMPFILE1 );
	iRet = access( pszPath, ACCESS_WR|ACCESS_RD );
	if ( iRet ) {
		iErr = DRNOGETMODE;
		goto labelDFARASdump_quit;
	}

	iErr = GW_tsh_dump( DUMP_DFARAS );


labelDFARASdump_quit:
	if ( pszPath != NULL )
		GW_free( pszPath, 0 );

	SetThreadGroupID( iCurTG_ID );

	return iErr;
}
