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
 * $Log: uadmuser.c,v $
 * Revision 1.1.2.2  1996/03/11  14:09:18  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:23  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:21:19  takano
 * 	First Submit
 * 	[1996/02/21  08:21:14  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <advanced.h>
#include <process.h>
#include <time.h>
#include <nwsnut.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <nwlocale.h>
#include <nwbindry.h>
#include <errno.h>
#include <niterror.h>

#include "uadmmlh.h"
#include "uadmdef.h"
#include "uadmext.h"

int iLongUserNum;
char aszGNWUser[MAX_NWUSERNAME];
char aszGDCEUser[MAX_DCEUSERNAME];
char aszGDCEPwd[MAX_DCEPASSWD];
char aszGDCERePwd[MAX_DCEPASSWD];


/*EnglishComment**************************************************************/
/*  ID   = DFAMUserList                                                      */
/*  name = "Gateway Users Screen" creation routine                           */
/*  func = Makes "Gateway Users Screen."                                     */
/*                                                                           */
/*  i/o  = none                                                              */
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

void DFAMUserList(void)
{
    LIST *pPtr;
    LIST *pPtr1;
    WORD ushType;
    LONG uiCcode;
    long iId;
    int iRc;
    int iRtn;
    int iWidth;
    BYTE aszSegment[SEGMENTNUM];
    char aszNWUserName[MAX_NWUSERNAME];
    char cProperty;
    char cFlag;
    char cSecurity;
    BYTE byFlags;
    BYTE byMore;

    NWSInitList(pHandle, free);

    NWSStartWait(WAITUSERROW, WAITUSERCOL, pHandle);

    iId = (long)-1;
    iLongUserNum = 0;

    while (1) {
        iRc = ScanBinderyObject("*", OT_USER, &iId, aszNWUserName, &ushType, &cProperty, &cFlag,
                                &cSecurity);

        if (iRc == ESUCCESS) {
            iRtn = ReadPropertyValue(aszNWUserName, OT_USER, DCE_USER_PROPERTY, 1, aszSegment,
                                     &byMore, &byFlags);

            if (iRtn == ESUCCESS) {
                pPtr = NWSAppendToList(aszNWUserName, (void *)0, pHandle);
                if (pPtr == (LIST *)NULL) {
                    NWSEndWait(pHandle);
                    NWSAlert(0, 0, pHandle, SCREENPREFAILED);
                    goto lbUserEnd;
                }

                if (strlen(aszNWUserName) > MIN_USERLISTWIDTH) {
                    iLongUserNum++;
                }
            }

            else if ((iRtn == ERR_NO_SUCH_PROPERTY)
                   ||(iRtn == ERR_NO_SUCH_OBJECT)) {
                ;
            }

            else if (iRtn == ERR_SERVER_OUT_OF_MEMORY) {
                NWSEndWait(pHandle);
                NWSAlert(0, 0, pHandle, OUTOFMEMORY);
                goto lbUserEnd;
            }

            else {
                NWSEndWait(pHandle);
                sprintf(aszError, "%d", iRtn);
                AlertErrorCode(BINDERYERROR, aszError);
                goto lbUserEnd;
            }
        }

        else if (iRc == ERR_NO_SUCH_OBJECT) {
            break;
        }

        else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
            NWSEndWait(pHandle);
            NWSAlert(0, 0, pHandle, OUTOFMEMORY);
            goto lbUserEnd;
        }

        else {
            NWSEndWait(pHandle);
            sprintf(aszError, "%d", iRc);
            AlertErrorCode(BINDERYERROR, aszError);
            goto lbUserEnd;
        }
    }

    pPtr1 = NULL;
    NWSEndWait(pHandle);

    do {

        iWidth = MIN_USERLISTWIDTH;
        if (iLongUserNum > 0) {
            iWidth = MAX_USERLISTWIDTH;
        }

        uiCcode = NWSList(DFAMUSERLIST, USERLISTROW, USERLISTCOL, USERLISTHEIGHT, iWidth,
                          M_ESCAPE | M_SELECT | M_DELETE | M_INSERT, &pPtr1, pHandle, NULL,
                          DFAMUserAction, NULL);

    } while (uiCcode > 1);

lbUserEnd:
    NWSDestroyList(pHandle);
    return;

}

