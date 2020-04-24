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
 * $Log: gdirhdl.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:47  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:11  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:47:00  takano
 * 	First Submit
 * 	[1996/02/21  07:46:56  takano]
 * 
 * $EndLog$
 */
#include "gdir.h"

/*********************************************************** EnglishComment **/
/* ID = TouchFile                                                            */
/*                                                                           */
/* name = Making a sparse file and setting a time stamp                      */
/* func = Makes a sparse file that has the designated name and size.  If the */
/*        file already exists, changes the size of the file, and changes the */
/*        time stamp to the designated one.                                  */
/*                                                                           */
/* i/o  = pszNWPath:char *  :inp: sparse file name to be made                */
/*        iSize    :LONG    :inp: sparse file size to be made                */
/*        iTime    :LONG    :inp: time stamp                                 */
/*        pStat    :struct stat *:inp: if the file already exists, this      */
/*                                function issues stat() to the file, and    */
/*                                sets the results to pStat.                 */
/*        iMode    :int     :inp: indicator of already exist/not exists:     */
/*                                   DMMkModeNot  : does not exist           */
/*                                   DMMkModeExist: already exists           */
/*                                   DMMkModeUnknown --                      */
/*                                                                           */
/* return = 0: normal end                                                    */
/*	    EDMsystemfile                                                        */
/*          EDMwarning                                                       */
/*          (DMMakeSparseFile)default                                        */
/*          EDMdiskspace                                                     */
/*          EDMdirectoryblock                                                */
/*          EDMcreat                                                         */
/*          EDMseek                                                          */
/*          EDMwrite                                                         */
/*          EDMclose                                                         */
/*          (ChangeFileTime)default                                          */
/*          EDMutime                                                         */
/*                                                                           */
/* note = If failed to generate the sparse file, DMMakeSparseFile() deletes  */
/*        the name conversion table.                                         */
/* date = November 30, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int	TouchFile( char *pszNWPath,
				long iSize, time_t uiTime,
				struct stat *pStat, int iMode )
{
	int		iErr = 0;
	int		iRet;
	char	*pszPath = NULL;


	ThreadSwitch();

	pszPath = (char *)GW_malloc( NW_MAX_PATH + 1 );
	if ( pszNWPath == NULL ) {
		iErr = EDMmalloc;
		goto labelTouchFile_q;
	}

	NWDirname( pszPath, pszNWPath );
	if ( (pszPath[strlen(pszPath)-1] == ':') && (CheckSystemFile(pszNWPath)) ) {
		DeleteName_tByName( pszNWPath );
		iErr = EDMsystemfile;
		goto labelTouchFile_q;
	}

	switch ( iMode ) {
	case DMMkModeNot:
		if ( pStat->st_size != iSize ) {
			iRet = DMMakeSparseFile( pszNWPath, iSize, iMode );
			switch ( iRet ) {
			case 0:
				break;
			case EDMdiskspace:	case EDMdirectoryblock:
				iErr = iRet;
				goto labelTouchFile_q;
			case EDMcreat:
				DeleteName_tByName( pszNWPath );
				iErr = iRet;
				goto labelTouchFile_q;
			case EDMwrite:
			case EDMseek:
			case EDMclose:
				iErr = iRet;
				goto labelTouchFile_q;
			default:
				iErr = iRet;
				goto labelTouchFile_q;
			}
		}

		iRet = ChangeFileTime( pszNWPath, uiTime );
		switch ( iRet ) {
		case 0:
			break;
		case EDMutime:
			iErr = iRet;
			goto labelTouchFile_q;
		default:
			iErr = iRet;
			goto labelTouchFile_q;
		}

		break;

	case DMMkModeExist:
		if ( pStat->st_size != iSize ) {
			iRet = DMMakeSparseFile( pszNWPath, iSize, iMode );
			switch ( iRet ) {
			case 0:
				break;
			case EDMdiskspace:	case EDMdirectoryblock:
				iErr = iRet;
				goto labelTouchFile_q;
			case EDMcreat:
				iErr = iRet;
				goto labelTouchFile_q;
			case EDMwrite:
			case EDMseek:
			case EDMclose:
				iErr = iRet;
				goto labelTouchFile_q;
			default:
				iErr = iRet;
				goto labelTouchFile_q;
			}

			iRet = ChangeFileTime( pszNWPath, uiTime );
			switch ( iRet ) {
			case 0:
				break;
			case EDMutime:
				iErr = iRet;
				goto labelTouchFile_q;
			default:
				iErr = iRet;
				goto labelTouchFile_q;
			}

		} else if ( pStat->st_mtime != uiTime ) {
			iRet = ChangeFileTime( pszNWPath, uiTime );
			switch ( iRet ) {
			case 0:
				break;
			case EDMutime:
				iErr = iRet;
				goto labelTouchFile_q;
			default:
				iErr = iRet;
				goto labelTouchFile_q;
			}
		}

		break;

	default:

		iErr = EDMwarning;
		goto labelTouchFile_q;
	}

	iErr = 0;
labelTouchFile_q:
	if ( pszPath != NULL )
		GW_free( pszPath, 0 );
	return iErr;

}


/*********************************************************** EnglishComment **/
/* ID = TouchFileByDFSPath                                                   */
/*                                                                           */
/* name = Making a sparse file and setting a time stamp                      */
/* func = Makes a sparse file that has the designated name and size.  If the */
/*        file already exists, changes the size of the file, and changes the */
/*        time stamp to the designated one.  If the path is a reserved file, */
/*        nonprocessing is made.                                             */
/*                                                                           */
/* i/o  = struct GVolume_t:char *:inp: Volume where the sparse file is made  */
/*        pszDFSPath      :char *:inp: sparse file name to be made           */
/*        iSize           :LONG  :inp: sparse file size to be made           */
/*        iTime           :LONG  :inp: time stamp                            */
/*                                                                           */
/* return = 0: normal end                                                    */
/*	    EDMmalloc:                                                           */
/*	    EDMagentdie                                                          */
/*	    EDMshutdownreq                                                       */
/*	    EDMnameconv                                                          */
/*	    EDMlongpath                                                          */
/*	    (TouchFile)                                                          */
/*	    ...                                                                  */
/*	    others                                                               */
/*                                                                           */
/* common = none                                                             */
/*                                                                           */
/* note = (1) This function converts the DFS path name, and requests         */
/*            TouchFile() to generate a sparse file.                         */
/*        (2) If failed to generate the sparse file, DMMakeSparseFile()      */
/*            deletes the name conversion table.                             */
/* date = November 30, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int	TouchFileByDFSPath(	struct GVolume_t *pVolTbl, char *pszDFSPath,
						long iSize, time_t uiTime )
{
	int						iRet;
	int						iErr = 0;
	struct stat 			Stat;
	char					*pszNWPath = NULL;


	if ( (pszNWPath = GW_malloc( NW_MAX_PATH + 1 )) == NULL ) {
		iErr = EDMmalloc;
		goto TouchFileByDFSPath_Q;
	}


	iRet = GetNWPath2( pszDFSPath, pVolTbl->aszNWVol, pszNWPath );
	switch ( iRet ) {
	case 0:
		break;
	default:
		iErr = iRet;
		goto TouchFileByDFSPath_Q;
	}


	if ( stat( pszNWPath, &Stat ) ) {
		switch ( errno ) {
		case ENOENT:
			TouchFile( pszNWPath, iSize, uiTime, NULL, DMMkModeNot );
			break;
		default:
			break;
		}
	} else {
		if ( S_ISDIR(Stat.st_mode) ) {
			if ( iRet = DeleteTree( pszNWPath, DMDelModeSync ) ) {
				iErr = iRet;
			} else {
				iRet = TouchFile( pszNWPath, iSize, uiTime, NULL, DMMkModeNot );
				iErr = iRet;
			}
		} else  {
			iRet = TouchFile( pszNWPath, iSize, uiTime, &Stat, DMMkModeExist );
			iErr = iRet;
		}
	}


TouchFileByDFSPath_Q:
	if ( pszNWPath != NULL ) {
		DMfree( pszNWPath, "TouchFileByDFSPath" );
	}
	return iErr;
}

/*********************************************************** EnglishComment **/
/* ID = TouchDir                                                             */
/*                                                                           */
/* name = Making a directory and setting a time stamp                        */
/* func = Makes a directory with a designated name, and changes the time     */
/*        to a designated one.                                               */
/*                                                                           */
/* i/o  = pszNWPath:char *  :inp: directory path name to be made             */
/*        iTime    :LONG    :inp: time stamp                                 */
/*        pStat    :struct stat *:inp: if the directory already exists, this */
/*                                function issues stat() to the directory,   */
/*                                and sets the results to pStat.             */
/*        iMode    :int     :inp: indicator of already exist/not exists:     */
/*                                   DMMkModeNot  : does not exist           */
/*                                   DMMkModeExist: already exists           */
/* return = 0: normal end                                                    */
/*	    EDMsystemfile                                                        */
/*	    EDMdiskspace                                                         */
/*	    EDMdirectoryblock                                                    */
/*	    EDMdirlevel                                                          */
/*	    EDMmkdir                                                             */
/*	    <-ChangeDirectoryTime                                                */
/*	    EDMwarning                                                           */
/* common = none                                                             */
/* :::                                                                       */
/* note = If failed to generate the directory, deletes the entry of the      */
/*        name conversion table.                                             */
/* date = January 16, 1995 by Ryuji Shinchi                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int	TouchDir( char *pszNWPath,
				time_t uiTime, struct stat *pStat, int iMode )
{
	int		iErr = 0;
	int		iRet;
	int		iLen;
	int		iNWE;
	char	*pszPath = NULL;



	ThreadSwitch();

	pszPath = (char *)GW_malloc( NW_MAX_PATH + 1 );
	if ( pszPath == NULL ) {
		iErr = EDMmalloc;
		goto labelTouchDir_q;
	}

	NWDirname( pszPath, pszNWPath );
	if ( (pszPath[strlen(pszPath)-1] == ':') && (CheckSystemFile(pszNWPath)) ) {
		DeleteName_tByName( pszNWPath );
		iErr = EDMsystemfile;
		goto labelTouchDir_q;
	}

	iLen = strlen( pszNWPath );
	if ( (iLen > 2 ) && (pszNWPath[iLen-2]==':' ) && ( pszNWPath[iLen-1]=='\\' ) ) {

		iErr = 0;
		goto labelTouchDir_q;
	}

	switch ( iMode ) {
	case DMMkModeNot:
		WaitOnLocalSemaphore( uiCopyFile_sh );
		if ( DMCheckAccessFileList( pszNWPath ) ) {
			SignalLocalSemaphore( uiCopyFile_sh );
			iErr = EDMusingfile;
			goto labelTouchDir_q;
		}
		iRet = mkdir( pszNWPath );
		iNWE = NetWareErrno;
		SignalLocalSemaphore( uiCopyFile_sh );
		if ( iRet ) {
			switch ( iNWE ) {
			case 0:
				break;
			case ERR_INSUFFICIENT_SPACE:
				DeleteName_tByName( pszNWPath );
				iErr = EDMdiskspace;
				goto labelTouchDir_q;
			case ERR_DIRECTORY_FULL:
				DeleteName_tByName( pszNWPath );
				iErr = EDMdirectoryblock;
				goto labelTouchDir_q;
			case ERR_INVALID_PATH:
				DeleteName_tByName( pszNWPath );
				iErr = EDMdirlevel;
				goto labelTouchDir_q;
			default:
				DeleteName_tByName( pszNWPath );
				iErr = EDMmkdir;
				goto labelTouchDir_q;
			}
		}

		iRet = ChangeDirectoryTime( pszNWPath, uiTime );
		switch ( iRet ) {
		case 0:
			break;
		case EDMutime:
			iErr = iRet;
			goto labelTouchDir_q;
		default:
			iErr = iRet;
			goto labelTouchDir_q;
		}

		break;

	case DMMkModeExist:
		if ( pStat->st_ctime != uiTime ) {
			iRet = ChangeDirectoryTime( pszNWPath, uiTime );
			switch ( iRet ) {
			case 0:
				break;
			case EDMutime:
				iErr = iRet;
				goto labelTouchDir_q;
			default:
				iErr = iRet;
				goto labelTouchDir_q;
			}
		}

		break;

	default:

		iErr = EDMwarning;
		goto labelTouchDir_q;
	}

	iErr = 0;

labelTouchDir_q:
	if ( pszPath != NULL )
		GW_free( pszPath, 0 );

	return iErr;
}

/*********************************************************** EnglishComment **/
/* ID = DMMakeSparseFile                                                     */
/*                                                                           */
/* name = Making a sparse file                                               */
/* func = Makes a sparse file that has the designated name and size.  If the */
/*        file already exists, changes the size of the file.                 */
/*                                                                           */
/* i/o  = pszNWPath:char *  :inp: sparse file name to be made                */
/*        iSize    :LONG    :inp: sparse file size to be made                */
/*        iMode    :int     :inp: indicator of already exist/not exists:     */
/*                                   DMMkModeNot  : does not exist           */
/*                                   DMMkModeExist: already exists           */
/* return = 0: normal end                                                    */
/*	    EDMusingfile                                                         */
/*	    EDMdiskspace                                                         */
/*	    EDMcreat                                                             */
/*	    EDMwrite                                                             */
/*	    EDMclose                                                             */
/* common = none                                                             */
/* :::                                                                       */
/* note = If failed to generate a file when iMode==0, deletes the entry of   */
/*        the name conversion table.                                         */
/* date = November 30, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int	DMMakeSparseFile( char *pszNWPath, long iSize, int iMode)
{
	int		iFileHdl;
	int		iRet;
	int		iE;
	BYTE	byDummyData = 0x00;


	WaitOnLocalSemaphore( uiCopyFile_sh );
	if ( DMCheckAccessFileList( pszNWPath ) ) {
		SignalLocalSemaphore( uiCopyFile_sh );
		return EDMusingfile;
	}



	iFileHdl = creat( pszNWPath, _A_NORMAL );
	iE = errno;
	if ( iFileHdl == -1 ) {
		if ( iMode == DMMkModeNot ) {
			DeleteName_tByName( pszNWPath );
		}
		SignalLocalSemaphore( uiCopyFile_sh );
		switch ( iE ) {
		case ENOSPC:
			return EDMdiskspace;
		default:
			return EDMcreat;
		}
	}

	if ( iSize != 0L ) {

		iRet = lseek( iFileHdl, (unsigned long)iSize - 1, SEEK_END );
		iE = errno;
		if ( iRet == - 1 ) {
			SignalLocalSemaphore( uiCopyFile_sh );
			switch ( iE ) {
			case ENOSPC:
				iRet = close( iFileHdl );
				return EDMdiskspace;
			default:
				iRet = close( iFileHdl );
				return EDMseek;
			}
		}
		iRet = write( iFileHdl, (char *)&byDummyData, 1L);
		iE = errno;
		if ( iRet == - 1 ) {
			SignalLocalSemaphore( uiCopyFile_sh );
			switch ( iE ) {
			case ENOSPC:
				iRet = close( iFileHdl );
				return EDMdiskspace;
			default:
				iRet = close( iFileHdl );
				return EDMwrite;
			}
		}
	}

	iRet = close( iFileHdl );
	SignalLocalSemaphore( uiCopyFile_sh );
	if ( iRet == - 1 ) {
		return EDMclose;
	}

	return 0;
}

/*********************************************************** EnglishComment **/
/* ID = DeleteFile                                                           */
/*                                                                           */
/* name = File deletion                                                      */
/* func = Deletes a file.                                                    */
/*        (1) Asks a user if OK to delete a file (DelModeInit).              */
/*        (2) If, under the directory, there are files being opened by       */
/*            a hook routine, those files and the directories reaching the   */
/*            files are not deleted (DelModeSync).                           */
/*        (3) Deletes a file without asking a user (DelModeClear).           */
/*                                                                           */
/* i/o  = pszNWPath:char *:inp: path name of the file to be deleted          */
/*        iMode    :int   :inp: processing indicator:                        */
/*                              0: initialization (DMDelModeInit)            */
/*                              1: synchronization (DMDelModeSync)           */
/*                              2: termination (DMDelModeClear)              */
/* return = 0: normal end                                                    */
/*	    EDMusingfile                                                         */
/*	    <-DeleteFileOrDir                                                    */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = November 30, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DeleteFile( char *pszNWPath, int iMode )
{
	int		iRet;

	iRet = DeleteFileOrDir( pszNWPath, DMFILE_T, iMode );

	return iRet;
}

