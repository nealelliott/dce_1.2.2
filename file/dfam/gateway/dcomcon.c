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
 * $Log: dcomcon.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:09  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:39  marty]
 *
 * Revision 1.1.2.1  1996/02/21  06:38:55  takano
 * 	First Submit
 * 	[1996/02/21  06:38:46  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <string.h>
#include <stropts.h>
#include <nwsemaph.h>
#include <errno.h>
#include <io.h>
#include <netdb.h>
#include <sys\socket.h>
#include <sys\types.h>
#include <sys\time.h>
#include <sys\filio.h>
#include <netinet\in.h>
#include <arpa\inet.h>
#include <nwtypes.h>
#include <process.h>
#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "dceapi.h"

// EnglishComment
/*****************************************************************************/
/*  ID   =  AgtConnectReq                                                    */
/*  name =  Connection requesting routine                                    */
/*  func =  Attempts the connection establishment between DFAM Gateway and   */
/*          DFAM Agent.  This routine does not check the validity of the     */
/*          parameters.                                                      */
/*           1. Establishes the connection.                                  */
/*              1-1. Gets an IP address from the host name.                  */
/*              1-2. Gets a port number form the service name, protocol type.*/
/*              1-3. Opens a socket.                                         */
/*              1-4. Changes the mode.                                       */
/*              1-5. Establishes the connection.                             */
/*              1-6. Changes the send/receive buffer.                        */
/*           2. Sends the connection identification request.                 */
/*                                                                           */
/*          * prerequisite: The semaphore handle must be open before this    */
/*                          function is invoked.                             */
/*                                                                           */
/*          The value set to the connection type (ushConAssort):             */
/*              GW_CONTROL_CON  : administration connection                  */
/*              GW_WATCHDOG_CON : watchdog connection                        */
/*              GW_USER_CON     : user connection                            */
/*  i/o  =                                                                   */
/*      pAgent      : struct GAgent_t *:inp: DFAM Agent table to be connected*/
/*        ->pMaep               : struct GAgent_t * :not used:               */
/*        ->pUshirop            : struct GAgent_t * :not used:               */
/*        ->aszAgentName        : BYTE              :inp:                    */
/*        ->iChipherType        : int               :inp:                    */
/*        ->bIsConnection_live  : BOOL              :not used:               */
/*        ->bLogin_Stop         : BOOL              :not used:               */
/*        ->iCom_SendBuffLen    : int               :inp:                    */
/*        ->iCom_ResponseWaitTime:long              :inp:                    */
/*        ->iWatchDogWaitTime   : long              :inp:                    */
/*        ->ushDFAMAgtTranceVer : unsigned short    :out:                    */
/*        ->uiOpenFileNum       : unsigned long     :out:                    */
/*      ushConAssort: unsigned short   :inp: connection type                 */
/*      pComTbl     : void *           :inp: thread group table              */
/*        ->iSockFd     : int               :out:                            */
/*        ->uiCConn_sh  : LONG              :inp:                            */
/*        ->pTsh_Trace  : void *            :inp:                            */
/*        ->iConErrFlag : int               :out:                            */
/*        ->uiConnNum   : LONG              :inp:                            */
/*        ->pAgent      : struct GAgent_t * :out:                            */
/*        ->uiTsh_Size  : size_t            :inp:                            */
/*  return = See the detailed error information in "note."                   */
/*  common = pszDFAMServName : char *:inp: service name                      */
/*           pszDFAMProtcol  : char *:inp: protocol type                     */
/*  note = error information                                                 */
/*         DENOERR        : normal end                                       */
/*         DEHOSTFILE     : host name, HOST file invalid                     */
/*         DESERVICESFILE : Services file invalid                            */
/*         DECOMNOMEM     : shortage of memory                               */
/*         DEREFUSEAGT    : connection was not established                   */
/*         DENONBKIO      : non blocking I/O change error                    */
/*         DERESPONCEDATA : invalid response data                            */
/*         DEUNLOADTCPIP  : TCPIP.NLM was unloaded                           */
/*         DEDFAMDCESYSTEM: internal error                                   */
/*         DEDFAMTONOVELL : other errors besides errors on the manuals       */
/*         DENETWORKENV   : invalid network environment in NetWare server    */
/*         DEDFAMTERM     : DFAM terminated (UNLOAD was detected)            */
/*  date   = Jan. 10, 1995 by Masakazu Mitsuhashi                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int AgtConnectReq(struct GAgent_t *pAgent,unsigned short ushConAssort,void *pComTbl)
{
    struct hostent      *pHost;
    struct servent      *pServ;
    struct sockaddr_in  Addr;
    int     iCode;
    BOOL    bCode;
    char    cNonBk;
    int     iRetCode;
    int     iErrCode;
    struct ComTGroup_t  *pCom;
    struct nwsockent Nwsktent;
    char aszIPaddres[sizeof(nwinet_scratch)];
    int iSaveErrno;
    BOOL    bLoop;
    clock_t uiStartTime;
    clock_t uiNowTime;

#ifdef DCETRC
    char aszTRCIPaddres[DCOMEYEC+sizeof(nwinet_scratch)];
#endif

    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszAgtConnectReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,AGTCONNECT_TRACE_S,-1,0,aszEyeChach);
#endif

    iRetCode = DENOERR;

    pCom->iSockfd = socket(PF_INET,SOCK_STREAM,0);
    if (pCom->iSockfd == -1){
        iSaveErrno = errno;
#ifdef DCELOG
        DCELogSoketSet(pCom->iSockfd,ushConAssort);
#endif
        switch (iSaveErrno){
            case ENOBUFS :
                iRetCode = DECOMNOMEM;
#ifdef DCELOG
                DCELogOutput(FUNC_E,0,0,pCom->uiConnNum,"socket",iSaveErrno);
#endif
                break;
            case EPROTONOSUPPORT :
            case EPROTOTYPE :
                iRetCode = DEDFAMDCESYSTEM;
#ifdef DCELOG
                DCELogOutput(FUNC_E,0,0,pCom->uiConnNum,"socket",iSaveErrno);
#endif
                break;
            case ENETDOWN :
                iRetCode = DEUNLOADTCPIP;
#ifdef DCELOG
                DCELogOutput(FUNC_E,0,0,pCom->uiConnNum,"socket",iSaveErrno);
#endif
                ResumeMainT();
                break;
            case EMFILE :
            case ENFILE :
                iRetCode = DEDFAMTONOVELL;
#ifdef DCELOG
                DCELogOutput(FUNC_E,0,0,pCom->uiConnNum,"socket U",iSaveErrno);
#endif
                break;
            default :
                iRetCode = DEDFAMTONOVELL;
#ifdef DCELOG
                DCELogOutput(FUNC_E,0,0,pCom->uiConnNum,"socket N",iSaveErrno);
#endif
                break;
        }
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SOCKET_ERR,pCom->iSockfd,iSaveErrno,aszEyeChach);
#endif
        goto ErrExit;
    }

#ifdef DCELOG
    DCELogSoketSet(pCom->iSockfd,ushConAssort);
#endif

#ifdef DCELOG
    DCELogOutput(HOST_N,0,pCom->iSockfd,pCom->uiConnNum,pAgent->aszAgentName,0);
#endif
    pHost = NWgethostbyname(&Nwsktent,pAgent->aszAgentName);
    if (pHost == NULL){
        if (Nwsktent.nse_h_errno == HOST_NOT_FOUND){
            iRetCode = DEHOSTFILE;
        } else {
            iRetCode = DEDFAMTONOVELL;
        }
        close(pCom->iSockfd);
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,GETHOSTBYNAME_ERR,EFAILURE,Nwsktent.nse_h_errno,aszEyeChach);
#endif
#ifdef DCELOG
        DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"NWgethostbyname",Nwsktent.nse_h_errno);
#endif
        goto ErrExit;
    }


    memset(&Addr,0,sizeof(Addr));
    memcpy(&Addr.sin_addr,pHost->h_addr,pHost->h_length);

    NWinet_ntoa(aszIPaddres,Addr.sin_addr);

#ifdef DCELOG
    DCELogOutput(IPADDR,0,pCom->iSockfd,pCom->uiConnNum,aszIPaddres,0);
#endif
#ifdef DCETRC
    *(unsigned long *)aszTRCIPaddres = *(unsigned long *)aszEyeChach;
    memcpy(&aszTRCIPaddres[4],aszIPaddres,8);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,IP_ADDRESS,0,0,aszTRCIPaddres);
    memcpy(&aszTRCIPaddres[4],&aszIPaddres[8],8);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,IP_ADDRESS,0,1,aszTRCIPaddres);
#endif

    pServ = NWgetservbyname(&Nwsktent,aszDFAMServName,aszDFAMProtcol);
    if (pServ == NULL){
        iRetCode = DESERVICESFILE;
        close(pCom->iSockfd);
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,GETSERVICES_ERR,EFAILURE,0,aszEyeChach);
#endif
#ifdef DCELOG
        DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"NWgetservbyname",0);
#endif
        goto ErrExit;
    }
    Addr.sin_family = AF_INET;
    Addr.sin_port = (unsigned short)(pServ->s_port >> 16);


#ifdef DCELOG
    DCELogOutput(PORT_N,0,pCom->iSockfd,pCom->uiConnNum,NULL,ntohl(pServ->s_port));
#endif
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,GETSERVICES_ERR,ESUCCESS,ntohl(pServ->s_port),aszEyeChach);
#endif

    cNonBk = 1;
    iCode = ioctl(pCom->iSockfd,FIONBIO,&cNonBk);
    if(iCode < 0){
        iSaveErrno = errno;
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,IOCTL_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
        DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"ioctl",iSaveErrno);
#endif
        shutdown(pCom->iSockfd,2);
        close(pCom->iSockfd);
        iRetCode = DENONBKIO;
        goto ErrExit;
    }

    uiStartTime = clock();
    for (bLoop = TRUE; bLoop == TRUE;){
        uiNowTime = clock();
        if ((uiNowTime - uiStartTime) / CLOCKS_PER_SEC >= pAgent->uiConnectWaitTime){

            close(pCom->iSockfd);
            iRetCode = DETIMEOUTAGT;
#ifdef DCELOG
            DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"connect timeout",0);
#endif
            goto ErrExit;
        }
        iCode = connect(pCom->iSockfd,(struct sockaddr *)&Addr,sizeof(Addr));
        if (iCode == -1){
            iSaveErrno = errno;
            switch(iSaveErrno) {
                case EINPROGRESS :
                case EALREADY :
                    ThreadSwitch();
                    break;
                case EISCONN :
                    bLoop = FALSE;
                    break;
                case ETIMEDOUT :
                case ECONNREFUSED :
                    close(pCom->iSockfd);
                    iRetCode = DEREFUSEAGT;
#ifdef DCETRC
                    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CONNECT_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                    DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"connect",iSaveErrno);
#endif
                    goto ErrExit;
                case EBADF :
                case ENOTSOCK :
                case EAFNOSUPPORT :
                case EINVAL :
                case EADDRINUSE :
                    close(pCom->iSockfd);
                    iRetCode = DEDFAMDCESYSTEM;
#ifdef DCETRC
                    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CONNECT_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                    DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"connect",iSaveErrno);
#endif
                    goto ErrExit;
                case EADDRNOTAVAIL :
                case ENETUNREACH :
                    close(pCom->iSockfd);
                    iRetCode = DENETWORKENV;
#ifdef DCETRC
                    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CONNECT_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                    DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"connect",iSaveErrno);
#endif
                    goto ErrExit;
                case ENETDOWN :
                    close(pCom->iSockfd);
                    iRetCode = DEUNLOADTCPIP;
                    ResumeMainT();
#ifdef DCETRC
                    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CONNECT_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                    DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"connect",iSaveErrno);
#endif
                    goto ErrExit;
                case EFAULT :
                case ENOBUFS :
                case EACCES :
                    close(pCom->iSockfd);
                    iRetCode = DEDFAMTONOVELL;
#ifdef DCETRC
                    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CONNECT_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                    DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"connect U",iSaveErrno);
#endif
                    goto ErrExit;
                default :
                    close(pCom->iSockfd);
                    iRetCode = DEDFAMTONOVELL;
#ifdef DCETRC
                    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CONNECT_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                    DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"connect N",iSaveErrno);
#endif
                    goto ErrExit;
            }
        }
    }

    iCode = setsockopt(pCom->iSockfd,SOL_SOCKET,SO_SNDBUF,(char *)&pAgent->iCom_SendBuffLen,
             sizeof(pAgent->iCom_SendBuffLen));
    if (iCode != 0){
        iSaveErrno = errno;
        switch(iSaveErrno){
            case EBADF :
            case EINVAL :
            case ENOTSOCK :
            case ENOPROTOOPT :
                iRetCode = DEDFAMDCESYSTEM;
#ifdef DCELOG
                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"setsockopt:SO_SNDBUF",iSaveErrno);
#endif
                break;
            case ENETDOWN :
                iRetCode = DEUNLOADTCPIP;
                pCom->iConErrFlag = iRetCode;
                ResumeMainT();
#ifdef DCELOG
                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"setsockopt:SO_SNDBUF",iSaveErrno);
#endif
                break;
            case EFAULT :
            case ENOBUFS :
                iRetCode = DEDFAMTONOVELL;
#ifdef DCELOG
                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"setsockopt:SO_SNDBUF U",iSaveErrno);
#endif
                break;
            default :
                iRetCode = DEDFAMTONOVELL;
#ifdef DCELOG
                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"setsockopt:SO_SNDBUF N",iSaveErrno);
#endif
                break;
        }
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SETSOCKOPT_S_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
        shutdown(pCom->iSockfd,2);
        close(pCom->iSockfd);
        goto ErrExit;
    }

    iCode = setsockopt(pCom->iSockfd,SOL_SOCKET,SO_RCVBUF,(char *)&pAgent->iCom_SendBuffLen,
            sizeof(pAgent->iCom_SendBuffLen));
    if (iCode != 0){
        iSaveErrno = errno;
        switch(iSaveErrno){
            case EBADF :
            case EINVAL :
            case ENOTSOCK :
            case ENOPROTOOPT :
                iRetCode = DEDFAMDCESYSTEM;
#ifdef DCELOG
                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"setsockopt:SO_RCVBUF",iSaveErrno);
#endif
                break;
            case ENETDOWN :
                iRetCode = DEUNLOADTCPIP;
                pCom->iConErrFlag = iRetCode;
                ResumeMainT();
#ifdef DCELOG
                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"setsockopt:SO_RCVBUF",iSaveErrno);
#endif
            case EFAULT :
            case ENOBUFS :
                iRetCode = DEDFAMTONOVELL;
#ifdef DCELOG
                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"setsockopt:SO_RCVBUF U",iSaveErrno);
#endif
            default :
                iRetCode = DEDFAMTONOVELL;
#ifdef DCELOG
                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"setsockopt:SO_RCVBUF N",iSaveErrno);
#endif
                break;
        }
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SETSOCKOPT_R_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
        shutdown(pCom->iSockfd,2);
        close(pCom->iSockfd);
        goto ErrExit;
    }

    pCom->pAgent = pAgent;
    bCode = ConnectDiscrimReq(pCom,ushConAssort,&iErrCode);

    if (bCode == FALSE){
        iRetCode = iErrCode;
        shutdown(pCom->iSockfd,2);
        close(pCom->iSockfd);
    } else {
        pCom->iConErrFlag = DENOERR;
    }
ErrExit:
    if (iRetCode != DENOERR){
        DCELogSoketReset(pCom->iSockfd);
        pCom->iSockfd = -1;
        pCom->pAgent = NULL;
    }
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,AGTCONNECT_TRACE_E,iRetCode,pCom->iSockfd,aszEyeChach);
    GW_tsh_functrace(pszAgtConnectReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  ConnectDiscrimReq                                                */
/*  name =  Connection establishment requesting routine                      */
/*  func = Sends DFAM Agent a connection identifying request with using a    */
/*         socket descriptor passed from a caller.  Then receives the        */
/*         results or error information from DFAM Agent, and sends them to   */
/*         the caller.  This function does not check if iErrCode is NULL.    */
/*           1. Sets a semaphore.                                            */
/*           2. Sends header information.                                    */
/*           3. Sends Protocol Version, Connection Type, send/receive buffer */
/*              size.                                                        */
/*           4. Receives header information.                                 */
/*           5. Receives Protocol Version.                                   */
/*           6. Frees the semaphore.                                         */
/*           7. Notifies the caller the results.                             */
/*                                                                           */
/*           * This function is a library internal function.                 */
/*                                                                           */
/*  i/o   = pCom        : struct ComTGroup_t *  :inp: thread group table     */
/*          ushConAssort: unsigned short        :inp: connection type        */
/*          piErrCode   : int *                 :out: error information      */
/*  return = TRUE : successful                                               */
/*           FALSE: failed                                                   */
/*  common = ushDFAMGWTranceVer: unsigned short:inp: Gateway protocol version*/
/*  note = error information:                                                */
/*         DENOERR        : normal return                                    */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*                          ----> frees connection (internal error).         */
/*         DEREFUSEAGT    : failed to establish connection                   */
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
BOOL ConnectDiscrimReq(struct ComTGroup_t *pCom,unsigned short ushConAssort,int *piErrCode)
{
    unsigned long   uiStatus;
    BOOL            bCode;
    BOOL            bSemFlag;
    int             iRecvSize;
    int             iCode;
    unsigned short  ushComRsp;
    BOOL            bRetCode = TRUE;
    int             iCtr;
    char            acSendBuf[DDA_HED_LEN+DDA_CON_REQ_SIZE];

    char            acRecvBuf[DDA_CON_RSP_SIZE];

    iCtr = 0;
    bSemFlag = FALSE;

#ifdef DCETRC
    GW_tsh_functrace(pszConnectDiscrimReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CONNECTDISCRIM_TRACE_S,pCom->iSockfd,0,aszEyeChach);
#endif

    *(unsigned short *)&acSendBuf[iCtr] = htons(DDA_CONNECT_COM_REQ);
    iCtr += DDA_HED_COM_LEN;

    *(unsigned short *)&acSendBuf[iCtr] = htons(DDA_RESERVE);
    iCtr += DDA_HED_RESERVE_LEN;

    *(unsigned long *)&acSendBuf[iCtr] = htonl(DDA_CON_REQ_SIZE);
    iCtr += DDA_HED_DATASIZE_LEN;

    *(unsigned short *)&acSendBuf[iCtr] = htons(ushDFAMGWTranceVer);
    iCtr += DDA_CON_REQ_P1_SIZE;

    *(unsigned short *)&acSendBuf[iCtr] = htons(ushConAssort);
    iCtr += DDA_CON_REQ_P2_SIZE;

    *(unsigned long *)&acSendBuf[iCtr] = htonl((unsigned long)pCom->pAgent->iCom_SendBuffLen);
    iCtr += DDA_CON_REQ_P3_SIZE;

    *(unsigned long *)&acSendBuf[iCtr] = htonl(pCom->uiConnNum);
    iCtr += DDA_CON_REQ_P4_SIZE;

    *((unsigned long *)&acSendBuf[iCtr]) = htonl((unsigned long)pCom->pAgent->iCipherType);
    iCtr += DDA_CON_REQ_P5_SIZE;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_HEAD_INF,
                (int)DDA_CONNECT_COM_REQ << 16 | (int)DDA_RESERVE,DDA_CON_REQ_SIZE,aszEyeChach);
#endif
    iCode = AgtSendReq(pCom,acSendBuf,iCtr,piErrCode,iCtr);
    if (iCode == -1){
        bRetCode = FALSE;
        goto ErrExit;
    }

    bCode = AgtRecvHedReq(pCom,&ushComRsp,&iRecvSize,piErrCode);
    if (bCode == FALSE){
        bRetCode = FALSE;

        goto ErrExit;
    }
    switch(ushComRsp){
        case DDA_CONNECT_COM_RSP :
            break;
        default :
            bRetCode = FALSE;
            *piErrCode = DERESPONCEDATA;
        goto ErrExit;
    }

    iCode = AgtRecvReq(pCom,acRecvBuf,DDA_CON_RSP_SIZE,piErrCode,DDA_CON_RSP_SIZE);
    if (iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,RECV_DATA2_ERR,iCode,*piErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }
    iRecvSize -= iCode;

    iCtr = 0;
    uiStatus = ntohl(*(unsigned long *)&acRecvBuf[iCtr]);
    iCtr += DDA_CON_RSP_P1_SIZE;
    switch (uiStatus){
        case 0x00000000 :
            break;
        case 0x00000001 :
            bRetCode = FALSE;
            *piErrCode = DECIPHER;
            break;
        case 0x00000002 :
        case 0x00000003 :
        case 0xFFFFFFFF :
        default :
            bRetCode = FALSE;
            *piErrCode = DEREFUSEAGT;
            break;
    }

    pCom->pAgent->ushDFAMAgtTranceVer = ntohs(*(unsigned short *)&acRecvBuf[iCtr]);
    iCtr += DDA_CON_RSP_P2_SIZE;

    pCom->pAgent->uiOpenFileNum = ntohl(*(unsigned long *)&acRecvBuf[iCtr]);
    iCtr += DDA_CON_RSP_P3_SIZE;

    if (pCom->pAgent->ushDFAMAgtTranceVer != ushDFAMGWTranceVer){

    }
    if(iRecvSize > 0){
        iCode = DustRecv(pCom,iRecvSize,piErrCode);
        if (iCode == -1){
            bRetCode = FALSE;
            goto ErrExit;
        }
    }
ErrExit:
    if (bSemFlag == TRUE){
        SignalLocalSemaphore(pCom->uiCConn_sh);
    }
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CONNECTDISRIM_TRACE_E,bRetCode,*piErrCode,aszEyeChach);
    GW_tsh_functrace(pszConnectDiscrimReq,GW_FUNC_END,bRetCode);
