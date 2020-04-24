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
 * $Log: gliolgin.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:27  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:44  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:03:35  takano
 * 	First Submit
 * 	[1996/02/21  08:03:31  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <process.h>
#include <ncpext.h>
#include <errno.h>
#include <niterror.h>
#include <nwconn.h>
#include <nwbindry.h>
#include <nwsemaph.h>
#include <stdlib.h>
#include <nwdir.h>

#include "dfamdef.h"
#include "dfamext.h"
#include "dfamstr.h"
#include "dfamprt.h"
#include "dfammsg.h"
#include "cmnprt.h"


/*EnglishComment**************************************************************/
/*  ID   = NCP_DCELogin                                                      */
/*  name = DCE login preprocessing routine                                   */
/*  func = Preprocesses for DCE login, and makes response data.              */
/*         (1) Makes user thread group table.                                */
/*         (2) Sets a semaphore for connection with Agent.                   */
/*         (3) Makes a user thread group, and requests DCE login processing  */
/*             to the main thread of the thread group (this routine is       */
/*             suspended while the request is issued, and is resumed by the  */
/*             main thread of the user thread group).                        */
/*         (4) Sets a semaphore for File Handle Table if the DCE login was   */
/*             successful.                                                   */
/*         (5) Makes response to the client.                                 */
/*                                                                           */
/*  i/o  = uiConNum    : LONG            : i   : connection number           */
/*         pReplydata  : LGINRecvFrame * : i   : sending buffer address      */
/*         puiReplylen : LONG *          : i/o : sending data length         */
/*                                                                           */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note   = The return value of the function trace is same as the return    */
/*           value when returned to a client.                                */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/


void NCP_DCELogin(uiConNum, pReplydata, puiReplylen)
LONG uiConNum;
struct LGINRecvFrame *pReplydata;
LONG *puiReplylen;
{
    struct GTGroup_t *pTGroup;
    struct LginReturn_t *pWptr;
    int iRc;
    int iThreadGroupID;
    int iErrnoSave;
    char aszLogComment[12];

    GW_tsh_functrace(FUNC_NCP_DCELOGIN, GW_FUNC_START, 0);

    memcpy(pReplydata->asComName, NCPLOGIN, COMLEN);

    iRc = AddTGrpID(uiConNum, &pTGroup);

    switch (iRc) {
      case  0 :
        break;

      case -1 :
        pReplydata->ushRc = (unsigned short)NCP_OUT_OF_MEMORY;
        pReplydata->ushLen = (unsigned short)0;
        *puiReplylen = (LONG)(sizeof(struct LGINRecvFrame) - 1);
        GW_Message(DFAM_CON, 0, LOG_YES, KDDS11214);
        NCP_LogComment(pTGroup->uiConnNum, NULL);

        goto LGINend;
        break;

      case -3 :
        pReplydata->ushRc = (unsigned short)NCP_BUSY;
        pReplydata->ushLen = (unsigned short)0;
        *puiReplylen = (LONG)(sizeof(struct LGINRecvFrame) - 1);

        goto LGINend;
        break;

      default :
        pReplydata->ushRc = (unsigned short)LGIN_ALREADY_LOGIN;
        pReplydata->ushLen = (unsigned short)0;
        *puiReplylen = (LONG)(sizeof(struct LGINRecvFrame) - 1);

        goto LGINend;
        break;
    }

    if (iMaxUsersNum <= iLoginUsersNum) {
        pReplydata->ushRc = (unsigned short)LGIN_OVER_USERS_NUM;
        pReplydata->ushLen = (unsigned short)0;
        *puiReplylen = (LONG)(sizeof(struct LGINRecvFrame) - 1);
        DelTGrpID(uiConNum);
        goto LGINend;
    }

    iLoginUsersNum++;




    pTGroup->uiCConn_sh = OpenLocalSemaphore((LONG)1);
    pTGroup->iReqThreadID = GetThreadID();

    iThreadGroupID = BeginThreadGroup(fnLoginThread, NULL, ST_SIZE, (void *)pTGroup);
    if (iThreadGroupID == EFAILURE) {
        iErrnoSave = errno;
        iLoginUsersNum--;




        CloseLocalSemaphore(pTGroup->uiCConn_sh );
        DelTGrpID(uiConNum);

        switch (iErrnoSave) {
          case ENOMEM :
            pReplydata->ushRc = (unsigned short)NCP_OUT_OF_MEMORY;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11214);
            break;

          case EINVAL :
            pReplydata->ushRc = (unsigned short)NCP_FAILURE;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11201, EINVAL);
            break;

          default :
            pReplydata->ushRc = (unsigned short)NCP_FAILURE;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11205, iErrnoSave);
            break;
        }




        sprintf(aszLogComment, "%d", iErrnoSave);
        GW_tsh_log(1301, aszLogComment);
        NCP_LogComment(uiConNum, NULL);

        pReplydata->ushLen = (unsigned short)0;
        *puiReplylen = (LONG)(sizeof(struct LGINRecvFrame) - 1);

        goto LGINend;
    }

    SuspendThread(GetThreadID());

    if (pTGroup->iThreadID == -1) {

        iLoginUsersNum--;




        CloseLocalSemaphore(pTGroup->uiCConn_sh );
        DelTGrpID(uiConNum);
        GW_Message(DFAM_CON, 0, LOG_YES, KDDS11214);
        NCP_LogComment(pTGroup->uiConnNum, NULL);
        pReplydata->ushRc = (unsigned short)NCP_OUT_OF_MEMORY;
        pReplydata->ushLen = (unsigned short)0;
        *puiReplylen = (LONG)(sizeof(struct LGINRecvFrame) - 1);

        goto LGINend;
    }

    pWptr = pTGroup->pLgin;

    if (pWptr->ushRc != (unsigned short)0) {

        iLoginUsersNum--;




        pReplydata->ushRc = pWptr->ushRc;
        pReplydata->ushLen = pWptr->ushLen;
        *puiReplylen = (LONG)(sizeof(struct LGINRecvFrame) - 1);
        if (pWptr->pszBuf != NULL) {
            memcpy(pReplydata->aszDCE_uid, pWptr->pszBuf, pReplydata->ushLen);

            *puiReplylen = (LONG)(sizeof(struct LGINRecvFrame) + pReplydata->ushLen - 1);
            GW_free((unsigned char *)pWptr->pszBuf, 0);
        }
        GW_free((struct LginReturn *)pWptr, 0);

        CloseLocalSemaphore(pTGroup->uiCConn_sh );
        DelTGrpID(uiConNum);

        goto LGINend;
    }


    pTGroup->uiFHandle_sh = OpenLocalSemaphore((LONG)pAgent_sp->uiOpenFileNum);

    pReplydata->ushLen = (unsigned short)strlen(pWptr->pszBuf);
    memcpy(pReplydata->aszDCE_uid, pWptr->pszBuf, pReplydata->ushLen);
    *puiReplylen = (LONG)(sizeof(struct LGINRecvFrame) + pReplydata->ushLen - 1);
    pReplydata->ushRc = (unsigned short)LGIN_SUCCESS;

    GW_free((unsigned char *)pWptr->pszBuf, 0);
    GW_free((struct LginReturn *)pWptr, 0);

    if (bTimeSync == TRUE) {
        Timesync(pTGroup);
    }

    pTGroup->iLoginStatus = 0;