/*********************************************************** EnglishComment **/
/* ID = DeleteDir                                                            */
/*                                                                           */
/* name = Directory deletion                                                 */
/* func = Deletes a directory.                                               */
/*        (1) Asks a user if OK to delete a directory (DelModeInit).         */
/*        (2) If, under the directory, there are files being opened by       */
/*            a hook routine, those files and the directories reaching the   */
/*            files are not deleted (DelModeSync).                           */
/*        (3) Deletes a directory without asking a user (DelModeClear).      */
/*                                                                           */
/* i/o  = pszNWPath:char *:inp: path name of the directory to be deleted     */
/*        iMode    :int   :inp: processing indicator:                        */
/*                              0: initialization (DMDelModeInit)            */
/*                              1: synchronization (DMDelModeSync)           */
/*                              2: termination (DMDelModeClear)              */
/* return = 0: normal end                                                    */
/*	    <-DeleteFileOrDir                                                    */
/* common =                                                                  */
/* :::                                                                       */
/* note = none                                                               */
/* date = November 30, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DeleteDir( char *pszNWPath, int iMode )
{
	int		iRet;

	iRet = DeleteFileOrDir( pszNWPath, DMDIR_T, iMode );
	return iRet;
}

/*********************************************************** EnglishComment **/
/* ID = DeleteFileOrDir                                                      */
/*                                                                           */
/* name = File deletion                                                      */
/* func = Deletes a file.                                                    */
/*        (1) Asks a user if OK to delete a file (DelModeInit).              */
/*        (2) If, under the directory, there are files being opened by       */
/*            a hook routine, those files and the directories reaching the   */
/*            files are not deleted (DelModeSync).                           */
/*        (3) Deletes a file without asking a user (DelModeClear).           */
/*                                                                           */
/* i/o  = pszNWPath:char *:inp: path name of the file to be deleted          */
/*        iType    :int   :inp: file/directory indicator                     */
/*                              0: file  (T_FILE)                            */
/*                              1: directory (T_DIR)                         */
/*        iMode    :int   :inp: processing indicator:                        */
/*                              0: initialization (DMDelModeInit)            */
/*                              1: synchronization (DMDelModeSync)           */
/*                              2: termination (DMDelModeClear)              */
/* return = 0: normal end                                                    */
/*          EDMsystemfile                                                    */
/*          <-EraseDFAMFile                                                  */
/*          <-EraseDFAMDir                                                   */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = November 30, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DeleteFileOrDir( char *pszNWPath, int iType, int iMode )
{
	int		iErr = 0;
	int		iRet;
	char	*pszPath = NULL;


	int		iErrno;
	int		iNerrno;

	pszPath = (char *)GW_malloc( NW_MAX_PATH + 1 );
	if ( pszPath == NULL ) {
		iErr = EDMmalloc;
		goto labelDeleteFileOrDir_q;
	}

	NWDirname( pszPath, pszNWPath );
	if ( (pszPath[strlen(pszPath)-1] == ':') && (CheckSystemFile(pszNWPath)) ) {
		iErr = EDMsystemfile;
		goto labelDeleteFileOrDir_q;
	}

	switch ( iMode ) {
	case DMDelModeInit:
		break;
	case DMDelModeSync:
		break;
	case DMDelModeClear:
		break;
	}


	WaitOnLocalSemaphore( uiCopyFile_sh );
	if ( DMCheckAccessFileList( pszNWPath ) ) {
		SignalLocalSemaphore( uiCopyFile_sh );
		iErr = EDMusingfile;
		goto labelDeleteFileOrDir_q;
	}

	if ( iType == DMFILE_T ) {

		iRet = EraseDFAMFile( pszNWPath, &iErrno );
		if ( (iRet) && (iMode != DMDelModeClear) ) {
			SignalLocalSemaphore( uiCopyFile_sh );
			iErr = iRet;
			goto labelDeleteFileOrDir_q;
		}
	} else {

		iRet = EraseDFAMDir( pszNWPath, &iErrno, &iNerrno );
		if ( (iRet) && (iMode != DMDelModeClear) ) {
			SignalLocalSemaphore( uiCopyFile_sh );
			iErr = iRet;
			goto labelDeleteFileOrDir_q;
		}
	}

	SignalLocalSemaphore( uiCopyFile_sh );

labelDeleteFileOrDir_q:
	if ( pszPath != NULL )
		GW_free( pszPath, 0 );
	return iErr;
}

