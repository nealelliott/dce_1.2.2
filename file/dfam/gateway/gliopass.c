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
 * $Log: gliopass.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:30  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:46  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:04:24  takano
 * 	First Submit
 * 	[1996/02/21  08:04:17  takano]
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
#include <conio.h>

#include "dfamdef.h"
#include "dfamext.h"
#include "dfamstr.h"
#include "dfamprt.h"
#include "dfammsg.h"


/*EnglishComment**************************************************************/
/*  ID   = NCP_DCEOldPasswd                                                  */
/*  name = Old password checking routine                                     */
/*  func = Checks an old password.                                           */
/*         (1) Reads an old password from the bindery, and decrypts the      */
/*             password.                                                     */
/*         (2) Decrypts the password sent from a client.                     */
/*         (3) Compares (1) with (2).                                        */
/*         (4) Provides reply data to a NetWare client.                      */
/*                                                                           */
/*  i/o  = uiConNum    : LONG            : i   : connection number           */
/*         pReqdat     : PWDOSendFrame * : i   : receiving buffer address    */
/*         uiReqlen    : LONG            : i   : received data length        */
/*         pReplydata  : LGINRecvFrame * : i/o : sending buffer address      */
/*         puiReplylen : LONG *          : i/o : sending data length         */
/*                                                                           */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/


void NCP_DCEOldPasswd(uiConNum, pReqdat, uiReqlen, pReplydata, puiReplylen)
LONG uiConNum;
struct PWDOSendFrame *pReqdat;
LONG uiReqlen;
struct PWDORecvFrame *pReplydata;
LONG *puiReplylen;
{
    long iObjID;
    int iRc;
    WORD ushObjType;
    BYTE byLoginTime[7];
    char aszObjName[MAX_NWUSERNAME];
    char aszDCEpwd[MAX_DCEPASSWORD];
    char aszEncPassword[MAX_DCEPASSWORD];
    char aszLogComment[12];

    uiReqlen = uiReqlen;

    GW_tsh_functrace(FUNC_NCP_DCEOLDPASSWD, GW_FUNC_START, 0);

    strncpy(pReplydata->asComName, NCPOLDPASSWD, COMLEN);
    *puiReplylen = (LONG)(sizeof(struct PWDORecvFrame));

    GetConnectionInformation((WORD)uiConNum, aszObjName, &ushObjType, &iObjID, byLoginTime);

    if (ushObjType != OT_USER) {
        pReplydata->ushRc = (unsigned short)NCP_NOT_OT_USER;


        sprintf(aszLogComment, "%hu", ushObjType);
        GW_tsh_log(1500, aszLogComment);
        NCP_LogComment(uiConNum, NULL);

        GW_tsh_functrace(FUNC_NCP_DCENEWPASSWD, GW_FUNC_END, NCP_FAILURE);
        return;
    }

    memset(aszDCEpwd, 0x00, MAX_DCEPASSWORD);


    iRc = GetDCEPassword(aszObjName, aszDCEpwd);

    switch (iRc) {
      case 0 :
        break;

      case -2 :
        pReplydata->ushRc = (unsigned short)NCP_NOT_DFAM_USER;
        GW_tsh_functrace(FUNC_NCP_DCENEWPASSWD, GW_FUNC_END, NCP_NOT_DFAM_USER);
        return;
        break;

      case -3 :
        pReplydata->ushRc = (unsigned short)NCP_OUT_OF_MEMORY;
        GW_Message(DFAM_CON, 0, LOG_YES, KDDS11214);
        NCP_LogComment(uiConNum, aszObjName);
        GW_tsh_functrace(FUNC_NCP_DCENEWPASSWD, GW_FUNC_END, NCP_OUT_OF_MEMORY);
        return;
        break;

      case -1 :
      case -4 :
        pReplydata->ushRc = (unsigned short)NCP_BINDERY_ACCESS_FAILURE;
        GW_Message(DFAM_CON, 0, LOG_YES, KDDS11215, iRc);

        sprintf(aszLogComment, "%d", iRc);
        GW_tsh_log(1501, aszLogComment);
        NCP_LogComment(uiConNum, aszObjName);

        GW_tsh_functrace(FUNC_NCP_DCENEWPASSWD, GW_FUNC_END, NCP_BINDERY_ACCESS_FAILURE);
        return;
        break;

      default :
        pReplydata->ushRc = (unsigned short)NCP_FAILURE;
        GW_Message(DFAM_CON, 0, LOG_YES, KDDS11201, iRc);


        sprintf(aszLogComment, "%d", iRc);
        GW_tsh_log(1502, aszLogComment);
        NCP_LogComment(uiConNum, aszObjName);

        GW_tsh_functrace(FUNC_NCP_DCENEWPASSWD, GW_FUNC_END, NCP_FAILURE);
        return;
        break;
    }

    memset(aszEncPassword, 0x00, MAX_DCEPASSWORD);
    NCP_GetDCEPassword(pReqdat->aszOldPwd, pReqdat->ushLen, aszDCEpwd, aszEncPassword);


    if (strcmp(aszDCEpwd, aszEncPassword) == 0) {
        pReplydata->ushRc = PWDO_SUCCESS;
    }
    else {
        pReplydata->ushRc = PWDO_NOT_OLDPASSWORD;
    }

    memset(aszDCEpwd, 0x00, MAX_DCEPASSWORD);
    memset(aszEncPassword, 0x00, MAX_DCEPASSWORD);

    GW_tsh_functrace(FUNC_NCP_DCEOLDPASSWD, GW_FUNC_END, pReplydata->ushRc);
    return;
}