/*EnglishComment**************************************************************/
/*  ID   = DFAMUserAction                                                    */
/*  name = "Gateway Users Screen" processing routine                         */
/*  func = (1) Goes back to "Menu Screen" if the Esc. key was pressed.       */
/*         (2) Displays "Delete Gateway User Screen" if the Del key was      */
/*             pressed.  If the Gateway user is deletable, this routine      */
/*             deletes the DCE user name, DCE password, Admin Utility        */
/*             execution property from t he bindery.  Also deletes the       */
/*             Gateway user name from "Gateway Users Screen."                */
/*         (3) Displays "Not Gateway Users Screen" if the Ins key was        */
/*             pressed.                                                      */
/*         (4) Displays "Gateway User Informaiton Screen" if the Enter key   */
/*             was pressed.                                                  */
/*                                                                           */
/*  i/o  = uiKey      : LONG    :: pressed key                               */
/*         pSelect    : LIST ** :: selected element                          */
/*         puiItemNum : LONG    :: item number (not used)                    */
/*         pPar       : void *  :: parameter (not used)                      */
/*                                                                           */
/*  return = -1 : Goes back to "Gateway Users Screen."                       */
/*            0 : Goes back to "Menu Screen."                                */
/*            2 : Needs modification on "Gateway Users Screen" due to the    */
/*                creation or deletion of the Gateway users.                 */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int DFAMUserAction(uiKey, pSelect, puiItemNum, pPar)
LONG uiKey;
LIST **pSelect;
LONG *puiItemNum;
void *pPar;
{
    LIST *pPtr;
    WORD ushType;
    LONG uiCcode;
    long iId;
    int iRc;
    int iRtn;
    int bAflg;
    int iWidth;
    int iError;

    BYTE aszSegment[SEGMENTNUM];
    char aszNWUserName[MAX_NWUSERNAME];
    char cProperty;
    char cFlag;
    char cSecurity;
    BYTE byFlags;
    BYTE byMore;

    puiItemNum = puiItemNum;
    pPar = pPar;

    if (uiKey == M_ESCAPE) {
        return 0;
    }

    if (uiKey == M_DELETE) {
        uiCcode = NWSConfirm(DELDFAMUSER, DELUSERROW, DELUSERCOL, DELUSERHEIGHT, NULL, pHandle,
                             NULL);

        if (uiCcode == (LONG)1) {

            iRc = DeleteProperty((*pSelect)->text, OT_USER, DCE_USER_PROPERTY);
            if ((iRc == ESUCCESS)
              ||(iRc == ERR_NO_SUCH_PROPERTY)) {
                ;
            }

            else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
                NWSAlert(0, 0, pHandle, OUTOFMEMORY);
                return -1;
            }

            else {
                sprintf(aszError, "%d", iRc);
                AlertErrorCode(BINDERYERROR, aszError);
                return -1;
            }

            iError = 0;

            if (strlen((*pSelect)->text) > MIN_USERLISTWIDTH) {
                iLongUserNum--;
            }


            iRc = DeleteProperty((*pSelect)->text, OT_USER, DCE_PWD_PROPERTY);
            if ((iRc == ESUCCESS)
              ||(iRc == ERR_NO_SUCH_PROPERTY)) {
                ;
            }

            else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
                NWSAlert(0, 0, pHandle, OUTOFMEMORY);
                iError = 1;
            }

            else {
                sprintf(aszError, "%d", iRc);
                AlertErrorCode(BINDERYERROR, aszError);
                iError = 1;
            }

            if (strcmp((*pSelect)->text,"SUPERVISOR")) {

                iRc = DeleteProperty((*pSelect)->text, OT_USER, ADMIN_EXEC_PROPERTY);
                if ((iRc == ESUCCESS)
                  ||(iRc == ERR_NO_SUCH_PROPERTY)) {
                    ;
                }

                else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
                    if (iError == 0) {
                        NWSAlert(0, 0, pHandle, OUTOFMEMORY);
                    }
                }

                else {
                    if (iError == 0) {
                        sprintf(aszError, "%d", iRc);
                        AlertErrorCode(BINDERYERROR, aszError);
                    }
                }
            }

            pPtr = NWSDeleteFromList(*pSelect, pHandle);
            *pSelect = pPtr;

            return 2;
        }

        else {
            return -1;
        }
    }

    if (uiKey == M_INSERT) {
        uiCcode = NWSPushList(pHandle);
        if (uiCcode == (LONG)0) {
            NWSAlert(0, 0, pHandle, SCREENPREFAILED);
            return -1;
        }

        NWSInitList(pHandle, free);

        NWSStartWait(WAITNOTUSERROW, WAITNOTUSERCOL, pHandle);

        iId = (long)-1;
        iWidth = MIN_NOTUSERLISTWIDTH;

        while (1) {
            iRc = ScanBinderyObject("*", OT_USER, &iId, aszNWUserName, &ushType, &cProperty,
                                    &cFlag, &cSecurity);

            if (iRc == ESUCCESS) {
                iRtn = ReadPropertyValue(aszNWUserName, OT_USER, DCE_USER_PROPERTY, 1,
                                         aszSegment, &byMore, &byFlags);

                if (iRtn == ERR_NO_SUCH_PROPERTY) {
                    pPtr = NWSAppendToList(aszNWUserName, (void *)0, pHandle);
                    if (pPtr == (LIST *)NULL) {
                        NWSEndWait(pHandle);
                        NWSAlert(0, 0, pHandle, SCREENPREFAILED);
                        NWSDestroyList(pHandle);
                        NWSPopList(pHandle);
                        return -1;
                    }

                    if (strlen(aszNWUserName) > MIN_NOTUSERLISTWIDTH) {
                        iWidth = MAX_NOTUSERLISTWIDTH;
                    }
                }

                else if (iRtn == ESUCCESS) {
                    ;
                }

                else if (iRtn == ERR_SERVER_OUT_OF_MEMORY) {
                    NWSEndWait(pHandle);
                    NWSAlert(0, 0, pHandle, OUTOFMEMORY);
                    NWSDestroyList(pHandle);
                    NWSPopList(pHandle);
                    return -1;
                }

                else {
                    NWSEndWait(pHandle);
                    sprintf(aszError, "%d", iRtn);
                    AlertErrorCode(BINDERYERROR, aszError);
                    NWSDestroyList(pHandle);
                    NWSPopList(pHandle);
                    return -1;
                }
            }

            else if (iRc == ERR_NO_SUCH_OBJECT) {
                break;
            }

            else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
                NWSEndWait(pHandle);
                NWSAlert(0, 0, pHandle, OUTOFMEMORY);
                NWSDestroyList(pHandle);
                NWSPopList(pHandle);
                return -1;
            }

            else {
                NWSEndWait(pHandle);
                sprintf(aszError, "%d", iRc);
                AlertErrorCode(BINDERYERROR, aszError);
                NWSDestroyList(pHandle);
                NWSPopList(pHandle);
                return -1;
            }
        }

        NWSEndWait(pHandle);

        uiCcode = NWSList(NOTDFAMUSERLIST, NOTUSERLISTROW, NOTUSERLISTCOL, NOTUSERLISTHEIGHT,
                          iWidth, M_ESCAPE | M_SELECT, NULL, pHandle, NULL,
                          NOTDFAMUserAction, NULL);

        NWSDestroyList(pHandle);
        NWSPopList(pHandle);

        if (uiCcode == (LONG)2) {
            pPtr = NWSAppendToList(aszGNWUser, (void *)0, pHandle);
            if (pPtr == (LIST *)NULL) {
                NWSAlert(0, 0, pHandle, SCREENPREFAILED);
                return -1;
            }
            if (strlen(aszGNWUser) > MIN_USERLISTWIDTH) {
                iLongUserNum++;
            }

            *pSelect = pPtr;
            return 2;
        }
        return -1;
    }

    else {

        strcpy(aszGNWUser,(*pSelect)->text);

        uiCcode = NWSPushList(pHandle);
        if (uiCcode == (LONG)0) {
            NWSAlert(0, 0, pHandle, SCREENPREFAILED);
            return -1;
        }

        bAflg = 0;
        NWSInitList(pHandle, free);

        pPtr = NWSAppendToList(NWSGetMessage(CHANGEDCEUSER, &(pHandle->messages)),
                               (void *)0, pHandle);
        if (pPtr == (LIST *)NULL) {
            bAflg = 1;
        }

        pPtr = NWSAppendToList(NWSGetMessage(SETADMINEXEC, &(pHandle->messages)),
                               (void *)0, pHandle);
        if (pPtr == (LIST *)NULL) {
            bAflg = 1;
        }

        if (bAflg != 0) {
            NWSAlert(0, 0, pHandle, SCREENPREFAILED);
            NWSDestroyList(pHandle);
            NWSPopList(pHandle);
            return -1;
        }

        uiCcode = NWSList(DFAMUSERINFO, USERINFOROW, USERINFOCOL, USERINFOHEIGHT, USERINFOWIDTH,
                          M_ESCAPE | M_SELECT, NULL, pHandle, NULL, DFAMInfoAction, NULL);

        NWSDestroyList(pHandle);
        NWSPopList(pHandle);
        return -1;
    }
}


