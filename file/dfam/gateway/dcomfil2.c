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
 * $Log: dcomfil2.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:14  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:43  marty]
 *
 * Revision 1.1.2.1  1996/02/21  06:42:33  takano
 * 	First Submit
 * 	[1996/02/21  06:42:26  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <string.h>
#include <nwsemaph.h>
#include <nwtypes.h>
#include <netinet\in.h>
#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "dceapi.h"

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSrmdir                                                         */
/*  name =  DFS directory deletion routine                                   */
/*  func = Sends DFAM Agent a DFS directory deletion request with using the  */
/*         to-be-made directory path name and permission bits passed from    */
/*         the caller.  Then receives the results and error information      */
/*         from DFAM Agent to send them to the caller.  Does not check the   */
/*         correctness of DFSErrno, and pComTbl.                             */
/*           1. In the case that path=NULL or the path length is 0, this     */
/*              function does not send data to DFAM Agent, but returns       */
/*              DENOENT as error information.                                */
/*              In the case that the path length exceeds DGW_PATH_MAX(1024), */
/*              this function returns DENAMETOOLONG as error information.    */
/*           2. Sets a semaphore.                                            */
/*           3. Sends the header information.                                */
/*           4. Sends the path name length and path name.                    */
/*           5. Receives the header information.                             */
/*           6. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           7. Frees the semaphore.                                         */
/*           8. Sets the results to parameters, and returns to the caller.   */
/*                                                                           */
/*                                                                           */
/*  i/o  = path       : char * :inp: DFS path file                           */
/*         mode       : dmode_t:inp: permission bits                         */
/*                     (unsigned short)                                      */
/*         DFSErrno   : int *  :out: DFS error information area              */
/*         pComTbl    : void * :i/o: thread group table                      */
/*                                                                           */
/*  return =  0 : successful                                                 */
/*           -1 : failure                                                    */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : detected line disconnection                      */
/*         DETIMEOUTAGT   : time out                                         */
/*         DERESPONCEDATA : response data invalid                            */
/*         DEUNLOADTCPIP  : TCPIP.NLM was unloaded                           */
/*         DEDFAMDCESYSTEM: internal error                                   */
/*         DEDFAMTONOVELL : errors other than listed in manuals              */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*  date =  Jan. 10, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFSrmdirReq(char *path,int *DFSErrno,void *pComTbl)
{
    int             iErrCode;
    int             iRetCode;
    unsigned long   uiAgtRetCode;
    unsigned long   uiAgtErrCode;
    BOOL            bSemFlag;
    unsigned short  ushComRsp;
    int             iAgtSendSize;
    int             iCode;
    BOOL            bCode;
    int             iTotalSendSize;
    unsigned int    tmPathLen;
    unsigned int    tmNPathLen;
    struct ComTGroup_t *pCom;

    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSrmdirReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSRMDIR_TRACE_S,pCom->iSockfd,0,aszEyeChach);
#endif

    iRetCode = -1;
    iErrCode = DENOERR;
    bSemFlag = FALSE;

    if (pCom->iConErrFlag != DENOERR){
        iErrCode = pCom->iConErrFlag;
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CONERRFLAG_ERR,pCom->iConErrFlag,0,aszEyeChach);
#endif
        goto ErrExit;
    }


    tmPathLen = PathChk(path,&iErrCode);
    if (tmPathLen == 0xFFFFFFFF){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,PATH_ERR,tmPathLen,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iTotalSendSize = DDA_RMDIR_REQ_L1_SIZE + (int)tmPathLen;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_RMDIR_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    tmNPathLen = (unsigned int)htonl((unsigned long)tmPathLen);

    iCode = AgtSendReq(pCom,(char *)&tmNPathLen,DDA_RMDIR_REQ_L1_SIZE,&iErrCode,DDA_RMDIR_REQ_L1_SIZE);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA2_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iCode = AgtSendReq(pCom,path,tmPathLen,&iErrCode,tmPathLen);
    if (iCode == -1){

        goto ErrExit;
    }

    bCode = AgtRecvHedReq(pCom,&ushComRsp,&iAgtSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }
    switch(ushComRsp){
        case DDA_RMDIR_COM_RSP :
            break;
        default :
            iErrCode = DERESPONCEDATA;
            pCom->iConErrFlag = iErrCode;
            goto ErrExit;
    }

    bCode = AgtRecvDefData(pCom,&uiAgtRetCode,&uiAgtErrCode,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }
    iAgtSendSize -= DDA_DEFAULT_RECV_SIZE;

    if (iAgtSendSize > 0){
        iCode = DustRecv(pCom,iAgtSendSize,&iErrCode);
        if (iCode == -1){
            goto ErrExit;
        }
    }

    iRetCode = (int)uiAgtRetCode;
    iErrCode = (int)uiAgtErrCode;

ErrExit:
    if (bSemFlag == TRUE){
        SignalLocalSemaphore(pCom->uiCConn_sh);
    }

    *DFSErrno = iErrCode;
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSRMDIR_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSrmdirReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSopendirReq                                                    */
/*  name =  DFS directory open routine                                       */
/*  func = Sends DFAM Agent a DFS directory open request with using the      */
/*         directory path name passed from the caller.  Then receives the    */
/*         results and error information from DFAM Agent to send them to     */
/*         the caller.  Does not check the correctness of dirp, DFSErrno,    */
/*         and pComTbl.                                                      */
/*           1. In the case that path=NULL or the path length is 0, this     */
/*              function does not send data to DFAM Agent, but returns       */
/*              DENOENT as error information.                                */
/*              In the case that the path length exceeds DGW_PATH_MAX(1024), */
/*              this function returns DENAMETOOLONG as error information.    */
/*           2. Sets a semaphore.                                            */
/*           3. Sends the header information.                                */
/*           4. Sends the path name length and path name.                    */
/*           5. Receives the header information.                             */
/*           6. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           7. Sends the pointer value if the return value is 0x00000000.   */
/*           8. Frees the semaphore.                                         */
/*                                                                           */
/*  i/o  = dirname    : char * :inp: DFS path name                           */
/*         dirp       : DDIR * :out: directory pointer storing area          */
/*         DFSErrno   : int *  :out: DFS error information area              */
/*         pComTbl    : void * :i/o: thread group table                      */
/*                                                                           */
/*  return =  0 : successful                                                 */
/*           -1 : failure                                                    */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : detected line disconnection                      */
/*         DETIMEOUTAGT   : time out                                         */
/*         DERESPONCEDATA : response data invalid                            */
/*         DEUNLOADTCPIP  : TCPIP.NLM was unloaded                           */
/*         DEDFAMDCESYSTEM: internal error                                   */
/*         DEDFAMTONOVELL : errors other than listed in manuals              */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*                                                                           */
/*  date =  Jan. 10, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFSopendirReq(char *dirname,DDIR *dirp,int *DFSErrno,void *pComTbl)
{
    int             iErrCode;
    int             iRetCode;
    unsigned long   uiAgtRetCode;
    unsigned long   uiAgtErrCode;
    BOOL            bSemFlag;
    unsigned short  ushComRsp;
    int             iAgtSendSize;
    int             iCode;
    BOOL            bCode;
    int             iTotalSendSize;
    unsigned int    tmPathLen;
    unsigned int    tmNPathLen;
    struct ComTGroup_t *pCom;

    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSopendirReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSOPENDIR_TRACE_S,pCom->iSockfd,0,aszEyeChach);
#endif

    iRetCode = -1;
    iErrCode = DENOERR;
    bSemFlag = FALSE;

    if (pCom->iConErrFlag != DENOERR){
        iErrCode = pCom->iConErrFlag;
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CONERRFLAG_ERR,pCom->iConErrFlag,0,aszEyeChach);
#endif
        goto ErrExit;
    }


    tmPathLen = PathChk(dirname,&iErrCode);
    if (tmPathLen == 0xFFFFFFFF){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,PATH_ERR,tmPathLen,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iTotalSendSize = DDA_OPENDIR_REQ_L1_SIZE+tmPathLen;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_OPENDIR_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    tmNPathLen = (unsigned int)htonl((unsigned long)tmPathLen);

    iCode = AgtSendReq(pCom,(char *)&tmNPathLen,DDA_OPENDIR_REQ_L1_SIZE,&iErrCode,DDA_OPENDIR_REQ_L1_SIZE);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA2_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iCode = AgtSendReq(pCom,dirname,tmPathLen,&iErrCode,tmPathLen);
    if (iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA3_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    bCode = AgtRecvHedReq(pCom,&ushComRsp,&iAgtSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }
    switch(ushComRsp){
        case DDA_OPENDIR_COM_RSP :
            break;
        default :
            iErrCode = DERESPONCEDATA;
            pCom->iConErrFlag = iErrCode;
            goto ErrExit;
    }

    bCode = AgtRecvDefData(pCom,&uiAgtRetCode,&uiAgtErrCode,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }
    iAgtSendSize -= DDA_DEFAULT_RECV_SIZE;

    if (uiAgtRetCode == 0x00000000){
        iCode = AgtRecvReq(pCom,dirp->ptr,DDA_OPENDIR_RSP_P3_SIZE,&iErrCode,DDA_OPENDIR_RSP_P3_SIZE);
        if (iCode == -1){
#ifdef DCETRC
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,RECV_DATA3_ERR,iCode,iErrCode,aszEyeChach);
#endif
            goto ErrExit;
        }
        iAgtSendSize -= iCode;
    }

    if (iAgtSendSize > 0){
        iCode = DustRecv(pCom,iAgtSendSize,&iErrCode);
        if (iCode == -1){
            goto ErrExit;
        }
    }

    iRetCode = (int)uiAgtRetCode;
    iErrCode = (int)uiAgtErrCode;

ErrExit:
    if (bSemFlag == TRUE){
        SignalLocalSemaphore(pCom->uiCConn_sh);

    }

    *DFSErrno = iErrCode;
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSOPENDIR_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSopendirReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSreadstatReq                                                   */
/*  name =  DFS directory read and file information get routine              */
/*  func = Sends DFAM Agent a DFS file/directory read and information get    */
/*         request with using the values in dirp and stat mode passed from   */
/*         the caller.  Then receives the results and error information      */
/*         from DFAM Agent to send them to the caller.  Does not check the   */
/*         correctness of ststmode, DFSErrno, and pComTbl.                   */
/*           1. In the case that dirp=NULL, this function does not send      */
/*              data to DFAM Agent, but returns DEBADF as error information. */
/*              In the case that direntry=NULL, this function does not       */
/*              DFAM Agent, but returns DEFAULT.                             */
/*           2. Sets a semaphore.                                            */
/*           3. Sends the header information.                                */
/*           4. Sends the pointer value and ststmode.                        */
/*           5. Receives the header information.                             */
/*           6. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           7. Receives the path name length and path name.                 */
/*           8. Receives the last update time and file size.                 */
/*                                                                           */
/*         stat mode: DS_GETSUBDIR   : get subdirectory                      */
/*                    DS_GETFILENAME : get file                              */
/*                    (if this is not selected, DFAM Agent will ignore the   */
/*                     the file)                                             */
/*                    DS_GETTIME     : get the last update time              */
/*                    DS_GETOWN_SIZE : get Owner and file size               */
/*                                                                           */
/*         The accepted combinations are ......                              */
/*         However, due to an imcomplete processing at DFAM Agent, not all   */
/*         the combinations work properly.  The certified combinations       */
/*         are: ........                                                     */
/*                                                                           */
/*         struct ddirent {                                                  */
/*             char d_name[DGW_PATH_MAX+1];                                  */
/*             dmode_t d_mode;                                               */
/*             dtime_t d_mtime;                                              */
/*             char d_ownname[DGW_OWNER_MAX+1];                              */
/*             doff_t d_size;                                                */
/*         };                                                                */
/*         *1: The variables that are not targetted do not change.           */
/*         *2: All the variables do not change if the return value is -1.    */
/*                                                                           */
/*         typedef struct d__dirdesc {                                       */
/*             char ptr[8];                                                  */
/*         } DDIR;                                                           */
/*                                                                           */
/*  i/o  = dirp       : DDIR *       :out: directory pointer storing area    */
/*         statmode   : short        :inp: stat mode                         */
/*         direntry   : struct ddir *:out: pointer to the results            */
/*         DFSErrno   : int *        :out: DFS error information area        */
/*         pComTbl    : void *       :i/o: thread group table                */
/*                                                                           */
/*  return =  0 : successful                                                 */
/*           -1 : failure                                                    */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : detected line disconnection                      */
/*         DETIMEOUTAGT   : time out                                         */
/*         DERESPONCEDATA : response data invalid                            */
/*         DEUNLOADTCPIP  : TCPIP.NLM was unloaded                           */
/*         DEDFAMDCESYSTEM: internal error                                   */
/*         DEDFAMTONOVELL : errors other than listed in manuals              */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*                                                                           */
/*  date =  Jan. 10, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFSreadstatReq(DDIR *dirp,short statmode,struct ddirent *direntry,int *DFSErrno,void *pComTbl)
{
    int             iErrCode;
    int             iRetCode;
    unsigned long   uiAgtRetCode;
    unsigned long   uiAgtErrCode;
    BOOL            bSemFlag;
    unsigned short  ushComRsp;
    int             iTotalSendSize;
    int             iPtrCtr;
    int             iAgtSendSize;
    int             iCode;
    BOOL            bCode;
    unsigned int    iFileNameLen;
    unsigned int    iOwnerLen;
    struct ComTGroup_t *pCom;
    char            acSendBuf[DDA_READSTAT_REQ_SIZE];


    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSreadstatReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSREADSTAT_TRACE_S,pCom->iSockfd,0,aszEyeChach);
#endif

    iRetCode = -1;
    iErrCode = DENOERR;
    bSemFlag = FALSE;

    if (pCom->iConErrFlag != DENOERR){
        iErrCode = pCom->iConErrFlag;
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CONERRFLAG_ERR,pCom->iConErrFlag,0,aszEyeChach);
#endif
        goto ErrExit;
    }

    if (dirp == NULL){
        iErrCode = DEBADF;
        goto ErrExit;
    }

    if (direntry == NULL){
        iErrCode = DEFAULT;
        goto ErrExit;
    }

    iTotalSendSize = DDA_READSTAT_REQ_SIZE;


    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_READSTAT_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    iPtrCtr = 0;
    memcpy(&acSendBuf[iPtrCtr],dirp->ptr,DDA_READSTAT_REQ_P1_SIZE);
    iPtrCtr += DDA_READSTAT_REQ_P1_SIZE;

    *(unsigned short *)&acSendBuf[iPtrCtr] = htons(statmode);
    iPtrCtr += DDA_READSTAT_REQ_P2_SIZE;

    iCode = AgtSendReq(pCom,acSendBuf,DDA_READSTAT_REQ_SIZE,&iErrCode,DDA_READSTAT_REQ_SIZE);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA2_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    bCode = AgtRecvHedReq(pCom,&ushComRsp,&iAgtSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    switch(ushComRsp){
        case DDA_READSTAT_COM_RSP :
            break;
        default :
            iErrCode = DERESPONCEDATA;
            pCom->iConErrFlag = iErrCode;
            goto ErrExit;
    }

    bCode = AgtRecvDefData(pCom,&uiAgtRetCode,&uiAgtErrCode,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }
    iAgtSendSize -= DDA_DEFAULT_RECV_SIZE;

    if (uiAgtRetCode == 0x00000000){
        iCode = AgtRecvReq(pCom,(char *)&iFileNameLen,DDA_READSTAT_RSP_L3_SIZE,&iErrCode,DDA_READSTAT_RSP_L3_SIZE);
        if (iCode == -1){
#ifdef DCETRC
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,RECV_DATA3_ERR,iCode,iErrCode,aszEyeChach);
#endif
            goto ErrExit;
        }
        iFileNameLen = (unsigned int)ntohl((unsigned long)iFileNameLen);

        iAgtSendSize -= DDA_READSTAT_RSP_L3_SIZE;

        iCode = AgtRecvReq(pCom,direntry->d_name,iFileNameLen,&iErrCode,iFileNameLen);
        if (iCode == -1){
#ifdef DCETRC
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,RECV_DATA4_ERR,iCode,iErrCode,aszEyeChach);
#endif
            goto ErrExit;
        }

        direntry->d_name[iFileNameLen] = '\0';
        iAgtSendSize -= iFileNameLen;

        iCode = AgtRecvReq(pCom,(char *)&direntry->d_mode,DDA_READSTAT_RSP_P4_SIZE,&iErrCode,DDA_READSTAT_RSP_P4_SIZE);
        if (iCode == -1){
#ifdef DCETRC
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,RECV_DATA5_ERR,iCode,iErrCode,aszEyeChach);
#endif
            goto ErrExit;
        }
        direntry->d_mode = ntohs(direntry->d_mode);

        iAgtSendSize -= DDA_READSTAT_RSP_P4_SIZE;

        if (statmode & DS_GETTIME){
            iCode = AgtRecvReq(pCom,(char *)&direntry->d_mtime,DDA_READSTAT_RSP_P5_SIZE,&iErrCode,DDA_READSTAT_RSP_P5_SIZE);
            if (iCode == -1){
#ifdef DCETRC
                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,RECV_DATA6_ERR,iCode,iErrCode,aszEyeChach);
#endif
                goto ErrExit;
            }
            direntry->d_mtime = ntohl(direntry->d_mtime);
            iAgtSendSize -= iCode;

            if (statmode & DS_GETOWN_SIZE){

                iCode = AgtRecvReq(pCom,(char *)&iOwnerLen,DDA_READSTAT_RSP_L6_SIZE,&iErrCode,DDA_READSTAT_RSP_L6_SIZE);
                if (iCode == -1){
#ifdef DCETRC
                    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,RECV_DATA7_ERR,iCode,iErrCode,aszEyeChach);
#endif
                    goto ErrExit;
                }
                iOwnerLen = (unsigned int)ntohl((unsigned long)iOwnerLen);
                iAgtSendSize -= iCode;

                if (iOwnerLen != 0xFFFFFFFF){

                    iCode = AgtRecvReq(pCom,direntry->d_ownname,iOwnerLen,&iErrCode,iOwnerLen);
                    if (iCode == -1){
#ifdef DCETRC
                        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,RECV_DATA8_ERR,iCode,iErrCode,aszEyeChach);
#endif
                        goto ErrExit;
                    }
                    direntry->d_ownname[iOwnerLen] = '\0';
                    iAgtSendSize -= iCode;
                } else {
                    direntry->d_ownname[0] = '\0';
                }

                iCode = AgtRecvReq(pCom,(char *)&direntry->d_size,DDA_READSTAT_RSP_P7_SIZE,&iErrCode,DDA_READSTAT_RSP_P7_SIZE);
                if (iCode == -1){
#ifdef DCETRC
                    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,RECV_DATA9_ERR,iCode,iErrCode,aszEyeChach);
#endif
                    goto ErrExit;
                }
                direntry->d_size = ntohl(direntry->d_size);
                iAgtSendSize -= iCode;
            }
        }
    }


    if (iAgtSendSize > 0){
        iCode = DustRecv(pCom,iAgtSendSize,&iErrCode);
        if (iCode == -1){
            goto ErrExit;
        }
    }

    iRetCode = (int)uiAgtRetCode;
    iErrCode = (int)uiAgtErrCode;

ErrExit:
    if (bSemFlag == TRUE){
        SignalLocalSemaphore(pCom->uiCConn_sh);
    }

    *DFSErrno = iErrCode;
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSREADSTAT_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSreadstatReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSclosedirReq                                                   */
/*  name =  DFS directory close                                              */
/*  func = Sends DFAM Agent a DFS directory close request with using the     */
/*         directory path name passed from the caller.  Then receives the    */
/*         results and error information from DFAM Agent to send them to     */
/*         the caller.  Does not check the correctness of DFSErrno,          */
/*         and pComTbl.                                                      */
/*           1. In the case that dirp=NULL, this function does not send      */
/*              request to DFAM Agent, but returns DEBADF as error           */
/*              information.                                                 */
/*           2. Sets a semaphore.                                            */
/*           3. Sends the header information.                                */
/*           4. Sends the pointer value.                                     */
/*           5. Receives the header information.                             */
/*           6. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           7. Frees the semaphore.                                         */
/*           8. Sets the results to parameters, and returns to the caller.   */
/*                                                                           */
/*  i/o  = dirp       : DDIR * :inp: pointer to the directory to be closed   */
/*         DFSErrno   : int *  :out: DFS error information area              */
/*         pComTbl    : void * :i/o: thread group table                      */
/*                                                                           */
/*  return =  0 : successful                                                 */
/*           -1 : failure                                                    */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*  return =  0 : successful                                                 */
/*           -1 : failure                                                    */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : detected line disconnection                      */
/*         DETIMEOUTAGT   : time out                                         */
/*         DERESPONCEDATA : response data invalid                            */
/*         DEUNLOADTCPIP  : TCPIP.NLM was unloaded                           */
/*         DEDFAMDCESYSTEM: internal error                                   */
/*         DEDFAMTONOVELL : errors other than listed in manuals              */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*  date =  Jan. 10, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFSclosedirReq(DDIR *dirp,int *DFSErrno,void *pComTbl)
{
    int             iErrCode;
    int             iRetCode;
    unsigned long   uiAgtRetCode;
    unsigned long   uiAgtErrCode;
    BOOL            bSemFlag;
    unsigned short  ushComRsp;
    int             iAgtSendSize;
    int             iCode;
    BOOL            bCode;
    int             iTotalSendSize;
    struct ComTGroup_t *pCom;

    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSclosedirReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSCLOSEDIR_TRACE_S,pCom->iSockfd,0,aszEyeChach);
#endif
    if (pCom->iConErrFlag != DENOERR){
        iErrCode = pCom->iConErrFlag;
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CONERRFLAG_ERR,pCom->iConErrFlag,0,aszEyeChach);
#endif
        goto ErrExit;
    }


    iRetCode = -1;
    iErrCode = DENOERR;
    bSemFlag = FALSE;

    if (dirp == NULL){
        iErrCode = DEBADF;
        goto ErrExit;
    }

    iTotalSendSize = DDA_CLOSEDIR_REQ_SIZE;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_CLOSEDIR_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    iCode = AgtSendReq(pCom,dirp->ptr,DDA_CLOSEDIR_REQ_P1_SIZE,&iErrCode,DDA_CLOSEDIR_REQ_P1_SIZE);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA2_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    bCode = AgtRecvHedReq(pCom,&ushComRsp,&iAgtSendSize,&iErrCode);
    if (bCode == FALSE){
        goto ErrExit;
    }
    switch(ushComRsp){
        case DDA_CLOSEDIR_COM_RSP :
            break;
        default :
            iErrCode = DERESPONCEDATA;
            pCom->iConErrFlag = iErrCode;
        goto ErrExit;
    }

    bCode = AgtRecvDefData(pCom,&uiAgtRetCode,&uiAgtErrCode,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }
    iAgtSendSize -= DDA_DEFAULT_RECV_SIZE;

    if (iAgtSendSize > 0){
        iCode = DustRecv(pCom,iAgtSendSize,&iErrCode);
        if (iCode == -1){
            goto ErrExit;
        }
    }

    iRetCode = (int)uiAgtRetCode;
    iErrCode = (int)uiAgtErrCode;

    if (!iRetCode){
        memset(dirp->ptr,0,sizeof(dirp->ptr));
    }

