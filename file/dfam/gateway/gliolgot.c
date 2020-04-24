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
 * $Log: gliolgot.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:29  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:45  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:03:58  takano
 * 	First Submit
 * 	[1996/02/21  08:03:54  takano]
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

#include "dfamdef.h"
#include "dfamext.h"
#include "dfamstr.h"
#include "dfamprt.h"
#include "dfammsg.h"

#include "cmnprt.h"

/*EnglishComment**************************************************************/
/*  ID   = NCP_DCELogout                                                     */
/*  name = DCE logout preliminary processing routine                         */
/*  func = Preprocesses for DCE logout, and makes response data (DCE logout  */
/*         request).                                                         */
/*         (1) Issues a DCE logout request.                                  */
/*         (2) Resumes a login thread.                                       */
/*         (3) Makes response data to a NetWare client.                      */
/*         (4) Waits till User Thread Group Table is actually freed.         */
/*                                                                           */
/*  i/o  = uiConNum    : LONG            : i   : connection number           */
/*         pszObjName  : char *          : i   : NetWare user name           */
/*         pReplydata  : LGINRecvFrame * : i   : sending buffer address      */
/*         puiReplylen : LONG *          : i/o : sending data length         */
/*                                                                           */
/*  return = none                                                            */
/*  common = Clears Gateway even if failed to log out from DCE ( i.e.,       */
/*           frees a connection and user thread group table, etc.)           */
/*                                                                           */
/*  note   = The return value of the function trace is same as the           */
/*           return value when returned to a client.                         */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/


void NCP_DCELogout(pTGroup, pReplydata, puiReplylen)

struct GTGroup_t *pTGroup;
struct LGOTRecvFrame *pReplydata;
LONG *puiReplylen;
{
    LONG uiConNum;
    long iObjID;
    long iRtn;
    int iRc;
    int iDFSErrno;
    int iFlag;

    WORD ushObjType;
    char aszDCEuid[MAX_DCEUSERNAME];
    char aszObjName[MAX_NWUSERNAME];
    char aszLogComment[12];
    BYTE byLoginTime[7];

    GW_tsh_functrace(FUNC_NCP_DCELOGOUT, GW_FUNC_START, 0);

    memcpy(pReplydata->asComName, NCPLOGOUT, COMLEN);

    uiConNum = pTGroup->uiConnNum;


    GetConnectionInformation((WORD)pTGroup->uiConnNum, aszObjName, &ushObjType, &iObjID, byLoginTime);

    if (ushObjType != OT_USER) {
        pReplydata->ushRc = NCP_NOT_OT_USER;
        pReplydata->ushLen = (unsigned short)0;
        *puiReplylen = (LONG)(sizeof(struct LGOTRecvFrame) - 1);



        sprintf(aszLogComment, "%hu", ushObjType);
        GW_tsh_log(1400, aszLogComment);
        NCP_LogComment(pTGroup->uiConnNum, NULL);
        GW_tsh_functrace(FUNC_NCP_DCELOGOUT, GW_FUNC_END, pReplydata->ushRc);
        return;
    }

    iRc = GetDCEUserName(aszObjName, aszDCEuid);

    switch (iRc) {
      case  0 :
        break;

      default :
        aszDCEuid[0] = '\0';
        break;
    }

   	DelayCutoff(pTGroup);


    iFlag = 0;

    while ((pTGroup->iExistThread != 0)
         ||(pTGroup->iExistCommand != 0)) {
        delay(SHORTWAITTIME);

        if (iFlag == 0) {
            sprintf(aszLogComment, "%d", pTGroup->iExistThread);
            GW_tsh_log(1401, aszLogComment);

            sprintf(aszLogComment, "%d", pTGroup->iExistCommand);
            GW_tsh_log(1414, aszLogComment);

            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            iFlag = 1;
        }
    }




    iRtn = DFSLoutReq(&iDFSErrno, pTGroup);
    if (iRtn == (LONG)0) {
        pReplydata->ushRc = (unsigned short)LGOT_SUCCESS;
        pReplydata->ushLen = (unsigned short)strlen(aszDCEuid);
        strncpy(pReplydata->aszDCE_uid, aszDCEuid, pReplydata->ushLen);
    }
    else {
        switch (iDFSErrno) {
          case DEREFUSEAGT :
          case DETIMEOUTAGT :
          case DERESPONCEDATA :
            pReplydata->ushRc = NCP_COM_ERROR;
            *puiReplylen = (LONG)(sizeof(struct LGOTRecvFrame) - 1);
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11216);
            sprintf(aszLogComment, "%d", iDFSErrno);
            GW_tsh_log(1402, aszLogComment);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            break;

          case DEUNLOADTCPIP :
            pReplydata->ushRc = NCP_FAILURE;
            *puiReplylen = (LONG)(sizeof(struct LGOTRecvFrame) - 1);
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11204);
            break;

          case DEDFAMTONOVELL :
            pReplydata->ushRc = NCP_FAILURE;
            *puiReplylen = (LONG)(sizeof(struct LGOTRecvFrame) - 1);
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11205, iDFSErrno);



            sprintf(aszLogComment, "%d", iDFSErrno);
            GW_tsh_log(1403, aszLogComment);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            break;

          case DEDFAMTERM :
            pReplydata->ushRc = NCP_BUSY;
            *puiReplylen = (LONG)(sizeof(struct LGOTRecvFrame) - 1);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            break;

          default :
            pReplydata->ushRc = NCP_FAILURE;
            *puiReplylen = (LONG)(sizeof(struct LGOTRecvFrame) - 1);
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11201, iDFSErrno);


            sprintf(aszLogComment, "%d", iDFSErrno);
            GW_tsh_log(1404, aszLogComment);
            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            break;
        }
        pTGroup->iResumeThreadMode = 1;
    }

    *puiReplylen = (LONG)(sizeof(struct LGOTRecvFrame) + pReplydata->ushLen - 1);
    GW_tsh_functrace(FUNC_NCP_DCELOGOUT, GW_FUNC_END, pReplydata->ushRc);

    iRc = ResumeThread(pTGroup->iThreadID);


    while (GetTGrp(uiConNum) != NULL) {
        delay(SHORTWAITTIME);
    }




    return;
}


