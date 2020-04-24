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
 * $Log: dcomfil3.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:15  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:45  marty]
 *
 * Revision 1.1.2.1  1996/02/21  06:43:35  takano
 * 	First Submit
 * 	[1996/02/21  06:43:24  takano]
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
/*  ID   =  DFSCpFirstReadReq                                                */
/*  name =  Reading DFS file (a function for starting file copying)          */
/*  func = With using the file descriptor passed from the caller, sends      */
/*         DFAM Agent a DFS file copy/read request.  Does not check the      */
/*         contents of fildes, nbyte, DFSErrno, or pComTbl.                  */
/*         1. Sets semaphore.                                                */
/*         2. Sends header information.                                      */
/*         3. Sends a file descriptor and the number of bytes to be read.    */
/*         4. Sets the results to parameters to notify the caller.           */
/*                                                                           */
/*  i/o  = fildes  :int                  :inp: file descriptor               */
/*         nbyte   :dsize_t(unsigned int):inp: number of bytes to be read    */
/*         DFSErrno:int *                :out: DFS error information         */
/*         pComTbl :void *               :i/o: thread group table            */
/*                                                                           */
/*  return =  0: normal end                                                  */
/*           -1: abnormal end                                                */
/*  common = none                                                            */
/*      :::                                                                  */
/*  note = If this function is used, call DFSCpLastReadReq() even if         */
/*         an error occurred.                                                */
/*         error information: the following information is added to the      */
/*                            error occurred in DFAM Agent:                  */
/*         DENOERR        : normal end                                       */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : connection closed                                */
/*         DETIMEOUTAGT   : time out                                         */
/*         DERESPONCEDATA : invalid response                                 */
/*         DEUNLOADTCPIP  : TCPIP.NLM unloaded                               */
/*         DEDFAMDCESYSTEM: internal error (self-contradiction)              */
/*         DEDFAMTONOVELL : errors not listed on the manuals                 */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*         DENOSUPPORT    : Agent not supported                              */
/*                                                                           */
/*  date   = August 9, 1995 by Masakazu Mitsuhashi                           */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFSCpFirstReadReq(int fildes,dsize_t nbyte,int *DFSErrno,void *pComTbl)
{
    int             iErrCode;
    int             iRetCode;
    int             iPtrCtr;
    int             iCode;
    int             iTotalSendSize;
    struct ComTGroup_t *pCom;
    char            acSendBuf[DDA_HED_LEN+DDA_CPREAD_REQ_SIZE];


    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSCpFirstReadReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSCPFIRSTREAD_TRACE_S,pCom->iSockfd,0,aszEyeChach);
#endif

    iRetCode = -1;
    iErrCode = DENOERR;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);


    if (pCom->iConErrFlag != DENOERR){
        iErrCode = pCom->iConErrFlag;
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CONERRFLAG_ERR,pCom->iConErrFlag,0,aszEyeChach);
#endif
        goto ErrExit;
    }

    if (pCom->pAgent->ushDFAMAgtTranceVer < CONPV_0200){
        iErrCode = DENOSUPPORT;
        goto ErrExit;
    }

    iTotalSendSize = DDA_CPREAD_REQ_SIZE;

    iPtrCtr = 0;

    *(unsigned short *)&acSendBuf[iPtrCtr] = htons(DDA_CPREAD_COM_REQ);
    iPtrCtr += DDA_HED_COM_LEN;

    *(unsigned short *)&acSendBuf[iPtrCtr] = htons(DDA_RESERVE);
    iPtrCtr += DDA_HED_RESERVE_LEN;

    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)iTotalSendSize);
    iPtrCtr += DDA_HED_DATASIZE_LEN;

    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)fildes);
    iPtrCtr += DDA_CPREAD_REQ_P1_SIZE;

    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)nbyte);
    iPtrCtr += DDA_CPREAD_REQ_P2_SIZE;

#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_HEAD_INF,
                (int)DDA_CPREAD_COM_REQ << 16 | (int)DDA_RESERVE,iTotalSendSize,aszEyeChach);
#endif

    iCode = AgtSendReq(pCom,acSendBuf,iPtrCtr,&iErrCode,iPtrCtr);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA1_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iRetCode = 0;
