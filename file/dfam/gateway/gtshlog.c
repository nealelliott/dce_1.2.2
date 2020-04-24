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
 * $Log: gtshlog.c,v $
 * Revision 1.1.2.2  1996/03/11  14:09:04  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:13  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:18:04  takano
 * 	First Submit
 * 	[1996/02/21  08:18:00  takano]
 * 
 * $EndLog$
 */

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamprt.h"
#include "dfammain.h"
#include "dfammsg.h"
#include "dfamer.h"

	void    LibCleanUp( void );
	int     iNLMID;
	int     ilibThrGID ;
	int     icCode;

	int GW_tsh_log ( int , char * );
	int GW_tsh_trace( void *, size_t, int, int, int, char * );
	int GW_tsh_functrace( char *, int, int );
	int GW_Message ( int , int* , int , char *, ... );

/******************************************************** EnglishComment ****/
/*   ID         = main                                                      */
/*   name       = main function for DFALIB.NLM module                       */
/*   func       = setup DFALIB.NLM supplying common global resources for    */
/*                 Gateway modules.                                         */
/*   io         = void                                                      */
/*   return     = void                                                      */
/*                                                                          */
/* date = May 19, 1995  by T.Sawahata                                       */
/*                                                                          */
/* Trade Secret of Hitachi, Ltd.                                            */
/* Do not disclose without written concession from Hitachi, Ltd.            */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/******************************************************** EnglishComment ****/
void main()  {
	strcpy( aszDFALIB_Ver, "01.01" );
	icCode = EnterCritSec();
	if ( icCode != 0 ) {
		printf(KDDS11804);

		return;
	}
	ilibThrGID = GetThreadGroupID();
	iNLMID = GetNLMID();



	ExitCritSec();
	ExitThread (TSR_THREAD, 0);
  }



/******************************************************** EnglishComment ****/
/*   ID         = GW_tsh_log                                                */
/*   name       = Logging Errors                                            */
/*   func       = Log Requested Errors to File                              */
/*   io         =                                                           */
/*           ierrNumber : int  : Error Number                               */
/*           comment    : char : Any comment Requester wants to write       */
/*                                                                          */
/*   return     =                                                           */
/*      0       : Succeeded to log                                          */
/*      -1      : Couldn't access the Error Log File                        */
/*      -2      : Another Error                                             */
/*   common     =                                                           */
/*      FILE                : *hErrorLog_fh                                 */
/*      Semaphore Handle    : uiErrLog_sh                                   */
/*   note       =                                                           */
/*                                                                          */
/* date = May 19, 1995  by T.Sawahata                                       */
/*                                                                          */
/* Trade Secret of Hitachi, Ltd.                                            */
/* Do not disclose without written concession from Hitachi, Ltd.            */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/******************************************************** EnglishComment ****/

int GW_tsh_log ( int ierrNumber, char *comment ) {




	off_t  			tmErrlogfsize;
	int				icCode;
	struct stat 	*pstatInfo;
	time_t			itimer;
	char            *ctimer;
	char            cthName[18];
	size_t			tmlen;
	char			cLogInfo[70];
	int             ithreadID;
	char            cSpace[16] = "                ";
	int             iCharNum;
	char            *commentwk;
	int             ierrnosv;


	pstatInfo = ( struct stat *)GW_malloc(sizeof(struct stat));

	if (pstatInfo == NULL ) {
		return(-2);
	}

	if ( uiErrLog_sh == 0 )
		return -1;

	WaitOnLocalSemaphore( uiErrLog_sh );

	if ( pszCur_Err_Fname == NULL ) {
		GW_free( pstatInfo, 0 );
		goto ErrReturn;
	}


	icCode = stat( pszCur_Err_Fname, pstatInfo );

	if ( icCode != 0 ) {
		if ( errno == ENOENT ) {
			hErrorLog_fh = fopen ( pszCur_Err_Fname, "w" );
			tmErrlogfsize = 0;
			GW_free(pstatInfo,0);
		} else {
			GW_free( pstatInfo, 0 );
			goto ErrReturn;
		}
	} else {
		tmErrlogfsize  = pstatInfo->st_size;
		GW_free(pstatInfo,0);
		hErrorLog_fh = fopen ( pszCur_Err_Fname, "a" );
	}

	if ( hErrorLog_fh == NULL ) {
		ierrnosv = errno;
		goto ErrReturn;
	}
	if ( (tmErrlogfsize + ERRECLEN) >= ERLOGMAX ) {
		icCode = fclose( hErrorLog_fh );
		if ( icCode != 0 ) {
			goto ErrReturn;
		}
		hErrorLog_fh = NULL;
		if ( strcmp(pszCur_Err_Fname,aszErrorLogFileName1) == 0 ) {

			pszCur_Err_Fname = aszErrorLogFileName2;
		} else if ( strcmp(pszCur_Err_Fname,aszErrorLogFileName1) != 0 ) {

			pszCur_Err_Fname = aszErrorLogFileName1;
		} else {
			goto ErrReturn;
		}
		unlink( pszCur_Err_Fname );
		hErrorLog_fh = fopen ( pszCur_Err_Fname, "w" );
		if ( hErrorLog_fh == NULL ) {
			goto ErrReturn;
		}
	}
	ithreadID = GetThreadID();
	GetThreadName( ithreadID, cthName );
	iCharNum = 0;
	while( iCharNum < 17 ) {
		if ( (cthName[ iCharNum ]) == '\0' ) {
			while ( iCharNum < 17 ) {
				memset( (cthName+iCharNum), ' ', 1 );
				++iCharNum;
			}
		} else {
			++iCharNum;
		}
	}
	memset( (cthName + 17), '\0', 1 );
	itimer  = time(NULL);
	ctimer  = ctime( &itimer );
	ctimer += LOG_WEEK;
	tmlen   = LOG_TIME + 4;
	strncpy ( cLogInfo, ctimer, tmlen );
	strncpy ( ( cLogInfo + tmlen ),"\0",1);
	commentwk = NULL;
	commentwk = GW_malloc(20);
	if ( commentwk == NULL ){
		if ( fclose( hErrorLog_fh ) == 0 ) {
			hErrorLog_fh = NULL;
		}
		goto ErrReturn;
	}
	if ( comment == NULL ){
		strncpy ( commentwk, cSpace, 11 );
	} else {
		strncpy ( commentwk, comment, 11 );
		iCharNum = 0;
		while( iCharNum < 11 ) {
			if ( strncmp( (commentwk + iCharNum),"\0",1 ) == 0 ) {
				while ( iCharNum < 11 ) {
					memset( (commentwk+iCharNum), ' ', 1 );
					++iCharNum;
				}
			} else {
				++iCharNum;
			}
		}
	}
	memset( (commentwk+11), '\0', 1 );
	fprintf ( hErrorLog_fh,"%10X%s%4d%s%s\n", ithreadID, cthName
                     , ierrNumber, cLogInfo, commentwk );
	icCode = fclose( hErrorLog_fh );
	if ( commentwk != NULL ) {
		GW_free( commentwk, 0 );
	}
	if ( icCode != 0 ) {
		goto ErrReturn;
	}
	hErrorLog_fh = NULL;
	SignalLocalSemaphore( uiErrLog_sh );
	return(0);

ErrReturn:
	if ( pGErrFile_p != NULL ) {
		if ( strcmp(pszCur_Err_Fname,aszErrorLogFileName1) == 0 ) {
			(pGErrFile_p->iErrLog001) = ierrnosv;
		} else if ( strcmp(pszCur_Err_Fname,aszErrorLogFileName2) == 0 ){
			(pGErrFile_p->iErrLog002) = ierrnosv;
		}
	}
	SignalLocalSemaphore( uiErrLog_sh );
	return(-1);
}
/******************************************************** EnglishComment ****/
/*   ID         = GW_tsh_trace                                              */
/*   name       = Take the tace in each Thread                              */
/*   func       = Trace Requested Errors to File                            */
/*   io         =                                                           */
/*           ierrNumber : int  : Error Number                               */
/*           comment    : char : Any comment Requester wants to write       */
/*                                                                          */
/*   return     =                                                           */
/*      0       : Succeeded to log                                          */
/*      -1      : Couldn't access the Error Log File                        */
/*      -2      : Another Error                                             */
/*   common     =                                                           */
/*      FILE                : *hErrorLog_fh                                 */
/*      Semaphore Handle    : uiErrLog_sh                                   */
/*   note       =                                                           */
/*                                                                          */
/* date = May 25, 1995  by T.Sawahata                                       */
/*                                                                          */
/* Trade Secret of Hitachi, Ltd.                                            */
/* Do not disclose without written concession from Hitachi, Ltd.            */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/******************************************************** EnglishComment ****/