/*EnglishComment**************************************************************/
/*  ID   = NCP_DCELogoutDown                                                 */
/*  name = DCE logout provision routine                                      */
/*  func = Provides for DCE logout (Connection Event Handler and unload)     */
/*         (1) Issues a DCE logout request.                                  */
/*         (2) Resumes a login thread.                                       */
/*                                                                           */
/*  i/o  = uiConNum    : LONG            : inp : connection number           */
/*                                                                           */
/*  return =  0 : normal end                                                 */
/*           -1 : abnormal end (failed to log out by some reason)            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = NCP_DCELogoutDown(), made from NCP_DCELogout() with deleting    */
/*           the functions that handle a response to a client, is invoked    */
/*           only from:                                                      */
/*             (1) Connection Event Handler (ConDown())                      */
/*             (2) logout request when unloaded (ResumeAllUserThread())      */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/


int NCP_DCELogoutDown(pTGroup)
struct GTGroup_t *pTGroup;
{
    LONG uiConNum;
    long iObjID;
    long iRtn;
    int iDFSErrno;
    int iFlag;

    WORD ushObjType;
    char aszObjName[MAX_NWUSERNAME];
    BYTE byLoginTime[7];
    char aszLogComment[12];

    GW_tsh_functrace(FUNC_NCP_DCELOGOUTDOWN, GW_FUNC_START, 0);

    uiConNum = pTGroup->uiConnNum;

    GetConnectionInformation((WORD)pTGroup->uiConnNum, aszObjName, &ushObjType, &iObjID, byLoginTime);
    if (ushObjType != OT_USER) {



        sprintf(aszLogComment, "%hu", ushObjType);
        GW_tsh_log(1406, aszLogComment);
        NCP_LogComment(pTGroup->uiConnNum, NULL);
    }

   	DelayCutoff(pTGroup);



    iFlag = 0;

    while ((pTGroup->iExistThread != 0)
         ||(pTGroup->iExistCommand != 0)) {
        delay(SHORTWAITTIME);

        if (iFlag == 0) {
            sprintf(aszLogComment, "%d", pTGroup->iExistThread);
            GW_tsh_log(1407, aszLogComment);

            sprintf(aszLogComment, "%d", pTGroup->iExistCommand);
            GW_tsh_log(1415, aszLogComment);

            NCP_LogComment(pTGroup->uiConnNum, aszObjName);
            iFlag = 1;
        }
    }




    iRtn = DFSLoutReq(&iDFSErrno, pTGroup);

    if (iRtn == (LONG)0) {
        GW_tsh_log(1413, "Logout");
        NCP_LogComment(pTGroup->uiConnNum, aszObjName);
        GW_tsh_functrace(FUNC_NCP_DCELOGOUTDOWN, GW_FUNC_END, 0);
    }
    else {
        switch (iDFSErrno) {
          case DEREFUSEAGT :
          case DETIMEOUTAGT :
          case DERESPONCEDATA :
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11216);
            sprintf(aszLogComment, "%d", iDFSErrno);
            GW_tsh_log(1408, aszLogComment);
            GW_tsh_functrace(FUNC_NCP_DCELOGOUTDOWN, GW_FUNC_END, NCP_FAILURE);
            break;

          case DEUNLOADTCPIP :

            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11204);
            GW_tsh_functrace(FUNC_NCP_DCELOGOUTDOWN, GW_FUNC_END, NCP_FAILURE);
            break;