ErrExit:

    *DFSErrno = iErrCode;
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSCPFIRSTREAD_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSCpFirstReadReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSCpMiddleReadReq                                               */
/*                                                                           */
/*  name =  Reading DFS file (a function for handling file copying)          */
/*  func = Receives the file data from Agent.  Does not check the            */
/*         contents of fildes, nbyte, DFSErrno, or pComTbl.                  */
/*         1. Gets the data sent by Agent.                                   */
/*         2. Sets the results to parameters to notify the caller.           */
/*                                                                           */
/*  i/o  = buf     :void *               :out: buffer for storing data       */
/*         DFSErrno:int *                :out: DFS error information         */
/*         pComTbl :void *               :i/o: thread group table            */
/*                                                                           */
/*  return =  0: end of file                                                 */
/*           -1: abnormal end                                                */
/*           others: the number of actually inputted bytes                   */
/*                                                                           */
/*  common = none                                                            */
/*  note = Call DFSCpFirstReadReq() before using this function.              */
/*         error information: the following information is added to the      */
/*                            error occurred in DFAM Agent:                  */
/*         DENOERR        : normal end                                       */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : connection closed                                */
/*         DETIMEOUTAGT   : time out                                         */
/*         DERESPONCEDATA : invalid response                                 */
/*         DEUNLOADTCPIP  : TCPIP.NLM unloaded                               */
/*         DEDFAMDCESYSTEM: internal error (self-contradiction)              */
/*         DEDFAMTONOVELL : errors not listed on the manuals                 */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*         DENOSUPPORT    : Agent not supported                              */
/*                                                                           */
/*  date   = August 9, 1995 by Masakazu Mitsuhashi                           */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFSCpMiddleReadReq(void *buf,int *DFSErrno,void *pComTbl)
{
    int             iErrCode;
    int             iRetCode;
    unsigned long   uiAgtRetCode;
    unsigned long   uiAgtErrCode;
    struct ComTGroup_t *pCom;
    unsigned short  ushComRsp;
    int             iAgtSendSize;
    int             iCode;
    BOOL            bCode;

    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSCpMiddleReadReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSCPMIDDLEREAD_TRACE_S,pCom->iSockfd,0,aszEyeChach);