ErrExit:
    if (bSemFlag == TRUE){
        SignalLocalSemaphore(pCom->uiCConn_sh);
    }

    *DFSErrno = iErrCode;
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSCLOSEDIR_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSclosedirReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSutimeReq                                                      */
/*  name =  DFS file access and changed-time set routine                     */
/*  func = Sends DFAM Agent a DFS file access and new time set request with  */
/*         using the file path name and new time passed from                 */
/*         the caller.  Then receives the results and error information      */
/*         from DFAM Agent to send them to the caller.  Does not check the   */
/*         correctness of DFSErrno, and pComTbl.                             */
/*           1. In the case that path=NULL or the path length is 0, this     */
/*              function does not send a request to DFAM Agent, but returns  */
/*              DENOENT as error information.                                */
/*              In the case that the path length exceeds DGW_PATH_MAX(1024), */
/*              this function returns DENAMETOOLONG as error information.    */
/*           2. Sets a semaphore.                                            */
/*           3. Sends the header information.                                */
/*           4. Sends the last access time, last update time, path name      */
/*              length and path name.  If times is NULL, the last access     */
/*              time and last update time are not sent (only requests DFAM   */
/*              Agent to set the current time).                              */
/*           5. Receives the header information.                             */
/*           6. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           7. Frees the semaphore.                                         */
/*           8. Sets the results to parameters, and returns to the caller.   */
/*                                                                           */
/*         struct dutimbuf {                                                 */
/*             dtime_t actime;                                               */
/*             dtime_t modtime;                                              */
/*         };                                                                */
/*                                                                           */
/*  i/o  = path    : char *                 :inp: DFS full path name         */
/*         times   : const struct dutimbuf *:inp: structure for storing the  */
/*                                                value of the time          */
/*         DFSErrno: int *                  :out: DFS error information area */
/*         pComTbl : void *                 :i/o: thread group table         */
/*                                                                           */
/*  return =  0 : successful                                                 */
/*           -1 : failure                                                    */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : detected line disconnection                      */
/*         DETIMEOUTAGT   : time out                                         */
/*         DERESPONCEDATA : response data invalid                            */
/*         DEUNLOADTCPIP  : TCPIP.NLM was unloaded                           */
/*         DEDFAMDCESYSTEM: internal error                                   */
/*         DEDFAMTONOVELL : errors other than listed in manuals              */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*  date =  Jan. 10, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFSutimeReq(char *path,const struct dutimbuf *times,int *DFSErrno,void *pComTbl)
{
    int             iErrCode;
    int             iRetCode;
    unsigned long   uiAgtRetCode;
    unsigned long   uiAgtErrCode;
    BOOL            bSemFlag;
    unsigned short  ushComRsp;
    int             iAgtSendSize;
    int             iCode;
    BOOL            bCode;
    int             iTotalSendSize;
    int             iCtr;
    unsigned int    tmPathLen;
    struct ComTGroup_t *pCom;
    char            acSendBuf[DDA_UTIME_REQ_P1_SIZE+DDA_UTIME_REQ_P2_SIZE+DDA_UTIME_REQ_P3_SIZE+DDA_UTIME_REQ_L4_SIZE];

    unsigned short  ushTimeFlag;

    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSutimeReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSUTIME_TRACE_S,pCom->iSockfd,0,aszEyeChach);
