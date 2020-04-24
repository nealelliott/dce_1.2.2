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
 * $Log: gdir.h,v $
 * Revision 1.1.2.2  1996/03/11  14:07:43  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:08  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:36:18  takano
 * 	First Submit
 * 	[1996/02/21  07:36:13  takano]
 * 
 * $EndLog$
 */
/*EnglishComment**************************************************************
 * GDIR.H                                                                    *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <share.h>
#include <direct.h>
#include <time.h>
#include <sys/utime.h>
#include <sys/stat.h>
#include <nwdir.h>
#include <nwsemaph.h>
#include <process.h>
#include <errno.h>
#include <niterror.h>
#include <time.h>
#include <dos.h>
#include <io.h>
#include <conio.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamprt.h"
#include "dfamext.h"
#include "dfammsg.h"


enum DMDelMode {
	DMDelModeInit,
	DMDelModeSync,
	DMDelModeClear,
};

enum DMMkMode {
	DMMkModeNot,
	DMMkModeExist,
	DMMkModeUnknown,
};

enum DMReconnect {
	DMReconnectAdmin,
	DMReconnectWatch,
};

#define AGENT_ALIVE			0
#define AGENT_DIED			1

#define NEWFILE				0
#define OLDFILE				1

#define DMFILE_T				0
#define DMDIR_T					1

#define	DFS_MAX_PATH		1024
#define DFS_MAX_FILE_NAME	1024
#define	NW_MAX_PATH			255
#define	NW_MAX_VOL_NAME		16
#define	NW_MAX_FILE_NAME		16
#define DMtraceSize			8192

#define	GWLOGDMFREE			4003

#define DMDefaultShortInterval 500
#define DMDefaultReconnectInterval 15000

#define DFSreadstatAll	DS_GETSUBDIR|DS_GETFILENAME|DS_GETTIME|DS_GETOWN_SIZE

#define	LOG_BUF				64

#define	DFAMADMIN			"SUPERVISOR"

enum DMSTAT {
	DMSTATinit,
	DMSTATmakeup,
	DMSTATsync,
	DMSTATinterval,
	DMSTATshutdown,
	DMSTATabend,
};




enum EDM {
	EDMwarning = 1600,
	EDMmalloc,
	EDMcreat = 1610,
	EDMmkdir,
	EDMunlink,
	EDMrmdir,
	EDMwrite,
	EDMseek,
	EDMclose,
	EDMutime,
	EDMopendir,
	EDMclosedir,
	EDMreaddir,
	EDMstat,
	EDMlongpath = 1640,
	EDMdirlevel,
	EDMdiskspace,
	EDMdirectoryblock,
	EDMnameconv = 1680,
	EDMnullptr,
	EDMbug,
	EDMlogin = 1750,
	EDMlogout,
 	EDMadminid,
 	EDMadminpwd,
 	EDMbindery,
 	EDMhosts,
 	EDMservices,
 	EDMtcpip,
 	EDMunknown,
 	EDMcipher,
 	EDMtrans,
	EDMinitfatal = 1770,
	EDManytimeout,
	EDMvolumefatal,
	EDMvolumemap,
	EDMvolumenotexist,
	EDMvolumeinfo,
	EDMusingfile,
	EDMkeepfile,
	EDMsystemfile,
	EDMshutdownreq = 1780,
	EDMagentdie,
	EDMyes = 1798,
	EDMno = 1799,
};

enum DMLOG {
	DMLOGnoadmin,
	DMLOGnodfamvolume,
	DMLOGvolumenotmapped,
	DMLOGvolumenotexist,
	DMLOGvolumeinfo,
	DMLOGvolumemap,
	DMLOGvolumeinit,
	DMLOGreflect,
	DMLOGtimereq,
	DMLOGkeepfile,
	DMLOGusingfile,
	DMLOGsystemfile,
	DMLOGinitializeabort,
	DMLOGconnection,
	DMLOGlogin,
	DMLOGlogout,
	DMLOGdisconnection,
	DMLOGagentdown,
	DMLOGdfsdirinfo,
	DMLOGdfsfileinfo,
	DMLOGdfsreadstat,
	DMLOGdfsclosedir,
	DMLOGdfsdirsync,
	DMLOGfileinfo,
	DMLOGdirinfo,
	DMLOGdeletefile,
	DMLOGdeletedir,
	DMLOGnameconv,
	DMLOGlongpath,
	DMLOGdirlevel,
	DMLOGopendir,
	DMLOGstat,
	DMLOGcreat,
	DMLOGwrite,
	DMLOGseek,
	DMLOGclose,
	DMLOGutime,
	DMLOGmkdir,
	DMLOGgetsemhandle,
	DMLOGrelsemhandle,
	DMLOGmalloc,
	DMLOGbug,
	DMLOGnullptr,
};


#define DMNotOpen		0
#define DMDeleteOK		1
#define DMUsed			2

#define	DMDelModeInit	0
#define	DMDelModeSync	1
#define	DMDelModeClear	2


struct NWFakeDirEntList {
	struct GVolume_t		*pVolTbl;
	char					acDirname[NW_MAX_PATH + 1];
	struct NWFakeDirEnt		*TopDirEnt;
	struct NWFakeDirEnt		*CurDirEnt;
};

struct DFSFakeDirEntList {
	struct GVolume_t		*pVolTbl;
	char					acDirname[DFS_MAX_PATH + 1];
	struct DFSFakeDirEnt	*TopDirEnt;
	struct DFSFakeDirEnt	*CurDirEnt;
};


struct ModifiedDir {
	char			 	*pszDFSModifiedPath;
	struct ModifiedDir	*pPrev;
	struct ModifiedDir	*pNext;
	time_t				uiTime;
};

struct LogMsg {
	int		iMsgCode;
	char	*pszMsg;
};

extern unsigned int			uigAllocatedMem;

extern struct ComTGroup_t	gComS;

extern int					igThreadID;
extern int					igThreadGroupID;

extern struct ModifiedDir	*pgChangedDirList;
extern struct ModifiedDir	*pgEndOfChangedDirList;

extern time_t				igTimeGap;

extern int					igDMInitRun;
extern int					igDMSyncRun;
extern int					igDMClearRun;

extern int					igSemHandleFlag;
extern unsigned int			uigSemHandle;
extern int					igAdmSockfdFlag;
extern int					igAdmSockfd;
extern int					igAdmConReqFlag;
extern int					igDCELoginFlag;

extern int					igAdmConReqCount;

extern int					igDelaySlice;
extern int					igInitErr;
extern int					igInitAct;
extern char					*apszNWLogFile[];


int CallGetNWPath( char *pszDFSPath, char *pszVolName, char *pszNWPath );
int	CallTimeSync(void);
int ChangeFileTime( char *pszNWPath, time_t uiTime );
int CheckOpen( char *pszNWPath );
int CheckSystemFile( char *pszNWPath );
int CheckVolumeMap( char *pszVolume );
int ClearVolume( struct GVolume_t *pVolTbl, int iDeleteMode );
int DMDCELogin( void );
int DMDCELogout( void );
void DFSBasename( char *pszBasename, char *pszDFSPath );
int DFSChainFakeDirEnt(		char	*pszDFSDir,
							struct	DFSFakeDirEnt **ppFirst,
							int		iLevel,
							int		*piLink);
int DFSCatFakeDirEnt( char *pszDFSPath, struct DFSFakeDirEnt *pTail );
int DFSDeleteFakeDirEnt(	struct DFSFakeDirEnt *pCur,
							struct DFSFakeDirEnt **ppNew );
void DFSDirname( char *pszParentPath, char *pszDFSPath );
int DFSGetFullPathFromDirEnt(
		char *pszDFSFullPath, char *pszDFSTopPath, struct DFSFakeDirEnt *pDirEnt );
int DFSInsertDirEnt(	struct DFSFakeDirEnt *pDFSCurDirDirEnt,
						struct DFSFakeDirEnt *pDFSCurFileDirEnt );
int DFSMakeFakeDirEntUndeleteable( struct DFSFakeDirEnt *pDir );
int DFSMoveToNextFakeDirEnt(	struct DFSFakeDirEnt *pCur,
								struct DFSFakeDirEnt **ppNew );
int DMclear( int iMode );
int	DMErr( int iDFSErr );
int DMinit( int *piSuccess );
int	DMSleep( void );
int DMYesNo( char *pszNWPath );
void DMmain( void );
int DMmainSetdown( void );
int DMmainSetUp( void );
int DMreconnect( int iStat );
int DMsync( void );
int DMsyncDir(struct GVolume_t *pVolTbl,
				char *pszDFSModifiedPath, time_t uiTopDirTime,
				int iDelMode);
int DMsyncVol( struct GVolume_t *pVolTbl, time_t uiPrevTime );
int DeleteDir( char *pszNWPath, int iMode );
int DeleteFile( char *pszNWPath, int iMode );
int DeleteTree( char *pszNWPath, int iMode );
int DeleteUnknownFile(
			char					*pszNWDirname,
			struct NWFakeDirEnt		*pNW1stDirEnt,
			struct NWFakeDirEnt		*pNW1stDirEntInVol,
			int						iDelMode );
void DFSFreeFakeDirEntList( struct DFSFakeDirEnt *pHead );
void FormatTime( WORD *pwDate, WORD *pwTime, time_t t );
void NWFreeFakeDirEntList( struct NWFakeDirEnt *pHead );
int GetNWPath2( BYTE *pszDFSPath, BYTE *pszVol, BYTE *pszNWPath );
int GetAdminID( char *pszNWID, char *pszDCEID );
char *GetDMLogMsg( int iDMErr );
int	GetSemHandle(void);
int	GetThreadID(void);
int DMconnect(	struct GAgent_t *pAgent,
				unsigned short ushConAssort,
				void *pComTbl);
int DMInitVolume( struct GVolume_t *pVolTbl );
int	DMMakeSparseFile( char *pszNWPath, long iSize, int iMode);
void DMfree( void *pPtr, char *pszLoc );
int MakeTree( struct GVolume_t *pVolTbl, char *pszDFSTopPath,
				char *pszNWTopPath);
void NoticeDMInitAbort( void );
void NoticeDMInitSuccess( void );
void NWBasename( char *pszBasename, char *pszNWPath );
int NWCatFakeDirEnt( char *pszNWPath, struct NWFakeDirEnt *pTail );
int NWChainFakeDirEnt(		char					*pszNWFullPath,
							struct NWFakeDirEnt 	**ppNWHead,
							int						iLevel,
							int						*piLink);
int  NWChainFakeDirEntFromDFSOne(	struct GVolume_t		*pVolTbl,
									char 					*pszDFSDirname,
									struct DFSFakeDirEnt	*pDFSHead,
									struct NWFakeDirEnt		**ppNWHead );
int NWDeleteFakeDirEnt(	struct NWFakeDirEnt *pCur,
						struct NWFakeDirEnt **ppNew );
void NWDirname( char *pszParentPath, char *pszNWPath );
int NWGetFullPathFromDirEnt(
		char *pszNWFullPath, char *pszNWTopPath, struct NWFakeDirEnt *pDirEnt );
int NWInsertDirEnt(	struct NWFakeDirEnt *pNWCurDirDirEnt,
					struct NWFakeDirEnt *pNWCurFileDirEnt );
int NWMakeFakeDirEntUndeleteable( struct NWFakeDirEnt *pDir );
int NWMoveToNextFakeDirEnt(	struct NWFakeDirEnt *pCur,
							struct NWFakeDirEnt **ppNew );
int PopModifiedDirList( void );
int PushModifiedDirList( char *pszModifiedDir );
int ReadEndOfModifiedDirList( char *pszBuff );
int	RelSemHandle(void);
void ResetDeathFlag( void );
void SetDeathFlag( void );
int ShutdownReqOccur( void );
int	SyncAttr( char *pszNWPath, time_t uiTime, int iAltAttr, int iNewAttr );
int	TouchDir( char *pszNWPath, time_t uiTime, struct stat *pStat, int iMode );
int	TouchFile( char *pszNWPath, long iSize, time_t uiTime, struct stat *pStat,
				int iMode );
int	TouchFileByDFSPath(	struct GVolume_t *pVolTbl, char *pszDFSPath,
						long iSize, time_t uiTime );
int ShutdownReq( void );
int AgentDie( void );


