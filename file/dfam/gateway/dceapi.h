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
 * $Log: dceapi.h,v $
 * Revision 1.1.2.2  1996/03/11  14:07:07  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:37  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:12:32  takano
 * 	First Submit
 * 	[1996/02/21  07:12:15  takano]
 * 
 * $EndLog$
 */
/*EnglishComment**************************************************************
 * DCEAPI.H                                                                  *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
#ifndef DCEAPI_H
#define DCEAPI_H
#include "dfamstr.h"

#define DCOMEYEC 4

extern char aszEyeChach[DCOMEYEC+1];

enum {

    GETHOSTBYNAME_ERR = 0x0001,
    GETSERVICES_ERR,
    SOCKET_ERR,
    CONNECT_ERR,
    IOCTL_ERR,
    SETSOCKOPT_S_ERR,
    SETSOCKOPT_R_ERR,
    SELECT_ERR,
    SEND_ERR,
    RECV_ERR,
    SHUTDOWN_ERR,
    CLOSE_ERR,
    IP_ADDRESS,

    SEND_SIZE_ERR,
    RECV_SIZE_ERR,
    CONERRFLAG_ERR,
    PATH_ERR,
    OTHER_ERR,

    SEND_HEAD_INF,
    RECV_HEAD_INF,
    RECV_DEFAULT_INF,
    DUST_RECV_INF,

    SEND_DATA1_ERR,
    SEND_DATA2_ERR,
    SEND_DATA3_ERR,
    SEND_DATA4_ERR,
    SEND_DATA5_ERR,
    SEND_DATA6_ERR,
    SEND_DATA7_ERR,
    SEND_DATA8_ERR,
    SEND_DATA9_ERR,

    RECV_DATA1_ERR,
    RECV_DATA2_ERR,
    RECV_DATA3_ERR,
    RECV_DATA4_ERR,
    RECV_DATA5_ERR,
    RECV_DATA6_ERR,
    RECV_DATA7_ERR,
    RECV_DATA8_ERR,
    RECV_DATA9_ERR,

    AGTCONNECT_TRACE_S,
    CONNECTDISCRIM_TRACE_S,
    AGTDISCONNECT_TRACE_S,
    WATCHDOGSTATRECV_TRACE_S,

    DFSOPEN_TRACE_S,
    DFSCLOSE_TRACE_S,
    DFSCREAT_TRACE_S,
    DFSREMOVE_TRACE_S,
    DFSRENAME_TRACE_S,
    DFSREAD_TRACE_S,
    DFSWRITE_TRACE_S,
    DFSMKDIR_TRACE_S,
    DFSRMDIR_TRACE_S,
    DFSGETSYSCALL_TRACE_S,
    DFSSETSYSCALL_TRACE_S,
    DFSDELSYSCALL_TRACE_S,
    DFSRIGHTS_TRACE_S,
    DFSOPENDIR_TRACE_S,
    DFSREADSTAT_TRACE_S,
    DFSCLOSEDIR_TRACE_S,
    DFSTIME_TRACE_S,
    DFSDIRSYNC_TRACE_S,
    DFSSTAT_TRACE_S,
    DFSUTIME_TRACE_S,
    DFSCHOWN_TRACE_S,
    DFSLINGETID_TRACE_S,
    DFSLINGETPASSWD_TRACE_S,
    DFSLINRESETKEY_TRACE_S,
    DFSLOUT_TRACE_S,
    DFSLSEEK_TRACE_S,

    AGTCONNECT_TRACE_E,
    CONNECTDISRIM_TRACE_E,
    AGTDISCONNECT_TRACE_E,
    WATCHDOGSTATRECV_TRACE_E,

    DFSOPEN_TRACE_E,
    DFSCLOSE_TRACE_E,
    DFSCREAT_TRACE_E,
    DFSREMOVE_TRACE_E,
    DFSRENAME_TRACE_E,
    DFSREAD_TRACE_E,
    DFSWRITE_TRACE_E,
    DFSMKDIR_TRACE_E,
    DFSRMDIR_TRACE_E,
    DFSGETACLSYSCALL_TRACE_E,
    DFSSETACLSYSCALL_TRACE_E,
    DFSDELACLSYSCALL_TRACE_E,
    DFSRIGHTS_TRACE_E,
    DFSOPENDIR_TRACE_E,
    DFSREADSTAT_TRACE_E,
    DFSCLOSEDIR_TRACE_E,
    DFSTIME_TRACE_E,
    DFSDIRSYNC_TRACE_E,
    DFSSTAT_TRACE_E,
    DFSUTIME_TRACE_E,
    DFSCHOWN_TRACE_E,
    DFSLINGETID_TRACE_E,
    DFSLINGETPASSWD_TRACE_E,
    DFSLINRESETKEY_TRACE_E,
    DFSLOUT_TRACE_E,
    DFSLSEEK_TRACE_E,

    RESUME_ERR,

    DFSCPFIRSTREAD_TRACE_S,
    DFSCPFIRSTREAD_TRACE_E,
    DFSCPMIDDLEREAD_TRACE_S,
    DFSCPMIDDLEREAD_TRACE_E,
    DFSCPLASTREAD_TRACE_S,
    DFSCPLASTREAD_TRACE_E,
    DFSCPBKFIRSTWRITE_TRACE_S,
    DFSCPBKFIRSTWRITE_TRACE_E,
    DFSCPBKMIDDLEWRITE_TRACE_S,
    DFSCPBKMIDDLEWRITE_TRACE_E,
    DFSCPBKLASTWRITE_TRACE_S,
    DFSCPBKLASTWRITE_TRACE_E
};

extern char *pszResumeMainT;

extern char *pszAgtConnectReq;
extern char *pszAgentDisConnectReq;
extern char *pszConnectDiscrimReq;
extern char *pszDustRecvReq;
extern char *pszAgtSendHedReq;
extern char *pszAgtSendReq;
extern char *pszAgtRecvHedReq;
extern char *pszAgtRecvDefData;
extern char *pszAgtRecvReq;
extern char *pszWatchDogStatRecvReq;

extern char *pszDFSopenReq;
extern char *pszDFScloseReq;
extern char *pszDFScreatReq;
extern char *pszDFSremoveReq;
extern char *pszDFSrenameReq;
extern char *pszDFSreadReq;
extern char *pszDFSwriteReq;
extern char *pszDFSmkdirReq;
extern char *pszDFSrmdirReq;
extern char *pszDFSopendirReq;
extern char *pszDFSreadstatReq;
extern char *pszDFSclosedirReq;
extern char *pszDFSutimeReq;
extern char *pszDFSchownReq;
extern char *pszDFSstatReq;
extern char *pszDFSCpFirstReadReq;
extern char *pszDFSCpMiddleReadReq;
extern char *pszDFSCpLastReadReq;
extern char *pszDFSCpbkFirstWriteReq;
extern char *pszDFSCpbkMiddleWriteReq;
extern char *pszDFSCpbkLastWriteReq;

extern char *pszDFSdirsyncReq;

extern char *pszDFStimeReq;

extern char *pszDFSgetacl_syscallReq;
extern char *pszDFSsetacl_syscallReq;
extern char *pszDFSdelacl_syscallReq;
extern char *pszDFSrightsReq;

extern char *pszDFSLinGetIdReq;
extern char *pszDFSLinGetPasswdReq;
extern char *pszDFSLinResetKeyReq;
extern char *pszDFSLoutReq;

extern char *pszDFSlseekReq;

#define DDA_HED_COM_LEN             0x0002
#define DDA_HED_RESERVE_LEN         0x0002
#define DDA_HED_DATASIZE_LEN        0x0004

#define DDA_HED_LEN (DDA_HED_COM_LEN+DDA_HED_RESERVE_LEN+DDA_HED_DATASIZE_LEN)

#define DDA_RESERVE                 0x0000

#define DDA_RETURN_SIZE             0x0004
#define DDA_ERRNO_SIZE              0x0004
#define DDA_DEFAULT_RECV_SIZE       (DDA_RETURN_SIZE+DDA_ERRNO_SIZE)

#define DDA_OPEN_REQ_P1_SIZE        0x0004
#define DDA_OPEN_REQ_P2_SIZE        0x0004
#define DDA_OPEN_REQ_L3_SIZE        0x0004


#define DDA_CLOSE_REQ_P1_SIZE       0x0004
#define DDA_CLOSE_REQ_SIZE          DDA_CLOSE_REQ_P1_SIZE


#define DDA_CREAT_REQ_P1_SIZE       0x0002
#define DDA_CREAT_REQ_L2_SIZE       0x0004


#define DDA_REMOVE_REQ_L1_SIZE      0x0004


#define DDA_RENAME_REQ_L1_SIZE      0x0004
#define DDA_RENAME_REQ_L2_SIZE      0x0004


#define DDA_READ_REQ_P1_SIZE        0x0004
#define DDA_READ_REQ_P2_SIZE        0x0004
#define DDA_READ_REQ_SIZE           (DDA_READ_REQ_P1_SIZE+DDA_READ_REQ_P2_SIZE)


#define DDA_WRITE_REQ_P1_SIZE       0x0004
#define DDA_WRITE_REQ_P2_SIZE       0x0004


#define DDA_MKDIR_REQ_P1_SIZE       0x0002
#define DDA_MKDIR_REQ_L2_SIZE       0x0004


#define DDA_RMDIR_REQ_L1_SIZE       0x0004


#define DDA_ACL_REQ_P1_SIZE         0x0004
#define DDA_ACL_REQ_P2_SIZE         0x0002
#define DDA_ACL_REQ_L3_SIZE         0x0004
#define DDA_ACL_REQ_P4_SIZE         0x0004

#define DDA_ACL_RSP_L3_SIZE         0x0004

#define DDA_RIGHTS_REQ_L1_SIZE      0x0004


#define DDA_OPENDIR_REQ_L1_SIZE     0x0004

#define DDA_OPENDIR_RSP_P3_SIZE     0x0008
#define DDA_OPENDIR_RSP_SIZE        (DDA_DEFAULT_SIZE+DDA_OPENDIR_RSP_P3_SIZE)

#define DDA_READSTAT_REQ_P1_SIZE    0x0008
#define DDA_READSTAT_REQ_P2_SIZE    0x0002
#define DDA_READSTAT_REQ_SIZE       (DDA_READSTAT_REQ_P1_SIZE+DDA_READSTAT_REQ_P2_SIZE)

#define DDA_READSTAT_RSP_L3_SIZE    0x0004
#define DDA_READSTAT_RSP_P4_SIZE    0x0002
#define DDA_READSTAT_RSP_P5_SIZE    0x0004
#define DDA_READSTAT_RSP_L6_SIZE    0x0004
#define DDA_READSTAT_RSP_P7_SIZE    0x0004

#define DDA_CLOSEDIR_REQ_P1_SIZE    0x0008
#define DDA_CLOSEDIR_REQ_SIZE       DDA_CLOSEDIR_REQ_P1_SIZE


#define DDA_TIME_REQ_SIZE           0x0000


#define DDA_DIRSYNC_REQ_P1_SIZE     0x0004
#define DDA_DIRSYNC_REQ_L2_SIZE     0x0004

#define DDA_DIRSYNC_RSP_P3_SIZE     0x0004
#define DDA_DIRSYNC_RSP_P5_SIZE     0x0004
#define DDA_DIRSYNC_RSP_P6_SIZE     0x0004

#define DDA_CON_REQ_P1_SIZE         0x0002
#define DDA_CON_REQ_P2_SIZE         0x0002
#define DDA_CON_REQ_P3_SIZE         0x0004
#define DDA_CON_REQ_P4_SIZE         0x0004
#define DDA_CON_REQ_P5_SIZE         0x0004
#define DDA_CON_REQ_SIZE            (DDA_CON_REQ_P1_SIZE+DDA_CON_REQ_P2_SIZE+DDA_CON_REQ_P3_SIZE+DDA_CON_REQ_P4_SIZE+DDA_CON_REQ_P5_SIZE)

#define DDA_CON_RSP_P1_SIZE         0x0004
#define DDA_CON_RSP_P2_SIZE         0x0002
#define DDA_CON_RSP_P3_SIZE         0x0004
#define DDA_CON_RSP_SIZE            (DDA_CON_RSP_P1_SIZE + DDA_CON_RSP_P2_SIZE + DDA_CON_RSP_P3_SIZE)

#define DDA_HEALTHCHK_REQ_SIZE      0x0000
#define DDA_HEALTHCHK_RSP_SIZE      0x0000


#define DDA_GWMESSAGE_REQ_P1_SIZE   0x0004
#define DDA_GWMESSAGE_REQ_P2_SIZE   0x0004

#define DDA_STAT_REQ_L1_SIZE        0x0004

#define DDA_STAT_RSP_P3_SIZE        0x0002
#define DDA_STAT_RSP_P4_SIZE        0x0004
#define DDA_STAT_RSP_P5_SIZE        0x0004
#define DDA_STAT_RSP_L6_SIZE        0x0004

#define DDA_UTIME_REQ_P1_SIZE       0x0002
#define DDA_UTIME_REQ_P2_SIZE       0x0004
#define DDA_UTIME_REQ_P3_SIZE       0x0004
#define DDA_UTIME_REQ_L4_SIZE       0x0004


#define DDA_CHOWN_REQ_L1_SIZE       0x0004
#define DDA_CHOWN_REQ_L2_SIZE       0x0004
#define DDA_CHOWN_REQ_L3_SIZE       0x0004


#define DDA_LINGETID_REQ_P1_SIZE    0x00004

#define DDA_LINGETID_RSP_P3_SIZE    0x00004

#define DDA_LINGETPASSWD_REQ_P1_SIZE 0x0004


#define DDA_LINRESETKEY_REQ_SIZE    0x0000


#define DDA_LOUT_REQ_SIZE           0x0000


#define DDA_LSEEK_REQ_P1_SIZE   0x0004
#define DDA_LSEEK_REQ_P2_SIZE   0x0004
#define DDA_LSEEK_REQ_P3_SIZE   0x0004
#define DDA_LSEEK_REQ_SIZE  (DDA_LSEEK_REQ_P1_SIZE+DDA_LSEEK_REQ_P2_SIZE+DDA_LSEEK_REQ_P3_SIZE)


#define DDA_CPREAD_REQ_P1_SIZE          0x0004
#define DDA_CPREAD_REQ_P2_SIZE          0x0004
#define DDA_CPREAD_REQ_SIZE             (DDA_CPREAD_REQ_P1_SIZE+DDA_CPREAD_REQ_P2_SIZE)

#define DDA_CPBKWRITE_REQ_P1_SIZE       0x0004
#define DDA_CPBKWRITE_REQ_P2_SIZE       0x0004


#define DDA_CPBKWRITETERM_REQ_P1_SIZE   0x0004

#define CONPV_0100              0x0100
#define CONPV_0200              0x0200


#define DDA_OPEN_COM_REQ        0x0001
#define DDA_CLOSE_COM_REQ       0x0002
#define DDA_CREAT_COM_REQ       0x0003
#define DDA_REMOVE_COM_REQ      0x0004
#define DDA_RENAME_COM_REQ      0x0005
#define DDA_READ_COM_REQ        0x0006
#define DDA_WRITE_COM_REQ       0x0007
#define DDA_MKDIR_COM_REQ       0x0008
#define DDA_RMDIR_COM_REQ       0x0009
#define DDA_AFSSYSCALL_COM_REQ  0x000A
#define DDA_RIGHTS_COM_REQ      0x000B
#define DDA_OPENDIR_COM_REQ     0x000C
#define DDA_READSTAT_COM_REQ    0x000D
#define DDA_CLOSEDIR_COM_REQ    0x000E
#define DDA_TIME_COM_REQ        0x000F
#define DDA_DIRSYNC_COM_REQ     0x0010
#define DDA_CONNECT_COM_REQ     0x0011
#define DDA_HEALTHCHK_COM_REQ   0x0012
#define DDA_MESSAGE_COM_REQ     0x0013
#define DDA_STAT_COM_REQ        0x0014
#define DDA_UTIME_COM_REQ       0x0015
#define DDA_CHOWN_COM_REQ       0x0016
#define DDA_LINGETID_COM_REQ    0x0017
#define DDA_LINGETPASSWD_COM_REQ 0x0018
#define DDA_LINRESETKEY_COM_REQ 0x0019
#define DDA_LOUT_COM_REQ        0x001A
#define DDA_LSEEK_COM_REQ       0x001B
#define DDA_CPREAD_COM_REQ      0x001C
#define DDA_CPBKWRITE_COM_REQ   0x001D
#define DDA_CPBKWRITETERM_COM_REQ 0x001E

#define DDA_OPEN_COM_RSP        0x8001
#define DDA_CLOSE_COM_RSP       0x8002
#define DDA_CREAT_COM_RSP       0x8003
#define DDA_REMOVE_COM_RSP      0x8004
#define DDA_RENAME_COM_RSP      0x8005
#define DDA_READ_COM_RSP        0x8006
#define DDA_WRITE_COM_RSP       0x8007
#define DDA_MKDIR_COM_RSP       0x8008
#define DDA_RMDIR_COM_RSP       0x8009
#define DDA_AFSSYSCALL_COM_RSP  0x800A
#define DDA_RIGHTS_COM_RSP       0x800B
#define DDA_OPENDIR_COM_RSP     0x800C
#define DDA_READSTAT_COM_RSP    0x800D
#define DDA_CLOSEDIR_COM_RSP    0x800E
#define DDA_TIME_COM_RSP        0x800F
#define DDA_DIRSYNC_COM_RSP     0x8010
#define DDA_CONNECT_COM_RSP     0x8011
#define DDA_HEALTHCHK_COM_RSP   0x8012
#define DDA_STAT_COM_RSP        0x8014
#define DDA_UTIME_COM_RSP       0x8015
#define DDA_CHOWN_COM_RSP       0x8016
#define DDA_LINGETID_COM_RSP    0x8017
#define DDA_LINGETPASSWD_COM_RSP 0x8018
#define DDA_LINRESETKEY_COM_RSP 0x8019
#define DDA_LOUT_COM_RSP        0x801A
#define DDA_LSEEK_COM_RSP       0x801B
#define DDA_CPREAD_COM_RSP      0x801C
#define DDA_CPBKWRITETERM_COM_RSP 0x801E
#define DDA_NOSUPPORT_COM_RSP   0xFFFF

#define DVNX_OPCODE_GETACL  0x00000000
#define DVNX_OPCODE_SETACL  0x00000001
#define DVNX_OPCODE_DELACL  0x00000002

extern char aszDFAMServName[];
extern char aszDFAMProtcol[];
extern unsigned short ushDFAMGWTranceVer;
extern int idfadceNLMID;
extern int idfadceTGID;
#define COMMUNICATION_WATCHOVERTIME 10

extern void fnDCEAPICleanUp(int i);

extern BOOL ConnectDiscrimReq(struct ComTGroup_t *pCom,unsigned short ushConAssort,int *piErrCode);
extern int DustRecv(struct ComTGroup_t *pCom,int iDustSize,int *piErrCode);
extern BOOL AgtSendHedReq(struct ComTGroup_t *pCom,unsigned short ushComReq,int iSendDataSize,int *piErrCode);
extern int AgtSendReq(struct ComTGroup_t *pCom,char *pcSendData,int iSendDataLen,int *piErrCode,int iLogLen);
extern BOOL AgtRecvHedReq(struct ComTGroup_t *pCom,unsigned short *pushComRsp,int *piRecvDataLen,int *piErrCode);
extern BOOL AgtRecvDefData(struct ComTGroup_t *pCom,unsigned long *puiAgtRetCode,unsigned long *puiAgtErrCode,int *piErrCode);
extern int AgtRecvReq(struct ComTGroup_t *pCom,char *pcRecvData,int iRecvDataLen,int *piErrCode,int iLogLen);
extern int RecvWaitTime(struct ComTGroup_t *pCom,int iWaitTime,int *piErrCode);

extern unsigned int PathChk(const char *pszPath,int *piErrCode);

extern void ACLChanghton(char *pcAcl);

extern void ResumeMainT(void);

#ifdef DCELOG

#define MESSAG 0x0001
#define FUNC_E 0x0002
#define SDDATA 0x0003
#define RVDATA 0x0004
#define HLTCHK 0x0005
#define HOST_N 0x0006
#define IPADDR 0x0007
#define PORT_N 0x0008
#define DISCON 0x0009

enum {
    DLNOERR = 0,
    DLREMOVEERR = 1,
    DLWRITEERR = 2,
    DLOPENERR = 3,
    DLCLOSEERR = 4,
    DLFSEEKERR = 5,
    DLFFLUSHERR = 6
};

extern int DCELogInit(BOOL bFlag);
extern int DCELogTerm(void);
extern void DCELogSoketSet(int iSockfd,unsigned short ushConAssort);
extern void DCELogSoketReset(int iSockfd);
extern int DCELogOutput(LONG uiEvent,int iLength,int iSockfd,LONG uiComNum,void *pData,int iErr);
extern int DCELogOptChk(int argc,char **argv);
#endif

extern int DCEMemInit(void);
extern int DCEMemTerm(void);
extern int DCEMemCpComLog(char *pcData,long lSize);
#endif