#endif

    iRetCode = -1;
    iErrCode = DENOERR;


    if (pCom->pAgent->ushDFAMAgtTranceVer < CONPV_0200){
        iErrCode = DENOSUPPORT;
        goto ErrExit;
    }

    bCode = AgtRecvHedReq(pCom,&ushComRsp,&iAgtSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }
    switch(ushComRsp){
        case DDA_CPREAD_COM_RSP :
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

    *DFSErrno = iErrCode;
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSCPMIDDLEREAD_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSCpMiddleReadReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSCpLastReadReq                                                 */
/*                                                                           */
/*  name =  Reading DFS file (a function for terminating file copying)       */
/*  func = Frees the resource locking.  Does not check the                   */
/*         contents of DFSErrno, or pComTbl.                                 */
/*                                                                           */
/*  i/o  = DFSErrno:int *                :out: DFS error information         */
/*         pComTbl :void *               :i/o: thread group table            */
/*                                                                           */
/*  return =  0: normal end                                                  */
/*           -1: abnormal end                                                */
/*                                                                           */
/*  common = none                                                            */
/*  note =  In order to deactivate the deadlock management, use this         */
/*          function after DFSCpFirstReadReq() was invoked.                  */
/*         error information: the following information is added to the      */
/*                            error occurred in DFAM Agent:                  */
/*         DEUNLOADTCPIP  : TCPIP.NLM unloaded                               */
/*         DEDFAMDCESYSTEM: internal error (self-contradiction)              */
/*                                                                           */
/*  date   = August 9, 1995 by Masakazu Mitsuhashi                           */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFSCpLastReadReq(int *DFSErrno,void *pComTbl)
{
    SignalLocalSemaphore(((struct ComTGroup_t *)pComTbl)->uiCConn_sh);
    *DFSErrno = DENOERR;
    return (0);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSCpbkFirstWriteReq                                             */
/*                                                                           */
/*  name =  Writing to DFS file (a function for starting file copyback)      */
/*  func = With using the file descriptor, number of data bytes to be        */
/*         outputted, and binary data passed from the caller, sends          */
/*         DFAM Agent a "DFS file copyback write request."  Does not         */
/*         check the contents of fildes, buf, nbyte, DFSErrno, or pComTbl.   */
/*         1. Sets semaphore.                                                */
/*         2. Sends header information.                                      */
/*         3. Sends a file descriptor and the number of bytes to be written, */
/*            and binary data.                                               */
/*         4. Sets the results to parameters to notify the caller.           */
/*                                                                           */
/*  i/o  = fildes  :int                  :inp: file descriptor               */
/*         buf     :void *               :inp: buffer for storing data       */
/*         nbyte   :dsize_t(unsigned int):inp: number of bytes to be outputtd*/
/*         DFSErrno:int *                :out: DFS error information         */
/*         pComTbl :void *               :i/o: thread group table            */
/*                                                                           */
/*  return =  0: normal end                                                  */
/*           -1: abnormal end                                                */
/*  common = none                                                            */
/*      :::                                                                  */
/*  note = If this function is used, call DFSCpbkLastWriteReq() even if      */
/*         an error occurred.                                                */
/*         error information: the following information is added to the      */
/*                            error occurred in DFAM Agent:                  */
/*         DENOERR        : normal end                                       */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : connection closed                                */
/*         DETIMEOUTAGT   : time out                                         */
/*         DERESPONCEDATA : invalid response                                 */
/*         DEUNLOADTCPIP  : TCPIP.NLM unloaded                               */
/*         DEDFAMDCESYSTEM: internal error (self-contradiction)              */
/*         DEDFAMTONOVELL : errors not listed on the manuals                 */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*         DENOSUPPORT    : Agent not supported                              */
/*                                                                           */
/*  date   = January 10, 1995 by Masakazu Mitsuhashi                         */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFSCpbkFirstWriteReq(int fildes,void *buf,dsize_t nbyte,int *DFSErrno,void *pComTbl)
{
    int             iErrCode;
    int             iRetCode;
    int             iTotalSendSize;
    int             iPtrCtr;
    int             iCode;
    struct ComTGroup_t *pCom;
    char            acSendBuf[DDA_HED_LEN+DDA_CPBKWRITE_REQ_P1_SIZE+DDA_CPBKWRITE_REQ_P2_SIZE];


    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSCpbkFirstWriteReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSCPBKFIRSTWRITE_TRACE_S,pCom->iSockfd,0,aszEyeChach);
#endif

    iRetCode = -1;
    iErrCode = DENOERR;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);

    if (pCom->iConErrFlag != DENOERR){
        iErrCode = pCom->iConErrFlag;
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CONERRFLAG_ERR,pCom->iConErrFlag,0,aszEyeChach);
#endif
        goto ErrExit;
    }

    if (pCom->pAgent->ushDFAMAgtTranceVer < CONPV_0200){
        iErrCode = DENOSUPPORT;
        goto ErrExit;
    }

    iTotalSendSize = DDA_CPBKWRITE_REQ_P1_SIZE + DDA_CPBKWRITE_REQ_P2_SIZE + nbyte;

    iPtrCtr = 0;

    *(unsigned short *)&acSendBuf[iPtrCtr] = htons(DDA_CPBKWRITE_COM_REQ);
    iPtrCtr += DDA_HED_COM_LEN;

    *(unsigned short *)&acSendBuf[iPtrCtr] = htons(DDA_RESERVE);
    iPtrCtr += DDA_HED_RESERVE_LEN;

    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)iTotalSendSize);
    iPtrCtr += DDA_HED_DATASIZE_LEN;

    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)fildes);
    iPtrCtr += DDA_CPBKWRITE_REQ_P1_SIZE;

    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)nbyte);
    iPtrCtr += DDA_CPBKWRITE_REQ_P2_SIZE;

#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_HEAD_INF,
                (int)DDA_CPBKWRITE_COM_REQ << 16 | (int)DDA_RESERVE,iTotalSendSize,aszEyeChach);
#endif

    iCode = AgtSendReq(pCom,acSendBuf,iPtrCtr,&iErrCode,iPtrCtr);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA1_ERR,iCode,iErrCode,aszEyeChach);
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
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA2_ERR,iCode,iErrCode,aszEyeChach);
#endif
            goto ErrExit;
        }
    }

    iRetCode = 0;