#endif
    return(bRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DustRecv                                                         */
/*  name =  Remaining data processing routine                                */
/*  func = Reads and discards the designated length of data with using the   */
/*         connection of the socket descriptor passed from a caller.         */
/*                                                                           */
/*           * This function is a library internal function.                 */
/*                                                                           */
/*  i/o  = pCom     : struct ComTGroup_t *:i/o: socket descriptor            */
/*         iDustSize: int                 :inp: received data length         */
/*         piErrCode: int *               :out: error information            */
/*  return = -1           : failed                                           */
/*           other than -1: size of the received data (in bytes)             */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : detected line disconnection                      */
/*         DETIMEOUTAGT   : time out                                         */
/*                          ----> frees connection.                          */
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
int DustRecv(struct ComTGroup_t *pCom,int iDustSize,int *piErrCode)
{
    char acWork[BUFSIZ];
    int iSetRecvSize;
    int iGetRecvSize;
    int iTotalRecvSize;

#ifdef DCETRC
    GW_tsh_functrace(pszDustRecvReq,GW_FUNC_START,0);
#endif
    for (iTotalRecvSize = 0; iTotalRecvSize < iDustSize;){
        iSetRecvSize = (iDustSize - iTotalRecvSize > sizeof(acWork)) ? sizeof(acWork) : iDustSize - iTotalRecvSize;
        iGetRecvSize = AgtRecvReq(pCom,acWork,iSetRecvSize,piErrCode,iSetRecvSize);
        if (iGetRecvSize == -1){
            iTotalRecvSize = -1;
            break;
        }
        iTotalRecvSize += iGetRecvSize;
    }
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DUST_RECV_INF,iTotalRecvSize,iDustSize,aszEyeChach);
    GW_tsh_functrace(pszDustRecvReq,GW_FUNC_END,iTotalRecvSize);
#endif
    return(iTotalRecvSize);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  AgentDisConnectReq                                               */
/*  name =  Connection freeing routine                                       */
/*  func =  Frees the connection with DFAM Agent.                            */
/*           1. Disconnects the network I/O.                                 */
/*           2. Closes the socket descriptor.                                */
/*  i/o  = pComTbl : void * :i/o: thread group table                         */
/*  return = See error information in "note."                                */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEUNLOADTCPIP  : TCPIP.NLM was unloaded                           */
/*         DEDFAMDCESYSTEM: internal error                                   */
/*         DEDFAMTONOVELL : errors other than listed in manuals              */
/*                          ----> frees connection.                          */
/*  date =  Jan. 09, 1995 Masakazu Mitsuhashi                                */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int AgentDisConnectReq(void *pComTbl)
{
    int iCode;
    struct ComTGroup_t *pCom;
    int iRetCode = DENOERR;
    int iSaveErrno;

    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszAgentDisConnectReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,AGTDISCONNECT_TRACE_S,pCom->iSockfd,0,aszEyeChach);
#endif
#ifdef DCELOG
    DCELogOutput(DISCON,0,pCom->iSockfd,pCom->uiConnNum,NULL,pCom->iSockfd);
#endif

    if (pCom->iSockfd == -1){
        goto ErrExit;
    }

    iCode = shutdown(pCom->iSockfd,2);
    switch(iCode){
        case -1 :
            iSaveErrno = errno;
            switch(iSaveErrno){
                case ENOTCONN :
#ifdef DCETRC
                    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SHUTDOWN_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                    DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"shutdown(-1)",iSaveErrno);
#endif
                    break;
                case EBADF :
                case ENOTSOCK :
                    iRetCode = DEINVALSOCKET;
#ifdef DCETRC
                    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SHUTDOWN_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                    DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"shutdown(-1)",iSaveErrno);
#endif
                    goto ErrExit;
                case ENETDOWN :
                    iRetCode = DEUNLOADTCPIP;
                    ResumeMainT();
#ifdef DCETRC
                    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SHUTDOWN_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                    DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"shutdown(-1)",iSaveErrno);