LGINend:

    GW_tsh_functrace(FUNC_NCP_DCELOGIN, GW_FUNC_END, pReplydata->ushRc);

    return;
}


/*EnglishComment**************************************************************/
/*  ID   = fnLoginThread                                                     */
/*  name = DCE login request issuing routine                                 */
/*  func = Issues a DCE login request to the authentication module.          */
/*         (1) Assigns an area to set the DCE login result.                  */
/*         (2) Sets Information to user thread group table.                  */
/*         (3) Reads a DCE user name and DCE password from the bindery,      */
/*             and decrypts them.                                            */
/*         (4) Establishes a connection with Agent.                          */
/*         (5) Issues a DCE login request to the authentication module.      */
/*         (6) Stores the DCE login result to the area assigned at (1), and  */
/*             resumes the thread that requested the DCE login.  Suspends    */
/*             LoginThread.                                                  */
/*         (7) If LoginThread is resumed (DLOGOUT or Unload), this routine   */
/*             waits till there is no thread in the thread group.            */
/*         (8) Frees the semaphore for the connection with client and the    */
/*             semaphore for File Handle Table.                              */
/*         (9) Frees the user thread group table.                            */
/*                                                                           */
/*  i/o  = pTGroup_wp : void *  : inp : pointer to User Thread               */
/*                                                  Group Table              */
/*                                                                           */
/*  return = none                                                            */
/*  common = The login thread does not handle the DCE logout even if it is   */
/*           resumed.  Thus the DCE logout request should be issued by the   */
/*           routine that resumes the login thread.                          */
/*  note   = The return value of the function trace is same as the return    */
/*           value when returned to a client.                                */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/


