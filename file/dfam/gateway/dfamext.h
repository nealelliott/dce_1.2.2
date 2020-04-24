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
 * $Log: dfamext.h,v $
 * Revision 1.1.2.2  1996/03/11  14:07:33  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:59  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:19:31  takano
 * 	First Submit
 * 	[1996/02/21  07:19:25  takano]
 * 
 * $EndLog$
 */
/*EnglishComment**************************************************************
 * DFAMEXT.H                                                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/

#ifndef DFAMEXT_H
#define DFAMEXT_H
extern  int     iSystem_ccode;
extern  int     iUnloadTcpipFlag;



extern  BOOL    bUnload;


extern  BOOL    bGet_Dump;

extern  FILE    *hErrorLog_fh;
extern  FILE    *hMessageLog_fh;

extern  char    aszMyServerName[48];
extern  BYTE	aszGWName[16];

extern  LONG    uiAdHandle;

extern	int		iDFANLM_ID;

extern  BYTE    aszDFAGW_Ver[8];
extern  BYTE    aszDFALIB_Ver[8];
extern  BYTE    aszDFADCE_Ver[8];

extern  BYTE    aszOSVer[5];
extern  BYTE    aszNLMVer[8];

extern  int iMain_tgid;
extern  int iMain_tid;

extern  int iWatch_tgid;
extern  int iWatch_tid;
extern  int iWatch_status;

extern  int iAdmin_tgid;
extern  int iAdmin_tid;
extern  int iAdmin_status;

extern  void    *pFtrace_p;

extern  struct  GMemManage_t    *pMemManage_p;
extern  struct  GVolume_t       *pVolume_sp;
extern  struct  GDFAMGrp_t      *pDFAMGrp_sp;
extern  struct  GCopyFile_t     *pCopyFile_sp;
extern  struct  GTGroup_t       *pTGroup_sp;
extern  struct  GName_t         *pName_sp;
extern  struct  GAgent_t        *pAgent_sp;
extern  struct  GFHandle_t      *pFHandle_sp;
extern  struct  GNCPExt_t       *pNCPExt_p;
extern  struct  GCopyBack_t     *pCopyBack_sp;
extern  struct  GErrFile_t      *pGErrFile_p;

extern  struct GNCPExt_t *GNCPExt_p;

extern	struct	FileAccessList	*pAcsList_sp;

extern struct DMAccessList	AccessListRoot;

extern  LONG    uiDirSyncInterval;
extern  int     iTraceFlushCount;
extern  int     iFuncFlushCount;
extern  int     iFileSparseDelayTime;
extern  WORD    usCopyBackDelayTime;
extern  BOOL    bTimeSync;
extern  BOOL    bMountWarning;
extern  char    aszCopyBackDirName[241];
extern  char    aszMessageLogFileName1[256];
extern  char    aszMessageLogFileName2[256];
extern  char    aszErrorLogFileName1[256];
extern  char    aszErrorLogFileName2[256];
extern	char	*pszCur_Msg_Fname;
extern	char	*pszCur_Err_Fname;
extern  size_t  tmWatchdogMSGBuflen;

extern  int     iMaxUsersNum;
extern  int     iLoginUsersNum;
extern  int     iResumeAll;
extern  int     iScreenHandle;

extern  int     iMainSockfd;
extern  int     iWatchSockfd;

extern  LONG    uiMainConn_sh;
extern  LONG    uiCopyFile_sh;
extern  LONG    uiErrLog_sh;
extern  LONG    uiMalloc_sh;
extern  LONG    uiTrace_sh;
extern  LONG    uiFTrace_sh;
extern  LONG    uiKeyFile_sh;
extern 	LONG    uiWatchStat_sh;
extern  LONG    uiAdminStat_sh;
extern  LONG    uiMsg_sh;
extern  LONG    uiDump_sh;
extern  LONG    uiRAS_sh;

extern  void * pMainTracep;

extern  void * pAdminTracep;

extern  void * pWatchdogTracep;

#endif