#endif
            goto ErrExit;
                default :
                    iRetCode = DEDFAMTONOVELL;
#ifdef DCETRC
                    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SHUTDOWN_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                    DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"shutdown(-1) N",iSaveErrno);
#endif
                    goto ErrExit;
            }
            break;
        case 0 :
            break;
        default :
            iSaveErrno = errno;
            iRetCode = DEDFAMTONOVELL;
#ifdef DCETRC
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SHUTDOWN_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
            DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"shutdown(?) N",iSaveErrno);
#endif
            goto ErrExit;
    }

    iCode = close(pCom->iSockfd);
    switch(iCode){
        case -1 :
            iSaveErrno = errno;
            switch (iSaveErrno){
                case EBADF :
                    iRetCode = DEINVALSOCKET;
#ifdef DCELOG
                    DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"close(-1)",iSaveErrno);
#endif
                    break;
                case ENETDOWN :
                    iRetCode = DEUNLOADTCPIP;
                    ResumeMainT();
#ifdef DCELOG
                    DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"close(-1)",iSaveErrno);
#endif
                    break;
                case ENOSPC :
                    iRetCode = DEDFAMTONOVELL;
#ifdef DCELOG
                    DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"close(-1) U",iSaveErrno);
#endif
                    break;
                default :
                    iRetCode = DEDFAMTONOVELL;