void fnLoginThread(pTGroup_wp)
void *pTGroup_wp;
{
    struct GTGroup_t *pTGroup;
    struct LginReturn_t *pWptr;
    long uiObjID;
    WORD usObjType;
    int iRc;
    int iDFSErrno;
    int iPwdLen;
    int iFlag;

    BYTE byLoginTime[7];
    char aszObjName[MAX_NWUSERNAME];
    char aszDCEuid[MAX_DCEUSERNAME];
    char aszDCEpwd[MAX_DCEPASSWORD];
    char aszDCEuidErr[20];
    char aszLogComment[12];

    GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_START, 0);

    pTGroup = (struct GTGroup_t *)pTGroup_wp;

    pWptr = (struct LginReturn_t *)GW_malloc(sizeof(struct LginReturn_t));

    if (pWptr == (struct LginReturn_t *)NULL) {
        pTGroup->iThreadID = -1;
        GW_Message(DFAM_CON, 0, LOG_YES, KDDS11214);
        NCP_LogComment(pTGroup->uiConnNum, aszObjName);
        GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, NCP_OUT_OF_MEMORY);
        ResumeThread(pTGroup->iReqThreadID);
        ExitThread(EXIT_THREAD, 0);
    }

    pWptr->ushLen = (unsigned short)0;
    pWptr->pszBuf = NULL;

    pTGroup->iTGrpID = GetThreadGroupID();
    pTGroup->iThreadID = GetThreadID();


    GetConnectionInformation((WORD)(pTGroup->uiConnNum), aszObjName, &usObjType,  &uiObjID, byLoginTime);

    if (usObjType != OT_USER) {
        pWptr->ushRc = (unsigned short)NCP_NOT_OT_USER;
        pTGroup->pLgin = pWptr;



        sprintf(aszLogComment, "%hu", usObjType);
        GW_tsh_log(1302, aszLogComment);
        NCP_LogComment(pTGroup->uiConnNum, NULL);

        GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, NCP_FAILURE);
        ResumeThread(pTGroup->iReqThreadID);
        ExitThread(EXIT_THREAD, 0);
    }

    iRc = AddGWVolTrustee(uiObjID);

    if(iRc != 0) {
        pWptr->ushRc = (unsigned short)NCP_INVALID_GW_ENV;
        pTGroup->pLgin = pWptr;
        ResumeThread(pTGroup->iReqThreadID);
        ExitThread(EXIT_THREAD, 0);
    }

    iRc = GetDCEUserName(aszObjName, aszDCEuid);
    switch (iRc) {
      case 0 :
        break;

      case -2 :
        pWptr->ushRc = (unsigned short)NCP_NOT_DFAM_USER;
        pTGroup->pLgin = pWptr;
        GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, NCP_NOT_DFAM_USER);
        DelGWVolTrustee(uiObjID, aszObjName);
        ResumeThread(pTGroup->iReqThreadID);
        ExitThread(EXIT_THREAD, 0);
        break;

      case -3 :
        pWptr->ushRc = (unsigned short)NCP_OUT_OF_MEMORY;
        pTGroup->pLgin = pWptr;
        GW_Message(DFAM_CON, 0, LOG_YES, KDDS11214);
        NCP_LogComment(pTGroup->uiConnNum, aszObjName);
        GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, NCP_OUT_OF_MEMORY);
        DelGWVolTrustee(uiObjID, aszObjName);
        ResumeThread(pTGroup->iReqThreadID);
        ExitThread(EXIT_THREAD, 0);
        break;

      case -1 :
      case -4 :
        pWptr->ushRc = (unsigned short)NCP_BINDERY_ACCESS_FAILURE;
        pTGroup->pLgin = pWptr;
        GW_Message(DFAM_CON, 0, LOG_YES, KDDS11215, iRc);
        GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, NCP_BINDERY_ACCESS_FAILURE);

        DelGWVolTrustee(uiObjID, aszObjName);




        sprintf(aszLogComment, "%d", iRc);
        GW_tsh_log(1303, aszLogComment);
        NCP_LogComment(pTGroup->uiConnNum, aszObjName);

        ResumeThread(pTGroup->iReqThreadID);
        ExitThread(EXIT_THREAD, 0);
        break;

      default :
        pWptr->ushRc = (unsigned short)NCP_FAILURE;
        pTGroup->pLgin = pWptr;
        GW_Message(DFAM_CON, 0, LOG_YES, KDDS11201, iRc);
        DelGWVolTrustee(uiObjID, aszObjName);




        sprintf(aszLogComment, "%d", iRc);
        GW_tsh_log(1304, aszLogComment);
        NCP_LogComment(pTGroup->uiConnNum, aszObjName);

        GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, NCP_FAILURE);
        ResumeThread(pTGroup->iReqThreadID);
        ExitThread(EXIT_THREAD, 0);
        break;
    }

    iRc = GetDCEPassword(aszObjName, aszDCEpwd);
    switch (iRc) {
      case 0 :
        iPwdLen = strlen(aszDCEpwd);
        break;

      case -2 :
        pWptr->ushRc = (unsigned short)NCP_NOT_DFAM_USER;
        pTGroup->pLgin = pWptr;
        GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, NCP_NOT_DFAM_USER);
        DelGWVolTrustee(uiObjID, aszObjName);
        ResumeThread(pTGroup->iReqThreadID);
        ExitThread(EXIT_THREAD, 0);
        break;

      case -3 :
        pWptr->ushRc = (unsigned short)NCP_OUT_OF_MEMORY;
        pTGroup->pLgin = pWptr;
        GW_Message(DFAM_CON, 0, LOG_YES, KDDS11214);
        NCP_LogComment(pTGroup->uiConnNum, aszObjName);
        GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, NCP_OUT_OF_MEMORY);
        DelGWVolTrustee(uiObjID, aszObjName);
        ResumeThread(pTGroup->iReqThreadID);
        ExitThread(EXIT_THREAD, 0);
        break;

      case -1 :
      case -4 :
        pWptr->ushRc = (unsigned short)NCP_BINDERY_ACCESS_FAILURE;
        pTGroup->pLgin = pWptr;
        GW_Message(DFAM_CON, 0, LOG_YES, KDDS11215, iRc);
        GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, NCP_BINDERY_ACCESS_FAILURE);

        DelGWVolTrustee(uiObjID, aszObjName);




        sprintf(aszLogComment, "%d", iRc);
        GW_tsh_log(1305, aszLogComment);
        NCP_LogComment(pTGroup->uiConnNum, aszObjName);

        ResumeThread(pTGroup->iReqThreadID);
        ExitThread(EXIT_THREAD, 0);
        break;

      default :
        pWptr->ushRc = (unsigned short)NCP_FAILURE;
        pTGroup->pLgin = pWptr;
        GW_Message(DFAM_CON, 0, LOG_YES, KDDS11201, iRc);
        DelGWVolTrustee(uiObjID, aszObjName);




        sprintf(aszLogComment, "%d", iRc);
        GW_tsh_log(1306, aszLogComment);
        NCP_LogComment(pTGroup->uiConnNum, aszObjName);

        GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, NCP_FAILURE);
        ResumeThread(pTGroup->iReqThreadID);
        ExitThread(EXIT_THREAD, 0);
        break;
    }

    pWptr->pszBuf = (unsigned char *)GW_malloc(strlen(aszDCEuid) + 1);

    if (pWptr->pszBuf == (unsigned char *)NULL) {

        memset(aszDCEpwd, '\0', iPwdLen);
        pWptr->ushRc = (unsigned short)NCP_OUT_OF_MEMORY;
        pTGroup->pLgin = pWptr;
        GW_Message(DFAM_CON, 0, LOG_YES, KDDS11214);
        NCP_LogComment(pTGroup->uiConnNum, aszObjName);
        GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, NCP_OUT_OF_MEMORY);
        DelGWVolTrustee(uiObjID, aszObjName);
        ResumeThread(pTGroup->iReqThreadID);
        ExitThread(EXIT_THREAD, 0);
    }

    iRc = AgtConnectReq(pAgent_sp, GW_USER_CON, pTGroup);
    if(iRc != DENOERR) {
        switch (iRc) {
          case DEHOSTFILE :
            memset(aszDCEpwd, '\0', iPwdLen);
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            pWptr->ushRc = (unsigned short)NCP_INVALID_GW_ENV;
            pTGroup->pLgin = pWptr;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11202);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, NCP_INVALID_GW_ENV);
            break;

          case DESERVICESFILE :
            memset(aszDCEpwd, '\0', iPwdLen);
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            pWptr->ushRc = (unsigned short)NCP_INVALID_GW_ENV;
            pTGroup->pLgin = pWptr;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11203);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, NCP_INVALID_GW_ENV);
            break;

          case DECOMNOMEM :
            memset(aszDCEpwd, '\0', iPwdLen);
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            pWptr->ushRc = (unsigned short)NCP_OUT_OF_MEMORY;
            pTGroup->pLgin = pWptr;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11214);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, NCP_OUT_OF_MEMORY);
            break;

          case DEREFUSEAGT :
          case DENETWORKENV :
          case DETIMEOUTAGT :
            memset(aszDCEpwd, '\0', iPwdLen);
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            pWptr->ushRc = (unsigned short)NCP_COM_ERROR;
            pTGroup->pLgin = pWptr;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11216);
            sprintf(aszLogComment, "%d", iRc);
            GW_tsh_log(1307, aszLogComment);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, NCP_COM_ERROR);
            break;

          case DEUNLOADTCPIP :
            memset(aszDCEpwd, '\0', iPwdLen);
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            pWptr->ushRc = (unsigned short)NCP_FAILURE;
            pTGroup->pLgin = pWptr;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11204);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, NCP_FAILURE);
            break;

          case DEDFAMTONOVELL :
            memset(aszDCEpwd, '\0', iPwdLen);
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            pWptr->ushRc = (unsigned short)NCP_FAILURE;
            pTGroup->pLgin = pWptr;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11205, iRc);



            sprintf(aszLogComment, "%d", iRc);
            GW_tsh_log(1308, aszLogComment);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);

            GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, NCP_FAILURE);
            break;

          case DEDFAMTERM :
            memset(aszDCEpwd, '\0', iPwdLen);
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            pWptr->ushRc = (unsigned short)NCP_BUSY;
            pTGroup->pLgin = pWptr;
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, NCP_BUSY);
            break;

          case DECIPHER :
          default :
            memset(aszDCEpwd, '\0', iPwdLen);
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            pWptr->ushRc = (unsigned short)NCP_FAILURE;
            pTGroup->pLgin = pWptr;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11201, iRc);


            sprintf(aszLogComment, "%d", iRc);
            GW_tsh_log(1309, aszLogComment);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);

            GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, NCP_FAILURE);
            break;
        }

        iRc = AgentDisConnectReq(pTGroup);

        DelGWVolTrustee(uiObjID, aszObjName);
        ResumeThread(pTGroup->iReqThreadID);
        ExitThread(EXIT_THREAD, 0);

    }





    iRc = DCE_Login(pTGroup, aszDCEuid, aszDCEpwd, &iDFSErrno);

    memset(aszDCEpwd, '\0', iPwdLen);

    if (iRc == 0) {
        pWptr->ushRc = (unsigned short)LGIN_SUCCESS;
        pWptr->ushLen = (unsigned short)strlen(aszDCEuid);
        strcpy(pWptr->pszBuf, aszDCEuid);
        pTGroup->pLgin = pWptr;
        pTGroup->uiObjectID = uiObjID;
        pTGroup->iLoginStatus = 0;
        ResumeThread(pTGroup->iReqThreadID);

        SuspendThread(GetThreadID());
    }
    else {
        switch (iDFSErrno) {

          case DENOACNT :

            pWptr->ushRc = LGIN_NOT_DCE_USER;
            pWptr->ushLen = (unsigned short)strlen(aszDCEuid);
            strcpy(pWptr->pszBuf, aszDCEuid);
            break;

          case DEINVALPSWD :
          case DENULLPSWD :
            pWptr->ushRc = LGIN_INVALID_PASSWORD;
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            break;

          case DENOKFILE :
          case DENOKEY :
          case DEKEYSIZE :
            pWptr->ushRc = NCP_INVALID_GW_ENV;
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11206);
            sprintf(aszLogComment, "%d", iDFSErrno);
            GW_tsh_log(1310, aszLogComment);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            break;

          case DENOAGTKF :
          case DEAGTKSIZE :
          case DENOGWENT :
            pWptr->ushRc = NCP_INVALID_AGT_ENV;
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11208);
            sprintf(aszLogComment, "%d", iDFSErrno);
            GW_tsh_log(1311, aszLogComment);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            break;

          case DEINVALACNT :
          case DELOCAL :
            pWptr->ushRc = NCP_INVALID_DCE_ENV;
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;

            strncpy(aszDCEuidErr, aszDCEuid, 16);
            if (strlen(aszDCEuid) > 16) {
                strcpy(aszDCEuidErr + 16, "...");
            }

            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11209, aszDCEuidErr);
            sprintf(aszLogComment, "%d", iDFSErrno);
            GW_tsh_log(1312, aszLogComment);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            break;

          case DEUNAVAILSRV :
            pWptr->ushRc = NCP_INVALID_DCE_ENV;
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11211);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            break;

          case DEINVALAGT :
            pWptr->ushRc = NCP_INVALID_AGT_ENV;
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11210);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            break;

          case DEINVALGTWY :
            pWptr->ushRc = NCP_INVALID_GW_ENV;
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11210);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            break;

          case DEUPDATEKEY :
            pWptr->ushRc = NCP_INVALID_GW_ENV;
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11207);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            break;

          case DENOMEM :
            pWptr->ushRc = NCP_OUT_OF_MEMORY;
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11214);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            break;

          case DECOM :
          case DEREFUSEAGT :
          case DETIMEOUTAGT :
          case DERESPONCEDATA :
            pWptr->ushRc = NCP_COM_ERROR;
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11216);
            sprintf(aszLogComment, "%d", iDFSErrno);
            GW_tsh_log(1313, aszLogComment);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            break;

          case DEUNLOADTCPIP :
            pWptr->ushRc = (unsigned short)NCP_FAILURE;
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11204);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, NCP_FAILURE);
            break;

          case DENFILE :
          case DENOLCK :
          case DEMFILE :
          case DENOSPC :
            pWptr->ushRc = NCP_INVALID_DCE_ENV;
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11223);

            sprintf(aszLogComment, "%d", iDFSErrno);
            GW_tsh_log(1314, aszLogComment);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            break;

          case DEUNKNOWN :
          case DEDFAMTONOVELL :
            pWptr->ushRc = NCP_FAILURE;
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11205, iDFSErrno);


            sprintf(aszLogComment, "%d", iDFSErrno);
            GW_tsh_log(1315, aszLogComment);

            if (strlen(aszDCEuid) < 12) {
                strcpy(aszLogComment, aszDCEuid);
            }
            else {
                strncpy(aszLogComment, aszDCEuid, 11);
                aszDCEuid[11] = '\0';
            }
            GW_tsh_log(1316, aszLogComment);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);

            break;

          case DEDFAMTERM :
            pWptr->ushRc = NCP_BUSY;
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            break;

          default :

            pWptr->ushRc = NCP_FAILURE;
            GW_free((unsigned char *)pWptr->pszBuf, 0);
            pWptr->pszBuf = (unsigned char *)NULL;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11201, iDFSErrno);


            sprintf(aszLogComment, "%d", iDFSErrno);
            GW_tsh_log(1317, aszLogComment);

            if (strlen(aszDCEuid) < 12) {
                strcpy(aszLogComment, aszDCEuid);
            }
            else {
                strncpy(aszLogComment, aszDCEuid, 11);
                aszDCEuid[11] = '\0';
            }
            GW_tsh_log(1318, aszLogComment);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);

            break;
        }

        pTGroup->pLgin = pWptr;
        iRc = AgentDisConnectReq(pTGroup);

        GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, pWptr->ushRc);
        DelGWVolTrustee(uiObjID, aszObjName);
        ResumeThread(pTGroup->iReqThreadID);
        ExitThread(EXIT_THREAD, 0);
    }






	DelayCutoff(pTGroup);

    iFlag = 0;

    while ((pTGroup->iExistThread != 0)
         ||(pTGroup->iExistCommand != 0)) {
        delay(SHORTWAITTIME);

        if (iFlag == 0) {
            sprintf(aszLogComment, "%d", pTGroup->iExistThread);
            GW_tsh_log(1328, aszLogComment);

            sprintf(aszLogComment, "%d", pTGroup->iExistCommand);
            GW_tsh_log(1329, aszLogComment);

            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            iFlag = 1;
        }
    }


    iRc = AgentDisConnectReq(pTGroup);

    switch (iRc) {

      case DENOERR :
        break;

      case DEUNLOADTCPIP :
        if (pTGroup->iResumeThreadMode == 0) {
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11204);
        }
        NCP_LogComment(pTGroup->uiConnNum, aszObjName);
        break;

      case DEDFAMTONOVELL :
        if (pTGroup->iResumeThreadMode == 0) {
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11205, iRc);
        }


        sprintf(aszLogComment, "%d", iRc);
        GW_tsh_log(1319, aszLogComment);
        NCP_LogComment(pTGroup->uiConnNum, aszObjName);
        break;

      default :
        if (pTGroup->iResumeThreadMode == 0) {
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11201, iRc);
        }


        sprintf(aszLogComment, "%d", iRc);
        GW_tsh_log(1320, aszLogComment);
        NCP_LogComment(pTGroup->uiConnNum, aszObjName);

        break;
    }



    if (pTGroup->pTblKeep_sp == NULL) {
        GW_tsh_log(1321, "Not Exist");
    }
    else {
        GW_tsh_log(1321, "Exist");
    }

    fnDelAllTask(pTGroup);




    if (pTGroup->pOFile_sp == NULL) {
        GW_tsh_log(1322, "Not Exist");
    }
    else {
        GW_tsh_log(1322, "Exist");
    }

    Terminate_Grp(pTGroup);

    sprintf(aszLogComment, "%d", iLoginUsersNum - 1);
    GW_tsh_log(1323, aszLogComment);




    CloseLocalSemaphore(pTGroup->uiCConn_sh );
    CloseLocalSemaphore(pTGroup->uiFHandle_sh );

    DelGWVolTrustee(uiObjID, aszObjName);
    DelTGrpID(pTGroup->uiConnNum);

    sprintf(aszLogComment, "%p", pTGroup_sp);
    GW_tsh_log(1330, aszLogComment);

    iLoginUsersNum--;




    GW_tsh_functrace(FUNC_FNLOGINTHREAD, GW_FUNC_END, pWptr->ushRc);




    ExitThread(EXIT_THREAD, 0);
}