int GW_tsh_trace( void *pAreaPtr, size_t uisize, int iwhatEvent,
                   int icomm1, int icomm2, char *comm3 ) {



	struct tr_Init 		*ptr_InitHead;
	struct tr_Record	*ptr_Record;
	char				filename[20];
	LONG                llen;
	LONG                llen2;
	int 				iErrorCode;
	int					iThreadID;
	int					icCode;
	int					ifileHandle;
	int					iworkint;
	int					iopenmodeflag;
	struct stat			*pstinfo;
	off_t               tmFileSize;

	iThreadID = GetThreadID();

	if ( uiTrace_sh == 0 )
		return -2;

	WaitOnLocalSemaphore( uiTrace_sh );

	if ( pAreaPtr == NULL ) {
		goto ErrReturn2;
	}
	if ( uisize < 1380 ) {
		goto ErrReturn2;
	}
	iworkint = (uisize - sizeof(struct tr_Init)) / sizeof(struct tr_Record);

	uisize = ( iworkint * sizeof(struct tr_Record) ) + sizeof(struct tr_Init);


	ptr_InitHead = (struct tr_Init *)pAreaPtr;
	if ( ptr_InitHead->iI_thID == 0 ) {
		strcpy( ptr_InitHead->cEyeCatch, "THREAD GROUP TRACE AREA" );

		ptr_InitHead->pHere = pAreaPtr;
		ptr_InitHead->iI_thID = iThreadID;
		GetThreadName( iThreadID, ptr_InitHead->cI_thName );
		ptr_InitHead->uiI_tr_size = uisize;
		ptr_InitHead->pI_lastPtr = (void*)((char*)ptr_InitHead + TRHEADEND);

		ptr_InitHead->iRecCnt = 0;
		ptr_InitHead->uiBaseTime = time(NULL);
		ptr_InitHead->uiBaseClock = clock();
		ptr_InitHead->pStrtWrTrp = NULL;
	}

	ptr_Record = ( struct tr_Record *)( ptr_InitHead->pI_lastPtr );

	ptr_Record->uitr_clock = clock();
	ptr_Record->iEventType = iwhatEvent;
	ptr_Record->icmt1      = icomm1;
	ptr_Record->icmt2      = icomm2;
	if( comm3 != NULL ) {
		strncpy( ptr_Record->cmt3, comm3, 12 );
	}
	ptr_Record->ithID      = iThreadID;
	GetThreadName( iThreadID, ptr_InitHead->cI_thName );

	ptr_InitHead->pI_lastPtr = (void*)((char *)(ptr_InitHead->pI_lastPtr) + sizeof(struct tr_Record));

	if ( (ptr_InitHead->pI_lastPtr) > (void*)((char*)ptr_InitHead + uisize
                                          - sizeof(struct tr_Record))){

		(ptr_InitHead->pI_lastPtr) = (void*)((char*)ptr_InitHead + TRHEADEND);
	}


	++(ptr_InitHead->iRecCnt);


	if ( iTraceFlushCount != 0 ) {
		if ( (ptr_InitHead->iRecCnt >= iTraceFlushCount)
				|| (iwhatEvent == GW_MAIN_TRACE_FLUSH)
				|| (iwhatEvent == GW_ADMIN_TRACE_FLUSH)
				|| (iwhatEvent == GW_WATCHDOG_TRACE_FLUSH) ) {
			iErrorCode = 0;
			iopenmodeflag = 0;
			( ptr_InitHead->iRecCnt ) = 0;
			if ( (iThreadID == iWatch_tid)
					|| (iwhatEvent == GW_WATCHDOG_TRACE_FLUSH) ) {
				strcpy(filename, TRCFNAME_WATCH );
				iopenmodeflag = ptr_InitHead->iWatchTRC;

				if ( iopenmodeflag == 0 ) {
					ptr_InitHead->iWatchTRC = 1;
				}

			} else if ( ( iThreadID == iAdmin_tid )
					|| (iwhatEvent == GW_ADMIN_TRACE_FLUSH) ) {
				strcpy( filename, TRCFNAME_ADMIN );
				iopenmodeflag = ptr_InitHead->iAdminTRC;
				if ( iopenmodeflag == 0 ) {
					ptr_InitHead->iAdminTRC = 1;
				}

			} else {

				strcpy( filename, TRCFNAME_MAIN );
				iopenmodeflag = ptr_InitHead->iMainTRC;
				if ( iopenmodeflag == 0 ) {
					ptr_InitHead->iMainTRC = 1;
				}

			}

			icCode = access( filename, ACCESS_WR);
			if ( icCode == 0 ) {


				pstinfo = GW_malloc( sizeof(struct stat));
				if ( pstinfo == NULL ) {
					iErrorCode = -1;
					goto ErrReturn;
				}
				icCode = stat( filename, pstinfo );
				if ( icCode != 0 ) {
					GW_free( pstinfo, 0 );
					iErrorCode = -1;
					goto ErrReturn;
				}
				tmFileSize = ( pstinfo->st_size ) + (iTraceFlushCount * sizeof(struct tr_Record));

				GW_free( pstinfo, 0 );
				if ( (tmFileSize >= MAXTRFLEN)
						|| (iwhatEvent == GW_MAIN_TRACE_FLUSH)
						|| (iwhatEvent == GW_ADMIN_TRACE_FLUSH)
						|| (iwhatEvent == GW_WATCHDOG_TRACE_FLUSH) ) {
					iopenmodeflag = 0;
				}

				if ( iopenmodeflag == 0 ) {
					unlink( filename );

					ifileHandle = creat ( filename, S_IWRITE );

				} else {
					ifileHandle = open ( filename, O_APPEND | O_WRONLY );
				}

				if ( ifileHandle == EFAILURE ) {
					iErrorCode = -1;
					goto ErrReturn;
				}

				if ( iopenmodeflag == 0 )  {
					llen = uisize;
					icCode = write ( ifileHandle, ptr_InitHead, llen );


					if ( icCode == EFAILURE ) {
						close( ifileHandle );
						iErrorCode = -1;
						goto ErrReturn;
					}

					ptr_InitHead->pStrtWrTrp = (ptr_InitHead->pI_lastPtr);

				} else {
					llen = iTraceFlushCount * sizeof(struct tr_Record);

					if ( ((char*)(ptr_InitHead->pStrtWrTrp) + llen) >=
                                         ((char*)(ptr_InitHead) + uisize )) {

						llen2 = ((char*)(ptr_InitHead) + uisize )
										- (char*)(ptr_InitHead->pStrtWrTrp);
						if ( llen2 != 0 ){

							icCode = write ( ifileHandle, (ptr_InitHead->pStrtWrTrp), llen2 );
							if ( icCode == EFAILURE ) {
								close( ifileHandle );
								iErrorCode = -1;
								goto ErrReturn;
							}
							ptr_InitHead->pStrtWrTrp = (void *)((char*)ptr_InitHead
                                                                + sizeof(struct tr_Init));
							llen2 = llen - llen2;
							icCode = write ( ifileHandle, (ptr_InitHead->pStrtWrTrp), llen2 );
							if ( icCode == EFAILURE ) {
								close( ifileHandle );
								iErrorCode = -1;
								goto ErrReturn;
							}
						} else {
							icCode = write ( ifileHandle, (ptr_InitHead->pStrtWrTrp), llen );
							if ( icCode == EFAILURE ) {
								close( ifileHandle );
								iErrorCode = -1;
								goto ErrReturn;
							}
						}
						ptr_InitHead->pStrtWrTrp = (ptr_InitHead->pI_lastPtr);


					} else {

						icCode = write( ifileHandle, (ptr_InitHead->pStrtWrTrp), llen );
						if ( icCode == EFAILURE ) {
							close( ifileHandle );
							iErrorCode = -1;
							goto ErrReturn;
						}
						ptr_InitHead->pStrtWrTrp = (void*)(ptr_InitHead->pI_lastPtr);
					}
				}
				icCode = close ( ifileHandle );
				if ( icCode != 0 ) {
					iErrorCode = -1;
					goto ErrReturn;
				}

				( ptr_InitHead->iRecCnt ) = 0;
			} else {
				if ( iThreadID == iWatch_tid ) {
					ptr_InitHead->iWatchTRC = 0;
				} else if ( iThreadID == iAdmin_tid ) {
					ptr_InitHead->iAdminTRC = 0;
				} else {
					ptr_InitHead->iMainTRC = 0;
				}
			}
			SignalLocalSemaphore( uiTrace_sh );
			return(0);
		}
	}
	SignalLocalSemaphore( uiTrace_sh );
	return(0);
ErrReturn:
	if ( (pGErrFile_p != NULL) && ( iErrorCode == -1 ) ) {
		if ( iThreadID == iWatch_tid ) {
			(pGErrFile_p->iWDogTrace) = errno;
		} else if ( iThreadID == iAdmin_tid ) {
			(pGErrFile_p->iAdmnTrace) = errno;
		} else {
			(pGErrFile_p->iMainTrace) = errno;
		}
	}

ErrReturn2:
	SignalLocalSemaphore( uiTrace_sh );
	return(-1);
}
/******************************************************** EnglishComment ****/
/*   ID         = GW_tsh_functrace                                          */
/*   name       = Take the Function Trace                                   */
/*   func       = Trace Requested Function to the File                      */
/*   io         =                                                           */
/*           ierrNumber : int  : Error Number                               */
/*           comment    : char : Any comment Requester wants to write       */
/*                                                                          */
/*   return     =                                                           */
/*      0       : Succeeded to log                                          */
/*      -1      : Couldn't access the Error Log File                        */
/*   common     =                                                           */
/*      Semaphore Handle    : uiFTrace_sh                                   */
/*   note       =                                                           */
/* date = May 19, 1995  by T.Sawahata                                       */
/*                                                                          */
/* Trade Secret of Hitachi, Ltd.                                            */
/* Do not disclose without written concession from Hitachi, Ltd.            */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/******************************************************** EnglishComment ****/