#ifdef DCELOG
                    DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"close(-1) N",iSaveErrno);
#endif
                    break;
            }
#ifdef DCETRC
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CLOSE_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
            goto ErrExit;
        case 0 :
            break;
        default :
            iSaveErrno = errno;
            iRetCode = DEDFAMTONOVELL;
#ifdef DCETRC
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,CLOSE_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
            DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"close(?)",iSaveErrno);
#endif
            goto ErrExit;
    }
ErrExit:
    pCom->pAgent = NULL;
#ifdef DCELOG
    DCELogSoketReset(pCom->iSockfd);
#endif
#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,AGTDISCONNECT_TRACE_E,iRetCode,0,aszEyeChach);
    GW_tsh_functrace(pszAgentDisConnectReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  AgtSendHedReq                                                    */
/*  name =  Header sending routine                                           */
/*  func =  Sends DFAM Agent header.                                         */
/*           1. Converts the command information and sending data size into  */
/*              a network byte order.                                        */
/*           2. Sends the header to DFAM Agent.                              */
/*                                                                           */
/*           * This function is a library internal function.                 */
/*  i/o  = iSockfd      : int           :inp: socket descriptor              */
/*         ushComReq    : unsigned short:inp: command information            */
/*         iSendDataSize: int           :inp: sending data size              */
/*         piErrCode    : int *         :out: error information              */
/*  return = TRUE : successfully sent                                        */
/*           FALSE: failed to send                                           */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : detected line disconnection                      */
/*         DETIMEOUTAGT   : time out                                         */
/*                          ----> frees connection.                          */
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
BOOL AgtSendHedReq(struct ComTGroup_t *pCom,unsigned short ushComReq,int iSendDataSize,int *piErrCode)
{
    char acBuf[DDA_HED_LEN];
    int iSendSize;
    BOOL bRetCode;
    int iCtr;

#ifdef DCETRC
    GW_tsh_functrace(pszAgtSendHedReq,GW_FUNC_START,0);
#endif

    bRetCode = TRUE;
    iCtr = 0;


    *(unsigned short *)&acBuf[iCtr] = htons(ushComReq);
    iCtr += DDA_HED_COM_LEN;

    *(unsigned short *)&acBuf[iCtr] = htons(DDA_RESERVE);
    iCtr += DDA_HED_RESERVE_LEN;

    *(unsigned long *)&acBuf[iCtr] = htonl((unsigned long)iSendDataSize);
    iCtr += DDA_HED_DATASIZE_LEN;


#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_HEAD_INF,
                (int)ushComReq << 16 | (int)DDA_RESERVE,iSendDataSize,aszEyeChach);
#endif
    iSendSize = AgtSendReq(pCom,acBuf,DDA_HED_LEN,piErrCode,DDA_HED_LEN);
    if (iSendSize == -1){
        bRetCode = FALSE;
    }
#ifdef DCETRC
    GW_tsh_functrace(pszAgtSendHedReq,GW_FUNC_END,bRetCode);
#endif
    return(bRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  AgtSendReq                                                       */
/*  name =  Data sending routine                                             */
/*  func = Sends data to DFAM Agent.                                         */
/*           1. Waits the data sending with using select().                  */
/*           2. Sends the data with send().                                  */
/*                                                                           */
/*           * This function is a library internal function.                 */
/*                                                                           */
/*  i/o  = iSockfd      : int           :inp: socket descriptor              */
/*         pcSendData   : char *        :inp: sending data                   */
/*         iSendDataSize: int           :inp: sending data size              */
/*         piErrCode    : int *         :out: error information              */
/*  return = -1           : failed (-1 is set even if an error occurred      */
/*                          after 1 byte or more was sent)                   */
/*           other than -1: sent data size (byte)                            */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : detected line disconnection                      */
/*         DETIMEOUTAGT   : time out                                         */
/*                          ----> frees connection.                          */
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
int AgtSendReq(struct ComTGroup_t *pCom,char *pcSendData,int iSendDataLen,int *piErrCode,int iLogLen)
{
    fd_set  writefds;
    int     iCode;
    int     iSendSize;
    int     iTotalSendSize;
    int     iSetSendSize;
    BOOL    bSendLoop;
    struct timeval timeout;
    char *pcPtr;
    clock_t uiStartTime;
    clock_t uiNowTime;
    clock_t uiSetTime;
    int iSaveErrno;

#ifdef DCETRC
    GW_tsh_functrace(pszAgtSendReq,GW_FUNC_START,0);
#endif

    uiStartTime = clock();
    uiSetTime = (clock_t)pCom->pAgent->iCom_ResponseWaitTime;

    if (iSendDataLen <= 0){
        *piErrCode = DEDFAMDCESYSTEM;
        pCom->iConErrFlag = *piErrCode;
        iTotalSendSize = -1;
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_SIZE_ERR,iSendDataLen,0,aszEyeChach);
#endif
        goto ErrExit;
    }

    iTotalSendSize = 0;
    *piErrCode = DENOERR;
    pcPtr = pcSendData;

    for (bSendLoop = TRUE; bSendLoop && iSendDataLen > 0; ){
        FD_ZERO(&writefds);
        FD_SET(pCom->iSockfd,&writefds);

        uiNowTime = clock();
        if ((uiNowTime - uiStartTime) / CLOCKS_PER_SEC >= uiSetTime){
            *piErrCode = DETIMEOUTAGT;
            iTotalSendSize = -1;
            pCom->iConErrFlag = *piErrCode;
#ifdef DCETRC
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size, SELECT_ERR,iCode,0,aszEyeChach);
#endif
#ifdef DCELOG
            DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"select(0)",0);
#endif
            goto ErrExit;
        }


        timeout.tv_sec = (uiSetTime - (uiNowTime - uiStartTime) / CLOCKS_PER_SEC) < COMMUNICATION_WATCHOVERTIME ?
                         (uiSetTime - (uiNowTime - uiStartTime) / CLOCKS_PER_SEC) : COMMUNICATION_WATCHOVERTIME;

        timeout.tv_usec = 0;

        iCode = select(pCom->iSockfd+1,(fd_set *)0,&writefds,(fd_set *)0,&timeout);
        switch(iCode){
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
                iTotalSendSize = -1;
                pCom->iConErrFlag = *piErrCode;
#ifdef DCETRC
                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SELECT_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
                goto ErrExit;
            case 0 :
                uiNowTime = clock();
                if ((uiNowTime - uiStartTime) / CLOCKS_PER_SEC >= uiSetTime){
                    iSaveErrno = errno;
                    *piErrCode = DETIMEOUTAGT;
                    iTotalSendSize = -1;
                    pCom->iConErrFlag = *piErrCode;
#ifdef DCETRC
                    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SELECT_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                    DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"select(0)",iSaveErrno);
#endif
                    goto ErrExit;
                }
                break;
            case 1 :
                iSetSendSize = (iSendDataLen < pCom->pAgent->iCom_SendBuffLen) ?
                                iSendDataLen : pCom->pAgent->iCom_SendBuffLen;

                iSendSize = send(pCom->iSockfd,pcPtr,iSetSendSize,0);
                switch(iSendSize){
                    case -1 :
                        iSaveErrno = errno;
                        switch(errno){
                            case EWOULDBLOCK :
#ifdef DCELOG
                                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"send(-1)",iSaveErrno);
#endif
                                break;
                            case EBADF :
                            case ENOTSOCK :
                                *piErrCode = DEINVALSOCKET;
                                iTotalSendSize = -1;
                                pCom->iConErrFlag = *piErrCode;
#ifdef DCETRC
                                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_ERR,iSendSize,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"send(-1)",iSaveErrno);
#endif
                                goto ErrExit;
                            case EMSGSIZE :
                            case ENOBUFS  :
                                *piErrCode = DEDFAMDCESYSTEM;
                                iTotalSendSize = -1;
                                pCom->iConErrFlag = *piErrCode;
#ifdef DCETRC
                                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_ERR,iSendSize,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"send(-1)",iSaveErrno);