/*EnglishComment**************************************************************/
/*  ID   = NotDFAMUserAction                                                 */
/*  name = "Not Gateway Users Screen" processing routine                     */
/*  func = (1) Goes back to "Gateway Users Screen" if the Esc key was        */
/*             pressed.                                                      */
/*         (2) Displays "DCE User Name Screen" if the Enter key was pressed. */
/*                                                                           */
/*  i/o  = uiKey      : LONG    :: pressed key                               */
/*         pSelect    : LIST ** :: selected element                          */
/*         puiItemNum : LONG    :: item number (not used)                    */
/*         pPar       : void *  :: parameter (not used)                      */
/*                                                                           */
/*  return = -1 : Goes back to "Not Gateway Users Screen"                    */
/*            0 : Goes back to "Gateway Users Screen"                        */
/*            2 : Gateway user was registered (needs to add Gateway user     */
/*                name to "Gateway Users Screen")                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*  common =                                                                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int NOTDFAMUserAction(uiKey, pSelect, puiItemNum, pPar)
LONG uiKey;
LIST **pSelect;
LONG *puiItemNum;
void *pPar;
{
    int iRc;

    puiItemNum = puiItemNum;
    pPar = pPar;

    if (uiKey == M_ESCAPE) {
        return 0;
    }

    strcpy(aszGNWUser,(*pSelect)->text);
    aszGDCEUser[0] = '\0';

    iRc = NWSEditString(DCEUSERROW, DCEUSERCOL, DCEUSERHEIGHT, DCEUSERWIDTH, DCEUSER, NOTHING,
                        aszGDCEUser, MAX_DCEUSERNAME - 1, EF_SET , pHandle,
                        NULL, DCEUserAction, CHAR_OF_STRING);

    if (iRc == 2) {
        return 2;
    }
    return -1;
}


/*EnglishComment**************************************************************/
/*  ID   = DCEUserAction                                                     */
/*  name = "DCE User Name Input Screen" processing routine                   */
/*  func = (1) Goes back to "Gateway Users Screen" if the Esc key was        */
/*             pressed.                                                      */
/*         (2) Displays "DCE User Name Screen" if the Enter key was          */
/*             pressed.                                                      */
/*                                                                           */
/*  i/o  = uiAction   : LONG    :: pressed key                               */
/*         pszDCEUser : BYTE *  :: inputed data (DCE user name)              */
/*         pPar       : void *  :: parameter (not used)                      */
/*                                                                           */
/*  return = -1 : Goes back to "DCE User Name Screen"                        */
/*            2 : DCE user was registered                                    */
/*                (needs to add Gateway user to "Gateway Users Screen")      */
/*            3 : Goes back to "Gateway Users Screen"                        */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int DCEUserAction(uiAction, pszDCEUser, pPar)
LONG uiAction;
BYTE *pszDCEUser;
void *pPar;
{
    int iRc;

    pPar = pPar;

    if ((uiAction & E_ESCAPE) != (LONG)0) {
        *pszDCEUser = '\0';
        return 3;
    }

    if (*pszDCEUser == '\0') {
        NWSAlert(0, 0, pHandle, DCEUSERNAMEFAILURE);
        return -1;
    }

    *aszGDCEPwd = '\0';


    iRc = NWSEditString(DCEPASSROW, DCEPASSCOL, DCEPASSHEIGHT, DCEPASSWIDTH, DCEPASSWD, NULL,
                        aszGDCEPwd, MAX_DCEPASSWD - 1, EF_NOECHO | EF_SET, pHandle, NULL,
                        DCEPasswdAction, CHAR_OF_STRING);

    if (iRc == -2) {
        return -1;
    }
    else if (iRc == 2) {
        return 2;
    }
    return 3;
}