#endif

    iRetCode = -1;
    iErrCode = DENOERR;
    bSemFlag = FALSE;

    if (pCom->iConErrFlag != DENOERR){
        iErrCode = pCom->iConErrFlag;
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CONERRFLAG_ERR,pCom->iConErrFlag,0,aszEyeChach);
#endif
        goto ErrExit;
    }


    tmPathLen = PathChk(path,&iErrCode);
    if (tmPathLen == 0xFFFFFFFF){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,PATH_ERR,tmPathLen,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iCtr = 0;
    iTotalSendSize = DDA_UTIME_REQ_P1_SIZE + DDA_UTIME_REQ_L4_SIZE + tmPathLen;

    ushTimeFlag = (times != NULL) ? 0x0000 : 0xFFFF;



    *(unsigned short *)&acSendBuf[iCtr] = htons(ushTimeFlag);
    iCtr += DDA_UTIME_REQ_P1_SIZE;

    if (times != NULL){
        iTotalSendSize += DDA_UTIME_REQ_P2_SIZE + DDA_UTIME_REQ_P3_SIZE;

        *(unsigned long *)&acSendBuf[iCtr] = htonl(times->actime);
        iCtr += DDA_UTIME_REQ_P2_SIZE;

        *(unsigned long *)&acSendBuf[iCtr] = htonl(times->modtime);
        iCtr += DDA_UTIME_REQ_P3_SIZE;
    }

    *(unsigned long *)&acSendBuf[iCtr] = htonl((unsigned long)tmPathLen);
    iCtr += DDA_UTIME_REQ_L4_SIZE;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_UTIME_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    iCode = AgtSendReq(pCom,acSendBuf,iCtr,&iErrCode,iCtr);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA2_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iCode = AgtSendReq(pCom,path,tmPathLen,&iErrCode,tmPathLen);
    if (iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA3_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    bCode = AgtRecvHedReq(pCom,&ushComRsp,&iAgtSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }
    switch(ushComRsp){
        case DDA_UTIME_COM_RSP :
            break;
        default :
            iErrCode = DERESPONCEDATA;
            pCom->iConErrFlag = iErrCode;
            goto ErrExit;
    }

    bCode = AgtRecvDefData(pCom,&uiAgtRetCode,&uiAgtErrCode,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }
    iAgtSendSize -= DDA_DEFAULT_RECV_SIZE;

    if (iAgtSendSize > 0){
        iCode = DustRecv(pCom,iAgtSendSize,&iErrCode);
        if (iCode == -1){
            goto ErrExit;
        }
    }

    iRetCode = (int)uiAgtRetCode;
    iErrCode = (int)uiAgtErrCode;

ErrExit:
    if (bSemFlag == TRUE){
        SignalLocalSemaphore(pCom->uiCConn_sh);
    }

    *DFSErrno = iErrCode;
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSUTIME_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSutimeReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSchownReq                                                      */
/*  name =  File owner changing routine                                      */
/*  func = Sends DFAM Agent a DFS file owner change request with             */
/*         using the file path name, owner name and group name passed from   */
/*         the caller.  Then receives the results and error information      */
/*         from DFAM Agent to send them to the caller.  Does not check the   */
/*         correctness of DFSErrno, and pComTbl.                             */
/*           1. In the case that path=NULL or the path length is 0, this     */
/*              function does not send a request to DFAM Agent, but returns  */
/*              DENOENT as error information.                                */
/*              In the case that the path length exceeds DGW_PATH_MAX(1024), */
/*              this function returns DENAMETOOLONG as error information.    */
/*              In the case that the length of owner or group is 0, returns  */
/*              EINVAL as error information.                                 */
/*           2. Sets a semaphore.                                            */
/*           3. Sends the header information.                                */
/*           4. Sends the path name length, path name, owner, and group name.*/
/*              If NULL is set for owner or group, this function sets        */
/*              0xFFFFFFFF to the length, does not set the names.            */
/*           5. Receives the header information.                             */
/*           6. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           7. Frees the semaphore.                                         */
/*           8. Sets the results to parameters, and returns to the caller.   */
/*                                                                           */
/*         struct dutimbuf {                                                 */
/*             dtime_t actime;                                               */
/*             dtime_t modtime;                                              */
/*                                                                           */
/*  i/o  = path    : char * :inp: DFS full path name                         */
/*         owner   : char * :inp: owner name                                 */
/*         group   : char * :inp: group name                                 */
/*         DFSErrno: int *  :out: DFS error information area                 */
/*         pComTbl : void * :i/o: thread group table                         */
/*                                                                           */
/*  return =  0 : successful                                                 */
/*           -1 : failure                                                    */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : detected line disconnection                      */
/*         DETIMEOUTAGT   : time out                                         */
/*         DERESPONCEDATA : response data invalid                            */
/*         DEUNLOADTCPIP  : TCPIP.NLM was unloaded                           */
/*         DEDFAMDCESYSTEM: internal error                                   */
/*         DEDFAMTONOVELL : errors other than listed in manuals              */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*  date =  Jan. 10, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFSchownReq(char *path,char *owner,char *group,int *DFSErrno,void *pComTbl)
{
    int             iErrCode;
    int             iRetCode;
    unsigned long   uiAgtRetCode;
    unsigned long   uiAgtErrCode;
    BOOL            bSemFlag;
    unsigned short  ushComRsp;
    int             iAgtSendSize;
    int             iCode;
    BOOL            bCode;
    int             iTotalSendSize;
    struct ComTGroup_t *pCom;
    int             iPtrCtr;
    unsigned int    tmPathLen;
    unsigned int    tmOwnerLen;
    unsigned int    tmGroupLen;
    unsigned int    tmNPathLen;
    unsigned int    tmNOwnerLen;
    unsigned int    tmNGroupLen;

    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSchownReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSCHOWN_TRACE_S,pCom->iSockfd,0,aszEyeChach);
#endif

    iRetCode = -1;
    iErrCode = DENOERR;
    bSemFlag = FALSE;

    if (pCom->iConErrFlag != DENOERR){
        iErrCode = pCom->iConErrFlag;
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CONERRFLAG_ERR,pCom->iConErrFlag,0,aszEyeChach);
#endif
        goto ErrExit;
    }


    tmPathLen = PathChk(path,&iErrCode);
    if (tmPathLen == 0xFFFFFFFF){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,PATH_ERR,tmPathLen,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    if (owner == NULL){
        tmOwnerLen = 0xFFFFFFFF;
    } else {
        tmOwnerLen = strlen(owner);
        if (tmOwnerLen == DGW_OWNER_MIN){
            iErrCode = DEINVAL;
#ifdef DCETRC
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,OTHER_ERR,tmOwnerLen,iErrCode,aszEyeChach);
#endif
            goto ErrExit;
        }
    }

    if (group == NULL){
        tmGroupLen = 0xFFFFFFFF;
    } else {
        tmGroupLen = strlen(group);
        if (tmGroupLen == DGW_OWNER_MIN){
            iErrCode = DEINVAL;
#ifdef DCETRC
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,OTHER_ERR,tmGroupLen,iErrCode,aszEyeChach);
#endif
            goto ErrExit;
        }
    }

    iTotalSendSize = DDA_CHOWN_REQ_L1_SIZE + DDA_CHOWN_REQ_L2_SIZE + DDA_CHOWN_REQ_L3_SIZE + tmPathLen;
    iTotalSendSize = (tmOwnerLen == 0xFFFFFFFF) ? iTotalSendSize : iTotalSendSize + tmOwnerLen;
    iTotalSendSize = (tmGroupLen == 0xFFFFFFFF) ? iTotalSendSize : iTotalSendSize + tmGroupLen;


    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_CHOWN_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    iPtrCtr = 0;
    tmNPathLen = (unsigned int)htonl((unsigned long)tmPathLen);
    tmNOwnerLen = (unsigned int)htonl((unsigned long)tmOwnerLen);
    tmNGroupLen = (unsigned int)htonl((unsigned long)tmGroupLen);


    iCode = AgtSendReq(pCom,(char *)&tmNPathLen,DDA_CHOWN_REQ_L1_SIZE,&iErrCode,DDA_CHOWN_REQ_L1_SIZE);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA2_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iCode = AgtSendReq(pCom,path,tmPathLen,&iErrCode,tmPathLen);
    if (iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA3_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iCode = AgtSendReq(pCom,(char *)&tmNOwnerLen,DDA_CHOWN_REQ_L2_SIZE,&iErrCode,DDA_CHOWN_REQ_L2_SIZE);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA4_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }
    if (tmOwnerLen != 0xFFFFFFFF){
        iCode = AgtSendReq(pCom,owner,tmOwnerLen,&iErrCode,tmOwnerLen);
        if (iCode == -1){
#ifdef DCETRC
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA5_ERR,iCode,iErrCode,aszEyeChach);
#endif
            goto ErrExit;
        }
    }

    iCode = AgtSendReq(pCom,(char *)&tmNGroupLen,DDA_CHOWN_REQ_L3_SIZE,&iErrCode,DDA_CHOWN_REQ_L3_SIZE);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA6_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }
    if (tmGroupLen != 0xFFFFFFFF){
        iCode = AgtSendReq(pCom,group,tmGroupLen,&iErrCode,tmGroupLen);
        if (iCode == -1){
#ifdef DCETRC
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA7_ERR,iCode,iErrCode,aszEyeChach);
#endif
            goto ErrExit;
        }
    }

    bCode = AgtRecvHedReq(pCom,&ushComRsp,&iAgtSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }
    switch(ushComRsp){
        case DDA_CHOWN_COM_RSP :
            break;
        default :
            iErrCode = DERESPONCEDATA;
            pCom->iConErrFlag = iErrCode;
            goto ErrExit;
    }

    bCode = AgtRecvDefData(pCom,&uiAgtRetCode,&uiAgtErrCode,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }
    iAgtSendSize -= DDA_DEFAULT_RECV_SIZE;

    if (iAgtSendSize > 0){
        iCode = DustRecv(pCom,iAgtSendSize,&iErrCode);
        if (iCode == -1){
            goto ErrExit;
        }
    }

    iRetCode = (int)uiAgtRetCode;
    iErrCode = (int)uiAgtErrCode;

