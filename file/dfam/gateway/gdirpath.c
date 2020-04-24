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
 * $Log: gdirpath.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:56  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:18  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:52:47  takano
 * 	First Submit
 * 	[1996/02/21  07:52:43  takano]
 * 
 * $EndLog$
 */
#include "gdir.h"

/*********************************************************** EnglishComment **/
/* ID = NWBasename                                                           */
/*                                                                           */
/* name = Getting a file name from a full path (for NetWare)                 */
/* func = Extracts a file name from the NetWare full path, and returns the   */
/*        pointer to the file name.                                          */
/* i/o  = pszNWPath:char *:inp: pointer to a path name to be searched        */
/* return = void                                                             */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = November 30, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
void NWBasename( char *pszBasename, char *pszNWPath )
{
	char 	*pszLoc;

	pszLoc = D_strrchr( pszNWPath, '\\' );
	if ( pszLoc == NULL ) {
		pszLoc = pszNWPath;
	} else {
		pszLoc++;
	}

	strcpy( pszBasename, pszLoc );
}

/*********************************************************** EnglishComment **/
/* ID = DFSBasename                                                          */
/*                                                                           */
/* name = Getting a file name from a full path (for DFS)                     */
/* func = Extracts a file name from the DFS full path, and returns the       */
/*        pointer to the file name.                                          */
/* i/o  = pszNWPath:char *:inp: pointer to a path name to be searched        */
/* return = void                                                             */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = November 30, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
void DFSBasename( char *pszBasename, char *pszDFSPath )
{
	char 	*pszLoc;

	pszLoc = strrchr( pszDFSPath, '/' );
	if ( pszLoc == NULL ) {
		pszLoc = pszDFSPath;
	} else {
		pszLoc++;
	}

	strcpy( pszBasename, pszLoc );
}

/*********************************************************** EnglishComment **/
/* ID = NWDirname;                                                           */
/*                                                                           */
/* name = Getting a parent dorectory name by removing file/directory names   */
/*        from the fill path                                                 */
/* func = Extracts a file name from the NetWare full path, and returns the   */
/*        pointer to the file name.                                          */
/*                                                                           */
/* i/o  = pszParentPath:char *:inp: pointer to a parent directory name       */
/*        pszNWPath    :char *:inp: full path                                */
/* return = void                                                             */
/*                                                                           */
/* common = none                                                             */
/* :::                                                                       */
/* note = The format of the path name is limited to FOO:\ROOT\BAR.           */
/* date = November 30, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
void NWDirname( char *pszDirname, char *pszNWPath )
{
	char 	*pszLoc;
	int		iLength;

	pszLoc = D_strrchr( pszNWPath, '\\' );
	if ( pszLoc == NULL ) {
		iLength = strlen( pszNWPath );
	} else {
		iLength = pszLoc - pszNWPath;
	}
	memcpy( pszDirname, pszNWPath, iLength );
	pszDirname[iLength] = '\0';
}

/*********************************************************** EnglishComment **/
/* ID = DFSDirname;                                                          */
/*                                                                           */
/* name = Getting a parent dorectory name by removing file/directory names   */
/*        from the fill path                                                 */
/* func = Extracts a file name from the DFS full path, and returns the       */
/*        pointer to the file name.                                          */
/*                                                                           */
/* i/o  = pszParentPath:char *:inp: pointer to a parent directory name       */
/*        pszNWPath    :char *:inp: full path                                */
/* return = void                                                             */
/*                                                                           */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = November 30, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
void DFSDirname( char *pszDirname, char *pszDFSPath )
{
	char 	*pszLoc;
	int		iLength;

	pszLoc = strrchr( pszDFSPath, '/' );
	if ( pszLoc == NULL ) {
		iLength = strlen( pszDFSPath );
	} else {
		iLength = pszLoc - pszDFSPath;
	}
	memcpy( pszDirname, pszDFSPath, iLength );
	pszDirname[iLength] = '\0';
}

/*********************************************************** EnglishComment **/
/* ID = NWGetFullPathFromDirEnt;                                             */
/*                                                                           */
/* name = Getting NetWare full path from DirEnt                              */
/* func = Using the entered DirEnt, goes backward to the parent directory    */
/*        DirEnt to get the NetWare path.                                    */
/*                                                                           */
/* i/o  = pszNWFullPath:char *:inp: pointer to a parent directory name       */
/*        pszNWTopPath :char *:inp: paths (upper paths) that are not in      */
/*                                  FakeDirEnt list                          */
/*        pDirEnt:struct DirEnt *:inp: FakeDirEnt of the lowest file of the  */
/*                                     path name                             */
/* return = 0:           normal end                                           */
/*          EDMlongpath: generated path is too long                          */
/*                                                                           */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = November 30, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int NWGetFullPathFromDirEnt(
		char *pszNWFullPath, char *pszNWTopPath,
		struct NWFakeDirEnt *pDirEnt )
{
	char	*pszTmp = NULL;
	int		iErr = 0;