int GW_tsh_functrace( char *pcwhichFunc, int imode, int iFuncReturn ) {


	struct GFtrace_t {
		char    				cfuncHead[32];
		size_t					uiI_functr_size;
		struct	functr_Record	*pI_lastPtr;
		int						iRecCnt;
		struct	functr_Record	*pStrtWrTrp;
		int						iopenmodeflag;
		time_t					uiBaseTime;
		clock_t					uiBaseClock;
		char					cDummy[40];
	};

	struct functr_Record {
		clock_t	uifunctr_clock;
		char	cthFuncName[20];
		int		iStartEnd;
		int		ithID;
		char	cthName[20];
		int		iFuncReturn;

		char	cDummy[4];
	};

	off_t                   tmFileSize;
	struct GFtrace_t 		*pfunctr_InitHead;
	struct functr_Record	*pfunctr_Record;
	char				filename[20];
	LONG                llen;
	LONG                llen2;
	LONG                uisize;
	int 				iErrorCode;
	int					iworkint;
	int					ithreadID;
	int					ifileHandle;
	int					icCode;
	struct stat         *pstinfo;



	ithreadID = GetThreadID();

	if ( uiFTrace_sh == 0 )
		return -1;

	WaitOnLocalSemaphore( uiFTrace_sh );

	if ( pFtrace_p == NULL ) {
		iErrorCode = NO_FUNCTR_AREA;
		goto ErrReturn2;
	}

	uisize = FUNC_TR_SIZE;
	pfunctr_InitHead = (void*)pFtrace_p;
	iworkint = ( uisize - sizeof(struct GFtrace_t))/sizeof(struct functr_Record);

	uisize = (iworkint * sizeof(struct functr_Record) ) + sizeof(struct GFtrace_t);

	if ( (pfunctr_InitHead->pI_lastPtr) == NULL ) {
		strcpy( pfunctr_InitHead->cfuncHead , "DFA FUNCTION TRACE AREA" );

		pfunctr_InitHead->uiI_functr_size = uisize;
		pfunctr_InitHead->pI_lastPtr = (void*)((char*)pfunctr_InitHead + FTRHEADEND);

		pfunctr_InitHead->iRecCnt = 0;
		pfunctr_InitHead->pStrtWrTrp = NULL;
		pfunctr_InitHead->uiBaseTime = time(NULL);
		pfunctr_InitHead->uiBaseClock = clock();
		pfunctr_InitHead->cDummy[38] = '\n';
	}

	pfunctr_Record = pfunctr_InitHead->pI_lastPtr;
	pfunctr_Record->uifunctr_clock = clock();
	strncpy ( (pfunctr_Record->cthFuncName),pcwhichFunc,20 );

	pfunctr_Record->iStartEnd = imode;
	pfunctr_Record->ithID = ithreadID;
	GetThreadName( ithreadID, pfunctr_Record->cthName );
	pfunctr_Record->iFuncReturn = 0;
	if ( imode == GW_FUNC_END ) {
		pfunctr_Record->iFuncReturn = iFuncReturn;
	} else {
		pfunctr_Record->iFuncReturn = 0;
	}
	pfunctr_Record->cDummy[2] = '\n';
	( pfunctr_InitHead->pI_lastPtr ) = (void*)((char*)( pfunctr_InitHead->pI_lastPtr )
                                                      + sizeof(struct functr_Record));
	if ( ( pfunctr_InitHead->pI_lastPtr ) > (void*)((char*)pfunctr_InitHead + uisize
                                                      - sizeof(struct functr_Record) )){
		( pfunctr_InitHead->pI_lastPtr ) = (void*)((char*)pfunctr_InitHead + FTRHEADEND);
	}



	++(pfunctr_InitHead->iRecCnt);
	if ( iFuncFlushCount != 0 ) {
		if ( (pfunctr_InitHead->iRecCnt >= iFuncFlushCount)
				|| (imode == GW_FUNC_FLUSH) ) {
           	iErrorCode = 0;
			( pfunctr_InitHead->iRecCnt ) = 0;
			strcpy( filename, TRCFNAME_FUNC );
			icCode = access( filename, ACCESS_WR);
			if ( icCode == 0 ) {
				pstinfo = GW_malloc( sizeof(struct stat));
				if ( pstinfo == NULL ) {
					goto ErrReturn;
				}
				icCode = stat( filename, pstinfo );
				if ( icCode != 0 ) {
					iErrorCode = -1;
					GW_free( pstinfo, 0 );
					goto ErrReturn;
				}
				tmFileSize = ( pstinfo->st_size ) + sizeof( struct functr_Record );

				GW_free( pstinfo, 0 );
				if ( (tmFileSize >= MAXTRFLEN)
					|| (imode == GW_FUNC_FLUSH) ) {
					pfunctr_InitHead->iopenmodeflag = 0;
				}

				if ( pfunctr_InitHead->iopenmodeflag == 0 ) {
					unlink( filename );
					ifileHandle = creat ( filename, S_IWRITE );
				} else {
					ifileHandle = open ( filename, O_APPEND | O_WRONLY );
                }
				if ( ifileHandle == -1 ) {
					iErrorCode = -1;
					goto ErrReturn;
				}
				if ( (pfunctr_InitHead->iopenmodeflag) == 0 )  {

					pfunctr_InitHead->iopenmodeflag = 1;
					llen = uisize ;
					icCode = write ( ifileHandle, pfunctr_InitHead, llen );


					if ( icCode == EFAILURE ) {
						close( ifileHandle );
						iErrorCode = -1;
						goto ErrReturn;
					}

					pfunctr_InitHead->pStrtWrTrp = (pfunctr_InitHead->pI_lastPtr);

				} else {
					llen = iFuncFlushCount * sizeof(struct functr_Record);

					if ( ((char*)(pfunctr_InitHead->pStrtWrTrp) + llen) >=
                    	                           ((char*)(pfunctr_InitHead) + uisize )) {

						llen2 = ((char*)(pfunctr_InitHead) + uisize )
										- (char*)(pfunctr_InitHead->pStrtWrTrp);
						if ( llen2 != 0 ){
							icCode = write ( ifileHandle, (pfunctr_InitHead->pStrtWrTrp), llen2 );
							if ( icCode == EFAILURE ) {
								close( ifileHandle );
								iErrorCode = -1;
								goto ErrReturn;
							}
						}
							llen2  = llen - llen2;
							icCode = write ( ifileHandle, (void *)((char*)pfunctr_InitHead
                                                 + sizeof( struct GFtrace_t )), llen2 );
							if ( icCode == EFAILURE ) {
								close( ifileHandle );
								iErrorCode = -1;
								goto ErrReturn;
							}
							pfunctr_InitHead->pStrtWrTrp = (void *)((char*)pfunctr_InitHead
														+ sizeof(struct GFtrace_t) + llen2);

					}else {
						icCode = write( ifileHandle, (pfunctr_InitHead->pStrtWrTrp), llen );
						if ( icCode == EFAILURE ) {
							close( ifileHandle );
							iErrorCode = -1;
							goto ErrReturn;
						}
						pfunctr_InitHead->pStrtWrTrp = pfunctr_InitHead->pI_lastPtr;
					}
				}
				icCode = close ( ifileHandle );
				if ( icCode != 0 ) {
					iErrorCode = -1;
					goto ErrReturn;
				}

				( pfunctr_InitHead->iRecCnt ) = 0;
			}
		}
	} else {
			pfunctr_InitHead->pStrtWrTrp = NULL;
			pfunctr_InitHead->iopenmodeflag = 0;
	}
	SignalLocalSemaphore( uiFTrace_sh );
	return(0);

ErrReturn:
	if ( ( pGErrFile_p != NULL ) && ( iErrorCode == -1 ) ) {
		( pGErrFile_p->iFuncTrace ) = errno;
	}

ErrReturn2:
	SignalLocalSemaphore( uiFTrace_sh );
	return(-1);
}
/********************************************************** EnglishComment ***/
/* ID     = GW_Message                                                       */
/* name   = Outputs formatted message under control of format like printf()  */
/* func   = Outputs formatted message to one of destinations listed below.   */
/*            (1) DFA Gateway's Console                                      */
/*            (2) all Clients' screen                                        */
/*            (3) one Client's screen                                        */
/*          The format character is limited printf-rule.  It allows          */
/*            %x, %X, %f, %d or %s,  but does't examine precision, e.g. %6d. */
/*          Length of letters of "Formatted" string must be less than 2047.  */
/* i/o    = Msg_mode:int :inp: Specifies Message Output Destination.         */
/*                  DFAM_CON         : NetWare Server Console                */
/*                  DFAM_NWC_ALL     : All NetWare Client who use Gateway    */
/*                  DFAM_NWC         : The connNum specified NetWare Client  */
/*                                               who uses Gateway            */
/*          connNum :int :inp: integer to specify receiver of message        */
/*                                when Msg_mode is setted as DFAM_NWC        */
/*          Message_ID:char *:inp: format character contains DFA-style       */
/*                                   message-id.                             */
/*          4th argument or later: value converted by format character       */
/* return =  0: Success                                                      */
/*          -1: bad arguments                                                */
/*          -2: error in writing log file                                    */
/*          -3: uiConnNum does not match with any available connections      */
/*          -5: memory shortage                                              */
/*                                                                           */
/* date = March 2, 1995  by Tomoharu Sawahata                                */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/
int GW_Message ( int Msg_mode, int *connNum, int Msglog_mode, char *Message_ID, ... ){

	va_list	pcap;
	char	*p, *sval;
	int		ival;
	double	dval;

	int 	iwkflag;
	WORD	connList[CONN_MAX];
	struct  GTGroup_t	*pGTG;
	int		iarrynum;
	int     iconnCount;

	time_t	timer;
	char    *ctimer;
	size_t	uilen;

	off_t  			Msglogfsize;
	int				cCode;
	struct stat 	*statInfo = NULL;
	char            *pbuf = NULL;
	char            *pspbuf = NULL;
	char            *pspbuf2 = NULL;
	int             ichar,ichar2,ichar3,ichar4;
	int             ichar5,ichar6;
	int             imsgoutlen;
	int             inopersent;
	int             ireturn;

	if ( uiMsg_sh == 0 ) {
		return -1;
	}

	WaitOnLocalSemaphore( uiMsg_sh );
	ireturn = 0;

	if ((Msg_mode == 0) || ( Msg_mode > ( DFAM_CON | DFAM_NWC_ALL | DFAM_NWC ) )){

		ireturn = -1;
		goto MsgReturn;
	}



	if (( Msglog_mode < 0 ) || ( Msglog_mode > LOG_YES ) ){

		ireturn = -1;
		goto MsgReturn;
	}


		va_start(pcap, Message_ID );
		pbuf    = GW_malloc( MAXMSGSIZE );
		pspbuf  = GW_malloc( MAXMSGSIZE );
		pspbuf2 = GW_malloc( MAXMSGSIZE );
		if ( pbuf == NULL ) {
			ireturn = -5;
			goto MsgReturn;
		}
		if ( pspbuf == NULL ) {
			ireturn = -5;
			goto MsgReturn;
		}
		if ( pspbuf2 == NULL ) {
			ireturn = -5;
			goto MsgReturn;
		}
		strncpy( pbuf, Message_ID, ( MAXMSGSIZE - 1 ) );
		memset ( (pbuf + MAXMSGSIZE - 1 ), '\0', 1 );
		ichar = 0;
		ichar2 = 0;
		ichar3 = 0;
		ichar4 = 0;
		inopersent = 0;
		for (p = pbuf ; *p; p++) {
			if ( *p != '%' ) {
				++ichar;
				continue;
			}
			switch (*++p) {
			case 'd':case 'x':case 'X':
				memset( (pbuf + ichar2 + ichar + 2), '\0', 1 );

				ival = va_arg(pcap, int);
				ichar3 = sprintf( (pspbuf+ichar4), (pbuf+ichar2), ival );
				ichar4 += ichar3;
				ichar2 += ichar + 2;
				++inopersent;
				strcpy( pbuf, Message_ID );
				ichar  = 0;
				break;
			case 'f':
				memset( ( pbuf + ichar2 + ichar + 2) , '\0', 1 );

				dval = va_arg(pcap, double);
				ichar3 = sprintf( (pspbuf+ichar4), (pbuf+ichar2), dval );
				ichar4 += ichar3;
				ichar2 += ichar + 2;
				++inopersent;
				strcpy( pbuf, Message_ID );
				ichar  = 0;
				break;
			case 's':
				memset( ( pbuf + ichar2 + ichar +2 ) , '\0', 1 );

				sval = va_arg( pcap, char* );
				ichar3 = sprintf( (pspbuf+ichar4), (pbuf+ichar2), sval );
				ichar4 += ichar3;
				ichar2 += ichar + 2;
				++inopersent;
				strcpy( pbuf, Message_ID );
				ichar  = 0;
				break;
			default:
				++ichar;
				break;
			}
		}
		va_end(pcap);
		ichar = sprintf( (pspbuf+ichar4), (pbuf+ichar2) );
		ichar4 += ichar;
		imsgoutlen = ichar4;
		iwkflag = 0;
		iwkflag = Msg_mode & DFAM_CON;
		if ( iwkflag == DFAM_CON ) {
			ichar5 = 0;
			while ( ichar5 <= ichar4 ) {

				++ichar5;
			}
			imsgoutlen = ichar5 - 1;
		}
		memcpy( pspbuf2, pspbuf, MAXMSGSIZE );
	iwkflag = 0;
	iwkflag = Msg_mode & DFAM_CON;
	if ( iwkflag == DFAM_CON ) {
		printf( pspbuf );
	}


	iwkflag = 0;
	iwkflag = Msg_mode & DFAM_NWC_ALL;
	if ( iwkflag == DFAM_NWC_ALL ) {

		pGTG = pTGroup_sp;
		iarrynum = 0;
		iconnCount = 0;
		while ( pGTG != NULL ) {
			connList[ iarrynum ] = ( WORD )( pGTG -> uiConnNum );

			pGTG = ( pGTG -> pNext );
			++iarrynum;
			++iconnCount;
		}
		if ( iconnCount == 0 ) {
			ireturn = -3;
			goto MsgReturn;
		}
		memset ( (pspbuf + MAXNWCLEN * 2 - 1 ), '\0', 1 );
		if ( imsgoutlen > MAXNWCLEN * 2 ) {
			imsgoutlen = MAXNWCLEN * 2;
		}
		ichar5 = 0;
		while ( ichar5 < MAXNWCLEN * 2 ) {
			if ( strncmp ( (pspbuf + ichar5) , "\n" , 1) == 0 ) {
				memset ( (pspbuf + ichar5) , '\0', 1 ) ;
				break;
			}
			++ichar5;
		}
		ichar5 = imsgoutlen;
		ichar5 += MAXNWCLEN;
		ichar4 = 0;
		ichar6 = 0;
		while ( ichar5 > MAXNWCLEN ) {
			if ( ichar5  < MAXNWCLEN * 2 ) {
				ichar4 += ichar5 - MAXNWCLEN - 1;
			} else {
				ichar4 += MAXNWCLEN;
			}
			memset ( (pspbuf + ichar4 ) , '\0', 1 );
			ichar5 -= MAXNWCLEN;
			iarrynum = SendConsoleBroadcast( pspbuf + (ichar6 * MAXNWCLEN), iconnCount, connList );
			delay(DFAMWALLWAIT);
			++ichar6;
			strcpy ( (pspbuf + ichar4),( pspbuf2 + ichar4 ));
		}



	}

	iwkflag = 0;
	iwkflag = Msg_mode & DFAM_NWC;
	if ( iwkflag == DFAM_NWC ) {
		connList[ 0 ] = ( WORD ) *connNum;
		iconnCount = 1;
		memset ( (pspbuf + MAXNWCLEN * 2 - 1 ), '\0', 1 );
		if ( imsgoutlen > MAXNWCLEN * 2 ) {
			imsgoutlen = MAXNWCLEN * 2;
		}
		ichar5 = 0;
		while ( ichar5 < MAXNWCLEN * 2 ) {
			if ( strncmp ( (pspbuf + ichar5) , "\n" , 1) == 0 ) {
				memset ( (pspbuf + ichar5) , '\0', 1 ) ;
				break;
			}
			++ichar5;
		}
		ichar5 = imsgoutlen;
		ichar5 += MAXNWCLEN;
		ichar4 = 0;
		ichar6 = 0;
		while ( ichar5 > MAXNWCLEN ){
			if ( ichar5  < MAXNWCLEN * 2 ) {
				ichar4 += ichar5 - MAXNWCLEN - 1;
			} else {
				ichar4 += MAXNWCLEN;
			}
			memset ( (pspbuf + ichar4 ) , '\0', 1 );
			ichar5 -= MAXNWCLEN;
			iarrynum = SendConsoleBroadcast( pspbuf + (ichar6 * MAXNWCLEN), iconnCount, connList );
			delay(DFAMWALLWAIT);
			++ichar6;
			strcpy ( (pspbuf + ichar4),( pspbuf2 + ichar4 ));
		}



	}





	iwkflag = 0;
	iwkflag = Msglog_mode & LOG_YES;

	if ( iwkflag == LOG_YES ) {

		timer = time(NULL);
		ctimer = ctime( &timer );
		uilen = LOG_TIME;
		ctimer += 4;
		strncpy ( pspbuf, ctimer, uilen );
		strcpy ( (pspbuf + LOG_TIME ), pspbuf2 );
		if ( pszCur_Msg_Fname == NULL ){
			ireturn = -2;
			goto MsgReturn;
		}
		statInfo = NULL;
		statInfo = ( struct stat *)GW_malloc(sizeof(struct stat));

		if ( statInfo == NULL ) {
			ireturn = -5;
			goto MsgReturn;
		}
		cCode = stat( pszCur_Msg_Fname, statInfo );
		if ( cCode != 0 ) {
			if ( errno == ENOENT ) {
				hMessageLog_fh = fopen( pszCur_Msg_Fname, "w" );
				if ( hMessageLog_fh == NULL ) {
					ireturn = -2;
					goto MsgReturn;
				}
				Msglogfsize = 0;
			} else {
				ireturn = -2;
				goto MsgReturn;
			}
		} else {
			Msglogfsize = statInfo->st_size;
			hMessageLog_fh = fopen( pszCur_Msg_Fname, "a" );
			if ( hMessageLog_fh == NULL ) {
				ireturn = -2;
				goto MsgReturn;
			}
		}
		if ( Msglogfsize + imsgoutlen >= MSGLOGMAX ) {
			cCode = fclose( hMessageLog_fh );
			if ( cCode != NULL ) {
				ireturn = -2;
				goto MsgReturn;
			}
			hMessageLog_fh = NULL;
			if ( strcmp( pszCur_Msg_Fname , aszMessageLogFileName1 ) == 0 ) {

				pszCur_Msg_Fname = aszMessageLogFileName2;

			} else if ( strcmp( pszCur_Msg_Fname , aszMessageLogFileName2 ) == 0 ){

				pszCur_Msg_Fname = aszMessageLogFileName1;

			}
			unlink( pszCur_Msg_Fname );
			hMessageLog_fh = fopen ( pszCur_Msg_Fname, "w" );

			if ( hMessageLog_fh == NULL ) {
				ireturn = -2;
				goto MsgReturn;
			}
		}
		cCode = fprintf ( hMessageLog_fh, pspbuf );
		if( cCode < 0 ) {
			if ( fclose( hMessageLog_fh ) == 0 ) {
				hMessageLog_fh = NULL;
			}
			ireturn = -2;
			goto MsgReturn;
		}
		cCode = fclose ( hMessageLog_fh );
		if ( cCode < 0 ) {
			ireturn = -2;
			goto MsgReturn;
		}
		hMessageLog_fh = NULL;
		ireturn = 0;
		goto MsgReturn;
	}
	ireturn = 0;
	goto MsgReturn;

MsgReturn:

	if ( pbuf != NULL ) {
		GW_free( pbuf, 0 );
		pbuf = NULL;
	}
	if (pspbuf != NULL ) {
		GW_free( pspbuf, 0 );
		pspbuf = NULL;
	}
	if (pspbuf2 != NULL ) {
		GW_free( pspbuf2, 0 );
		pspbuf2 = NULL;
	}
	if (statInfo != NULL ) {
		GW_free( statInfo, 0 );
		statInfo = NULL;
	}
	if ( ( ireturn == -2 ) && ( pGErrFile_p != NULL ) ){
		if ( strcmp( pszCur_Msg_Fname , aszMessageLogFileName1 ) == 0 ) {

			( pGErrFile_p->iErrMsg001 ) = errno;
		} else if ( strcmp( pszCur_Msg_Fname , aszMessageLogFileName2 ) == 0 ){

			( pGErrFile_p->iErrMsg001 ) = errno;
		}
	}
	SignalLocalSemaphore( uiMsg_sh );
	return( ireturn );
}