ErrExit:
    if (bSemFlag == TRUE){
        SignalLocalSemaphore(pCom->uiCConn_sh);
    }

    *DFSErrno = iErrCode;
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSCHOWN_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSchownReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSstatReq                                                       */
/*  name =  File/directory status retrieval routine                          */
/*  func = Sends DFAM Agent a DFS file/directory status inquire request      */
/*         with using the file path name passed from the caller.             */
/*              Then receives the results and error information              */
/*         from DFAM Agent to send them to the caller.  Does not check the   */
/*         correctness of DFSErrno, and pComTbl.                             */
/*           1. In the case that path=NULL or the path length is 0, this     */
/*              function does not send a request to DFAM Agent, but returns  */
/*              DENOENT as error information.                                */
/*              In the case that the path length exceeds DGW_PATH_MAX(1024), */
/*              this function returns DENAMETOOLONG as error information.    */
/*              In the case that buf is NULL, returns DEFAULT as error       */
/*              information.                                                 */
/*           2. Sets a semaphore.                                            */
/*           3. Sends the header information.                                */
/*           4. Sends the path name length, and path name.                   */
/*           5. Receives the header information.                             */
/*           6. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           7. Receives the access/file mode, file size, update time, user  */
/*              name length of the file owner, and file owner user name.     */
/*           8. Frees the semaphore.                                          */
/*           9. Sets the results to parameters, and returns to the caller.   */
/*                                                                           */
/*         struct dstat {                                                    */
/*             dmode_t d_mode;                                               */
/*             doff_t d_size;                                                */
/*             dtime_t d_mtime;                                              */
/*             char d_uid[DGW_OWNER_MAX+1];                                  */
/*         };                                                                */
/*                                                                           */
/*  i/o  = path    : char *        :inp: DFS full path name                  */
/*         buf     : struct dstat *:out: information setting area            */
/*         DFSErrno: int *         :out: DFS error information area          */
/*         pComTbl : void *        :i/o: thread group table                  */
/*                                                                           */
/*  return =  0 : successful                                                 */
/*           -1 : failure                                                    */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : detected line disconnection                      */
/*         DETIMEOUTAGT   : time out                                         */
/*         DERESPONCEDATA : response data invalid                            */
/*         DEUNLOADTCPIP  : TCPIP.NLM was unloaded                           */
/*         DEDFAMDCESYSTEM: internal error                                   */
/*         DEDFAMTONOVELL : errors other than listed in manuals              */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*  date =  Jan. 10, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFSstatReq(char *path,struct dstat *buf,int *DFSErrno,void *pComTbl)
{
    int             iErrCode;
    int             iRetCode;
    unsigned long   uiAgtRetCode;
    unsigned long   uiAgtErrCode;
    BOOL            bSemFlag;
    unsigned short  ushComRsp;
    int             iAgtSendSize;
    int             iCode;
    BOOL            bCode;
    int             iTotalSendSize;
    int             iPtrCtr;
    unsigned int    tmPathLen;
    unsigned int    tmNPathLen;
    unsigned int    tmSt_uidLen;
    struct ComTGroup_t *pCom;
    char            acRecvBuf[DDA_STAT_RSP_P3_SIZE+DDA_STAT_RSP_P4_SIZE+DDA_STAT_RSP_P5_SIZE+DDA_STAT_RSP_L6_SIZE];


    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSstatReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSSTAT_TRACE_S,pCom->iSockfd,0,aszEyeChach);
