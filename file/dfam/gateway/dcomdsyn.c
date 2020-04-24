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
 * $Log: dcomdsyn.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:11  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:41  marty]
 *
 * Revision 1.1.2.1  1996/02/21  06:40:37  takano
 * 	First Submit
 * 	[1996/02/21  06:39:14  takano]
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
/*  ID   =  DFSdirsyncReq                                                    */
/*  name =  Directory synchronization requesting routine                     */
/*  func = Sends DFAM Agent a "directory synchronization (searching          */
/*         directories that have newer time than iTime)" request.            */
/*              Then receives the results and error information              */
/*         from DFAM Agent to send them to the caller.  Does not check the   */
/*         contents of DFSErrno, pDsync, and pComTbl.                        */
/*              In the case that pszMpname is NULL or its length is 0, this  */
/*         function returns DENOENT as error information.                    */
/*              In the case that the pszMpname length exceeds DGW_PATH_MAX   */
/*         (1024), this function returns DENAMETOOLONG as error information. */
/*           1. Sets a semaphore.                                            */
/*           2. Sends the header information.                                */
/*           3. Sends the time for searching, path name length, and path     */
/*              name.                                                        */
/*           4. Receives the header information.                             */
/*           5. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           6. Receives the path name length, path name, last update time   */
/*              and date, and file size.                                     */
/*           7. Frees the semaphore.                                         */
/*           8. Sets the results to parameters, and returns to the caller.   */
/*                                                                           */
/*         struct ddirsync {                                                 */
/*             unsigned int d_namlen;       ---> path name length (*2)       */
/*             char d_name[DGW_PATH_MAX+1]; ---> path name                   */
/*             dtime_t d_mtime;             ---> last update time&date       */
/*             doff_t d_size;               ---> file size (*3)              */
/*         };                                                                */
/*         *1: If the return value is -1, the contents of the structure is   */
/*             not defined (sort of a wildcard)                              */
/*         *2: If the path length is 0xFFFFFFFF, the search was completed.   */
/*             If it has other value, the contents of the structure is       */
/*             not defined (sort of a wildcard)                              */
/*         *3: The file size of 0xFFFFFFFF indicates a directory.            */
/*                                                                           */
/*  i/o  = iTime    : dtime_t(long)    :inp: time to be compared at synchro- */
/*                                           nization (to see new or old)    */
/*         pszMpname: char *           :inp: mount point's DFS full path name*/
/*         DFSErrno : int *            :out: DFS error information           */
/*         pDsync   : struct ddirsync *:out: results                         */
/*         pComTbl  : void *           :i/o: thread group table              */
/*                                                                           */
/*  return =  0: successful                                                  */
/*           -1: failed                                                      */
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
int DFSdirsyncReq(dtime_t iTime,char *pszMpname,int *DFSErrno,struct ddirsync *pDsync,void *pComTbl)
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
    unsigned int    tmMpnameLen;
    struct ComTGroup_t *pCom;
    char            acSendBuf[DDA_DIRSYNC_REQ_P1_SIZE+DDA_DIRSYNC_REQ_L2_SIZE];

    char            acRecvBuf[DDA_DIRSYNC_RSP_P5_SIZE+DDA_DIRSYNC_RSP_P6_SIZE];


    pCom = (struct ComTGroup_t *)pComTbl;

#ifdef DCETRC
    GW_tsh_functrace(pszDFSdirsyncReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSDIRSYNC_TRACE_S,pCom->iSockfd,0,aszEyeChach);
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


    tmMpnameLen = PathChk(pszMpname,&iErrCode);
    if (tmMpnameLen == 0xFFFFFFFF){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,PATH_ERR,tmMpnameLen,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iTotalSendSize = DDA_DIRSYNC_REQ_P1_SIZE + DDA_DIRSYNC_REQ_L2_SIZE + (int)tmMpnameLen;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_DIRSYNC_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    iPtrCtr = 0;
    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl(iTime);
    iPtrCtr += DDA_DIRSYNC_REQ_P1_SIZE;

    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)tmMpnameLen);
    iPtrCtr += DDA_DIRSYNC_REQ_L2_SIZE;

    iCode = AgtSendReq(pCom,acSendBuf,iPtrCtr,&iErrCode,iPtrCtr);
    if (iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA2_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iCode = AgtSendReq(pCom,pszMpname,tmMpnameLen,&iErrCode,tmMpnameLen);
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
        case DDA_DIRSYNC_COM_RSP :
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

    if (uiAgtRetCode == 0xFFFFFFFF){
        if(iAgtSendSize > 0){
            iCode = DustRecv(pCom,iAgtSendSize,&iErrCode);
            if (iCode == -1){
                goto ErrExit;
            }
        }
        iRetCode = (int)uiAgtRetCode;
        iErrCode = (int)uiAgtErrCode;
        goto ErrExit;
    }

    iCode = AgtRecvReq(pCom,(char *)&pDsync->d_namlen,DDA_DIRSYNC_RSP_P3_SIZE,&iErrCode,DDA_DIRSYNC_RSP_P3_SIZE);
    if (iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,RECV_DATA3_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }
    iAgtSendSize -= iCode;

    pDsync->d_namlen = (unsigned int)ntohl((unsigned long)pDsync->d_namlen);

    if (pDsync->d_namlen == 0xFFFFFFFF){
        if(iAgtSendSize > 0){
            iCode = DustRecv(pCom,iAgtSendSize,&iErrCode);
            if (iCode == -1){
                goto ErrExit;
            }
        }
        iRetCode = (int)uiAgtRetCode;
        iErrCode = (int)uiAgtErrCode;
        goto ErrExit;
    }

    iCode = AgtRecvReq(pCom,pDsync->d_name,pDsync->d_namlen,&iErrCode,pDsync->d_namlen);
    if (iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,RECV_DATA4_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }
    iAgtSendSize -= iCode;
    pDsync->d_name[pDsync->d_namlen] = '\0';

    iCode = AgtRecvReq(pCom,acRecvBuf,sizeof(acRecvBuf),&iErrCode,sizeof(acRecvBuf));
    if (iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,RECV_DATA5_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }
    iAgtSendSize -= iCode;

    iPtrCtr = 0;
    pDsync->d_mtime = (dtime_t)ntohl(*(long *)&acRecvBuf[iPtrCtr]);
    iPtrCtr += DDA_DIRSYNC_RSP_P5_SIZE;

    pDsync->d_size = (doff_t)ntohl(*(long *)&acRecvBuf[iPtrCtr]);
    iPtrCtr += DDA_DIRSYNC_RSP_P6_SIZE;


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
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSDIRSYNC_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSdirsyncReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}