/*EnglishComment**************************************************************/
/*  ID   = NCP_DCENewPasswd                                                  */
/*  name = Old password checking and new password registration routine       */
/*  func = Checks an old password, and registers a new password.             */
/*         (1) Checks an old password (same as in NCP_DCEOldPasswd())        */
/*         (2) Decrypts a new password sent from a client, then encrypts the */
/*             password, and registers with the bindery.                     */
/*                                                                           */
/*  i/o  = uiConNum    : LONG            : i   : connection number           */
/*         pReplydata  : LGINRecvFrame * : i/o : sending buffer address      */
/*         puiReplylen : LONG *          : i/o : sending data length         */
/*                                                                           */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note   = The return value of the function trace is same as the           */
/*           return value when returned to a client.                         */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/


void NCP_DCENewPasswd(uiConNum, pReqdat, uiReqlen, pReplydata, puiReplylen)
LONG uiConNum;
struct PWDNSendFrame *pReqdat;
LONG uiReqlen;
struct PWDNRecvFrame *pReplydata;
LONG *puiReplylen;
{
    unsigned char *pszPwd;
    LONG auiKey[2];
    long iObjID;
    int iRc;
    int iRtn;
    int iCnt;
    int iSegmentNum;
    WORD ushObjType;
    char aszSegment[SEGMENTNUM];
    BYTE aszEnc[SEGMENTNUM];
    BYTE byLoginTime[7];
    BYTE byMore;
    char aszObjName[MAX_NWUSERNAME];
    char aszDCEpwd[MAX_DCEPASSWORD];
    char aszEncPassword[MAX_DCEPASSWORD];
    char aszLogComment[12];

    uiReqlen = uiReqlen;

    GW_tsh_functrace(FUNC_NCP_DCENEWPASSWD, GW_FUNC_START, 0);

    strncpy(pReplydata->asComName, NCPNEWPASSWD, COMLEN);
    *puiReplylen = (LONG)(sizeof(struct PWDNRecvFrame));

    GetConnectionInformation((WORD)uiConNum, aszObjName, &ushObjType, &iObjID, byLoginTime);

    if (ushObjType != OT_USER) {
        pReplydata->ushRc = (unsigned short)NCP_NOT_OT_USER;


        sprintf(aszLogComment, "%hu", ushObjType);
        GW_tsh_log(1503, aszLogComment);
        NCP_LogComment(uiConNum, NULL);

        GW_tsh_functrace(FUNC_NCP_DCENEWPASSWD, GW_FUNC_END, NCP_FAILURE);
        return;
    }

    memset(aszDCEpwd, 0x00, MAX_DCEPASSWORD);


    iRc = GetDCEPassword(aszObjName, aszDCEpwd);
    switch (iRc) {
      case 0 :
        break;

      case -2 :
        pReplydata->ushRc = (unsigned short)NCP_NOT_DFAM_USER;
        GW_tsh_functrace(FUNC_NCP_DCENEWPASSWD, GW_FUNC_END, NCP_NOT_DFAM_USER);
        return;
        break;

      case -3 :
        pReplydata->ushRc = (unsigned short)NCP_OUT_OF_MEMORY;
        GW_Message(DFAM_CON, 0, LOG_YES, KDDS11214);
        NCP_LogComment(uiConNum, aszObjName);
        GW_tsh_functrace(FUNC_NCP_DCENEWPASSWD, GW_FUNC_END, ERR_SERVER_OUT_OF_MEMORY);
        return;
        break;

      case -1 :
      case -4 :
        pReplydata->ushRc = (unsigned short)NCP_BINDERY_ACCESS_FAILURE;
        GW_Message(DFAM_CON, 0, LOG_YES, KDDS11215, iRc);

        sprintf(aszLogComment, "%d", iRc);
        GW_tsh_log(1504, aszLogComment);
        NCP_LogComment(uiConNum, aszObjName);

        GW_tsh_functrace(FUNC_NCP_DCENEWPASSWD, GW_FUNC_END, NCP_BINDERY_ACCESS_FAILURE);
        return;
        break;

      default :
        pReplydata->ushRc = (unsigned short)NCP_FAILURE;
        GW_Message(DFAM_CON, 0, LOG_YES, KDDS11201, iRc);


        sprintf(aszLogComment, "%d", iRc);
        GW_tsh_log(1505, aszLogComment);
        NCP_LogComment(uiConNum, aszObjName);

        GW_tsh_functrace(FUNC_NCP_DCENEWPASSWD, GW_FUNC_END, NCP_FAILURE);
        return;
        break;
    }

    memset(aszEncPassword, 0x00, MAX_DCEPASSWORD);
    NCP_GetDCEPassword(pReqdat->aszOldPwd, pReqdat->ushOlen, aszDCEpwd, aszEncPassword);


    if (strcmp(aszDCEpwd, aszEncPassword) != 0) {

        memset(aszDCEpwd, 0x00, MAX_DCEPASSWORD);
        memset(aszEncPassword, 0x00, MAX_DCEPASSWORD);
        pReplydata->ushRc = PWDN_NOT_OLDPASSWORD;
        GW_tsh_functrace(FUNC_NCP_DCENEWPASSWD, GW_FUNC_END, PWDN_NOT_OLDPASSWORD);
        return;
    }

    memset(aszEncPassword, 0x00, MAX_DCEPASSWORD);
    NCP_GetDCEPassword(pReqdat->aszNewPwd, pReqdat->ushNlen, aszDCEpwd, aszEncPassword);


    auiKey[0] = (LONG)iObjID;
    auiKey[1] = (LONG)iObjID;

    iSegmentNum = 1;
    byMore = 0xFF;
    pszPwd = aszEncPassword;

    while (1) {

        memset(aszSegment, '\0', SEGMENTNUM);
        memset(aszEnc, '\0', SEGMENTNUM);

        for (iCnt = 0; iCnt <= SEGMENTNUM - 1; iCnt++) {
            aszSegment[iCnt] = *pszPwd;
            if (*pszPwd == '\0') {
                byMore = 0x00;
                break;
            }
            pszPwd++;
        }

        fnSetEncryptData(auiKey, aszSegment, SEGMENTNUM, aszEnc);

        iRtn = WritePropertyValue(aszObjName, OT_USER, DCE_PWD_PROPERTY, iSegmentNum,
                                  aszEnc, byMore);

        if (iRtn == ESUCCESS) {

            iSegmentNum++;
            if (byMore == 0x00) {
                break;
            }
        }

        else if (iRtn == ERR_SERVER_OUT_OF_MEMORY) {

            memset(aszDCEpwd, 0x00, MAX_DCEPASSWORD);
            memset(aszEncPassword, 0x00, MAX_DCEPASSWORD);
            pReplydata->ushRc = (unsigned short)NCP_OUT_OF_MEMORY;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11214);
            NCP_LogComment(uiConNum, aszObjName);
            GW_tsh_functrace(FUNC_NCP_DCENEWPASSWD, GW_FUNC_END, NCP_OUT_OF_MEMORY);

            return;
        }

        else if ((iRtn == ERR_NO_SUCH_SEGMENT)
               ||(iRtn == ERR_NO_SUCH_PROPERTY)) {
            memset(aszDCEpwd, 0x00, MAX_DCEPASSWORD);
            memset(aszEncPassword, 0x00, MAX_DCEPASSWORD);
            pReplydata->ushRc = (unsigned short)NCP_NOT_DFAM_USER;
            GW_tsh_functrace(FUNC_NCP_DCENEWPASSWD, GW_FUNC_END, NCP_NOT_DFAM_USER);

            return;
        }

        else if (iRtn == ERR_WILDCARD_NOT_ALLOWED) {
            memset(aszDCEpwd, 0x00, MAX_DCEPASSWORD);
            memset(aszEncPassword, 0x00, MAX_DCEPASSWORD);
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11201, iRc);


            sprintf(aszLogComment, "%d", iRtn);
            GW_tsh_log(1506, aszLogComment);
            NCP_LogComment(uiConNum, aszObjName);

            GW_tsh_functrace(FUNC_NCP_DCENEWPASSWD, GW_FUNC_END, NCP_FAILURE);
            pReplydata->ushRc = (unsigned short)NCP_FAILURE;
            return;
        }

        else {

            memset(aszDCEpwd, 0x00, MAX_DCEPASSWORD);
            memset(aszEncPassword, 0x00, MAX_DCEPASSWORD);
            pReplydata->ushRc = (unsigned short)NCP_BINDERY_ACCESS_FAILURE;
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11215, iRc);


            sprintf(aszLogComment, "%d", iRtn);
            GW_tsh_log(1507, aszLogComment);
            NCP_LogComment(uiConNum, aszObjName);

            GW_tsh_functrace(FUNC_NCP_DCENEWPASSWD, GW_FUNC_END, NCP_BINDERY_ACCESS_FAILURE);

            return;
        }
    }

    memset(aszDCEpwd, 0x00, MAX_DCEPASSWORD);
    memset(aszEncPassword, 0x00, MAX_DCEPASSWORD);
    pReplydata->ushRc = (unsigned short)PWDN_SUCCESS;
    GW_tsh_functrace(FUNC_NCP_DCENEWPASSWD, GW_FUNC_END, PWDN_SUCCESS);
    return;
}