#endif

    iRetCode = -1;
    iErrCode = DENOERR;
    bSemFlag = FALSE;

    if (pCom->iConErrFlag != DENOERR){
        iErrCode = pCom->iConErrFlag;
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CONERRFLAG_ERR,pCom->iConErrFlag,0,aszEyeChach);
#endif
        goto ErrExit;
    }


    tmPathLen = PathChk(path,&iErrCode);
    if (tmPathLen == 0xFFFFFFFF){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,PATH_ERR,tmPathLen,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    if (buf == NULL){
        iErrCode = DEFAULT;
        goto ErrExit;
    }

    iTotalSendSize = DDA_STAT_REQ_L1_SIZE + (int)tmPathLen;


    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_STAT_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    iPtrCtr = 0;
    tmNPathLen = (unsigned int)htonl((unsigned long)tmPathLen);


    iCode = AgtSendReq(pCom,(char *)&tmNPathLen,DDA_STAT_REQ_L1_SIZE,&iErrCode,DDA_STAT_REQ_L1_SIZE);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA2_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iCode = AgtSendReq(pCom,path,tmPathLen,&iErrCode,tmPathLen);
    if (iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA3_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    bCode = AgtRecvHedReq(pCom,&ushComRsp,&iAgtSendSize,&iErrCode);
    if (bCode == FALSE){
        goto ErrExit;
    }
    switch(ushComRsp){
        case DDA_STAT_COM_RSP :
            break;
        default :
            iErrCode = DERESPONCEDATA;
            pCom->iConErrFlag = iErrCode;
        goto ErrExit;
    }

    bCode = AgtRecvDefData(pCom,&uiAgtRetCode,&uiAgtErrCode,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }
    iAgtSendSize -= DDA_DEFAULT_RECV_SIZE;

    if(uiAgtRetCode == 0x00000000){
        iCode = AgtRecvReq(pCom,acRecvBuf,sizeof(acRecvBuf),&iErrCode,sizeof(acRecvBuf));
        if (iCode == -1){
#ifdef DCETRC
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,RECV_DATA3_ERR,iCode,iErrCode,aszEyeChach);
#endif
            goto ErrExit;
        }
        iPtrCtr = 0;
        buf->d_mode = ntohs(*(unsigned short *)&acRecvBuf[iPtrCtr]);
        iPtrCtr += DDA_STAT_RSP_P3_SIZE;

        buf->d_size = ntohl(*(unsigned long *)&acRecvBuf[iPtrCtr]);
        iPtrCtr += DDA_STAT_RSP_P4_SIZE;

        buf->d_mtime = ntohl(*(unsigned long *)&acRecvBuf[iPtrCtr]);
        iPtrCtr += DDA_STAT_RSP_P5_SIZE;

        tmSt_uidLen = (unsigned int)ntohl(*(unsigned long *)&acRecvBuf[iPtrCtr]);
        iPtrCtr += DDA_STAT_RSP_L6_SIZE;

        iAgtSendSize -= iPtrCtr;

        if (tmSt_uidLen != 0xFFFFFFFF){
            iCode = AgtRecvReq(pCom,buf->d_uid,tmSt_uidLen,&iErrCode,tmSt_uidLen);
            if (iCode == -1){
#ifdef DCETRC
                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,RECV_DATA4_ERR,iCode,iErrCode,aszEyeChach);
#endif
                goto ErrExit;
            }
            iAgtSendSize -= iCode;
            buf->d_uid[tmSt_uidLen] = '\0';
        } else {
            buf->d_uid[0] = '\0';
        }
    }
    if (iAgtSendSize > 0){
        iCode = DustRecv(pCom,iAgtSendSize,&iErrCode);
        if (iCode == -1){
            goto ErrExit;
        }
    }

    iRetCode = (int)uiAgtRetCode;
    iErrCode = (int)uiAgtErrCode;