/*EnglishComment**************************************************************/
/*  ID   = NCP_DCELoginChk                                                   */
/*  name = DCE login checking routine                                        */
/*  func = Checks if the login was made through Gateway.                     */
/*         (1) Assumes "already logged in" if there is a user thread group   */
/*             table.                                                        */
/*         (2) Assumes "not logged in" if there is no  user thread group     */
/*             table.                                                        */
/*                                                                           */
/*  i/o  = uiConNum    : LONG            : i   : connection number           */
/*         pszObjName  : char *          : i   : NetWare user name           */
/*         pReplydata  : LGINRecvFrame * : i   : sending buffer address      */
/*         puiReplylen : LONG *          : i/o : sending data length         */
/*                                                                           */
/*  return = none                                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = (1) The return value of the function trace is same as the       */
/*               return value when returned to a client.                     */
/*           (2) If already logged in to DCE, this routine continues the     */
/*               processes even though there is no DCE user property (i.e.,  */
/*               Gateway user registration has been deleted).                */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/


void NCP_DCELoginChk(uiConNum, pszObjName, pReplydata, puiReplylen)

LONG uiConNum;
char *pszObjName;
struct LCHKRecvFrame *pReplydata;
LONG *puiReplylen;
{
    struct GTGroup_t *pTGroup;
    int iRc;
    char aszSegment[SEGMENTNUM];
    char aszLogComment[12];
    BYTE byFlags;
    BYTE byMore;

    GW_tsh_functrace(FUNC_NCP_DCELOGINCHK, GW_FUNC_START, 0);

    memcpy(pReplydata->asComName, NCPLOGINCHK, COMLEN);
    *puiReplylen = (LONG)(sizeof(struct LCHKRecvFrame));

    pTGroup = GetTGrp(uiConNum);

    if (pTGroup != NULL) {
        pReplydata->ushRc = (unsigned short)DCHK_ALREADY_LOGIN;
    }

    else {

        iRc = ReadPropertyValue(pszObjName, OT_USER, DCE_USER_PROPERTY, 1, aszSegment, &byMore, &byFlags);

        switch (iRc) {
          case ESUCCESS :
            pReplydata->ushRc = (unsigned short)DCHK_NOT_LOGIN;
            break;

          case ERR_NO_SUCH_PROPERTY :
          case ERR_NO_SUCH_SEGMENT :
            pReplydata->ushRc = (unsigned short)NCP_NOT_DFAM_USER;
            break;

          case ERR_SERVER_OUT_OF_MEMORY :
            pReplydata->ushRc = (unsigned short)NCP_OUT_OF_MEMORY;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11214);
            break;

          case ERR_WILDCARD_NOT_ALLOWED :
            pReplydata->ushRc = (unsigned short)NCP_FAILURE;


            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11201, iRc);

            sprintf(aszLogComment, "%d", iRc);
            GW_tsh_log(1324, aszLogComment);
            NCP_LogComment(uiConNum, pszObjName);

            break;

          default :
            pReplydata->ushRc = (unsigned short)NCP_BINDERY_ACCESS_FAILURE;


            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11215, iRc);
            sprintf(aszLogComment, "%d", iRc);

            GW_tsh_log(1325, aszLogComment);
            NCP_LogComment(uiConNum, pszObjName);

            break;
        }
    }

    GW_tsh_functrace(FUNC_NCP_DCELOGINCHK, GW_FUNC_END, pReplydata->ushRc);

    return;
 }