#endif
                                goto ErrExit;
                            case ENETDOWN :
                                *piErrCode = DEUNLOADTCPIP;
                                iTotalSendSize = -1;
                                pCom->iConErrFlag = *piErrCode;
#ifdef DCETRC
                                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_ERR,iSendSize,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"send(-1)",iSaveErrno);
#endif
                                ResumeMainT();
                                goto ErrExit;
                            case EPIPE :

                                *piErrCode = DEREFUSEAGT;
                                iTotalSendSize = -1;
                                pCom->iConErrFlag = *piErrCode;
#ifdef DCETRC
                                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_ERR,iSendSize,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"send(-1) U",iSaveErrno);
#endif
                                goto ErrExit;
                            case EFAULT :
                            case EACCES :
                            case EINVAL :
                            case ENOTCONN :
                            case EISCONN :
                            case EOPNOTSUPP :
                            case ENETUNREACH :
                                *piErrCode = DEDFAMTONOVELL;
                                iTotalSendSize = -1;
                                pCom->iConErrFlag = *piErrCode;
#ifdef DCETRC
                                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_ERR,iSendSize,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"send(-1) U",iSaveErrno);
#endif
                                goto ErrExit;
                            default :
                                *piErrCode = DEDFAMTONOVELL;
                                iTotalSendSize = -1;
                                pCom->iConErrFlag = *piErrCode;