ErrExit:
    if (bSemFlag == TRUE){
        SignalLocalSemaphore(pCom->uiCConn_sh);
    }

    *DFSErrno = iErrCode;
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSSTAT_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSstatReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSlseekReq                                                      */
/*  name =  DFS file descriptor move and file size change routine            */
/*  func = Sends DFAM Agent a file descriptor move and file size change      */
/*         request with using the file descriptor, lseek location from the   */
/*         file top, and file size after change passed from the caller.      */
/*              Then receives the results and error information              */
/*         from DFAM Agent to send them to the caller.  Does not check the   */
/*         correctness of parameters.                                        */
/*           1. Sets a semaphore.                                            */
/*           2. Sends the header information.                                */
/*           3. Sends the file descriptor, location from the top (byte), and */
/*              file size after the change.                                  */
/*           4. Receives the header information.                             */
/*           5. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           6. Frees the semaphore.                                         */
/*           7. Sets the results to parameters, and returns to the caller.   */
/*                                                                           */
/*         *1: No lssekp is made if lseek is -1.                             */
/*         *2: No size change is made if length is 0xFFFFFFFF.               */
/*          The above processings are actually done in DFAM Agent (the       */
/*          communication control of Gateway does not do anything).          */
/*                                                                           */
/*  i/o  = fildes  : int                  :inp: file descriptor              */
/*         lseekp  : doff_t(long)         :inp: lseek location from the top  */
/*                                              of the file                  */
/*         length  : dsize_t(unsigned int):inp: file size (after)            */
/*         DFSErrno: int *                :out: DFS error information area   */
/*         pComTbl : void *               :i/o: thread group table           */
/*                                                                           */
/*  return =  0 : successful                                                 */
/*           -1 : failure                                                    */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : detected line disconnection                      */
/*         DETIMEOUTAGT   : time out                                         */
/*         DERESPONCEDATA : response data invalid                            */
/*         DEUNLOADTCPIP  : TCPIP.NLM was unloaded                           */
/*         DEDFAMDCESYSTEM: internal error                                   */
/*         DEDFAMTONOVELL : errors other than listed in manuals              */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*  date =  Apr. 13, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFSlseekReq(int fildes,doff_t lseekp,dsize_t length,int *DFSErrno,void *pComTbl)
{
    int             iErrCode;
    int             iRetCode;
    unsigned long   uiAgtRetCode;
    unsigned long   uiAgtErrCode;
    BOOL            bSemFlag;
    unsigned short  ushComRsp;
    int             iAgtSendSize;
    int             iCode;
    BOOL            bCode;
    int             iTotalSendSize;
    int             iCtr;
    struct ComTGroup_t *pCom;
    char            acSendBuf[DDA_LSEEK_REQ_SIZE];


    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSlseekReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSLSEEK_TRACE_S,pCom->iSockfd,0,aszEyeChach);