/*EnglishComment**************************************************************/
/*  ID   = DCEPasswdAction                                                   */
/*  name = "DCE Password Screen" processing routine                          */
/*  func = (1) Goes back to "DCE Password Screen" if the Esc key was         */
/*             pressed.                                                      */
/*         (2) Displays "Re-enter DCE Password Screen" if the Enter key      */
/*             was pressed.                                                  */
/*                                                                           */
/*  i/o  = uiAction   : LONG    :: pressed key                               */
/*         pszDCEUser : BYTE *  :: inputed data (DCE user name)              */
/*         pPar       : void *  :: parameter (not used)                      */
/*                                                                           */
/*  return = -2 : Goes back to "DCE User Name Screen"                        */
/*           -1 : Goes back to "DCE Password Screen"                         */
/*            2 : DCE user was registered (needs to add Gateway user to      */
/*                "Gatway Users Screen")                                     */
/*            3 : Goes back to "Gateway Users Screen"                        */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int DCEPasswdAction(uiAction, pszDCEPwd, pPar)
LONG uiAction;
BYTE *pszDCEPwd;
void *pPar;
{
    int iLen;
    int iCount;
    int iRc;

    pPar = pPar;

    iLen = strlen(pszDCEPwd);

    if ((uiAction & E_ESCAPE) != (LONG)0) {
        for (iCount = 0; iCount < iLen; iCount++) {
            *pszDCEPwd = '\0';
        }
        return -2;
    }




    for (iCount = 0; iCount < iLen; iCount++) {
        if (isprint(pszDCEPwd[iCount]) == 0) {
            NWSAlert(0, 0, pHandle, DCEPASSWORDFAILURE);
            memset(pszDCEPwd, 0x00, iLen);
            return -1;
        }
    }

    if (*pszDCEPwd == '\0') {
        NWSAlert(0, 0, pHandle, DCEPASSWORDFAILURE);
        return -1;
    }

    *aszGDCERePwd = '\0';




    iRc = NWSEditString(DCEPASSROW, DCEPASSCOL, DCEPASSHEIGHT, DCEPASSWIDTH, REENTERDCEPASSWD, NULL,
                        aszGDCERePwd, MAX_DCEPASSWD - 1,  EF_NOECHO | EF_SET, pHandle, NULL,
                        DCEReEnterPasswdAction, CHAR_OF_STRING);

    for (iCount = 0; iCount < iLen; iCount++) {
        *pszDCEPwd = '\0';
    }

    if (iRc == -3) {
        iRc = -1;
    }

    return iRc;
}