/*********************************************************** EnglishComment **/
/* ID = ChangeFileTime                                                       */
/*                                                                           */
/* name = Changing the time stamp in a file                                  */
/* func = Changes the time stamp in a file to the designated one.            */
/*                                                                           */
/* i/o  = pszNWPath:char *:inp: path name of the file whose time stamp is    */
/*                              changed                                      */
/*        iTime    :LONG  :inp: time stamp                                   */
/*                                                                           */
/* return = 0: normal end                                                    */
/*          EDMutime:                                                        */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = November 30, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int ChangeFileTime( char *pszNWPath, time_t uiTime )
{
	int				iRet;
	struct utimbuf	Utimbuf;

	Utimbuf.acctime = uiTime;
	Utimbuf.modtime = uiTime;


	Utimbuf.acctime += igTimeGap;
	Utimbuf.modtime += igTimeGap;

	WaitOnLocalSemaphore( uiCopyFile_sh );
	if ( DMCheckAccessFileList( pszNWPath ) ) {
		SignalLocalSemaphore( uiCopyFile_sh );
		return EDMusingfile;
	}

	iRet = utime( pszNWPath, &Utimbuf );
	SignalLocalSemaphore( uiCopyFile_sh );
	if( iRet == -1 ){
		return EDMutime;
	}

	return 0;
}

