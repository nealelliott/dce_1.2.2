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
 * $Log: dfamstr.h,v $
 * Revision 1.1.2.3  1996/03/22  07:36:00  takano
 * 	modified the table deletion method in the POST routine.
 * 	[1996/03/22  07:32:31  takano]
 *
 * Revision 1.1.2.2  1996/03/11  14:07:37  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:02  marty]
 * 
 * Revision 1.1.2.1  1996/02/21  07:33:26  takano
 * 	First Submit
 * 	[1996/02/21  07:29:08  takano]
 * 
 * $EndLog$
 */
/*EnglishComment**************************************************************
 * DFAMSTR.H                                                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
#ifndef DFAMSTR_H
#define DFAMSTR_H


struct GVolume_t {
	BYTE	aszIcat[ICATCH_LEN];
    struct  GVolume_t *pMaep;
    struct  GVolume_t *pUshirop;
    struct  GName_t *pNamep;
    LONG    uiVolAndName_sh;
    int     iVolNumber;
    BYTE    aszNWVol[_MAX_VOLUME+1];
    BYTE    aszDFSPath[DFSPATH_MAX+1];
    time_t  uiSynctime;
    unsigned short usVolStatus;
};

#define VOL_ICAT	"@VOL_T"

#define NW_NOENT		0x0001
#define NW_USERSKIP		0x0002
#define NW_SPACE		0x0004
#define DFS_NOENT		0x0010
#define DFS_TIMEOUT		0x0020
#define DFS_ACCESS		0x0040
#define DFS_ERROR		0x0080
#define VOL_VIRGIN		0x0100
#define ALLCLEAR		0x0000
#define NWCLRMASK		0xFFF0
#define DFSCLRMASK		0xFF0F


struct GAgent_t {
	BYTE	aszIcat[ICATCH_LEN];
    struct  GAgent_t *pMaep;
    struct  GAgent_t *pUshirop;
    BYTE    aszAgentName[AGENTNAME_MAX+1];
    int     iCipherType;
    BOOL    bAdmin_live;
    BOOL    bWatch_live;
    BOOL    bLogin_Stop;
    int     iCom_SendBuffLen;
    unsigned long uiConnectWaitTime;
    long    iCom_ResponseWaitTime;
    long    iWatchDogWaitTime;
    unsigned short  ushDFAMAgtTranceVer;
    unsigned long   uiOpenFileNum;
};

#define bIsConnection_live(p)   (p->bAdmin_live & p->bWatch_live)

#define AGT_ICAT	"@AGT_T"

struct GNCPExt_t {
    long    aiDummy[8];
};


struct GName_t {
	BYTE	aszIcat[ICATCH_LEN];
    struct  GVolume_t   *pVolp;
    struct  GName_t     *pOyaDirp;
    struct  GName_t     *pKoDirp;
    struct  GName_t     *pMaeDirp;
    struct  GName_t     *pUshiroDirp;
    struct  GName_t     *pDelListTop;
    struct  GName_t     *pDelMaep;
    struct  GName_t     *pDelUshirop;
    time_t              uiDelTime;
    BOOL                bConvflg;
    BOOL                bExist;
    BYTE    aszEntryName[_MAX_NAME+1];
    BYTE    aszRealName[DFSENTRY_MAX+1];
};

#define NAME_ICAT	"@NAME_T"


struct GTGroup_t {
    int iSockFd;
    LONG uiCConn_sh;
    void *pTsh_Trace;
    int iConErrFlag;
    LONG uiConnNum;
    struct GAgent_t *pAgent;
    size_t uiTsh_Size;
    int iTGrpID;
    int iThreadID;
    int iReqThreadID;
    int iExistThread;
    int iExistCommand;
    int iLogoutStatus;

    int iResumeThreadMode;

	int	iSemaporCount;
    LONG uiObjectID;
    LONG uiFHandle_sh;
    int iLoginStatus;

    struct TableKeep *pTblKeep_sp;
    struct OpenFile_t *pOFile_sp;
    struct LginReturn_t *pLgin;
    struct GTGroup_t *pNext;
};

struct ComTGroup_t {
    int iSockfd;
    LONG uiCConn_sh;
    void *pTsh_Trace;
    int iConErrFlag;
    LONG uiConnNum;
    struct GAgent_t *pAgent;
    size_t uiTsh_Size;
};



typedef struct {
   LONG   connection;
   LONG   task;
   LONG   volume;
   LONG   dirBase;
   BYTE  *pathString;
   LONG   pathComponentCount;
   LONG   nameSpace;
   LONG   attributeMatchBits;
   LONG   requestedAccessRights;
   LONG   dataStreamNumber;
   LONG   *puiFileHandle;
   } xOpenFileCallBackStruct;



typedef struct {
   LONG   connection;
   LONG   task;
   LONG   volume;
   LONG   dirBase;
   BYTE   *pathString;
   LONG   pathComponentCount;
   LONG   nameSpace;
   LONG   createAttributeBits;
   LONG   requestedAccessRights;
   LONG   createFlagBits;
   LONG   dataStreamNumber;
   LONG   *puiFileHandle;
   } xCreateAndOpenCallBackStruct;


struct OpenFile_t{
    LONG    uiConnum;
    LONG    uiTasknum;
    LONG    uiVolnum;
    LONG    uiDirBase;
    BYTE    aszNWFullpath[_MAX_PATH + 1];
    LONG    uiPathCompCnt;
    LONG    uiNameSpace;
    LONG    uiAttrMatchBits;
    LONG    uiReqdAcsRights;
    LONG    uiRealAcsRights;
    LONG    uiCreFlagBits;
    LONG    uiDataStrmNum;
    LONG    uiNWHandle;
    LONG    uiClose_sh;
	int		iThreadID;
    int     iDFSHandle;
    int     iCopyBackTGID;
    int     iOpenFailure_flg;
	int		iCopybackThreadStatus;
    struct OpenFile_t   *pNextOF_t;
};


struct GCopyBack_t{
    BYTE    aszNWFullpath[_MAX_PATH + 1];
    int     iRead_cnt;
    int     iWrite_cnt;
    int     iCopyBack_flg;
    int     iOpenreq_flg;
    int     iRenamereq_flg;
    int     iErasereq_flg;
    int     iDelay_flg;
    LONG    uiTasknum;
    struct  GCopyBack_t *pNext_t;
};


struct	FileAccessList	{
	char	aszAccessFilePathName_b[258];
	char	aszAccessFilePathName_a[258];
	LONG	uiConnNum;
	LONG	uiTaskNum;
	LONG	uiAcces_sh;
	int		iAccessListThreadStatus;
	int		iThreadID;
	int		iAccessType;
	BOOL	bIsCalledPostHook;

	struct	FileAccessList	*pNext_t;
};

struct DMAccessFile {
    char                    aszEC[8];
    int                     iCount;
	int						iThID;
    struct DMAccessFile     *pNext;
    char                    *pszPath;
};

struct DMAccessList {
    struct DMAccessFile     *pAccessFile;
    int                     iAccessListMode;
};

#define DMAccessListModeInterval 0
#define DMAccessListModeSync     1
#define DMAccessListModeTerm     2


typedef struct d__dirdesc {
    char ptr[8];
} DDIR;

struct ddirent {
    char d_name[DGW_PATH_MAX + 1];
    dmode_t d_mode;
    dtime_t d_mtime;
    char d_ownname[DGW_OWNER_MAX + 1];
    doff_t d_size;
};


	struct GErrFile_t {
		BYTE	aszErrFileIC[8];
		BYTE    aszErrLog001[256];
		int     iErrLog001;
		BYTE    aszErrLog002[256];
		int     iErrLog002;
		BYTE    aszMsgLog001[256];
		int     iErrMsg001;
		BYTE    aszMsgLog002[256];
		int     iErrMsg002;
		BYTE    aszFuncTrace[256];
		int     iFuncTrace;
		BYTE    aszMainTrace[256];
		int     iMainTrace;
		BYTE    aszAdmnTrace[256];
		int     iAdmnTrace;
		BYTE    aszWDogTrace[256];
		int     iWDogTrace;
};

#define FERR_ICAT "@FERR_T"


struct ddirsync {
    unsigned int d_namlen;
    char d_name[DGW_PATH_MAX + 1];
    dtime_t d_mtime;
    doff_t d_size;
};

struct dutimbuf {
    dtime_t actime;
    dtime_t modtime;
};

struct dstat {
    dmode_t d_mode;
    doff_t d_size;
    dtime_t d_mtime;
    char d_uid[DGW_OWNER_MAX + 1];
};

    struct GMem_HeadFoot{
        char                cGMem_THNm[8];
        size_t              uiSizeM;
    };

    struct GMem_HInfo{
        struct GMemManage_t  *pPrevGMemTbl;
        struct GMemManage_t  *pNextGMemTbl;
        int                  iGMemTblCnt;
        struct GMem_AllcInfo *pGMemTblEnd;
        int                  iGMem_status;
    };
    struct GMem_AllcInfo{
        int                  iGMem_thID;
        void                 *pGMem_malc;
        char                 cGMem_thNm[8];
        size_t               uiGMem_size;
    };
    struct GMemManage_t {
        struct GMem_HInfo      SGMem_HInfo;
        struct GMem_AllcInfo   SGMem_AllcInfo[ MAXARRNUM ];
    };

    struct tr_Init {
        char                cEyeCatch[32];
        void                *pHere;
        int                 iI_thID;
        char                cI_thName[20];
        size_t              uiI_tr_size;
        struct tr_Record    *pI_lastPtr;
        int                 iRecCnt;
        struct tr_Record    *pStrtWrTrp;
        int                 iMainTRC;
        int                 iAdminTRC;
        int                 iWatchTRC;
        time_t              uiBaseTime;
        clock_t             uiBaseClock;
        char                cDummy[4];
    };

    struct tr_Record {
        clock_t uitr_clock;
        int     iEventType;
        int     icmt1,icmt2;
        char    cmt3[12];
        int     ithID;
    };


struct NWFakeDirEnt {
    int                 iLink;
    int                 iLevel;
    int                 iOrder;
    int                 iAttr;
    time_t              uiTime;
    long                iSize;
    struct NWFakeDirEnt *pPrev;
    struct NWFakeDirEnt *pNext;
    struct NWFakeDirEnt *pParent;
    char                szName[13];
    BYTE                byUndeleteable;
};

struct DFSFakeDirEnt {
    int                 iLink;
    int                 iLevel;
    int                 iOrder;
    unsigned short      usMode;
    time_t              uiTime;
    long                iSize;
    struct DFSFakeDirEnt    *pPrev;
    struct DFSFakeDirEnt    *pNext;
    struct DFSFakeDirEnt    *pParent;
    char                szName[DFSENTRY_MAX + 1];
    BYTE                byUndeleteable;
};

struct LginReturn_t {
    unsigned short ushRc;
    unsigned short ushLen;
    unsigned char *pszBuf;
};


struct LGINSendFrame {
    unsigned char asComName[COMLEN];
};

struct LGINRecvFrame {
    unsigned char asComName[COMLEN];
    unsigned short ushRc;
    unsigned short ushLen;
    unsigned char aszDCE_uid[1];
};

struct LGOTSendFrame {
    unsigned char asComName[COMLEN];
};

struct LGOTRecvFrame {
    unsigned char asComName[COMLEN];
    unsigned short ushRc;
    unsigned short ushLen;
    unsigned char aszDCE_uid[1];
};

struct PWDOSendFrame {
    unsigned char asComName[COMLEN];
    unsigned short ushLen;
    unsigned char aszOldPwd[1];
};

struct PWDORecvFrame {
    unsigned char asComName[COMLEN];
    unsigned short ushRc;
};

struct PWDNSendFrame {
    unsigned char asComName[COMLEN];
    unsigned short ushOlen;
    unsigned char aszOldPwd[1024];
    unsigned short ushNlen;
    unsigned char aszNewPwd[1];
};

struct PWDNRecvFrame {
    unsigned char asComName[COMLEN];
    unsigned short ushRc;
};

struct LCHKSendFrame {
    unsigned char asComName[COMLEN];
};

struct LCHKRecvFrame {
    unsigned char asComName[COMLEN];
    unsigned short ushRc;
};

struct NFILRecvFrame {
    unsigned char asComName[COMLEN];
    unsigned short ushRc;
};


struct GetTrustee
{
    unsigned int kyokabit;
    short        entrymode;
    unsigned int ug_length;
    char         ug_name[MAX_DCEUSERNAME];
};

#endif