#ifdef DCETRC
                                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_ERR,iSendSize,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"send(-1) N",iSaveErrno);
#endif
                                goto ErrExit;
                        }
                        break;
                    case 0 :
                        iSaveErrno = errno;
                        *piErrCode = DEREFUSEAGT;
                        iTotalSendSize = -1;
                        pCom->iConErrFlag = *piErrCode;
#ifdef DCETRC
                        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_ERR,iSendSize,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                        DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"send(0)",iSaveErrno);
#endif
                        goto ErrExit;
                    default :
                        iTotalSendSize += iSendSize;
                        iSendDataLen -= iSendSize;
                        pcPtr += iSendSize;
                        if (iSendDataLen <= 0){
                            bSendLoop = FALSE;
                        }
                        break;
                }
                break;
            default :
                iSaveErrno = errno;
                *piErrCode = DEDFAMDCESYSTEM;
                iTotalSendSize = -1;
                pCom->iConErrFlag = *piErrCode;
#ifdef DCETRC
                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SELECT_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"select(?)",iSaveErrno);
#endif
                goto ErrExit;
        }
    }
#ifdef DCELOG
    DCELogOutput(SDDATA,iLogLen,pCom->iSockfd,pCom->uiConnNum,pcSendData,0);
#endif
#ifdef DCETRC
    GW_tsh_functrace(pszAgtSendReq,GW_FUNC_END,iTotalSendSize);
#endif

ErrExit:
    return(iTotalSendSize);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  AgtRecvHedReq                                                    */