	pszTmp = (char *)GW_malloc( NW_MAX_PATH + 1 );
	if ( pszTmp == NULL ) {
		*pszNWFullPath = '\0';
	} else {

		iErr = NWCatFakeDirEnt( pszTmp, pDirEnt );

		sprintf( pszNWFullPath, "%s\\%s", pszNWTopPath, pszTmp );
		GW_free( pszTmp, 0 );
	}

	return iErr;
}

/*********************************************************** EnglishComment **/
/* ID = DFSGetFullPathFromDirEnt;                                            */
/*                                                                           */
/* name = Getting DFS full path from DirEnt                                  */
/* func = Using the entered DirEnt, goes backward to the parent directory    */
/*        DirEnt to get the NetWare path, and converts the path into a DFS   */
/*        path.                                                              */
/*                                                                           */
/* i/o  = pszDFSFullPath:char *:inp: pointer to a DFS full path              */
/*        pszNWTopPath  :char *:inp: paths (upper paths) that are not in the */
/*                                  FakeDirEnt list                          */
/*        pDirEnt:struct DirEnt *:inp: FakeDirEnt of the lowest file of the  */
/*                                     path name                             */
/* return = 0:           normal end                                           */
/*          EDMlongpath: generated path is too long                          */
/*                                                                           */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = November 30, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DFSGetFullPathFromDirEnt(
		char *pszDFSFullPath, char *pszDFSTopPath,
		struct DFSFakeDirEnt *pDirEnt )
{
	char	*pszTmp = NULL;
	int		iErr = 0;

	pszTmp = (char *)GW_malloc( DFS_MAX_PATH + 1 );
	if ( pszTmp == NULL ) {
		*pszDFSFullPath = '\0';
	} else {

		iErr = DFSCatFakeDirEnt( pszTmp, pDirEnt );

		sprintf( pszDFSFullPath, "%s/%s", pszDFSTopPath, pszTmp );
		GW_free( pszTmp, 0 );
	}

	return iErr;
}

/*********************************************************** EnglishComment **/
/* ID = CheckSystemFile                                                      */
/*                                                                           */
/* name = Comparison of a file name with a system reserved file name         */
/* func = Checks if the entered file name (NW) is a reserved file name       */
/*        by system.                                                         */
/* i/o  = pszNWPath  :char *  :inp: target file name (full path other than   */
/*                                  "VOLUME:")                               */
/*                                                                           */
/* return = 0: does not match with the system reserved file names            */
/*	    EDMsystemfile: the selected name is preempted by system              */
/*	    EDMmalloc:                                                           */
/* common = apszLogName: matrix registering the system reserved file names   */
/* :::                                                                       */
/* common = none                                                             */
/* :::                                                                       */
/* note = The names of the files reserved for system should be registered    */
/*        to the matrix, apszLogName.  ApszLogName is a matrix of char *,    */
/*        and the last entry should be NULL.                                 */
/* date = December 15, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int CheckSystemFile( char *pszNWPath )
{
	int		i = 0;
	char	*pszFilename = NULL;
	char	*pszDirname = NULL;
	int		iErr = 0;

	pszFilename = (char *)GW_malloc( NW_MAX_PATH + 1 );
	if ( pszFilename == NULL ) {
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11613);
		iErr = EDMmalloc;
		goto labelCheckSystemFile_Q;
	}
	pszDirname = (char *)GW_malloc( NW_MAX_PATH + 1 );
	if ( pszDirname == NULL ) {
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11613);
		iErr = EDMmalloc;
		goto labelCheckSystemFile_Q;
	}

	if ( ( D_strrchr( pszNWPath, '\\' ) != NULL ) &&
		 	(D_strrchr( pszNWPath, ':' ) != NULL ) ) {
		NWBasename( pszFilename, pszNWPath );
	} else {
		strcpy( pszFilename, pszNWPath );
	}
	for ( i = 0; apszNWLogFile[i] != NULL; i++ ) {
		if ( !strcmp( apszNWLogFile[i], pszFilename ) ) {
			iErr = EDMsystemfile;
			break;
		}
	}

labelCheckSystemFile_Q:
	if ( pszDirname ) {
		DMfree( pszDirname, "CheckSystemFile" );
	}
	if ( pszFilename ) {
		DMfree( pszFilename, "CheckSystemFile" );
	}
	return iErr;
}