/*EnglishComment**************************************************************/
/*  ID   = DCEReEnterPasswdAction                                            */
/*  name = "Re-Enter DCE Password Screen" processing routine                 */
/*  func = (1) Goes back to "DCE User Name Screen" if the Esc. key was       */
/*             pressed.                                                      */
/*         (2) Compares the DCE password entered at "DCE Password Screen"    */
/*             with the actual password.                                     */
/*         (3) If the password matched:                                      */
/*              - Allocates a property for a DCE user name, and registers    */
/*                the DCE user name entered at "DCE User Name Screen."       */
/*              - Allocates a property for a DCE password, and registers     */
/*                the DCE password entered at "DCE Password Screen."         */
/*              - Allocates a property for "Admin Utility Executable User,"  */
/*                and registers "NO (unexecutable)" to the property.         */
/*                                                                           */
/*  i/o  = uiAction    : LONG    :: pressed key                              */
/*         pszDCERePwd : BYTE *  :: inputed data (re-entered password)       */
/*         pPar        : void *  :: parameter (not used)                     */
/*                                                                           */
/*  return = -3 : Goes back to "DCE Password Screen"                         */
/*           -2 : Goes back to "DCE User Name Screen"                        */
/*           -1 : Goes back to "Re-Enter DCE Password Screen"                */
/*            2 : Gateway user was registered (needs to add Gateway user     */
/*                name to "Gateway Users Screen")                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int DCEReEnterPasswdAction(uiAction, pszDCERePwd, pPar)
LONG uiAction;
BYTE *pszDCERePwd;
void *pPar;
{
    char *pszDCEUser;
    char *pszDCEPwd;
    LONG auiKey[2];
    long iUserID;
    int iLen;
    int iCnt;
    int iSegmentNum;
    int iRc;
    int iRtn;
    BYTE byMore;
    char aszSegment[SEGMENTNUM];
    BYTE aszEnc[SEGMENTNUM];

    pPar = pPar;

    iLen = strlen(pszDCERePwd);

    if ((uiAction & E_ESCAPE) != (LONG)0) {
        for (iCnt = 0; iCnt < iLen; iCnt++) {
            pszDCERePwd[iCnt] = '\0';
        }
        return -2;
    }

    if (strcmp(pszDCERePwd, aszGDCEPwd) != 0) {
        NWSAlert(0, 0, pHandle, DCEPASSWORDNOTSAME);
        for (iCnt = 0; iCnt < iLen; iCnt++) {
            pszDCERePwd[iCnt] = '\0';
        }
        return -3;
    }

    iRc = GetBinderyObjectID(aszGNWUser, OT_USER, &iUserID);
    switch (iRc) {
      case ESUCCESS :
        break;

      case ERR_SERVER_OUT_OF_MEMORY :
        NWSAlert(0, 0, pHandle, OUTOFMEMORY);
        for (iCnt = 0; iCnt < iLen; iCnt++) {
            pszDCERePwd[iCnt] = '\0';
        }
        return -1;
        break;

      case ERR_INVALID_NAME :
      case ERR_WILDCARD_NOT_ALLOWED :
        sprintf(aszError, "%d", iRc);
        AlertErrorCode(ADMINERROR, aszError);

        for (iCnt = 0; iCnt < iLen; iCnt++) {
            pszDCERePwd[iCnt] = '\0';
        }
        return -1;
        break;

      default :
        sprintf(aszError, "%d", iRc);
        AlertErrorCode(BINDERYERROR, aszError);
        for (iCnt = 0; iCnt < iLen; iCnt++) {
            pszDCERePwd[iCnt] = '\0';
        }
        return -1;
        break;
    }

    auiKey[0] = (LONG)iUserID;
    auiKey[1] = (LONG)iUserID;


    if (strcmp(aszGNWUser,"SUPERVISOR")) {

        iRc = CreateProperty(aszGNWUser, OT_USER, ADMIN_EXEC_PROPERTY, BF_STATIC | BF_ITEM,
                             BS_SUPER_WRITE | BS_SUPER_READ);

        if (iRc == ESUCCESS) {

            iSegmentNum = 1;
            byMore = 0x00;
            memset(aszSegment, '\0', SEGMENTNUM);
            aszSegment[0] = 'N';
            aszSegment[1] = '\0';

            iRtn = WritePropertyValue(aszGNWUser, OT_USER, ADMIN_EXEC_PROPERTY, iSegmentNum,
                                      aszSegment,byMore);

            if (iRtn == ESUCCESS) {
                ;
            }

            else if (iRtn == ERR_SERVER_OUT_OF_MEMORY) {
                NWSAlert(0, 0, pHandle, OUTOFMEMORY);
                for (iCnt = 0; iCnt < iLen; iCnt++) {
                    pszDCERePwd[iCnt] = '\0';
                }
                DeleteProperty(aszGNWUser, OT_USER, ADMIN_EXEC_PROPERTY);
                return -1;
            }

            else {
                sprintf(aszError, "%d", iRtn);
                AlertErrorCode(BINDERYERROR, aszError);
                for (iCnt = 0; iCnt < iLen; iCnt++) {
                    pszDCERePwd[iCnt] = '\0';
                }
                DeleteProperty(aszGNWUser, OT_USER, ADMIN_EXEC_PROPERTY);
                return -1;
            }
        }

        else if (iRc == ERR_PROPERTY_ALREADY_EXISTS) {
            ;
        }

        else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
            NWSAlert(0, 0, pHandle, OUTOFMEMORY);
            for (iCnt = 0; iCnt < iLen; iCnt++) {
                pszDCERePwd[iCnt] = '\0';
            }
            return -1;
        }

        else {
            sprintf(aszError, "%d", iRc);
            AlertErrorCode(BINDERYERROR, aszError);
            for (iCnt = 0; iCnt < iLen; iCnt++) {
                pszDCERePwd[iCnt] = '\0';
            }
            return -1;
        }
    }


    iRc = CreateProperty(aszGNWUser, OT_USER, DCE_PWD_PROPERTY, BF_STATIC | BF_ITEM,
                         BS_SUPER_WRITE | BS_SUPER_READ);

    if ((iRc == ESUCCESS)
      ||(iRc == ERR_PROPERTY_ALREADY_EXISTS)) {

        iSegmentNum = 1;
        byMore = 0xFF;
        pszDCEPwd = aszGDCEPwd;

        while (1) {

            memset(aszSegment, '\0', SEGMENTNUM);
            memset(aszEnc, '\0', SEGMENTNUM);

            for (iCnt = 0; iCnt <= SEGMENTNUM - 1; iCnt++) {
                aszSegment[iCnt] = *pszDCEPwd;
                if (*pszDCEPwd == '\0') {
                    break;
                }
                pszDCEPwd++;
            }

            if (aszSegment[SEGMENTNUM - 1] == '\0') {
                byMore = 0x00;
            }

            fnSetEncryptData(auiKey, aszSegment, SEGMENTNUM, aszEnc);

            iRtn = WritePropertyValue(aszGNWUser, OT_USER, DCE_PWD_PROPERTY, iSegmentNum,
                                      aszEnc, byMore);

            if (iRtn == ESUCCESS) {

                iSegmentNum++;
                if (byMore == 0x00) {
                    break;
                }
            }

            else if (iRtn == ERR_SERVER_OUT_OF_MEMORY) {
                NWSAlert(0, 0, pHandle, OUTOFMEMORY);
                for (iCnt = 0; iCnt < iLen; iCnt++) {
                    pszDCERePwd[iCnt] = '\0';
                }
                DeleteProperty(aszGNWUser, OT_USER, DCE_PWD_PROPERTY);
                DeleteProperty(aszGNWUser, OT_USER, ADMIN_EXEC_PROPERTY);
                return -1;
            }

            else {
                sprintf(aszError, "%d", iRtn);
                AlertErrorCode(BINDERYERROR, aszError);
                for (iCnt = 0; iCnt < iLen; iCnt++) {
                    pszDCERePwd[iCnt] = '\0';
                }
                DeleteProperty(aszGNWUser, OT_USER, DCE_PWD_PROPERTY);
                DeleteProperty(aszGNWUser, OT_USER, ADMIN_EXEC_PROPERTY);
                return -1;
            }
        }
    }

    else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
        NWSAlert(0, 0, pHandle, OUTOFMEMORY);
        for (iCnt = 0; iCnt < iLen; iCnt++) {
            pszDCERePwd[iCnt] = '\0';
        }
        DeleteProperty(aszGNWUser, OT_USER, ADMIN_EXEC_PROPERTY);
        return -1;
    }

    else {
        sprintf(aszError, "%d", iRc);
        AlertErrorCode(BINDERYERROR, aszError);
        for (iCnt = 0; iCnt < iLen; iCnt++) {
            pszDCERePwd[iCnt] = '\0';
        }
        DeleteProperty(aszGNWUser, OT_USER, ADMIN_EXEC_PROPERTY);
        return -1;
    }

    for (iCnt = 0; iCnt < iLen; iCnt++) {
        pszDCERePwd[iCnt] = '\0';
    }


    iRc = CreateProperty(aszGNWUser, OT_USER, DCE_USER_PROPERTY, BF_STATIC | BF_ITEM,
                         BS_SUPER_WRITE | BS_ANY_READ);

    if ((iRc == ESUCCESS)
      ||(iRc == ERR_PROPERTY_ALREADY_EXISTS)) {
        iSegmentNum = 1;
        byMore = 0xFF;
        pszDCEUser = aszGDCEUser;

        while (1) {

            memset(aszSegment, '\0', SEGMENTNUM);
            memset(aszEnc, '\0', SEGMENTNUM);

            for (iCnt = 0; iCnt <= SEGMENTNUM - 1; iCnt++) {
                aszSegment[iCnt] = *pszDCEUser;
                if (*pszDCEUser == '\0') {
                    break;
                }
                pszDCEUser++;
            }

            if (aszSegment[SEGMENTNUM - 1] == '\0') {
                byMore = 0x00;
            }

            fnSetEncryptData(auiKey, aszSegment, SEGMENTNUM, aszEnc);

            iRtn = WritePropertyValue(aszGNWUser, OT_USER, DCE_USER_PROPERTY, iSegmentNum,
                                      aszEnc, byMore);

            if (iRtn == ESUCCESS) {

                iSegmentNum++;
                if (byMore == 0x00) {
                    break;
                }
            }

            else if (iRtn == ERR_SERVER_OUT_OF_MEMORY) {
                NWSAlert(0, 0, pHandle, OUTOFMEMORY);
                DeleteProperty(aszGNWUser, OT_USER, DCE_USER_PROPERTY);
                DeleteProperty(aszGNWUser, OT_USER, DCE_PWD_PROPERTY);
                DeleteProperty(aszGNWUser, OT_USER, ADMIN_EXEC_PROPERTY);
                return -1;
            }

            else {
                sprintf(aszError, "%d", iRtn);
                AlertErrorCode(BINDERYERROR, aszError);
                DeleteProperty(aszGNWUser, OT_USER, DCE_USER_PROPERTY);
                DeleteProperty(aszGNWUser, OT_USER, DCE_PWD_PROPERTY);
                DeleteProperty(aszGNWUser, OT_USER, ADMIN_EXEC_PROPERTY);
                return -1;
            }
        }
    }

    else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
        NWSAlert(0, 0, pHandle, OUTOFMEMORY);
        DeleteProperty(aszGNWUser, OT_USER, DCE_PWD_PROPERTY);
        DeleteProperty(aszGNWUser, OT_USER, ADMIN_EXEC_PROPERTY);
        return -1;
    }

    else {
        sprintf(aszError, "%d", iRc);
        AlertErrorCode(BINDERYERROR, aszError);
        DeleteProperty(aszGNWUser, OT_USER, DCE_PWD_PROPERTY);
        DeleteProperty(aszGNWUser, OT_USER, ADMIN_EXEC_PROPERTY);
        return -1;
    }

    return 2;
}


/*EnglishComment**************************************************************/
/*  ID   = DFAMInfoAction                                                    */
/*  name = "Gateway User Information Screen" processing routine              */
/*  func = (1) Goes back to "Gateway Users Screen" if the Esc. key was       */
/*             pressed.                                                      */
/*                                                                           */
/*  i/o  = uiKey      : LONG    :: pressed key                               */
/*         pSelect    : LIST ** :: selected element                          */
/*         puiItemNum : LONG    :: item number (not used)                    */
/*         pPar       : void *  :: parameter (not used)                      */
/*                                                                           */
/*  return = -1 : Goes back to "Gateway User Information Screen."            */
/*            2 : Goes back to "Gateway Users Screen."                       */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int DFAMInfoAction(uiKey, pSelect, puiItemNum, pPar)
LONG uiKey;
LIST **pSelect;
LONG *puiItemNum;
void *pPar;
{
    FIELD *pFptr;
    LONG uiCcode;
    int iRc;
    int bSpflg;
    int bAflg;
    int iErrno;
    char aszSegment[SEGMENTNUM];
    BYTE byFlags;
    BYTE byMore;
    BYTE byExecute;

    pPar = pPar;
    pSelect = pSelect;

    if (uiKey == M_ESCAPE) {
        return 2;
    }

    if ( (*puiItemNum + 1) == 1) {

        iRc = GetDCEUserName(aszGNWUser, aszGDCEUser, &iErrno);
        switch (iRc) {
          case 0 :
            break;

          case -3 :
            NWSAlert(0, 0, pHandle, OUTOFMEMORY);
            return -1;
            break;

          case -1 :
          case -2 :
          case -4 :
            sprintf(aszError, "%d", iErrno);
            AlertErrorCode(BINDERYERROR, aszError);
            return -1;
            break;

          default :
            sprintf(aszError, "%d", iRc);
            AlertErrorCode(ADMINERROR, aszError);
            return -1;
            break;
        }

        iRc = NWSEditString(DCEUSERROW, DCEUSERCOL, DCEUSERHEIGHT, DCEUSERWIDTH, DCEUSER, NOTHING,
                            aszGDCEUser, MAX_DCEUSERNAME - 1, EF_SET, pHandle, NULL,
                            DCEUserAction, CHAR_OF_STRING);

    }
    else {
        if (strcmp(aszGNWUser, "SUPERVISOR")) {
            byExecute = 0x00;
            bSpflg = 0;

            iRc = ReadPropertyValue(aszGNWUser, OT_USER, ADMIN_EXEC_PROPERTY, 1, aszSegment,
                                    &byMore, &byFlags);

            if (iRc == ESUCCESS) {
                if (aszSegment[0] == 'Y') {
                    byExecute = 0x01;
                }
            }

            else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
                NWSAlert(0, 0, pHandle, OUTOFMEMORY);
                return -1;
            }

            else {
                sprintf(aszError, "%d", iRc);
                AlertErrorCode(BINDERYERROR, aszError);
                return -1;
            }
        }
        else {
            byExecute = 0x01;
            bSpflg = 1;
        }

        uiCcode = NWSPushList(pHandle);
        if (uiCcode == (LONG)0) {
            NWSAlert(0, 0, pHandle, SCREENPREFAILED);
            return -1;
        }

        bAflg = 0;
        NWSInitForm(pHandle);

        pFptr = NWSAppendCommentField(0, 1, NWSGetMessage(EXECUTION, &(pHandle->messages)),
                                      pHandle);

        if (pFptr == (FIELD *)NULL) {
            bAflg = 1;
        }

        if (bSpflg == 0) {
            pFptr = NWSAppendBoolField(0, ADMINEXECFIELD, NORMAL_FIELD, &byExecute, NULL,
                                       pHandle);
            if (pFptr == (FIELD *)NULL) {
                bAflg = -1;
            }
        }
        else {
            pFptr = NWSAppendCommentField(0, ADMINEXECFIELD, "Yes", pHandle);
            if (pFptr == (FIELD *)NULL) {
                bAflg = -1;
            }
        }

        if (bAflg != 0) {
            NWSAlert(0, 0, pHandle, SCREENPREFAILED);
            NWSPopList(pHandle);
            return -1;
        }

        iRc = NWSEditPortalForm(ADMINEXECUTION, ADMINEXECROW, ADMINEXECCOL, ADMINEXECHEIGHT,
                                ADMINEXECWIDTH, F_NOVERIFY, F_NO_HELP, NULL, pHandle);

        if (bSpflg == 0) {
            aszSegment[0] = 'N';
            aszSegment[1] = '\0';
            if (byExecute == 0x01) {
                aszSegment[0] = 'Y';
            }

            iRc = WritePropertyValue(aszGNWUser, OT_USER, ADMIN_EXEC_PROPERTY, 1, aszSegment,
                                     0x00);

            if (iRc == ESUCCESS) {
                ;
            }

            else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
                NWSAlert(0, 0, pHandle, OUTOFMEMORY);
            }

            else {
                sprintf(aszError, "%d", iRc);
                AlertErrorCode(BINDERYERROR, aszError);
            }
        }
        NWSDestroyForm (pHandle);
        NWSPopList(pHandle);
    }
    return -1;
}