/*  name =  Header information receiving routine                             */
/*  func = Receives a header information from DFAM Agent.                    */
/*           1. Receives data from DFAM Agent.                               */
/*           2. Converts the command response and received data size into    */
/*              a host byte order.                                           */
/*           3. Returns information to a caller.                             */
/*                                                                           */
/*           * This function is a library internal function.                 */
/*                                                                           */
/*  i/o  = iSockfd      :int             :inp: socket descriptor             */
/*         pushComRsp   :unsigned short *:out: command response information  */
/*         piRecvDataLen:int *           :out: received data size            */
/*         piErrCode    :int *           :out: error information             */
/*  return = TRUE  : successfully received                                   */
/*           FALSE : receiving failed                                        */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : detected line disconnection                      */
/*         DETIMEOUTAGT   : time out                                         */
/*                          ----> frees connection.                          */
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
BOOL AgtRecvHedReq(struct ComTGroup_t *pCom,unsigned short *pushComRsp,int *piRecvDataLen,int *piErrCode)
{
    char acHedBuf[DDA_HED_LEN];
    int iCtr;
    int bRetCode;
    int iRecvSize;

#ifdef DCETRC
    GW_tsh_functrace(pszAgtRecvHedReq,GW_FUNC_START,0);
#endif

    iRecvSize = AgtRecvReq(pCom,acHedBuf,DDA_HED_LEN,piErrCode,DDA_HED_LEN);
    if (iRecvSize == -1){
        bRetCode = FALSE;
        goto ErrExit;
    }

    bRetCode = TRUE;
    iCtr = 0;


    *pushComRsp = ntohs(*(unsigned short *)&acHedBuf[iCtr]);
    iCtr += DDA_HED_COM_LEN;

    iCtr += DDA_HED_RESERVE_LEN;

    *piRecvDataLen = (int)ntohl(*(unsigned long *)&acHedBuf[iCtr]);
    iCtr += DDA_HED_DATASIZE_LEN;

#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,RECV_HEAD_INF,
                (int)*pushComRsp << 16 | (int)ntohs(*(unsigned short *)&acHedBuf[DDA_HED_COM_LEN])
                ,*piRecvDataLen,aszEyeChach);
#endif
ErrExit:
#ifdef DCETRC
    GW_tsh_functrace(pszAgtRecvHedReq,GW_FUNC_END,bRetCode);
#endif
    return(bRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  AgtRecvDefData                                                   */
/*  name =  DFAM Agent data receiving routine                                */
/*  func = Receives a return value and error information from DFAM Agent.    */
/*           1. Receives data from DFAM Agent.                               */
/*           2. Converts the return value and error information into         */
/*              a host byte order.                                           */
/*           3. Returns information to a caller.                             */
/*                                                                           */
/*           * This function is a library internal function.                 */
/*                                                                           */
/*  i/o  = iSockfd      :int             :inp: socket descriptor             */
/*         puiAgtRetCode: unsigned long *:out: DFAM Agent return value       */
/*         puiAgtErrCode: unsigned long *:out: DFAM Agent error information  */
/*         piErrCode    :int *           :out: error information             */
/*  return = TRUE : successful                                               */
/*           FALSE: failed                                                   */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : detected line disconnection                      */
/*         DETIMEOUTAGT   : time out                                         */
/*                          ----> frees connection.                          */
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
BOOL AgtRecvDefData(struct ComTGroup_t *pCom,unsigned long *puiAgtRetCode,unsigned long *puiAgtErrCode,int *piErrCode)
{
    BOOL bRetCode;
    char acRecvBuf[DDA_DEFAULT_RECV_SIZE];
    int iCode;
    int iCtr;

    bRetCode = FALSE;
#ifdef DCETRC
    GW_tsh_functrace(pszAgtRecvDefData,GW_FUNC_START,0);
#endif

    iCode = AgtRecvReq(pCom,acRecvBuf,DDA_DEFAULT_RECV_SIZE,piErrCode,DDA_DEFAULT_RECV_SIZE);
    if (iCode == -1){
        goto ErrExit;
    }

    iCtr = 0;
    *puiAgtRetCode = ntohl(*(unsigned long *)&acRecvBuf[iCtr]);
    iCtr += DDA_RETURN_SIZE;

    *puiAgtErrCode = ntohl(*(unsigned long *)&acRecvBuf[iCtr]);
    iCtr += DDA_ERRNO_SIZE;

    bRetCode = TRUE;

#ifdef DCETRC
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,RECV_DEFAULT_INF,*puiAgtRetCode,*puiAgtErrCode,aszEyeChach);
#endif
ErrExit:
#ifdef DCETRC
    GW_tsh_functrace(pszAgtRecvDefData,GW_FUNC_END,bRetCode);
#endif
    return(bRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  AgtRecvReq                                                       */
/*  name =  Data receiving routine                                           */
/*  func = Receives data from DFAM Agent.                                    */
/*           1. Waits data receiving with using select().                    */
/*           2. Receives data with using recv().                             */
/*                                                                           */
/*           * This function is a library internal function.                 */
/*                                                                           */
/*  i/o  = iSockfd      : int           :inp: socket descriptor              */
/*         pcRecvData   : char *        :out: buffer for data receive        */
/*         iRecvDataLen : int           :inp: receiving data size            */
/*         piErrCode    : int *         :out: error information              */
/*  return = -1           : failed (-1 is set even if an error occurred      */
/*                          after 1 byte or more was received)               */
/*           other than -1: received data size (byte)                        */
/*  common = none                                                            */
/*  note = error information:                                                */
/*         DENOERR        : no error                                         */
/*         DEINVALSOCKET  : invalid socket descriptor                        */
/*         DEREFUSEAGT    : detected line disconnection                      */
/*         DETIMEOUTAGT   : time out                                         */
/*                          ----> frees connection.                          */
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
int AgtRecvReq(struct ComTGroup_t *pCom,char *pcRecvData,int iRecvDataLen,int *piErrCode,int iLogLen)
{
    fd_set readfds;
    int iCode;
    int iRecvSize;
    int iTotalRecvSize;
    int iSetRecvSize;
    BOOL bRecvLoop;
    struct timeval timeout;
    char *pcPtr;
    clock_t uiStartTime;
    clock_t uiNowTime;
    clock_t uiSetTime;
    int iSaveErrno;

#ifdef DCETRC
    GW_tsh_functrace(pszAgtRecvReq,GW_FUNC_START,0);
#endif

    uiStartTime = clock();
    uiSetTime = (clock_t)pCom->pAgent->iCom_ResponseWaitTime;

    if (iRecvDataLen <= 0){
        *piErrCode = DEDFAMDCESYSTEM;
        pCom->iConErrFlag = *piErrCode;
        iTotalRecvSize = -1;
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size, RECV_SIZE_ERR,iRecvDataLen,0,aszEyeChach);
#endif
        goto ErrExit;
    }

    iTotalRecvSize = 0;
    *piErrCode = DENOERR;
    pcPtr = pcRecvData;

    for (bRecvLoop = TRUE; bRecvLoop && iRecvDataLen > 0; ){
        FD_ZERO(&readfds);
        FD_SET(pCom->iSockfd,&readfds);

        uiNowTime = clock();
        if ((uiNowTime - uiStartTime) / CLOCKS_PER_SEC >= uiSetTime){
            *piErrCode = DETIMEOUTAGT;
            iTotalRecvSize = -1;
            pCom->iConErrFlag = *piErrCode;
#ifdef DCETRC
            GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size, SELECT_ERR,iCode,0,aszEyeChach);
#endif
#ifdef DCELOG
            DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"select(0)",0);
#endif
            goto ErrExit;
        }
        timeout.tv_sec = (uiSetTime - (uiNowTime - uiStartTime) / CLOCKS_PER_SEC) < COMMUNICATION_WATCHOVERTIME ?
                         (uiSetTime - (uiNowTime - uiStartTime) / CLOCKS_PER_SEC) : COMMUNICATION_WATCHOVERTIME;

        timeout.tv_usec = 0;
        iCode = select(pCom->iSockfd+1,&readfds,(fd_set *)0,(fd_set *)0,&timeout);
        switch (iCode){
            case -1 :
                iSaveErrno = errno;
                switch(errno){
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
                iTotalRecvSize = -1;
                pCom->iConErrFlag = *piErrCode;
#ifdef DCETRC
                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size, SELECT_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
                goto ErrExit;
            case 0 :
                uiNowTime = clock();
                if ((uiNowTime - uiStartTime) / CLOCKS_PER_SEC >= uiSetTime){
                    *piErrCode = DETIMEOUTAGT;
                    iTotalRecvSize = -1;
                    pCom->iConErrFlag = *piErrCode;
#ifdef DCETRC
                    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size, SELECT_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                    DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"select(0)",iSaveErrno);
#endif
                    goto ErrExit;
                }
                break;
            case 1 :
                iSetRecvSize = (iRecvDataLen < pCom->pAgent->iCom_SendBuffLen) ?
                                iRecvDataLen : pCom->pAgent->iCom_SendBuffLen;

                iRecvSize = recv(pCom->iSockfd,pcPtr,iSetRecvSize,0);
                switch(iRecvSize){
                    case -1 :
                        iSaveErrno = errno;
                        switch(errno){
                            case EWOULDBLOCK :
#ifdef DCELOG
                                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"recv(-1)",iSaveErrno);
#endif
                                break;
                            case EBADF :
                            case ENOTSOCK :
                            case ENOTCONN :
                                *piErrCode = DEINVALSOCKET;
                                pCom->iConErrFlag = *piErrCode;
                                iTotalRecvSize = -1;
#ifdef DCETRC
                                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size, RECV_ERR,iRecvSize,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"recv(-1)",iSaveErrno);
#endif
                                goto ErrExit;
                            case EOPNOTSUPP :
                            case EINVAL :
                                *piErrCode = DEDFAMDCESYSTEM;
                                pCom->iConErrFlag = *piErrCode;
                                iTotalRecvSize = -1;
#ifdef DCETRC
                                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size, RECV_ERR,iRecvSize,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"recv(-1)",iSaveErrno);
#endif
                                goto ErrExit;
                            case ENETDOWN :
                                *piErrCode = DEUNLOADTCPIP;
                                pCom->iConErrFlag = *piErrCode;
                                ResumeMainT();
                                iTotalRecvSize = -1;
#ifdef DCETRC
                                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size, RECV_ERR,iRecvSize,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"recv(-1)",iSaveErrno);
