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
 * $Log: dcomlin.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:17  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:46  marty]
 *
 * Revision 1.1.2.1  1996/02/21  06:44:02  takano
 * 	First Submit
 * 	[1996/02/21  06:43:56  takano]
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

#define LINBUFSIZ   1034

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSLinGetIdReq                                                   */
/*  name =  DCE login ID getting routine                                     */
/*  func = Sends DFAM Agent a get-ID request for DCE login.  Then receives   */
/*         the results and error information from DFAM Agent to send them    */
/*         to a caller.  Since no parameter check is made, an internal error */
/*         may occur in the caller if iIndataLen=0)                          */
/*           1. Sets a semaphore.                                            */
/*           2. Sends the header information.                                */
/*           3. Sends the data length (iIndataLen) and data (pcIndata).      */
/*           4. Receives the header information.                             */
/*           5. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           6. Receives the data length and data (pcOutData).               */
/*           7. Frees the semaphore.                                         */
/*           8. Sets the results to parameters, and returns to the caller.   */
/*                                                                           */
/*  i/o  = pcIndata   : char *:inp: sending data                             */
/*         iIndataLen : long  :inp: sending data size                        */
/*         pcOutdata  : char *:out: sending data storing area                */
/*         DFSErrno   : int * :out: error information area                   */
/*         pComTbl    : void *:i/o: thread group table                       */
/*                                                                           */
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
/*  date =  Jan. 09, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
long DFSLinGetIdReq(char *pcIndata,long iIndataLen,char *pcOutdata,int *DFSErrno,void *pComTbl)
{
    int             iErrCode;
    long            iRetCode;
    unsigned long   uiAgtRetCode;
    unsigned long   uiAgtErrCode;
    BOOL            bSemFlag;
    unsigned short  ushComRsp;
    int             iAgtSendSize;
    int             iCode;
    BOOL            bCode;
    int             iTotalSendSize;
    long            iTmpDataLen;
    struct ComTGroup_t *pCom;
    int             iCtr;
    char            acSendBuf[DDA_HED_LEN+DDA_LINGETID_REQ_P1_SIZE+LINBUFSIZ];


    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSLinGetIdReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSLINGETID_TRACE_S,pCom->iSockfd,0,aszEyeChach);
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

    if (iIndataLen > LINBUFSIZ){
        iErrCode = DEDFAMDCESYSTEM;
        pCom->iConErrFlag = iErrCode;
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,OTHER_ERR,pCom->iConErrFlag,0,aszEyeChach);
#endif
        goto ErrExit;
    }
    iTotalSendSize = DDA_LINGETID_REQ_P1_SIZE + (int)iIndataLen;

    iCtr = 0;
    *(unsigned short *)&acSendBuf[iCtr] = htons(DDA_LINGETID_COM_REQ);
    iCtr += DDA_HED_COM_LEN;

    *(unsigned short *)&acSendBuf[iCtr] = htons(DDA_RESERVE);
    iCtr += DDA_HED_RESERVE_LEN;

    *(unsigned long *)&acSendBuf[iCtr] = htonl((unsigned long)iTotalSendSize);
    iCtr += DDA_HED_DATASIZE_LEN;

    *(unsigned long *)&acSendBuf[iCtr] = htonl(iIndataLen);
    iCtr += DDA_LINGETID_REQ_P1_SIZE;

    memcpy(&acSendBuf[iCtr],pcIndata,iIndataLen);
    iCtr += iIndataLen;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_HEAD_INF,
                (int)DDA_LINGETID_COM_REQ << 16 | (int)DDA_RESERVE,iTotalSendSize,aszEyeChach);
#endif
    iCode = AgtSendReq(pCom,acSendBuf,iCtr,&iErrCode,iCtr);
    if (iCode == -1){
        iRetCode = -1;
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size, SEND_DATA1_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    bCode = AgtRecvHedReq(pCom,&ushComRsp,&iAgtSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    switch(ushComRsp){
        case DDA_LINGETID_COM_RSP :
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
        iCode = AgtRecvReq(pCom,(char *)&iTmpDataLen,DDA_LINGETID_RSP_P3_SIZE,&iErrCode,DDA_LINGETID_RSP_P3_SIZE);
        if (iCode == -1){
#ifdef DCETRC
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,RECV_DATA3_ERR,iCode,iErrCode,aszEyeChach);
#endif
            goto ErrExit;
        }
        iAgtSendSize -= iCode;
        iTmpDataLen = ntohl(iTmpDataLen);

        iCode = AgtRecvReq(pCom,pcOutdata,iTmpDataLen,&iErrCode,iTmpDataLen);
        if (iCode == -1){
#ifdef DCETRC
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,RECV_DATA4_ERR,iCode,iErrCode,aszEyeChach);
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

    iRetCode = uiAgtRetCode;
    iErrCode = (int)uiAgtErrCode;

ErrExit:
    if (bSemFlag == TRUE){
        SignalLocalSemaphore(pCom->uiCConn_sh);
    }

    *DFSErrno = iErrCode;
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSLINGETID_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSLinGetIdReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSLinGetPasswdReq                                               */
/*  name =  DCE login password getting routine                               */
/*  func = Sends DFAM Agent a get-password request for DCE login.  Then      */
/*         receives  the results and error information from DFAM Agent to    */
/*         send them to a caller.  Since no parameter check is made, an      */
/*         internal error may occur in the caller if iIndataLen=0)           */
/*           1. Sets a semaphore.                                            */
/*           2. Sends the header information.                                */
/*           3. Sends the data length (iDataLen) and data (pcData).          */
/*           4. Receives the header information.                             */
/*           5. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           6. Frees the semaphore.                                         */
/*           7. Sets the results to parameters, and returns to the caller.   */
/*                                                                           */
/*  i/o  = pcData     : char *:inp: data                                     */
/*         iDataLen   : long  :inp: data length                              */
/*         DFSErrno   : int * :out: error information area                   */
/*         pComTbl    : void *:i/o: thread group table                       */
/*                                                                           */
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
/*  date =  Jan. 09, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
long DFSLinGetPasswdReq(char *pcData,long iDataLen,int *DFSErrno,void *pComTbl)
{
    int             iErrCode;
    long            iRetCode;
    unsigned long   uiAgtRetCode;
    unsigned long   uiAgtErrCode;
    BOOL            bSemFlag;
    unsigned short  ushComRsp;
    int             iAgtSendSize;
    int             iCode;
    BOOL            bCode;
    int             iTotalSendSize;
    struct ComTGroup_t *pCom;
    int             iCtr;
    char            acSendBuf[DDA_HED_LEN+DDA_LINGETPASSWD_REQ_P1_SIZE+LINBUFSIZ];

    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSLinGetPasswdReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSLINGETPASSWD_TRACE_S,pCom->iSockfd,0,aszEyeChach);
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

    if (iDataLen > LINBUFSIZ){
        iErrCode = DEDFAMDCESYSTEM;
        pCom->iConErrFlag = iErrCode;
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,OTHER_ERR,pCom->iConErrFlag,0,aszEyeChach);
#endif
        goto ErrExit;
    }

    iTotalSendSize = DDA_LINGETPASSWD_REQ_P1_SIZE + (int)iDataLen;


    iCtr = 0;

    *(unsigned short *)&acSendBuf[iCtr] = htons(DDA_LINGETPASSWD_COM_REQ);
    iCtr += DDA_HED_COM_LEN;

    *(unsigned short *)&acSendBuf[iCtr] = htons(DDA_RESERVE);
    iCtr += DDA_HED_RESERVE_LEN;

    *(unsigned long *)&acSendBuf[iCtr] = htonl((unsigned long)iTotalSendSize);
    iCtr += DDA_HED_DATASIZE_LEN;

    *(unsigned long *)&acSendBuf[iCtr] = htonl(iDataLen);
    iCtr += DDA_LINGETPASSWD_REQ_P1_SIZE;

    memcpy(&acSendBuf[iCtr],pcData,iDataLen);
    iCtr += iDataLen;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_HEAD_INF,
                (int)DDA_LINGETPASSWD_COM_REQ << 16 | (int)DDA_RESERVE,iTotalSendSize,aszEyeChach);
#endif
    iCode = AgtSendReq(pCom,acSendBuf,iCtr,&iErrCode,iCtr);
    if (iCode == -1){
        iRetCode = -1;
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size, SEND_DATA1_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    bCode = AgtRecvHedReq(pCom,&ushComRsp,&iAgtSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    switch(ushComRsp){
        case DDA_LINGETPASSWD_COM_RSP :
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

    iRetCode = uiAgtRetCode;
    iErrCode = (int)uiAgtErrCode;

ErrExit:
    if (bSemFlag == TRUE){
        SignalLocalSemaphore(pCom->uiCConn_sh);
    }

    *DFSErrno = iErrCode;
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSLINGETPASSWD_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSLinGetPasswdReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSLinResetKeyReq                                                */
/*  name =  DCE login key changing request routine                           */
/*  func = Sends DFAM Agent a change-DCE-login-key request.  Then receives   */
/*         the results and error information from DFAM Agent to send them    */
/*         to a caller.  Since no parameter check is made, an internal       */
/*         error may occur in the caller if iIndataLen=0)                    */
/*           1. Sets a semaphore.                                            */
/*           2. Sends the header information.                                */
/*           3. Receives the header information.                             */
/*           4. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           5. Frees the semaphore.                                         */
/*           6. Sets the results to parameters, and returns to the caller.   */
/*                                                                           */
/*  i/o  = DFSErrno   : int * :out: error information area                   */
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
/*  date =  Jan. 09, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
long DFSLinResetKeyReq(int *DFSErrno,void *pComTbl)
{
    int             iErrCode;
    long            iRetCode;
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
    GW_tsh_functrace(pszDFSLinResetKeyReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSLINRESETKEY_TRACE_S,pCom->iSockfd,0,aszEyeChach);
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

    iTotalSendSize = DDA_LINRESETKEY_REQ_SIZE;


    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_LINRESETKEY_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    bCode = AgtRecvHedReq(pCom,&ushComRsp,&iAgtSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    switch(ushComRsp){
        case DDA_LINRESETKEY_COM_RSP :
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

    iRetCode = uiAgtRetCode;
    iErrCode = (int)uiAgtErrCode;

ErrExit:
    if (bSemFlag == TRUE){
        SignalLocalSemaphore(pCom->uiCConn_sh);
    }

    *DFSErrno = iErrCode;
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSLINRESETKEY_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSLinResetKeyReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSLoutReq                                                       */
/*  name =  DCE logout routine                                               */
/*  func = Sends DFAM Agent a DCE-logout request.  Then receives             */
/*         the results and error information from DFAM Agent to send them    */
/*         to a caller.  Since no parameter check is made, an internal       */
/*         error may occur in the caller if iIndataLen=0)                    */
/*           1. Sets a semaphore.                                            */
/*           2. Sends the header information.                                */
/*           3. Receives the header information.                             */
/*           4. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           5. Frees the semaphore.                                         */
/*           6. Sets the results to parameters, and returns to the caller.   */
/*                                                                           */
/*  i/o  = DFSErrno   : int * :out: error information area                   */
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
/*                                                                           */
/*  date =  Jan. 09, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
long DFSLoutReq(int *DFSErrno,void *pComTbl)
{
    int             iErrCode;
    long            iRetCode;
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
    GW_tsh_functrace(pszDFSLoutReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSLOUT_TRACE_S,pCom->iSockfd,0,aszEyeChach);
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

    iTotalSendSize = DDA_LOUT_REQ_SIZE;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_LOUT_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    bCode = AgtRecvHedReq(pCom,&ushComRsp,&iAgtSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    switch(ushComRsp){
        case DDA_LOUT_COM_RSP :
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

    iRetCode = uiAgtRetCode;
    iErrCode = (int)uiAgtErrCode;

ErrExit:
    if (bSemFlag == TRUE){
        SignalLocalSemaphore(pCom->uiCConn_sh);
    }

    *DFSErrno = iErrCode;
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSLOUT_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSLoutReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}