/************************************************************ EnglishComment */
/*  ID      = DFARASlog                                                      */
/*                                                                           */
/* name = Flushing the error logging file                                    */
/* func = Switches the context to DFA.NLM, and flushes the error logging     */
/*        file.                                                              */
/*                                                                           */
/* i/o  = none                                                               */
/*                                                                           */
/* return = DRNOERR    : successfully completed                              */
/*          DRNOGETMODE: no file to get                                      */
/*          DRREMOVEERR: failed to delete a file                             */
/*                                                                           */
/* common = iDFANLM_ID                                                       */
/* note   = none                                                             */
/*                                                                           */
/* date = September 18, 1995  by Ryuji Shinchi                               */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/************************************************************ EnglishComment */
int DFARASlog( void )
{
	int				iErr = DRNOERR;
	int				iRet1;
	int				iRet2;
	int				iCurTG_ID;


	if ( iMain_tgid == 0 )
		return DRNORESOURCE;

	iCurTG_ID = SetThreadGroupID( iMain_tgid );

	if ( uiErrLog_sh == 0 ) {
		SetThreadGroupID( iCurTG_ID );
		return DRNORESOURCE;
	}
	WaitOnLocalSemaphore( uiErrLog_sh );


	iRet1 = access( ERRORLOGFILE1_DFL,  ACCESS_WR|ACCESS_RD );
	iRet2 = access( ERRORLOGFILE2_DFL,  ACCESS_WR|ACCESS_RD );
	if ( iRet1 && iRet2 ) {
		iErr = DRNOGETMODE;
		goto labelDFARASlog_quit;
	}

	if ( !iRet1 ) {
		iRet1 = unlink( ERRORLOGFILE1_DFL );
		if ( iRet1 ) {
			iErr = DRREMOVEERR;
			goto labelDFARASlog_quit;
		}
	}
	if ( !iRet2 ) {
		iRet2 = unlink( ERRORLOGFILE2_DFL );
		if ( iRet2 ) {
			iErr = DRREMOVEERR;
			goto labelDFARASlog_quit;
		}
	}



labelDFARASlog_quit:

	SignalLocalSemaphore( uiErrLog_sh );
	SetThreadGroupID( iCurTG_ID );

	return iErr;
}


