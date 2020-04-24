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
 * $Log: dcomacl.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:08  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:38  marty]
 *
 * Revision 1.1.2.1  1996/02/21  06:38:07  takano
 * 	First Submit
 * 	[1996/02/21  06:38:02  takano]
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
/*  ID   =  DFSgetacl_syscallReq                                             */
/*  name =  DFS ACL retrieval routine                                        */
/*  func = Sends DFAM Agent a DFS ACL retrieval request with using the       */
/*         file path name, options, and ACL length passed from the caller.   */
/*              Then receives the results and error information              */
/*         from DFAM Agent to send them to the caller.  Does not check the   */
/*         contents of option, acl, length, DFSErrno, and pComTbl.           */
/*           1. In the case that pathname=NULL or the pathname length is 0,  */
/*              this function does not send a request to DFAM Agent, but     */
/*              returns DENOENT as error information.                        */
/*              In the case that the pathname length exceeds DGW_PATH_MAX    */
/*              (1024), this function returns DENAMETOOLONG as error         */
/*              information.                                                 */
/*           2. Sets a semaphore.                                            */
/*           3. Sends the header information.                                */
/*           4. Sends the ACL operation mode, options, path name length,     */
/*              path name, and ACL length.                                   */
/*           5. Receives the header information.                             */
/*           6. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           7. Gets the ACL length and ACL.                                 */
/*           8. Frees the semaphore.                                         */
/*           9. Sets the results to parameters, and returns to the caller.   */
/*                                                                           */
/*  i/o  = pathname: char *         :inp: DFS path name                      */
/*         option  : unsigned short :inp: options                            */
/*         acl     : char *         :out: ACL information setting area       */
/*         length  : int *          :i/o: acl length                         */
/*         DFSErrno: int *          :out: DFS error information area         */
/*         pComTbl : void *         :i/o: thread group table                 */
/*                                                                           */
/*  return =  1 : re-send request (no enough buffer)                         */
/*            0 : successful                                                 */
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
/*  date =  Jan. 09, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFSgetacl_syscallReq(char *pathname,unsigned short option,char *acl,int *length,int *DFSErrno,
    void *pComTbl)
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
    unsigned long   uiNLength;
    struct ComTGroup_t *pCom;
    char            acSendBuf[DDA_ACL_REQ_P1_SIZE+DDA_ACL_REQ_P2_SIZE+DDA_ACL_REQ_L3_SIZE];


    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSgetacl_syscallReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSGETSYSCALL_TRACE_S,pCom->iSockfd,0,aszEyeChach);
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


    tmPathLen = PathChk(pathname,&iErrCode);
    if (tmPathLen == 0xFFFFFFFF){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,PATH_ERR,tmPathLen,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iTotalSendSize = DDA_ACL_REQ_P1_SIZE + DDA_ACL_REQ_P2_SIZE + DDA_ACL_REQ_L3_SIZE + tmPathLen + DDA_ACL_REQ_P4_SIZE;

    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_AFSSYSCALL_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    iPtrCtr = 0;
    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl(DVNX_OPCODE_GETACL);
    iPtrCtr += DDA_ACL_REQ_P1_SIZE;

    *(unsigned short *)&acSendBuf[iPtrCtr] = htons(option);
    iPtrCtr += DDA_ACL_REQ_P2_SIZE;

    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)tmPathLen);
    iPtrCtr += DDA_ACL_REQ_L3_SIZE;

    iCode = AgtSendReq(pCom,acSendBuf,iPtrCtr,&iErrCode,iPtrCtr);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA2_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iCode = AgtSendReq(pCom,pathname,tmPathLen,&iErrCode,tmPathLen);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA3_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    uiNLength = htonl((unsigned long)*length);

    iCode = AgtSendReq(pCom,(char *)&uiNLength,DDA_ACL_REQ_P4_SIZE,&iErrCode,DDA_ACL_REQ_P4_SIZE);
    if(iCode == -1){
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
        case DDA_AFSSYSCALL_COM_RSP :
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

    switch (uiAgtRetCode){
        case 0x00000000 :
            iCode = AgtRecvReq(pCom,(char *)&uiNLength,DDA_ACL_RSP_L3_SIZE,&iErrCode,DDA_ACL_RSP_L3_SIZE);
            if (iCode == -1){
#ifdef DCETRC
                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,RECV_DATA3_ERR,iCode,iErrCode,aszEyeChach);
#endif
                goto ErrExit;
            }
            iAgtSendSize -= iCode;
            *length = (int)htonl(uiNLength);

            iCode = AgtRecvReq(pCom,acl,*length,&iErrCode,*length);
            if (iCode == -1){
#ifdef DCETRC
                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,RECV_DATA4_ERR,iCode,iErrCode,aszEyeChach);
#endif
                goto ErrExit;
            }
            iAgtSendSize -= iCode;
            ACLChanghton(acl);
            break;
        case 0x00000001 :
            iCode = AgtRecvReq(pCom,(char *)&uiNLength,DDA_ACL_RSP_L3_SIZE,&iErrCode,DDA_ACL_RSP_L3_SIZE);
            if (iCode == -1){
#ifdef DCETRC
                GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,RECV_DATA3_ERR,iCode,iErrCode,aszEyeChach);
#endif
                goto ErrExit;
            }
            iAgtSendSize -= iCode;
            *length = (int)htonl(uiNLength);
            break;
        default :
            break;
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
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSGETACLSYSCALL_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSgetacl_syscallReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  ACLChanghton                                                     */
/*  name =  ACL byte order conversion routine                                */
/*  func =  Converts the byte order of the ACL information.                  */
/*          (from the network byte order to the host byte order)             */
/*  i/o  = pcAcl: char * : i/o : ACL information                             */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note =                                                                   */
/*  +---------+--------+--------+--------------------------+--------------+  */
/*  |number of|prmssion| entry  |        length of         |  user/group  |  */
/*  |ACL entry|  bit   |  type  |     user/group name      |     name     |  */
/*  +---------+--------+--------+--------------------------+--------------+  */
/*  |  4bytes | 4bytes | 2bytes |   4bytes (Name is omitted| not a target |  */
/*  |         |        |        | if this is 0xFFFFFFFF)   | of conversion|  */
/*  +---------+--------+--------+--------------------------+--------------+  */
/*            |<------   repeating for the number of ACL entries   ------>|  */
/*                                                                           */
/*  date =  Jan. 09, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void ACLChanghton(char *pcAcl)
{
    unsigned int uiACLEntryCtr;
    unsigned int uiUsrNameLen;




    *(unsigned long *)pcAcl = ntohl(*(unsigned long *)pcAcl);
    uiACLEntryCtr =  *(unsigned int *)pcAcl;
    pcAcl += sizeof(unsigned int);

    for ( ; uiACLEntryCtr > 0 ; uiACLEntryCtr--){
        *(unsigned int *)pcAcl = (unsigned int)ntohl(*(unsigned long *)pcAcl);
        pcAcl += sizeof(unsigned int);

        *(unsigned short *)pcAcl = ntohs(*(unsigned short *)pcAcl);
        pcAcl += sizeof(unsigned short);

        uiUsrNameLen = *(unsigned int *)pcAcl = (unsigned int)ntohl(*(unsigned long *)pcAcl);
        pcAcl += sizeof(unsigned int);

        if (uiUsrNameLen != 0xFFFFFFFF){
            pcAcl += uiUsrNameLen;
        }
    }



}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSsetacl_syscallReq                                             */
/*  name =  DFS ACL setting routine                                          */
/*  func = Sends DFAM Agent a DFS ACL set request with using the file path   */
/*         name, options, ACL length, and ACL passed from the caller.        */
/*              Then receives the results and error information              */
/*         from DFAM Agent to send them to the caller.  Does not check the   */
/*         contents of acl, option, length, DFSErrno, and pComTbl.           */
/*           1. In the case that pathname=NULL or the pathname length is 0,  */
/*              this function does not send a request to DFAM Agent, but     */
/*              returns DENOENT as error information.                        */
/*              In the case that the pathname length exceeds DGW_PATH_MAX    */
/*              (1024), this function returns DENAMETOOLONG as error         */
/*              information.                                                 */
/*           2. Sets a semaphore.                                            */
/*           3. Sends the header information.                                */
/*           4. Sends the ACL operation mode, options, path name length,     */
/*              path name, ACL length, and ACL.                              */
/*           5. Receives the header information.                             */
/*           6. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           7. Frees the semaphore.                                         */
/*           8. Sets the results to parameters, and returns to the caller.   */
/*                                                                           */
/*         *1: "length" must be 1 or higher.                                 */
/*                                                                           */
/*  i/o  = pathname: char *         :inp: DFS path name                      */
/*         option  : unsigned short :inp: options                            */
/*         acl     : char *         :inp: ACL information                    */
/*         length  : int *          :inp: acl length                         */
/*         DFSErrno: int *          :out: DFS error information              */
/*         pComTbl : void *         :i/o: thread group table                 */
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
/*  date =  Jan. 09, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFSsetacl_syscallReq(char *pathname,unsigned short option,char *acl,int *length,int *DFSErrno,
    void *pComTbl)
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
    unsigned long   uiNLength;
    struct ComTGroup_t *pCom;
    char            acSendBuf[DDA_ACL_REQ_P1_SIZE+DDA_ACL_REQ_P2_SIZE+DDA_ACL_REQ_L3_SIZE];


    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSsetacl_syscallReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSSETSYSCALL_TRACE_S,pCom->iSockfd,0,aszEyeChach);
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


    tmPathLen = PathChk(pathname,&iErrCode);
    if (tmPathLen == 0xFFFFFFFF){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,PATH_ERR,tmPathLen,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iTotalSendSize = DDA_ACL_REQ_P1_SIZE + DDA_ACL_REQ_P2_SIZE + DDA_ACL_REQ_L3_SIZE + tmPathLen +
        DDA_ACL_REQ_P4_SIZE + *length;
    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_AFSSYSCALL_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    iPtrCtr = 0;
    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl(DVNX_OPCODE_SETACL);
    iPtrCtr += DDA_ACL_REQ_P1_SIZE;

    *(unsigned short *)&acSendBuf[iPtrCtr] = htons(option);
    iPtrCtr += DDA_ACL_REQ_P2_SIZE;

    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)tmPathLen);
    iPtrCtr += DDA_ACL_REQ_L3_SIZE;

    iCode = AgtSendReq(pCom,acSendBuf,iPtrCtr,&iErrCode,iPtrCtr);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA2_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iCode = AgtSendReq(pCom,pathname,tmPathLen,&iErrCode,tmPathLen);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA3_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    uiNLength = htonl((unsigned long)*length);

    iCode = AgtSendReq(pCom,(char *)&uiNLength,DDA_ACL_REQ_P4_SIZE,&iErrCode,DDA_ACL_REQ_P4_SIZE);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA4_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    *(unsigned int *)acl = (unsigned int)htonl(*(unsigned long *)acl);
    *(unsigned int *)(acl+sizeof(unsigned int)) = (unsigned int)ntohl(*(unsigned long *)(acl+sizeof(unsigned int)));


    iCode = AgtSendReq(pCom,acl,*length,&iErrCode,*length);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA5_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    bCode = AgtRecvHedReq(pCom,&ushComRsp,&iAgtSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }
    switch(ushComRsp){
        case DDA_AFSSYSCALL_COM_RSP :
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
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSSETACLSYSCALL_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSsetacl_syscallReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSdelacl_syscallReq                                             */
/*  name =  DFS ACL (ser or group) deletion routine                          */
/*  func = Sends DFAM Agent a DFS ACL deletion request with using the file   */
/*         path name, options, ACL length, and ACL passed from the caller.   */
/*              Then receives the results and error information              */
/*         from DFAM Agent to send them to the caller.  Does not check the   */
/*         contents of acl, option, length, DFSErrno, and pComTbl.           */
/*           1. In the case that pathname=NULL or the pathname length is 0,  */
/*              this function does not send a request to DFAM Agent, but     */
/*              returns DENOENT as error information.                        */
/*              In the case that the pathname length exceeds DGW_PATH_MAX    */
/*              (1024), this function returns DENAMETOOLONG as error         */
/*              information.                                                 */
/*           2. Sets a semaphore.                                            */
/*           3. Sends the header information.                                */
/*           4. Sends the ACL operation mode, options, path name length,     */
/*              path name, ACL length, and ACL.                              */
/*           5. Receives the header information.                             */
/*           6. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           7. Frees the semaphore.                                         */
/*           8. Sets the results to parameters, and returns to the caller.   */
/*                                                                           */
/*         *1: "length" must be 1 or higher.                                 */
/*                                                                           */
/*  i/o  = pathname: char *         :inp: DFS path name                      */
/*         option  : unsigned short :inp: options                            */
/*         acl     : char *         :inp: ACL information                    */
/*         length  : int *          :inp: acl length                         */
/*         DFSErrno: int *          :out: DFS error information              */
/*         pComTbl : void *         :i/o: thread group table                 */
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
/*  date =  Jan. 09, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DFSdelacl_syscallReq(char *pathname,unsigned short option,char *acl,int *length,int *DFSErrno,
    void *pComTbl)
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
    unsigned long   uiNLength;
    struct ComTGroup_t *pCom;
    char            acSendBuf[DDA_ACL_REQ_P1_SIZE+DDA_ACL_REQ_P2_SIZE+DDA_ACL_REQ_L3_SIZE];


    pCom = (struct ComTGroup_t *)pComTbl;
#ifdef DCETRC
    GW_tsh_functrace(pszDFSdelacl_syscallReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSDELSYSCALL_TRACE_S,pCom->iSockfd,0,aszEyeChach);
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


    tmPathLen = PathChk(pathname,&iErrCode);
    if (tmPathLen == 0xFFFFFFFF){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,PATH_ERR,tmPathLen,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iTotalSendSize = DDA_ACL_REQ_P1_SIZE + DDA_ACL_REQ_P2_SIZE + DDA_ACL_REQ_L3_SIZE + tmPathLen +
        DDA_ACL_REQ_P4_SIZE + *length;
    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_AFSSYSCALL_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    iPtrCtr = 0;
    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl(DVNX_OPCODE_DELACL);
    iPtrCtr += DDA_ACL_REQ_P1_SIZE;

    *(unsigned short *)&acSendBuf[iPtrCtr] = htons(option);
    iPtrCtr += DDA_ACL_REQ_P2_SIZE;

    *(unsigned long *)&acSendBuf[iPtrCtr] = htonl((unsigned long)tmPathLen);
    iPtrCtr += DDA_ACL_REQ_L3_SIZE;

    iCode = AgtSendReq(pCom,acSendBuf,iPtrCtr,&iErrCode,iPtrCtr);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA2_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iCode = AgtSendReq(pCom,pathname,tmPathLen,&iErrCode,tmPathLen);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA3_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    uiNLength = htonl((unsigned long)*length);

    iCode = AgtSendReq(pCom,(char *)&uiNLength,DDA_ACL_REQ_P4_SIZE,&iErrCode,DDA_ACL_REQ_P4_SIZE);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA4_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    *(unsigned int *)acl = (unsigned int)htonl(*(unsigned long *)acl);


    iCode = AgtSendReq(pCom,acl,*length,&iErrCode,*length);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA5_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    bCode = AgtRecvHedReq(pCom,&ushComRsp,&iAgtSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }
    switch(ushComRsp){
        case DDA_AFSSYSCALL_COM_RSP :
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
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSDELACLSYSCALL_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSdelacl_syscallReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  DFSrightsReq                                                     */
/*  name =  Getting login users effective rights                             */
/*  func = Sends DFAM Agent a "login user's effective rights inquire" request*/
/*         with using the path name to retrieve the rights which is passed   */
/*         from the caller.                                                  */
/*              Then receives the results and error information              */
/*         from DFAM Agent to send them to the caller.  Does not check the   */
/*         contents of DFSErrno, and pComTbl.                                */
/*           1. In the case that pathname=NULL or the pathname length is 0,  */
/*              this function does not send a request to DFAM Agent, but     */
/*              returns DENOENT as error information.                        */
/*              In the case that the pathname length exceeds DGW_PATH_MAX    */
/*              (1024), this function returns DENAMETOOLONG as error         */
/*              information.                                                 */
/*           2. Sets a semaphore.                                            */
/*           3. Sends the header information.                                */
/*           4. Sends the path name length, and path name.                   */
/*           5. Receives the header information.                             */
/*           6. Receives a return value and error information (does not      */
/*              check if the contents are correct).                          */
/*           7. Frees the semaphore.                                         */
/*           8. Sets the results to parameters, and returns to the caller.   */
/*                                                                           */
/*  i/o  = pathname: char * :inp: DFS path name                              */
/*         DFSErrno: int *  :out: DFS error information                      */
/*         pComTbl : void * :i/o: thread group table                         */
/*                                                                           */
/*  return = 0xFFFFFFFF: failure                                             */
/*           others    : success (logical OR of the rights)                  */
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
unsigned int DFSrightsReq(char *pathname,int *DFSErrno,void *pComTbl)
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
    GW_tsh_functrace(pszDFSrightsReq,GW_FUNC_START,0);
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSRIGHTS_TRACE_S,pCom->iSockfd,0,aszEyeChach);
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


    tmPathLen = PathChk(pathname,&iErrCode);
    if (tmPathLen == 0xFFFFFFFF){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,PATH_ERR,tmPathLen,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iTotalSendSize = DDA_RIGHTS_REQ_L1_SIZE + tmPathLen;


    WaitOnLocalSemaphore(pCom->uiCConn_sh);
    bSemFlag = TRUE;

    bCode = AgtSendHedReq(pCom,DDA_RIGHTS_COM_REQ,iTotalSendSize,&iErrCode);
    if (bCode == FALSE){

        goto ErrExit;
    }

    tmNPathLen = (unsigned int)htonl((unsigned long)tmPathLen);

    iCode = AgtSendReq(pCom,(char *)&tmNPathLen,DDA_RIGHTS_REQ_L1_SIZE,&iErrCode,DDA_RIGHTS_REQ_L1_SIZE);
    if(iCode == -1){
#ifdef DCETRC
        GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,SEND_DATA2_ERR,iCode,iErrCode,aszEyeChach);
#endif
        goto ErrExit;
    }

    iCode = AgtSendReq(pCom,pathname,tmPathLen,&iErrCode,tmPathLen);
    if(iCode == -1){
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
        case DDA_RIGHTS_COM_RSP :
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
    GW_tsh_trace(pCom->pTsh_Trace,pCom->uiTsh_Size,DFSRIGHTS_TRACE_E,iRetCode,*DFSErrno,aszEyeChach);
    GW_tsh_functrace(pszDFSrightsReq,GW_FUNC_END,iRetCode);
#endif
    return(iRetCode);
}