/*EnglishComment**************************************************************/
/*  ID   = AddGWVolTrustee                                                   */
/*  name = Gateway volume trustee assignment routine                         */
/*  func = Assigns a trustee to Gateway volume.                              */
/*         (1) Assigns a trustee of a valid Gateway volume registered in the */
/*             volume table.                                                 */
/*         (2) Retuens an error if no trustee can be assigned to any Gateway */
/*             volume.                                                       */
/*                                                                           */
/*  i/o  = uiObjID     : long            : i   : user's object ID            */
/*                                                                           */
/*  return =  0 : trustee assignment successful                              */
/*           -1 : failed to assign a trustee                                 */
/*                                                                           */
/*  common =                                                                 */
/*  note   = This routine assumes it a success if the trustee was assigned   */
/*           to at least one of the multiple Gateway volumes.                */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/


int AddGWVolTrustee(uiObjID)
long uiObjID;
{
    int iRc;
    int iCc;
    struct GVolume_t *pVol;
    char aszGWVolName[_MAX_VOLUME + 2];
    char aszLogComment[12];

    GW_tsh_functrace(FUNC_ADDGWVOLTRUSTEE, GW_FUNC_START,0);

    iRc = -1;
    pVol = pVolume_sp;
    while( pVol != (struct GVolume_t *)0 ) {
        strcpy( aszGWVolName, pVol->aszNWVol );
        strcat( aszGWVolName, ":" );
        iCc = AddTrustee( aszGWVolName, uiObjID,
                        TA_READ | TA_WRITE | TA_CREATE | TA_DELETE | TA_SEEFILES | TA_MODIFY );

        switch (iCc) {
          case ESUCCESS :
            iRc = 0;
            break;

          default :
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11218, aszGWVolName);


            sprintf(aszLogComment, "%d", iCc);
            GW_tsh_log(1326, aszLogComment);
            break;
        }
        pVol = pVol->pUshirop;
    }

    GW_tsh_functrace(FUNC_ADDGWVOLTRUSTEE, GW_FUNC_END, iRc);
    return(iRc);
}