#endif
                                goto ErrExit;
                            case ECONNRESET :

                                *piErrCode = DEREFUSEAGT;
                                pCom->iConErrFlag = *piErrCode;
                                iTotalRecvSize = -1;
#ifdef DCETRC
                                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size, RECV_ERR,iRecvSize,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"recv(-1) U",iSaveErrno);
#endif
                                goto ErrExit;
                            case ETIMEDOUT :

                                *piErrCode = DETIMEOUTAGT;
                                pCom->iConErrFlag = *piErrCode;
                                iTotalRecvSize = -1;
#ifdef DCETRC
                                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size, RECV_ERR,iRecvSize,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"recv(-1) U",iSaveErrno);
#endif
                                goto ErrExit;
                            case EFAULT :
                            case ENOBUFS :
                                *piErrCode = DEDFAMTONOVELL;
                                pCom->iConErrFlag = *piErrCode;
                                iTotalRecvSize = -1;
#ifdef DCETRC
                                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size, RECV_ERR,iRecvSize,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"recv(-1) U",iSaveErrno);
#endif
                                goto ErrExit;
                            default :
                                *piErrCode = DEDFAMTONOVELL;
                                pCom->iConErrFlag = *piErrCode;
                                iTotalRecvSize = -1;
#ifdef DCETRC
                                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size, RECV_ERR,iRecvSize,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"recv(-1) N",iSaveErrno);
#endif
                                goto ErrExit;
                        }
                        break;
                    case 0 :
                        iSaveErrno = errno;
                        *piErrCode = DEREFUSEAGT;
                        pCom->iConErrFlag = *piErrCode;
                        iTotalRecvSize = -1;
#ifdef DCETRC
                        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size, RECV_ERR,iRecvSize,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                        DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"recv(0)",iSaveErrno);
#endif
                        goto ErrExit;
                    default :
                        iTotalRecvSize += iRecvSize;
                        iRecvDataLen -= iRecvSize;
                        pcPtr += iRecvSize;
                        if (iRecvDataLen <= 0){
                            bRecvLoop = FALSE;
                        }
                        break;
                }
                break;
            default :
                iSaveErrno = errno;
                iTotalRecvSize = -1;
                *piErrCode = DEDFAMDCESYSTEM;
                pCom->iConErrFlag = *piErrCode;
#ifdef DCETRC
                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size, SELECT_ERR,iCode,iSaveErrno,aszEyeChach);
#endif
#ifdef DCELOG
                DCELogOutput(FUNC_E,0,pCom->iSockfd,pCom->uiConnNum,"select(?)",iSaveErrno);
#endif
                goto ErrExit;
        }
    }

#ifdef DCELOG
    DCELogOutput(RVDATA,iLogLen,pCom->iSockfd,pCom->uiConnNum,pcRecvData,0);
#endif
#ifdef DCETRC
    GW_tsh_functrace(pszAgtRecvReq,GW_FUNC_END,iTotalRecvSize);
#endif

ErrExit:
    return(iTotalRecvSize);
}
