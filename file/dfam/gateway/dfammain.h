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
 * $Log: dfammain.h,v $
 * Revision 1.1.2.2  1996/03/11  14:07:33  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:59  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:20:15  takano
 * 	First Submit
 * 	[1996/02/21  07:20:10  takano]
 * 
 * $EndLog$
 */
/*EnglishComment**************************************************************
 * DFAMMAIN.H                                                                *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/

int     iSystem_ccode;
int     iUnloadTcpipFlag;



BOOL    bUnload;


BOOL    bGet_Dump;

FILE    *hErrorLog_fh;
FILE    *hMessageLog_fh;

char    aszMyServerName[48];

LONG    uiAdHandle;

int		iDFANLM_ID = 0;

BYTE    aszDFAGW_Ver[8];
BYTE    aszDFALIB_Ver[8];
BYTE    aszDFADCE_Ver[8];

BYTE    aszOSVer[5];
BYTE    aszNLMVer[8];

int iMain_tgid;
int iMain_tid;

int iWatch_tgid;
int iWatch_tid;
int iWatch_status;

int iAdmin_tgid;
int iAdmin_tid;
int iAdmin_status;

void    *pFtrace_p;

struct  GMemManage_t    *pMemManage_p;
struct  GVolume_t       *pVolume_sp;
struct  GDFAMGrp_t      *pDFAMGrp_sp;
struct  GCopyFile_t     *pCopyFile_sp;
struct  GTGroup_t       *pTGroup_sp;
struct  GName_t         *pName_sp;
struct  GAgent_t        *pAgent_sp;
struct  GFHandle_t      *pFHandle_sp;
struct  GNCPExt_t       *pNCPExt_p;
struct  GCopyBack_t     *pCopyBack_sp;
struct  GErrFile_t      *pGErrFile_p;

struct GNCPExt_t *GNCPExt_p;

struct	FileAccessList	*pAcsList_sp;

struct DMAccessList  AccessListRoot;



LONG    uiDirSyncInterval;
int     iTraceFlushCount;
int     iFuncFlushCount;
int     iFileSparseDelayTime;
WORD    usCopyBackDelayTime;
BOOL    bTimeSync;
BOOL    bMountWarning;
char    aszCopyBackDirName[241];
char    aszMessageLogFileName1[256];
char    aszMessageLogFileName2[256];
char    aszErrorLogFileName1[256];
char    aszErrorLogFileName2[256];
char	*pszCur_Msg_Fname;
char	*pszCur_Err_Fname;
size_t  tmWatchdogMSGBuflen
                = WATCHDOGMSGLEN_MAX;

int     iMaxUsersNum;
int     iLoginUsersNum;
int     iResumeAll;
int     iScreenHandle;

int     iMainSockfd;
int     iWatchSockfd;

LONG    uiMainConn_sh;
LONG    uiCopyFile_sh;
LONG    uiErrLog_sh;
LONG    uiMalloc_sh;
LONG    uiTrace_sh;
LONG    uiFTrace_sh;
LONG    uiKeyFile_sh;
LONG    uiWatchStat_sh;
LONG    uiAdminStat_sh;
LONG    uiMsg_sh;
LONG    uiDump_sh;
LONG    uiRAS_sh;

void * pMainTracep;
void * pAdminTracep;
void * pWatchdogTracep;