/************************************************************ EnglishComment */
/*  ID      = DFARASmessage                                                  */
/*                                                                           */
/* name = Flushing the message logging file                                  */
/* func = Switches the context to DFA.NLM, and flushes the message logging   */
/*        file.                                                              */
/*                                                                           */
/* i/o  = none                                                               */
/*                                                                           */
/* return = DRNOERR    : successfully completed                              */
/*          DRNOGETMODE: no file to get                                      */
/*          DRREMOVEERR: failed to delete a file                             */
/*                                                                           */
/* common = iDFANLM_ID                                                       */
/* note   = none                                                             */
/*                                                                           */
/* date = September 18, 1995  by Ryuji Shinchi                               */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/************************************************************ EnglishComment */
int DFARASmessage( void )
{
	int				iErr = DRNOERR;
	int				iRet1;
	int				iRet2;
	int				iCurTG_ID;


	if ( iMain_tgid == 0 )
		return DRNORESOURCE;

	iCurTG_ID = SetThreadGroupID( iMain_tgid );

	if ( uiMsg_sh == 0 ) {
		SetThreadGroupID( iCurTG_ID );
		return DRNORESOURCE;
	}
	WaitOnLocalSemaphore( uiMsg_sh );


	iRet1 = access( MESSAGELOGFILE1_DFL,  ACCESS_WR|ACCESS_RD );
	iRet2 = access( MESSAGELOGFILE2_DFL,  ACCESS_WR|ACCESS_RD );
	if ( iRet1 && iRet2 ) {
		iErr = DRNOGETMODE;
		goto labelDFARASmessage_quit;
	}

	if ( !iRet1 ) {
		iRet1 = unlink( MESSAGELOGFILE1_DFL );
		if ( iRet1 ) {
			iErr = DRREMOVEERR;
			goto labelDFARASmessage_quit;
		}
	}
	if ( !iRet2 ) {
		iRet2 = unlink( MESSAGELOGFILE2_DFL );
		if ( iRet2 ) {
			iErr = DRREMOVEERR;
			goto labelDFARASmessage_quit;
		}
	}



labelDFARASmessage_quit:

	SignalLocalSemaphore( uiMsg_sh );
	SetThreadGroupID( iCurTG_ID );

	return iErr;
}