/*EnglishComment**************************************************************/
/*  ID   = NCP_GetDCEPassword                                                */
/*  name = DCE password decryption routine                                   */
/*  func = Decrypts a DCE password sent from a client.                       */
/*         (1) Decrypts a DCE password with using the first 8 bytes of the   */
/*             old DCE password as a decryption key.                         */
/*                                                                           */
/*  i/o  = pszDecPassword : BYTE *        : inp: encrypted DCE password      */
/*         ushPlen        : unsigned short: inp: length of pasDcePassword    */
/*         pszBindPassword: BYTE *        : inp: old password registered     */
/*                                               the bindery (decrypted)     */
/*         pszEncPassword : BYTE *        : out: decrypted DCE password      */
/*                                                                           */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

void NCP_GetDCEPassword(pszDecPassword, ushPlen, pszBindPassword, pszEncPassword)
BYTE *pszDecPassword;
unsigned short ushPlen;
BYTE *pszBindPassword;
BYTE *pszEncPassword;
{
    union {
        unsigned long uiKey[2];
        char aszOldPassword[8];
    } keyFormat;

    GW_tsh_functrace(FUNC_NCP_GETDCEPASSWORD, GW_FUNC_START, 0);

    memset(keyFormat.aszOldPassword, 0x00, 8);
    strncpy(keyFormat.aszOldPassword, pszBindPassword, 8);

    fnGetDecryptData(keyFormat.uiKey, pszDecPassword, ushPlen, pszEncPassword);

    GW_tsh_functrace(FUNC_NCP_GETDCEPASSWORD, GW_FUNC_END, 0);

    return;
}