ErrExit:

    *DFSErrno = iErrCode;
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSCPBKFIRSTWRITE_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSCpbkFirstWriteReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}
// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSCpbkMiddleWriteReq                                            */
/*                                                                           */
/*  name =  Writing to DFS file (a function for file copyback)               */
/*  func = With using the file descriptor, number of data bytes to be        */
/*         outputted, and binary data passed from the caller, sends          */
/*         DFAM Agent a "DFS file copyback write request."  Does not         */
/*         check the contents of fildes, buf, nbyte, DFSErrno, or pComTbl.   */
/*         1. Sends header information.                                      */
/*         2. Sends a file descriptor and the number of bytes to be written, */
/*            and binary data.                                               */
/*         3. Sets the results to parameters to notify the caller.           */
/*                                                                           */
/*  i/o  = fildes  :int                  :inp: file descriptor               */
/*         buf     :void *               :out: buffer for storing data       */
/*         nbyte   :dsize_t(unsigned int):inp: number of bytes to be outputtd*/
/*         DFSErrno:int *                :out: DFS error information         */
/*         pComTbl :void *               :i/o: thread group table            */
/*                                                                           */
/*  return =  0: normal end                                                  */
/*           -1: abnormal end                                                */
/*  common = none                                                            */
/*      :::                                                                  */
/*  note = Call DFSCpbkFirstWriteReq() before calling this function.         */
/*         error information: the following information is added to the      */
/*                            error occurred in DFAM Agent:                  */
/*         DENOERR        : normal end                                       */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : connection closed                                */
/*         DETIMEOUTAGT   : time out                                         */
/*         DERESPONCEDATA : invalid response                                 */
/*         DEUNLOADTCPIP  : TCPIP.NLM unloaded                               */
/*         DEDFAMDCESYSTEM: internal error (self-contradiction)              */
/*         DEDFAMTONOVELL : errors not listed on the manuals                 */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*         DENOSUPPORT    : Agent not supported                              */
/*                                                                           */
/*  date   = January 10, 1995 by Masakazu Mitsuhashi                         */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFSCpbkMiddleWriteReq(int fildes,void *buf,dsize_t nbyte,int *DFSErrno,void *pComTbl)
{
    int             iErrCode;
    int             iRetCode;
    BOOL            bSemFlag;
    int             iTotalSendSize;
    int             iPtrCtr;
    int             iCode;
    struct ComTGroup_t *pCom;
    char            acSendBuf[DDA_HED_LEN+DDA_CPBKWRITE_REQ_P1_SIZE+DDA_CPBKWRITE_REQ_P2_SIZE];


    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSCpbkMiddleWriteReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSCPBKMIDDLEWRITE_TRACE_S,pCom->iSockfd,0,aszEyeChach);
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

    if (pCom->pAgent->ushDFAMAgtTranceVer < CONPV_0200){
        iErrCode = DENOSUPPORT;
        goto ErrExit;
    }

    iTotalSendSize = DDA_CPBKWRITE_REQ_P1_SIZE + DDA_CPBKWRITE_REQ_P1_SIZE + nbyte;

    iPtrCtr = 0;

    *(unsigned short *)&acSendBuf[iPtrCtr] = htons(DDA_CPBKWRITE_COM_REQ);
    iPtrCtr += DDA_HED_COM_LEN;

    *(unsigned short *)&acSendBuf[iPtrCtr] = htons(DDA_RESERVE);
    iPtrCtr += DDA_HED_RESERVE_LEN;

    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)iTotalSendSize);
    iPtrCtr += DDA_HED_DATASIZE_LEN;

    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)fildes);
    iPtrCtr += DDA_CPBKWRITE_REQ_P1_SIZE;

    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)nbyte);
    iPtrCtr += DDA_CPBKWRITE_REQ_P1_SIZE;

#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_HEAD_INF,
                (int)DDA_CPBKWRITE_COM_REQ << 16 | (int)DDA_RESERVE,iTotalSendSize,aszEyeChach);
#endif

    iCode = AgtSendReq(pCom,acSendBuf,iPtrCtr,&iErrCode,iPtrCtr);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA1_ERR,iCode,iErrCode,aszEyeChach);
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
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA2_ERR,iCode,iErrCode,aszEyeChach);
#endif
            goto ErrExit;
        }
    }

    iRetCode = 0;