/*********************************************************** EnglishComment **/
/*  ID      = DFARAStrace                                                    */
/*                                                                           */
/* name = Invoking a trace function                                          */
/* func = Switches the context to DFA.NLM, and invokes the trace outputting  */
/*        function.                                                          */
/*                                                                           */
/* i/o  = none                                                               */
/*                                                                           */
/* return = DRNOERR: successfully completed                                  */
/*          DRNOGETMODE: no file to get                                      */
/*          DRREMOVEERR: an error occurred while a file is processed         */
/*          DRFAILURE: some errors except listed above occurred              */
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
int DFARAStrace( void )
{
	int				iErr = DRNOERR;
	int				iFret = 0;
	int				iMret = 0;
	int				iAret = 0;
	int				iWret = 0;
	int				iFAret = 0;
	int				iMAret = 0;
	int				iAAret = 0;
	int				iWAret = 0;
	int				iFDret = 0;
	int				iMDret = 0;
	int				iADret = 0;
	int				iWDret = 0;

	int				iCurTG_ID;

	if ( iMain_tgid == 0 )
		return DRNORESOURCE;

	iCurTG_ID = SetThreadGroupID( iMain_tgid );

	iFAret = access( TRCFNAME_FUNC,  ACCESS_WR|ACCESS_RD );
	iMAret = access( TRCFNAME_MAIN,  ACCESS_WR|ACCESS_RD );
	iAAret = access( TRCFNAME_ADMIN, ACCESS_WR|ACCESS_RD );
	iWAret = access( TRCFNAME_WATCH, ACCESS_WR|ACCESS_RD );



	if ( iFAret && iMAret && iAAret && iWAret ) {
		iErr = DRNOGETMODE;
		goto labelDFARAStrace_quit;
	}


	if ( !iFAret ) {
		if ( (uiFTrace_sh == 0) || (pFtrace_p == NULL) ) {
			iFDret = unlink( TRCFNAME_FUNC );
		} else {
			iFret = GW_tsh_functrace("DFARAStrace", GW_FUNC_FLUSH, 0);
		}
	}



	if ( !iMAret ) {
		if ( (uiTrace_sh == 0) || (pMainTracep == NULL) ) {
			iMDret = unlink( TRCFNAME_MAIN );
		} else {
			iMret = GW_tsh_trace(pMainTracep, COM_TR_SIZE,
					GW_MAIN_TRACE_FLUSH, 0, 0, NULL);
		}
	}



	if ( !iAAret ) {
		if ( (uiTrace_sh == 0) || (pAdminTracep == NULL) ) {
			iADret = unlink( TRCFNAME_ADMIN );
		} else {
			iAret = GW_tsh_trace(pAdminTracep, ADM_TR_SIZE,
					GW_ADMIN_TRACE_FLUSH, 0, 0, NULL);
		}
	}



	if ( !iWAret ) {
		if ( (uiTrace_sh == 0) || (pWatchdogTracep == NULL) ) {
			iWDret = unlink( TRCFNAME_WATCH );
		} else {
			iWret = GW_tsh_trace(pWatchdogTracep, WDG_TR_SIZE,
					GW_WATCHDOG_TRACE_FLUSH, 0, 0, NULL);
		}
	}


	if ( iFDret || iMDret || iADret || iWDret ) {
		iErr = DRREMOVEERR;
	}

	if ( iFret || iMret || iAret || iWret ) {
		iErr = DRFAILURE;
	}

labelDFARAStrace_quit:
	SetThreadGroupID( iCurTG_ID );

	return iErr;
}

