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
 * $Log: dcomtime.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:22  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:50  marty]
 *
 * Revision 1.1.2.1  1996/02/21  06:46:33  takano
 * 	First Submit
 * 	[1996/02/21  06:46:22  takano]
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
/*  ID   =  DFStimeReq                                                       */
/*  name =  Time inquire routine                                             */
/*  func = Sends DFAM Agent a "time retrieval" request.  Then receives the   */
/*         results and error information from DFAM Agent to send them to     */
/*         the caller.  Does not check the contents of DFSErrno, and pComTbl.*/
/*           1. Sets a semaphore.                                            */
/*           2. Sends the header information.                                */
/*           3. Receives the header information.                             */
/*           4. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           5. Frees the semaphore.                                         */
/*           6. Sets the results to parameters, and returns to the caller.   */
/*              Does not set if tloc is NULL.                                */
/*                                                                           */
/*  i/o  = tloc     : dtime_t *:out: time                                    */
/*                   (long *)                                                */
/*         DFSErrno : int *    :out: DFS error information                   */
/*         pComTbl  : void *   :i/o: thread group table                      */
/*                                                                           */
/*  return = -1    : failed                                                  */
/*           others: time (sum total seconds from 00:00:00 GMT.Jan 1 1970)   */
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
/*  date =  Jan. 09, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
dtime_t DFStimeReq(dtime_t *tloc,int *DFSErrno,void *pComTbl)
{
    int             iErrCode;
    dtime_t         iRetCode;
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
    GW_tsh_functrace(pszDFStimeReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSTIME_TRACE_S,pCom->iSockfd,0,aszEyeChach);
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

    iTotalSendSize = DDA_TIME_REQ_SIZE;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_TIME_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    bCode = AgtRecvHedReq(pCom,&ushComRsp,&iAgtSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }
    switch(ushComRsp){
        case DDA_TIME_COM_RSP :
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

    if(iAgtSendSize > 0){
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
    if (tloc != NULL && iRetCode != -1){
        *tloc = (dtime_t)iRetCode;
    }
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSTIME_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFStimeReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}