          case DEDFAMTONOVELL :
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11205, iDFSErrno);


            sprintf(aszLogComment, "%d", iDFSErrno);
            GW_tsh_log(1409, aszLogComment);
            GW_tsh_functrace(FUNC_NCP_DCELOGOUTDOWN, GW_FUNC_END, NCP_FAILURE);
            break;

          case DEDFAMTERM :

            GW_tsh_functrace(FUNC_NCP_DCELOGOUTDOWN, GW_FUNC_END, NCP_FAILURE);
            break;

          default :
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11201, iDFSErrno);


            sprintf(aszLogComment, "%d", iDFSErrno);
            GW_tsh_log(1410, aszLogComment);
            GW_tsh_functrace(FUNC_NCP_DCELOGOUTDOWN, GW_FUNC_END, NCP_FAILURE);
            break;
        }

        pTGroup->iResumeThreadMode = 1;
        NCP_LogComment(pTGroup->uiConnNum, aszObjName);
    }

    ResumeThread(pTGroup->iThreadID);

    while (GetTGrp(uiConNum) != NULL) {
        delay(SHORTWAITTIME);
    }

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = ResumeAllUserThread                                               */
/*  name = DCE ser logout and user thread resume routine                     */
/*  func = Logs out all the DCE users, and resumes user threads.             */
/*         (1) Issues a DCE logout request if the live flag is on (when      */
/*             DFA.NLM is unloaded).                                         */
/*         (2) Resumes a login thread if the live flag is off.               */
/*                                                                           */
/*  i/o  = none                                                              */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/


void ResumeAllUserThread(iMode)
int iMode;
{
    struct GTGroup_t *pTGroup;
    LONG uiConNum;
    int iFlag;

    int iRc;

    iResumeAll = 1;

    GW_tsh_functrace(FUNC_RESUMEALLUSERTHREAD, GW_FUNC_START, 0);

    if (pTGroup_sp == (struct GTGroup_t *)NULL) {
        ;
    }
    else {

        GW_Message(DFAM_CON, 0, LOG_YES, KDDS11221);

        pTGroup = pTGroup_sp;
        while (pTGroup != NULL) {

            if (pTGroup->iLoginStatus == 1) {
                uiConNum = pTGroup->uiConnNum;

                iFlag = 0;

                while ((GetTGrp(uiConNum) != NULL)
                     &&(pTGroup->iLoginStatus == 1)) {
                    delay(SHORTWAITTIME);
                    if (iFlag == 0) {




                        GW_tsh_log(1412, "Wait");
                        NCP_LogComment(uiConNum, NULL);
                        iFlag = 1;
                    }
                }

                if (GetTGrp(uiConNum) == NULL) {
                    pTGroup = pTGroup_sp;
                    continue;
                }
            }

            uiConNum = pTGroup->uiConnNum;

            if (pTGroup->iLogoutStatus == 0) {
                pTGroup->iLogoutStatus = 1;

                if (iMode == BAD_ADMIN_CONN) {
                    pTGroup->iResumeThreadMode = 1;
                    ResumeThread(pTGroup->iThreadID);
                }
                else {
                    if (bIsConnection_live(pAgent_sp) == TRUE) {
                        iRc = NCP_DCELogoutDown(pTGroup);

                    }
                    else {
                        ResumeThread(pTGroup->iThreadID);
                    }
                }

                iFlag = 0;

                while (GetTGrp(uiConNum) != NULL) {
                    delay(SHORTWAITTIME);
                }

                if (iMode == BAD_ADMIN_CONN) {
                    GW_Message(DFAM_NWC, (int *)&uiConNum, LOG_YES, KDDS11225);
                }
                else {
                    GW_Message(DFAM_NWC, (int *)&uiConNum, LOG_YES, KDDS11224);
                }
                pTGroup = pTGroup_sp;
            }
            else {
                pTGroup = pTGroup->pNext;
            }
        }



        iFlag = 0;

        while (pTGroup_sp != NULL) {
            delay(SHORTWAITTIME);
            if (iFlag == 0) {
                GW_tsh_log(1413, "Wait");
                iFlag = 1;
            }
        }

        GW_Message(DFAM_CON, 0, LOG_YES, KDDS11222);

    }


    if (iMode == BAD_ADMIN_CONN) {
        iResumeAll = 0;
    }

    GW_tsh_functrace(FUNC_RESUMEALLUSERTHREAD, GW_FUNC_END, 0);
    return;
}