/*********************************************************** EnglishComment **/
/* ID = ChangeDirectoryTime                                                  */
/*                                                                           */
/* name = Changing the time stamp in a directory                             */
/* func = Changes the time stamp in a directory to the designated one.       */
/*                                                                           */
/* i/o  = pszNWPath:char *:inp: path name of the directory whose time stamp  */
/*                              is changed                                   */
/*        iTime    :LONG  :inp: time stamp                                   */
/*                                                                           */
/* return = 0: normal end                                                    */
/*          <-ChangeDirectoryEntry                                           */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = November 30, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int ChangeDirectoryTime( char *pszNWPath, time_t uiTime )
{
	struct ModifyStructure	MVect;
	int						iRet;

	uiTime += igTimeGap;

	FormatTime( &MVect.MCreateDate, &MVect.MCreateTime, uiTime );



	WaitOnLocalSemaphore( uiCopyFile_sh );
	if ( DMCheckAccessFileList( pszNWPath ) ) {
		SignalLocalSemaphore( uiCopyFile_sh );
		return EDMusingfile;
	}

	iRet = ChangeDirectoryEntry( pszNWPath, &MVect,

			MCreateDateBit | MCreateTimeBit,
			(LONG)0 );


	SignalLocalSemaphore( uiCopyFile_sh );
	return iRet;
}


