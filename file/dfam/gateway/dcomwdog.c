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
 * $Log: dcomwdog.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:23  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:51  marty]
 *
 * Revision 1.1.2.1  1996/02/21  06:47:00  takano
 * 	First Submit
 * 	[1996/02/21  06:46:52  takano]
 * 
 * $EndLog$
 */
#include <string.h>
#include <errno.h>
#include <io.h>
#include <sys\socket.h>
#include <sys\types.h>
#include <sys\time.h>
#include <netinet\in.h>
#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "dceapi.h"

// EnglishComment
/*****************************************************************************/
/*  ID   =  WatchdogStatRecvReq                                              */
/*  name =  DFAM Agent information receiving routine                         */
/*  func = Receives data from DFAM Agent (but does not check parameters).    */
/*           1. In the case that a health checking request is received:      */
/*                 Returns the health checking response, and notifies to a   */
/*                 caller.                                                   */
/*           2. In the case that a Gateway message output request is received*/
/*                 Notifies to a caller (Sends the caller the received data  */
/*                 untouched).                                               */
/*           3. In the case that no data received in a specificed period:    */
/*                 Notifies so to a caller.                                  */
/*                                                                           */
/*           If there is information, the following value is set to puiInfo  */
/*           (puiInfo has an undefined value if there is no information):    */
/*               HEALTHCHK_REQ : health checking request                     */
/*               MESSAGE_REQ   : Gateway message request                     */
/*                                                                           */
/*           If the received data size at the Gateway message output request */
/*           exceeds tmWatchdogMSGBuflen, the surplus is discarded (i.e.,    */
/*           puiMsgSize <= tmWatchgodMSGBuflen).                             */
/*                                                                           */
/*  i/o  = puiInfo   : unsigned long *:out: information type                 */
/*         puiMsgSize: size_t *       :out: message size                     */
/*         pcMsg     : char *         :out: message storing area             */
/*         piErrno   : int *          :out: error information                */
/*         pComTbl   : void *         :i/o: thread group table               */
/*  return =  1: no information exists.                                      */
/*            0: information exists.                                         */
/*           -1: error occurred.                                             */
/*  common = tmWatchdogMSGBuflen: size_t :inp: maximum receiving data size   */
/*                                             of Wachdog                    */
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
/*  date =  Jan. 09, 1995 Masakazu Mitsuhashi                                */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int WatchDogStatRecvReq(unsigned long *puiInfo,size_t *puiMsgSize,char *pcMsg,int *piErrno,void *pComTbl)
{
    int iRetCode;
    unsigned short ushComRspInfo;
    int iRecvDataLen;
    int iCode;
    BOOL bCode;
    struct ComTGroup_t *pCom;
    char acBuf[DDA_HED_LEN];
    int iCtr;

    iRetCode = 0;
    *piErrno = DENOERR;
    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszWatchDogStatRecvReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,WATCHDOGSTATRECV_TRACE_S,pCom->iSockfd,0,aszEyeChach);
#endif

    if (pCom->iConErrFlag != DENOERR){
        *piErrno = pCom->iConErrFlag;
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CONERRFLAG_ERR,pCom->iConErrFlag,0,aszEyeChach);
#endif
        goto ErrExit;
    }

    iCode = RecvWaitTime(pCom,pCom->pAgent->iWatchDogWaitTime,piErrno);
    switch(iCode){
        case 0 :
            break;
        case -1 :
            iRetCode = -1;
            break;
        case 1 :
            iRetCode = 1;
#ifndef A
            iCode = AgtRecvReq(pCom,acBuf,sizeof(acBuf),piErrno,0);
#else
            iCode = AgtRecvReq(pCom,acBuf,sizeof(acBuf),piErrno,sizeof(acBuf));
#endif
            if (iCode  == -1){
                switch(*piErrno){
                    case DETIMEOUTAGT :


                        iRetCode = 0;
                        break;
                    default :
                        iRetCode = -1;
                        break;
                }
                goto ErrExit;
            }
            iCtr = 0;
            ushComRspInfo = ntohs(*(unsigned short *)&acBuf[iCtr]);
            iCtr += DDA_HED_COM_LEN;

            iCtr += DDA_HED_RESERVE_LEN;

            iRecvDataLen = (int)ntohl(*(unsigned long *)&acBuf[iCtr]);
            iCtr += DDA_HED_DATASIZE_LEN;

            switch(ushComRspInfo){
                case DDA_HEALTHCHK_COM_REQ :
                    if (iRecvDataLen > 0){
                        iCode = DustRecv(pCom,iRecvDataLen,piErrno);
                        if (iRecvDataLen == -1){
                            iRetCode = -1;
                            goto ErrExit;
                        }
                    }
                    iCtr = 0;
                    *(unsigned short *)&acBuf[iCtr] = htons(DDA_HEALTHCHK_COM_RSP);
                    iCtr += DDA_HED_COM_LEN;

                    *(unsigned short *)&acBuf[iCtr] = htons(DDA_RESERVE);
                    iCtr += DDA_HED_RESERVE_LEN;

                    *(unsigned long *)&acBuf[iCtr] = htonl(DDA_HEALTHCHK_RSP_SIZE);
                    iCtr += DDA_HED_DATASIZE_LEN;

#ifndef A
                    iCode = AgtSendReq(pCom,acBuf,sizeof(acBuf),piErrno,0);
#else
                    iCode = AgtSendReq(pCom,acBuf,sizeof(acBuf),piErrno,sizeof(acBuf));
#endif
                    if (bCode == -1){
                        iRetCode = -1;
                        goto ErrExit;
                    }
#ifdef DCELOG
#ifndef A
                    DCELogOutput(HLTCHK,0,pCom->iSockfd,0xFFFFFFFF,"",0);
#endif
#endif
                    *puiInfo = HEALTHCHK_REQ;
                    break;
                case DDA_MESSAGE_COM_REQ :

#ifdef DCELOG
#ifndef A
                    DCELogOutput(RVDATA,sizeof(acBuf),pCom->iSockfd,0xFFFFFFFF,acBuf,0);
#endif
#endif
                    *puiMsgSize = (iRecvDataLen > tmWatchdogMSGBuflen) ? tmWatchdogMSGBuflen : iRecvDataLen;
                    iCode = AgtRecvReq(pCom,pcMsg,*puiMsgSize,piErrno,*puiMsgSize);
                    if (iCode == -1){
                        iRetCode = -1;
#ifdef DCETRC
                        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CONERRFLAG_ERR,iCode,*piErrno,aszEyeChach);
#endif
                        goto ErrExit;
                    }
                    iRecvDataLen -= iCode;
                    if (iRecvDataLen > 0){
                        iCode = DustRecv(pComTbl,iRecvDataLen,piErrno);
                        if (iRecvDataLen == -1){
                            iRetCode = -1;
                            goto ErrExit;
                        }
                    }
                    *puiInfo = MESSAGE_REQ;
                    break;
                default :
#ifdef DCELOG
#ifndef A
                    DCELogOutput(RVDATA,sizeof(acBuf),pCom->iSockfd,0xFFFFFFFF,acBuf,0);
#endif
#endif
                    iRetCode = 0;
                    if (iRecvDataLen > 0){
                        iCode = DustRecv(pCom,iRecvDataLen,piErrno);
                        if (iRecvDataLen == -1){
                            iRetCode = -1;
                            goto ErrExit;
                        }
                    }
                    bCode = AgtSendHedReq(pCom,DDA_NOSUPPORT_COM_RSP,0,piErrno);
                    if (bCode == FALSE){
                        iRetCode = -1;
                        goto ErrExit;
                    }
                    break;
            }
            break;
        default :
            iRetCode = -1;
            *piErrno = DEDFAMDCESYSTEM;
            pCom->iConErrFlag = *piErrno;