#endif
    if (pCom->iConErrFlag != DENOERR){
        iErrCode = pCom->iConErrFlag;
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CONERRFLAG_ERR,pCom->iConErrFlag,0,aszEyeChach);
#endif
        goto ErrExit;
    }


    iRetCode = -1;
    iErrCode = DENOERR;
    bSemFlag = FALSE;

    iTotalSendSize = DDA_LSEEK_REQ_SIZE;

    iCtr = 0;
    *(unsigned long *)&acSendBuf[iCtr] = htonl(fildes);
    iCtr += DDA_LSEEK_REQ_P1_SIZE;

    *(unsigned long *)&acSendBuf[iCtr] = htonl(lseekp);
    iCtr += DDA_LSEEK_REQ_P2_SIZE;

    *(unsigned long *)&acSendBuf[iCtr] = htonl(length);
    iCtr += DDA_LSEEK_REQ_P3_SIZE;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_LSEEK_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    iCode = AgtSendReq(pCom,acSendBuf,DDA_LSEEK_REQ_SIZE,&iErrCode,DDA_LSEEK_REQ_SIZE);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA2_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    bCode = AgtRecvHedReq(pCom,&ushComRsp,&iAgtSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }
    switch(ushComRsp){
        case DDA_LSEEK_COM_RSP :
            break;
        default :
            iErrCode = DERESPONCEDATA;
            pCom->iConErrFlag = iErrCode;
        goto ErrExit;
    }

    bCode = AgtRecvDefData(pCom,&uiAgtRetCode,&uiAgtErrCode,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }
    iAgtSendSize -= DDA_DEFAULT_RECV_SIZE;

    if (iAgtSendSize > 0){
        iCode = DustRecv(pCom,iAgtSendSize,&iErrCode);
        if (iCode == -1){
            goto ErrExit;
        }
    }

    iRetCode = (int)uiAgtRetCode;
    iErrCode = (int)uiAgtErrCode;