/*EnglishComment**************************************************************/
/*  ID   = GetDCEUserName                                                    */
/*  name = DCE user name processing routine                                  */
/*  func = Gets a DCE user name related to the designated Gateway user.      */
/*         (1) Checks if there is a DCE property in the property of the      */
/*             designated Gateway user.                                      */
/*         (2) If the DCE property exists, this routine reads a property     */
/*             value from the DCE property.                                  */
/*         (3) Encrypts the property value with using the NetWare user ID    */
/*             as a key.                                                     */
/*                                                                           */
/*  i/o  = pszDFAMUserName : char * : i : Gateway user name                  */
/*         pszDCEUserName  : char * : o : DCE user name                      */
/*         iErrno          : int *  : o : detailed error code                */
/*                                                                           */
/*  return = 0 : normally completed                                          */
/*          -1 : not a NetWare user                                          */
/*          -2 : not a Gateway user                                          */
/*          -3 : shortage of memory                                          */
/*          -4 : failed to access bindery                                    */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
int GetDCEUserName(pszDFAMUserName, pszDCEUserName, iErrno)
char *pszDFAMUserName;
char *pszDCEUserName;
int *iErrno;
{
    BYTE *pszDCEUser;
    LONG auiKey[2];
    long iUserID;
    int iRc;
    int iSegmentNum;
    BYTE aszSegment[SEGMENTNUM];
    BYTE aszDec[SEGMENTNUM];
    BYTE byFlag;
    BYTE byMore;

    *iErrno = 0;

    iRc = GetBinderyObjectID(pszDFAMUserName, OT_USER, &iUserID);
    switch (iRc) {
      case ESUCCESS :
        break;

      case ERR_SERVER_OUT_OF_MEMORY :
        *iErrno = iRc;
        return -3;
        break;

      case ERR_INVALID_NAME :
      case ERR_NO_SUCH_OBJECT :
        *iErrno = iRc;
        return -1;
        break;

      default :
        *iErrno = iRc;
        return -4;
        break;
    }

    iSegmentNum = 1;
    pszDCEUser = pszDCEUserName;

    auiKey[0] = (LONG)iUserID;
    auiKey[1] = (LONG)iUserID;

    while (1) {
        memset(aszSegment, '\0', SEGMENTNUM);
        iRc = ReadPropertyValue(pszDFAMUserName, OT_USER, DCE_USER_PROPERTY, iSegmentNum,
                                aszSegment, &byMore, &byFlag);

        if (iRc == ESUCCESS) {

            fnGetDecryptData(auiKey, aszSegment, SEGMENTNUM, aszDec);

            if (byMore == 0) {
                strcpy(pszDCEUser, aszDec);
                break;
            }

            strncpy(pszDCEUser ,aszDec ,SEGMENTNUM);
            pszDCEUser += SEGMENTNUM;
            iSegmentNum++;
        }

        else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
            *pszDCEUserName = '\0';
            *iErrno = iRc;
            return -3;
        }

        else if(iRc == ERR_NO_SUCH_PROPERTY) {
            *pszDCEUserName = '\0';
            *iErrno = iRc;
            return -2;
        }

        else {
            *pszDCEUserName = '\0';
            *iErrno = iRc;
            return -4;
        }
    }
    return 0;
}