/*********************************************************** EnglishComment **/
/* ID = CheckVolumeMap                                                       */
/*                                                                           */
/* name = Checking of Volume mapping                                         */
/* func = Checks if the volume is mapped.                                    */
/*                                                                           */
/* i/o  = pszNWPath:char *:inp: path name of the file whose time stamp       */
/*                              is changed                                   */
/*        iTime    :LONG  :inp: time stamp                                   */
/*                                                                           */
/* return = 0: normal end                                                    */
/*          EDMvolumenotexist;                                               */
/*          EDMvolumeinfo;                                                   */
/*          EDMvolumemap;                                                    */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = November 30, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int CheckVolumeMap( char *pszVolume )
{
	int				iRet;
	int				iVolNum;
	VOLUME_STATS	VolStats;

	iRet = GetVolumeNumber( pszVolume, &iVolNum );
	if ( iRet ) {
		return EDMvolumenotexist;
	}

	iRet = GetVolumeInformation(	(WORD)0                   ,
									iVolNum,
									sizeof( VOLUME_STATS ),
									&VolStats );
	if ( iRet ) {
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11603, pszVolume );
		return EDMvolumeinfo;
	}
	if ( !(VolStats.isMounted) ) {
		return EDMvolumemap;
	}

	return 0;
}

/*********************************************************** EnglishComment **/
/* ID = FormatTime                                                           */
/*                                                                           */
/* name = Conversion of GMT into the time stamp usable by SetDirectoryInfo() */
/* func = Converts GMT into the time stamp that SetDirectoryInfo() can handle*/
/*                                                                           */
/* i/o  = iTime    :LONG  :inp: GMT                                          */
/*                                                                           */
/* return = void                                                             */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = March 20, 1995 by Ryuji Shinchi                                    */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
void FormatTime( WORD *pwDate, WORD *pwTime, time_t t )
{



















	_ConvertTimeToDOS ( t,
			(struct _DOSDate *)pwDate,
			(struct _DOSTime *)pwTime);
}