ErrExit:
    if (bSemFlag == TRUE){
        SignalLocalSemaphore(pCom->uiCConn_sh);
    }

    *DFSErrno = iErrCode;
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSLSEEK_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSlseekReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  PathChk                                                          */
/*  name =  Path check routine                                               */
/*  func =  This function is invoked by the file-operating functions to      */
/*          check a path.                                                    */
/*                                                                           */
/*  i/o  = pszPath  : const char *:inp: target path name to be checked       */
/*         piErrCode: int *       :inp: error information                    */
/*                                                                           */
/*  return = 0xFFFFFFFF: failure (error in pszPath)                          */
/*           others    : successful (the length of pszPath)                  */
/*  common = none                                                            */
/*                                                                           */
/*  note = error information:                                                */
/*        1. In the case that pszPath is NULL, or the pszPath length is 0,   */
/*           this function returns 0xFFFFFFFF, and sets DENOENT to piErrCode.*/
/*        2. In the case that the pszPath length exceeds DGW_PATH_MAX(1024), */
/*           this function returns -1, and sets DENAMETOOLONG to piErrCode.  */
/*  date =  Jan. 10, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
unsigned int PathChk(const char *pszPath,int *piErrCode)
{
    unsigned int tmPathLen;
    int iRetCode;



    iRetCode = 0xFFFFFFFF;
    if (pszPath == NULL){
        *piErrCode = DENOENT;
        goto ErrExit;
    }
    tmPathLen = strlen(pszPath);
    if (tmPathLen == DGW_PATH_MIN){
        *piErrCode = DENOENT;
        goto ErrExit;
    }
    if(tmPathLen > DGW_PATH_MAX){
        *piErrCode = DENAMETOOLONG;
        goto ErrExit;
    }
    iRetCode = tmPathLen;
ErrExit:
    return(iRetCode);
}