/*EnglishComment**************************************************************/
/*  ID   = DelGWVolTrustee                                                   */
/*  name = Gateway volume trustee releasing routine                          */
/*  func = Frees the trustee of Gateway volume.                              */
/*         (1) Frees the trustee of a valid Gateway volume registered in the */
/*             volume table.                                                 */
/*         (2) Displays an error message if the trustee cannot be freed.     */
/*         (3) Does not execute DeleteTrustee if there is a same-named user  */
/*             in the other clients.                                         */
/*                                                                           */
/*  i/o  =  uiObjID     : long            : inp : user's object ID           */
/*          pszObjName  : char *          : inp : user name                  */
/*                                                                           */
/*  return = none                                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/


void DelGWVolTrustee(uiObjID, pszObjName)
long uiObjID;
char *pszObjName;
{
    int iCc;
    LONG uiUser;
    struct GVolume_t *pVol;
    struct GTGroup_t *pTGrp;
    char aszGWVolName[_MAX_VOLUME + 2];
    char aszLogComment[12];

    GW_tsh_functrace(FUNC_DELGWVOLTRUSTEE, GW_FUNC_START, 0);

    uiUser = 0;
    pTGrp = pTGroup_sp;
    while( pTGrp != (struct GTGroup_t *)0 ) {
        if( pTGrp->uiObjectID == uiObjID ) {
            uiUser++;
            if (uiUser > 1) {
                break;
            }
        }
        pTGrp = pTGrp->pNext;
    }

    if (uiUser < 2) {
        pVol = pVolume_sp;
        while( pVol != (struct GVolume_t *)0 ) {

            strcpy( aszGWVolName, pVol->aszNWVol );
            strcat( aszGWVolName, ":" );
            iCc = DeleteTrustee( aszGWVolName, uiObjID );

            switch (iCc) {
              case ESUCCESS :
              case ERR_TRUSTEE_NOT_FOUND :
                break;

              default :
                GW_Message(DFAM_CON, 0, LOG_YES, KDDS11219, aszGWVolName);
                GW_Message(DFAM_CON, 0, LOG_YES, KDDS11220, pszObjName);


                sprintf(aszLogComment, "%d", iCc);
                GW_tsh_log(1327, aszLogComment);
                break;
            }
            pVol = pVol->pUshirop;
        }
    }

    GW_tsh_functrace(FUNC_DELGWVOLTRUSTEE, GW_FUNC_END, 0);
    return;
}