ErrExit:

    *DFSErrno = iErrCode;
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSCPBKMIDDLEWRITE_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSCpbkMiddleWriteReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSCpbkLastWriteReq                                              */
/*                                                                           */
/*  name =  Writing to DFS file (a function for terminating file copyback)   */
/*  func = With using the file descriptor passed from the caller, sends      */
/*         DFAM Agent a "DFS file copyback write termination request."       */
/*         Does not check the contents of fildes, DFSErrno, or pComTbl.      */
/*         1. Sends header information.                                      */
/*         2. Sends file descriptor.                                         */
/*         3. Receives a return value and error information (their contents  */
/*            are not checked).                                              */
/*         4. Releases the semaphore.                                        */
/*         5. Sets the results to parameters to notify the caller.           */
/*                                                                           */
/*  i/o  = fildes  :int                  :inp: file descriptor               */
/*         DFSErrno:int *                :out: DFS error information         */
/*         pComTbl :void *               :i/o: thread group table            */
/*                                                                           */
/*  return = -1: abnormal end                                                */
/*           others: the number of bytes actually outputted                  */
/*  common = none                                                            */
/*      :::                                                                  */
/*  note = When DFSCpbkFirstWriteReq() was used, call this function to       */
/*         free the resource deadlock management.                            */
/*         error information: the following information is added to the      */
/*                            error occurred in DFAM Agent:                  */
/*         DENOERR        : normal end                                       */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : connection closed                                */
/*         DETIMEOUTAGT   : time out                                         */
/*         DERESPONCEDATA : invalid response                                 */
/*         DEUNLOADTCPIP  : TCPIP.NLM unloaded                               */
/*         DEDFAMDCESYSTEM: internal error (self-contradiction)              */
/*         DEDFAMTONOVELL : errors not listed on the manuals                 */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*         DENOSUPPORT    : Agent not supported                              */
/*                                                                           */
/*  date   = January 10, 1995 by Masakazu Mitsuhashi                         */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFSCpbkLastWriteReq(int fildes,int *DFSErrno,void *pComTbl)
{
    int             iErrCode;
    int             iRetCode;
    unsigned long   uiAgtRetCode;
    unsigned long   uiAgtErrCode;
    unsigned short  ushComRsp;
    int             iAgtSendSize;
    int             iTotalSendSize;
    int             iPtrCtr;
    int             iCode;
    BOOL            bCode;
    struct ComTGroup_t *pCom;
    char            acSendBuf[DDA_HED_LEN+DDA_CPBKWRITETERM_REQ_P1_SIZE];


    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSCpbkLastWriteReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSCPBKLASTWRITE_TRACE_S,pCom->iSockfd,0,aszEyeChach);
#endif

    iRetCode = -1;
    iErrCode = DENOERR;

    if (pCom->iConErrFlag != DENOERR){
        iErrCode = pCom->iConErrFlag;
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CONERRFLAG_ERR,pCom->iConErrFlag,0,aszEyeChach);
#endif
        goto ErrExit;
    }

    if (pCom->pAgent->ushDFAMAgtTranceVer < CONPV_0200){
        iErrCode = DENOSUPPORT;
        goto ErrExit;
    }

    iTotalSendSize = DDA_CPBKWRITETERM_REQ_P1_SIZE;

    iPtrCtr = 0;

    *(unsigned short *)&acSendBuf[iPtrCtr] = htons(DDA_CPBKWRITETERM_COM_REQ);
    iPtrCtr += DDA_HED_COM_LEN;

    *(unsigned short *)&acSendBuf[iPtrCtr] = htons(DDA_RESERVE);
    iPtrCtr += DDA_HED_RESERVE_LEN;

    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)iTotalSendSize);
    iPtrCtr += DDA_HED_DATASIZE_LEN;

    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)fildes);
    iPtrCtr += DDA_CPBKWRITETERM_REQ_P1_SIZE;

#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_HEAD_INF,
                (int)DDA_CPBKWRITETERM_COM_REQ << 16 | (int)DDA_RESERVE,iTotalSendSize,aszEyeChach);
#endif

    iCode = AgtSendReq(pCom,acSendBuf,iPtrCtr,&iErrCode,iPtrCtr);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA1_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    bCode = AgtRecvHedReq(pCom,&ushComRsp,&iAgtSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }
    switch(ushComRsp){
        case DDA_CPBKWRITETERM_COM_RSP :
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
    SignalLocalSemaphore(pCom->uiCConn_sh);

    *DFSErrno = iErrCode;
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSCPBKLASTWRITE_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSCpbkLastWriteReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