#ifdef DCETRC
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,OTHER_ERR,iCode,*piErrno,aszEyeChach);
#endif
            goto ErrExit;
    }
ErrExit:
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,WATCHDOGSTATRECV_TRACE_E,iRetCode,*piErrno,aszEyeChach);
    GW_tsh_functrace(pszWatchDogStatRecvReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  RecvWaitTime                                                     */
/*  name =  Receiving data waiting routine                                   */
/*  func =  Waites for data from DFAM Agent (waits for the specified period, */
/*          and notifies if there is received information).                  */
/*  i/o  = pCom     : struct ComTGroup_t *:inp: thread group table           */
/*         iWaitTime: int                 :inp: receive waiting time         */
/*         piErrCode: int *               :out: error information            */
/*                                                                           */
/*  return =  1: no information exists.                                      */
/*            0: information exists.                                         */
/*           -1: error occurred.                                             */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DETIMEOUTAGT   : time out                                         */
/*                          ----> frees connection.                          */
/*         DEUNLOADTCPIP  : TCPIP.NLM was unloaded                           */
/*                          ----> takes an exit.                             */
/*         DEDFAMDCESYSTEM: internal error                                   */
/*                          ----> frees connection.                          */
/*         DEDFAMTONOVELL : errors other than listed in manuals              */
/*                          ----> frees connection.                          */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*                                                                           */
/*  date =  Jan. 09, 1995 Masakazu Mitsuhashi                                */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int RecvWaitTime(struct ComTGroup_t *pCom,int iWaitTime,int *piErrCode)
{
    struct timeval timeout;
    fd_set readfds;
    int iCode;
    int iRetCode;
    int iSaveErrno;

    *piErrCode = DENOERR;
    iRetCode = 0;

    FD_ZERO(&readfds);
    FD_SET(pCom->iSockfd,&readfds);

    timeout.tv_sec = iWaitTime;
    timeout.tv_usec = 0;
    iCode = select(pCom->iSockfd+1,&readfds,(fd_set *)0,(fd_set *)0,&timeout);

    switch (iCode){
        case -1 :
            iSaveErrno = errno;
            switch(iSaveErrno){
                case EBADF :
                case EINVAL :
                case ENOTSOCK :
                    *piErrCode = DEINVALSOCKET;
#ifdef DCELOG
                    DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"select(-1)",iSaveErrno);
#endif
                    break;
                case ENETDOWN :
                    *piErrCode = DEUNLOADTCPIP;
                    ResumeMainT();
#ifdef DCELOG
                    DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"select(-1)",iSaveErrno);
#endif
                    break;
                case EFAULT :
                    *piErrCode = DEDFAMTONOVELL;
#ifdef DCELOG
                    DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"select(-1) U",iSaveErrno);
#endif
                    break;
                default :
                    *piErrCode = DEDFAMTONOVELL;
#ifdef DCELOG
                    DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"select(-1) N",iSaveErrno);
#endif
                    break;
            }
            iRetCode = -1;
            pCom->iConErrFlag = *piErrCode;
#ifdef DCETRC
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SELECT_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
            break;
        case 0 :
            iRetCode = 0;

            break;
        case 1 :
            iRetCode = 1;
            break;
        default :
            iSaveErrno = errno;
            iRetCode = -1;
            *piErrCode = DEDFAMDCESYSTEM;
            pCom->iConErrFlag = *piErrCode;
#ifdef DCETRC
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SELECT_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
            DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"select(?)",iSaveErrno);
#endif
            break;
    }
    return(iRetCode);
}

