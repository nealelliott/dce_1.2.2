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
 * $Log: dcomfil1.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:12  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:42  marty]
 *
 * Revision 1.1.2.1  1996/02/21  06:41:43  takano
 * 	First Submit
 * 	[1996/02/21  06:41:32  takano]
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
/*  ID   =  DFSopenReq                                                       */
/*  name =  DFS file open routine                                            */
/*  func = Sends DFAM Agent a DFS file open request with using the path      */
/*         name, opening type and sharing mode of the target file passed     */
/*         from the caller.  Then receives the results and error information */
/*         from DFAM Agent to send them to the caller.  Does not check the   */
/*         correctness of oflag, smode, DFSErrno, and pComTbl.               */
/*         error may occur in the caller if iIndataLen=0)                    */
/*           1. In the case that path=NULL or the path length is 0, this     */
/*              function does not send data to DFAM Agent, but returns       */
/*              DENOENT as error information.                                */
/*              In the case that the path length exceeds DGW_PATH_MAX(1024), */
/*              this function returns DENAMETOOLONG as error information.    */
/*           2. Sets a semaphore.                                            */
/*           3. Sends the header information.                                */
/*           4. Sends the opening type, sharing mode, path name length and   */
/*              path name.                                                   */
/*           5. Receives the header information.                             */
/*           6. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           7. Frees the semaphore.                                         */
/*           8. Sets the results to parameters, and returns to the caller.   */
/*                                                                           */
/*         opening type:DO_RDONLY:DO_WRONLY:DO_RDWR                          */
/*         sharing mode:DS_NOMODE:DS_DENYRD:DS_DENYWR:DS_COMPATI             */
/*                                                                           */
/*  i/o  = path       : char *:inp: DFS path name                            */
/*         oflag      : int   :inp: open type                                */
/*         smode      : long  :inp: sharing mode                             */
/*         DFSErrno   : int * :out: error information area                   */
/*         pComTbl    : void *:i/o: thread group table                       */
/*                                                                           */
/*  return = -1           : failure                                          */
/*           other than -1: the file descriptor of the opened file           */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : detected line disconnection                      */
/*         DETIMEOUTAGT   : time out                                         */
/*                          ----> frees connection.                          */
/*         DERESPONCEDATA : response data invalid                            */
/*         DEUNLOADTCPIP  : TCPIP.NLM was unloaded                           */
/*                          ----> takes an exit.                             */
/*         DEDFAMDCESYSTEM: internal error                                   */
/*                          ----> frees connection.                          */
/*         DEDFAMTONOVELL : errors other than listed in manuals              */
/*                          ----> frees connection.                          */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*                                                                           */
/*  date =  Jan. 10, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFSopenReq(char *path,int oflag,long smode,int *DFSErrno,void *pComTbl)
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
    struct ComTGroup_t *pCom;
    char            acSendBuf[DDA_OPEN_REQ_P1_SIZE+DDA_OPEN_REQ_P2_SIZE+DDA_OPEN_REQ_L3_SIZE];


    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSopenReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSOPEN_TRACE_S,pCom->iSockfd,0,aszEyeChach);
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

    iTotalSendSize = DDA_OPEN_REQ_P1_SIZE + DDA_OPEN_REQ_P2_SIZE + DDA_OPEN_REQ_L3_SIZE + (int)tmPathLen;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_OPEN_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    iPtrCtr = 0;
    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)oflag);
    iPtrCtr += DDA_OPEN_REQ_P1_SIZE;

    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl(smode);
    iPtrCtr += DDA_OPEN_REQ_P2_SIZE;

    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)tmPathLen);
    iPtrCtr += DDA_OPEN_REQ_L3_SIZE;

    iCode = AgtSendReq(pCom,acSendBuf,iPtrCtr,&iErrCode,iPtrCtr);
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
        case DDA_OPEN_COM_RSP :
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
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSOPEN_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSopenReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFScloseReq                                                      */
/*  name =  DFS file close routine                                           */
/*  func = Sends DFAM Agent a DFS file close request with using the file     */
/*         descriptor passed from the caller.  Then receives  the results    */
/*         and error information from DFAM Agent to send them to a caller.   */
/*         (Does not check the contents of the parameters)                   */
/*           1. Sets a semaphore.                                            */
/*           2. Sends the header information.                                */
/*           3. Sends the file descriptor.                                   */
/*           4. Receives the header information.                             */
/*           5. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           6. Frees the semaphore.                                         */
/*           7. Sets the results to parameters, and returns to the caller.   */
/*                                                                           */
/*                                                                           */
/*  i/o  = fildes     : int   :inp: file descriptor                          */
/*         DFSErrno   : int * :out: error information area                   */
/*         pComTbl    : void *:i/o: thread group table                       */
/*  return =  0: successful                                                  */
/*           -1: failed                                                      */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : detected line disconnection                      */
/*         DETIMEOUTAGT   : time out                                         */
/*                          ----> frees connection.                          */
/*         DERESPONCEDATA : response data invalid                            */
/*         DEUNLOADTCPIP  : TCPIP.NLM was unloaded                           */
/*                          ----> takes an exit.                             */
/*         DEDFAMDCESYSTEM: internal error                                   */
/*                          ----> frees connection.                          */
/*         DEDFAMTONOVELL : errors other than listed in manuals              */
/*                          ----> frees connection.                          */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*  date =  Jan. 10, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFScloseReq(int fildes,int *DFSErrno,void *pComTbl)
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
    GW_tsh_functrace(pszDFScloseReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSCLOSE_TRACE_S,pCom->iSockfd,0,aszEyeChach);
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

    iTotalSendSize = DDA_CLOSE_REQ_SIZE;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_CLOSE_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    fildes = (int)htonl((unsigned long)fildes);

    iCode = AgtSendReq(pCom,(char *)&fildes,DDA_CLOSE_REQ_P1_SIZE,&iErrCode,DDA_CLOSE_REQ_P1_SIZE);
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
        case DDA_CLOSE_COM_RSP :
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
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSCLOSE_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFScloseReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFScreatReq                                                      */
/*  name =  DFS file newly building routine                                  */
/*  func = Sends DFAM Agent a DFS file newly-building request with using the */
/*         new path name and access mode of the new target file passed       */
/*         from the caller.  Then receives the results and error information */
/*         from DFAM Agent to send them to the caller.  Does not check the   */
/*         correctness of mode, DFSErrno, and pComTbl.                       */
/*           1. In the case that path=NULL or the path length is 0, this     */
/*              function does not send data to DFAM Agent, but returns       */
/*              DENOENT as error information.                                */
/*              In the case that the path length exceeds DGW_PATH_MAX(1024), */
/*              this function returns DENAMETOOLONG as error information.    */
/*           2. Sets a semaphore.                                            */
/*           3. Sends the header information.                                */
/*           4. Sends the access mode, path name length and path name.       */
/*           5. Receives the header information.                             */
/*           6. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           7. Frees the semaphore.                                         */
/*           8. Sets the results to parameters, and returns to the caller.   */
/*                                                                           */
/*         access mode:DS_IRUSR :DS_IWUSR :DS_IXUSR :                        */
/*                     DS_IRGRP :DS_IWGRP :DS_IXGRP :                        */
/*                     DS_IROTH :DS_IWOTH :DS_IXOTH                          */
/*                                                                           */
/*  i/o  = path       : char * :inp: DFS file name to be built               */
/*         mode       : dmode_t:inp: access mode                             */
/*                   (unsigned  short)                                       */
/*         DFSErrno   : int *  :out: error information area                  */
/*         pComTbl    : void * :i/o: thread group table                      */
/*                                                                           */
/*  return = -1           : failure                                          */
/*           other than -1: the file descriptor of the opened file           */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : detected line disconnection                      */
/*         DETIMEOUTAGT   : time out                                         */
/*                          ----> frees connection.                          */
/*         DERESPONCEDATA : response data invalid                            */
/*         DEUNLOADTCPIP  : TCPIP.NLM was unloaded                           */
/*                          ----> takes an exit.                             */
/*         DEDFAMDCESYSTEM: internal error                                   */
/*                          ----> frees connection.                          */
/*         DEDFAMTONOVELL : errors other than listed in manuals              */
/*                          ----> frees connection.                          */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*  date =  Jan. 10, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFScreatReq(char *path,dmode_t mode,int *DFSErrno,void *pComTbl)
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
    struct ComTGroup_t *pCom;
    char            acSendBuf[DDA_CREAT_REQ_P1_SIZE+DDA_CREAT_REQ_L2_SIZE];


    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFScreatReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSCREAT_TRACE_S,pCom->iSockfd,0,aszEyeChach);
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

    iTotalSendSize = DDA_CREAT_REQ_P1_SIZE + DDA_CREAT_REQ_L2_SIZE + (int)tmPathLen;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_CREAT_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    iPtrCtr = 0;
    *(unsigned short *)&acSendBuf[iPtrCtr] = htons(mode);
    iPtrCtr += DDA_CREAT_REQ_P1_SIZE;

    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)tmPathLen);
    iPtrCtr += DDA_CREAT_REQ_L2_SIZE;

    iCode = AgtSendReq(pCom,acSendBuf,iPtrCtr,&iErrCode,iPtrCtr);
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
        case DDA_CREAT_COM_RSP :
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
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSCREAT_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFScreatReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSremoveReq                                                     */
/*  name =  DFS file deletion routine                                        */
/*  func = Sends DFAM Agent a DFS file deletion request with using the       */
/*         to-be-deleted file path name passed from the caller.  Then        */
/*         receives the results and error information from DFAM Agent to     */
/*         send them to the caller.  Does not check the correctness of       */
/*         DFSErrno, and pComTbl.                                            */
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
/*  i/o  = path       : char * :inp: DFS file name to be built               */
/*         DFSErrno   : int *  :out: error information area                  */
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
/*                          ----> frees connection.                          */
/*         DERESPONCEDATA : response data invalid                            */
/*         DEUNLOADTCPIP  : TCPIP.NLM was unloaded                           */
/*                          ----> takes an exit.                             */
/*         DEDFAMDCESYSTEM: internal error                                   */
/*                          ----> frees connection.                          */
/*         DEDFAMTONOVELL : errors other than listed in manuals              */
/*                          ----> frees connection.                          */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*  date =  Jan. 09, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFSremoveReq(char *path,int *DFSErrno,void *pComTbl)
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
    GW_tsh_functrace(pszDFSremoveReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSREMOVE_TRACE_S,pCom->iSockfd,0,aszEyeChach);
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

    iTotalSendSize = DDA_REMOVE_REQ_L1_SIZE + (int)tmPathLen;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_REMOVE_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    tmNPathLen = (unsigned int)htonl((unsigned long)tmPathLen);

    iCode = AgtSendReq(pCom,(char *)&tmNPathLen,DDA_REMOVE_REQ_L1_SIZE,&iErrCode,DDA_REMOVE_REQ_L1_SIZE);
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
        case DDA_REMOVE_COM_RSP :
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
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSREMOVE_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSremoveReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSrenameReq                                                     */
/*  name =  DFS file/directory rename/move routine                           */
/*  func = Sends DFAM Agent a DFS file/directory rename/move request with    */
/*         using the file path names (before and after) passed from the      */
/*         caller.  Then receives the results and error information from     */
/*         DFAM Agent to send them to the caller.  Does not check the        */
/*         correctness of DFSErrno and pComTbl.                              */
/*           1. In the case that source=NULL or the length of source is 0,   */
/*              this function does not send data to DFAM Agent, but returns  */
/*              DENOENT as error information.                                */
/*              In the case that the length of source exceeds DGW_PATH_MAX   */
/*              (1024), this function returns DENAMETOOLONG as error         */
/*              information.                                                 */
/*           2. In the case that target=NULL or the length of target is 0,   */
/*              this function does not send data to DFAM Agent, but returns  */
/*              DENOENT as error information.                                */
/*              In the case that the length of target exceeds DGW_PATH_MAX   */
/*              (1024), this function returns DENAMETOOLONG as error         */
/*              information.                                                 */
/*           3. Sets a semaphore.                                            */
/*           4. Sends the header information.                                */
/*           5. Sends the path name lengths and path names of source and     */
/*              target.                                                      */
/*           6. Receives the header information.                             */
/*           7. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           8. Frees the semaphore.                                          */
/*           9. Sets the results to parameters, and returns to the caller.   */
/*                                                                           */
/*  i/o  = source     : char * :inp: DFS file name (before)                  */
/*         target     : char * :inp: DFS file name (after)                   */
/*         DFSErrno   : int *  :out: error information area                  */
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
/*                          ----> frees connection.                          */
/*         DERESPONCEDATA : response data invalid                            */
/*         DEUNLOADTCPIP  : TCPIP.NLM was unloaded                           */
/*                          ----> takes an exit.                             */
/*         DEDFAMDCESYSTEM: internal error                                   */
/*                          ----> frees connection.                          */
/*         DEDFAMTONOVELL : errors other than listed in manuals              */
/*                          ----> frees connection.                          */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*  date =  Jan. 10, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFSrenameReq(char *source,char *target,int *DFSErrno,void *pComTbl)
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
    unsigned int    tmSourceLen;
    unsigned int    tmTargetLen;
    unsigned int    tmNSourceLen;
    unsigned int    tmNTargetLen;
    struct ComTGroup_t *pCom;

    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSrenameReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSRENAME_TRACE_S,pCom->iSockfd,0,aszEyeChach);
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


    tmSourceLen = PathChk(source,&iErrCode);
    if (tmSourceLen == 0xFFFFFFFF){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,PATH_ERR,tmSourceLen,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    tmTargetLen = PathChk(target,&iErrCode);
    if (tmTargetLen == 0xFFFFFFFF){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,PATH_ERR,tmTargetLen,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iTotalSendSize = DDA_RENAME_REQ_L1_SIZE + (int)tmSourceLen + DDA_RENAME_REQ_L2_SIZE + (int)tmTargetLen;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_RENAME_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    tmNSourceLen = (unsigned int)htonl((unsigned long)tmSourceLen);

    iCode = AgtSendReq(pCom,(char *)&tmNSourceLen,DDA_RENAME_REQ_L1_SIZE,&iErrCode,DDA_RENAME_REQ_L1_SIZE);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA2_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iCode = AgtSendReq(pCom,source,tmSourceLen,&iErrCode,tmSourceLen);
    if (iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA3_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    tmNTargetLen = (unsigned int)htonl((unsigned long)tmTargetLen);

    iCode = AgtSendReq(pCom,(char *)&tmNTargetLen,DDA_RENAME_REQ_L2_SIZE,&iErrCode,DDA_RENAME_REQ_L2_SIZE);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA4_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iCode = AgtSendReq(pCom,target,tmTargetLen,&iErrCode,tmTargetLen);
    if (iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA4_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    bCode = AgtRecvHedReq(pCom,&ushComRsp,&iAgtSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }
    switch(ushComRsp){
        case DDA_RENAME_COM_RSP :
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
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSRENAME_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSrenameReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSreadReq                                                       */
/*  name =  DFS file read routine                                            */
/*  func = Sends DFAM Agent a DFS file read request with using the file      */
/*         descriptor passed from the caller.  Then receives the results     */
/*         and error information from DFAM Agent to send them to the caller. */
/*         Does not check the correctness of files, DFSErrno and pComTbl.    */
/*           1. In the case that buf=NULL and the length of nbyte exceeds 0, */
/*              this function does not send data to DFAM Agent, but sets     */
/*              error information to DEFAULT. (Since this function is used   */
/*              for copying, nbyte never be 0.  Thus any non-zero value is   */
/*              assumed as a correct value).                                 */
/*           2. Sets a semaphore.                                            */
/*           3. Sends the header information.                                */
/*           4. Sends the length of the data to be read.                     */
/*           5. Receives the header information.                             */
/*           6. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           7. In the case that the return value is not 0x00000000 or       */
/*              0xFFFFFFFF, this function receives the binary data.          */
/*                                                                           */
/*         typedef int dssize_t;                                             */
/*         typedef unsigned int dsize_t;                                     */
/*                                                                           */
/*  i/o  = fildes     : int        :inp: file descriptor                     */
/*         buf        : void *     :out: buffer to set data                  */
/*         nbyte      : dsize_t    :inp: to-be-read data length              */
/*                   (unsigned int)                                          */
/*         DFSErrno   : int *      :out: error information area              */
/*         pComTbl    : void *     :i/o: thread group table                  */
/*                                                                           */
/*  return =  0    : end of file                                             */
/*           -1    : failure                                                 */
/*           others: the length of the inputted data                         */
/*                                                                           */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : detected line disconnection                      */
/*         DETIMEOUTAGT   : time out                                         */
/*                          ----> frees connection.                          */
/*         DERESPONCEDATA : response data invalid                            */
/*         DEUNLOADTCPIP  : TCPIP.NLM was unloaded                           */
/*                          ----> takes an exit.                             */
/*         DEDFAMDCESYSTEM: internal error                                   */
/*                          ----> frees connection.                          */
/*         DEDFAMTONOVELL : errors other than listed in manuals              */
/*                          ----> frees connection.                          */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*  date =  Jan. 09, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
dssize_t DFSreadReq(int fildes,void *buf,dsize_t nbyte,int *DFSErrno,void *pComTbl)
{
    int             iErrCode;
    int             iRetCode;
    unsigned long   uiAgtRetCode;
    unsigned long   uiAgtErrCode;
    BOOL            bSemFlag;
    unsigned short  ushComRsp;
    int             iAgtSendSize;
    int             iPtrCtr;
    int             iCode;
    BOOL            bCode;
    int             iTotalSendSize;
    struct ComTGroup_t *pCom;
    char            acSendBuf[DDA_READ_REQ_SIZE];


    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSreadReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSREAD_TRACE_S,pCom->iSockfd,0,aszEyeChach);
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


    if (buf == NULL && nbyte > 0){
        iErrCode = DEFAULT;
        goto ErrExit;
    }

    iTotalSendSize = DDA_READ_REQ_SIZE;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_READ_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    iPtrCtr = 0;
    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)fildes);
    iPtrCtr += DDA_READ_REQ_P1_SIZE;

    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)nbyte);
    iPtrCtr += DDA_READ_REQ_P2_SIZE;

    iCode = AgtSendReq(pCom,acSendBuf,DDA_READ_REQ_SIZE,&iErrCode,DDA_READ_REQ_SIZE);
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
        case DDA_READ_COM_RSP :
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

    if ((int)uiAgtRetCode > 0){
#ifndef A
        iCode = AgtRecvReq(pCom,buf,(int)uiAgtRetCode,&iErrCode,-1);
#else
        iCode = AgtRecvReq(pCom,buf,(int)uiAgtRetCode,&iErrCode,(int)uiAgtRetCode);
#endif
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
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSREAD_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSreadReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSwriteReq                                                      */
/*  name =  DFS file write routine                                           */
/*  func = Sends DFAM Agent a DFS file write request with using the file     */
/*         descriptor, length of to-be-written data, and binary data passed  */
/*         from the caller.  Then receives the results and error information */
/*         from DFAM Agent to send them to the caller.                       */
/*         Does not check the correctness of files, DFSErrno and pComTbl.    */
/*           1. In the case that buf=NULL and the length of nbyte exceeds 0, */
/*              this function does not send data to DFAM Agent, but sets     */
/*              error information to DEFAULT.                                */
/*              *1: The NULL Character is used in the UNIX worlds, this does */
/*                  not happen in the copyback processing (= error).         */
/*              *2: If buf is not NULL, the content of nbyte is not checked  */
/*                  (since the file of 0 byte is not an error).              */
/*           2. Sets a semaphore.                                            */
/*           3. Sends the header information.                                */
/*           4. Sends the file descriptor, length of the data to be written, */
/*              and binary data.                                             */
/*           5. Receives the header information.                             */
/*           6. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           7. Frees the semaphore.                                         */
/*           8. Sets the results to parameters, and returns to the caller.   */
/*                                                                           */
/*         typedef int dssize_t;                                             */
/*         typedef unsigned int dsize_t;                                     */
/*                                                                           */
/*  i/o  = fildes     : int        :inp: file descriptor                     */
/*         buf        : void *     :out: buffer to set data                  */
/*         nbyte      : dsize_t    :inp: to-be-written data length           */
/*                   (unsigned int)                                          */
/*         DFSErrno   : int *      :out: error information area              */
/*         pComTbl    : void *     :i/o: thread group table                  */
/*                                                                           */
/*  return = -1    : failure                                                 */
/*           others: the length of the outputted data                        */
/*                                                                           */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : detected line disconnection                      */
/*         DETIMEOUTAGT   : time out                                         */
/*                          ----> frees connection.                          */
/*         DERESPONCEDATA : response data invalid                            */
/*         DEUNLOADTCPIP  : TCPIP.NLM was unloaded                           */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*  date =  Jan. 10, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
dssize_t DFSwriteReq(int fildes,void *buf,dsize_t nbyte,int *DFSErrno,void *pComTbl)
{
    int             iErrCode;
    int             iRetCode;
    unsigned long   uiAgtRetCode;
    unsigned long   uiAgtErrCode;
    BOOL            bSemFlag;
    unsigned short  ushComRsp;
    int             iAgtSendSize;
    int             iTotalSendSize;
    int             iPtrCtr;
    int             iCode;
    BOOL            bCode;
    struct ComTGroup_t *pCom;
    char            acSendBuf[DDA_WRITE_REQ_P1_SIZE+DDA_WRITE_REQ_P2_SIZE];


    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSwriteReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSWRITE_TRACE_S,pCom->iSockfd,0,aszEyeChach);
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


    if (buf == NULL && nbyte > 0){
        iErrCode = DEFAULT;
        goto ErrExit;
    }

    iTotalSendSize = DDA_WRITE_REQ_P1_SIZE + DDA_WRITE_REQ_P2_SIZE + nbyte;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_WRITE_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    iPtrCtr = 0;
    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)fildes);
    iPtrCtr += DDA_WRITE_REQ_P1_SIZE;

    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)nbyte);
    iPtrCtr += DDA_WRITE_REQ_P2_SIZE;

    iCode = AgtSendReq(pCom,acSendBuf,DDA_WRITE_REQ_P1_SIZE+DDA_WRITE_REQ_P2_SIZE,&iErrCode,DDA_WRITE_REQ_P1_SIZE+DDA_WRITE_REQ_P2_SIZE);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA2_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }


    if (nbyte > 0){
#ifndef A
        iCode = AgtSendReq(pCom,buf,nbyte,&iErrCode,-1);
#else
        iCode = AgtSendReq(pCom,buf,nbyte,&iErrCode,nbyte);
#endif
        if(iCode == -1){
#ifdef DCETRC
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA3_ERR,iCode,iErrCode,aszEyeChach);
#endif
            goto ErrExit;
        }
    }

    bCode = AgtRecvHedReq(pCom,&ushComRsp,&iAgtSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }
    switch(ushComRsp){
        case DDA_WRITE_COM_RSP :
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
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSWRITE_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSwriteReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSmkdirReq                                                      */
/*  name =  DFS directory make routine                                       */
/*  func = Sends DFAM Agent a DFS directory make request with using the      */
/*         to-be-made directory path name and permission bits passed from    */
/*         the caller.  Then receives the results and error information      */
/*         from DFAM Agent to send them to the caller.  Does not check the   */
/*         correctness of mode, DFSErrno, and pComTbl.                       */
/*           1. In the case that path=NULL or the path length is 0, this     */
/*              function does not send data to DFAM Agent, but returns       */
/*              DENOENT as error information.                                */
/*              In the case that the path length exceeds DGW_PATH_MAX(1024), */
/*              this function returns DENAMETOOLONG as error information.    */
/*           2. Sets a semaphore.                                            */
/*           3. Sends the header information.                                */
/*           4. Sends the permission bits, path name length and path name.   */
/*           5. Receives the header information.                             */
/*           6. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           7. Frees the semaphore.                                         */
/*           8. Sets the results to parameters, and returns to the caller.   */
/*                                                                           */
/*         permission bits:DS_IRUSR :DS_IWUSR :DS_IXUSR :                    */
/*                         DS_IRGRP :DS_IWGRP :DS_IXGRP :                    */
/*                         DS_IROTH :DS_IWOTH :DS_IXOTH                      */
/*                                                                           */
/*  i/o  = path       : char * :inp: DFS path file                           */
/*         mode       : dmode_t:inp: permission bits                         */
/*                     (unsigned short)                                      */
/*         DFSErrno   : int *  :out: error information area                  */
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
int DFSmkdirReq(char *path,dmode_t mode,int *DFSErrno,void *pComTbl)
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
    struct ComTGroup_t *pCom;
    char            acSendBuf[DDA_MKDIR_REQ_P1_SIZE+DDA_MKDIR_REQ_L2_SIZE];


    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSmkdirReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSMKDIR_TRACE_S,pCom->iSockfd,0,aszEyeChach);
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

    iTotalSendSize = DDA_MKDIR_REQ_P1_SIZE + DDA_MKDIR_REQ_L2_SIZE + (int)tmPathLen;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_MKDIR_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    iPtrCtr = 0;
    *(unsigned short *)&acSendBuf[iPtrCtr] = htons(mode);
    iPtrCtr += DDA_MKDIR_REQ_P1_SIZE;

    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)tmPathLen);
    iPtrCtr += DDA_MKDIR_REQ_L2_SIZE;

    iCode = AgtSendReq(pCom,acSendBuf,iPtrCtr,&iErrCode,iPtrCtr);
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
        case DDA_MKDIR_COM_RSP :
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
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSMKDIR_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSmkdirReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

