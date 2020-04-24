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
 * $Log: uadmmain.c,v $
 * Revision 1.1.2.2  1996/03/11  14:09:13  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:19  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:20:16  takano
 * 	First Submit
 * 	[1996/02/21  08:20:07  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <io.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include <advanced.h>
#include <signal.h>
#include <time.h>
#include <nwsnut.h>
#include <nwlocale.h>
#include <nwbindry.h>
#include <nwenvrn.h>
#include <nwdir.h>
#include <errno.h>
#include <niterror.h>

#include "uadmdef.h"
#include "uadmmlh.h"
#include "uadmmlc.h"

NUTInfo *pHandle;
FILE *pVolFp;
FILE *pAgtFp;
FILE *pOthrFp;
LONG uiAllocTag;
unsigned int uiNLMHandle;
int MainThreadGroupID;
int iScreenID;
BYTE aszNWUser[MAX_NWUSERNAME];
char aszNWver[10];
char aszNWserver[_MAX_SERVER];
char aszError[16];

struct VolumeInfo *pVInfoTop;
struct AgentInfo *pAInfo;
struct OtherInfo *pOther;


/*EnglishComment**************************************************************/
/*  ID   = main                                                              */
/*  name = Admin Utility main routine                                        */
/*  func = (1) Initializes the Admin Utility.                                */
/*         (2) Processes the login for the Admin Utility (including the      */
/*             password handling).                                           */
/*         (3) Displays "Main Screen."                                       */
/*  i/o  = none                                                              */
/*  return =                                                                 */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

void main()
{
    LIST *pPtr;
    LONG uiCcode;
    long iCcode;
    int iRc;
    int bListflg;


    pVInfoTop = NULL;
    pAInfo = NULL;
    pOther = NULL;

    pVolFp = NULL;
    pAgtFp = NULL;
    pOthrFp = NULL;

    pHandle = (NUTInfo *)NULL;
    iScreenID = -1;

    aszError[0] = '\0';

    MainThreadGroupID = GetThreadGroupID();
    signal(SIGTERM, adminUnload);
    AtUnload(adminUnload2);

    uiNLMHandle = GetNLMHandle();

    iScreenID = CreateScreen(pMsgTable[ADMINSCREEN], DONT_CHECK_CTRL_CHARS | AUTO_DESTROY_SCREEN);
    if (iScreenID < 1) {
        ConsolePrintf("%s\n",pMsgTable[SCREENFAILED]);
        return;
    }

    getVerInfo();

    if (strcmp(aszNWver, "3.12") != 0) {
        printf("%s\n",pMsgTable[INVALSERVVER]);
        PressAnyKeyToContinue();
        DestroyScreen(iScreenID);
        return;
    }


    uiAllocTag = AllocateResourceTag(uiNLMHandle, "DFA Admin. Alloc Tag", AllocSignature);
    if (!uiAllocTag) {
        printf("%s\n",pMsgTable[RESOURCETAGNOTALLOC]);
        PressAnyKeyToContinue();
        DestroyScreen(iScreenID);
        return;
    }

    iRc = AdminLogin();
    if (iRc != 0) {
        PressAnyKeyToContinue();
        DestroyScreen(iScreenID);
        return;
    }


    iCcode = NWSInitializeNut(ADMIN, VERSION, NORMAL_HEADER, NUT_REVISION_LEVEL, pMsgTable,
                              NULL, iScreenID, uiAllocTag, &pHandle);

    if (iCcode) {
        printf("%s\n",pMsgTable[SCREENFAILED]);
        PressAnyKeyToContinue();
        DestroyScreen(iScreenID);
        return;
    }

    iRc = DisplayScreen(iScreenID);
    if (iRc != ESUCCESS) {
        NWSRestoreNut(pHandle);
        printf("%s\n",NWSGetMessage(SCREENFAILED, &(pHandle->messages)));
        PressAnyKeyToContinue();
        DestroyScreen(iScreenID);
        return;
    }


    bListflg = OFF;
    NWSInitList(pHandle, free);
    pPtr = NWSAppendToList(NWSGetMessage(DFAMVOLUME, &(pHandle->messages)),
                           (void *)0,  pHandle);
    if (pPtr == (LIST *)NULL) {
        bListflg = ON;
    }

    pPtr = NWSAppendToList(NWSGetMessage(DFAMAGENT, &(pHandle->messages)),
                           (void *)0, pHandle);
    if (pPtr == (LIST *)NULL) {
        bListflg = ON;
    }

    pPtr = NWSAppendToList(NWSGetMessage(DFAMUSER, &(pHandle->messages)),
                           (void *)0, pHandle);
    if (pPtr == (LIST *)NULL) {
        bListflg = ON;
    }

    pPtr = NWSAppendToList(NWSGetMessage(DFAMGROUP, &(pHandle->messages)),
                           (void *)0, pHandle);
    if (pPtr == (LIST *)NULL) {
        bListflg = ON;
    }

    pPtr = NWSAppendToList(NWSGetMessage(DFAMOTHER, &(pHandle->messages)),
                           (void *)0, pHandle);
    if (pPtr == (LIST *)NULL) {
        bListflg = ON;
    }

    pPtr = NWSAppendToList(NWSGetMessage(EXITADMIN, &(pHandle->messages)),
                           (void *)0, pHandle);
    if (pPtr == (LIST *)NULL) {
        bListflg = ON;
    }

    if (bListflg == ON) {
        NWSAlert(0, 0, pHandle, SCREENPREFAILED);
        goto lbAdminend;
    }

    uiCcode = NWSList(MENU,  MENUROW,  MENUCOL, MENUHEIGHT,
                      strlen( NWSGetMessage(DFAMVOLUME, &(pHandle->messages)) ) + 4,
                      M_ESCAPE | M_SELECT | M_NO_SORT, NULL, pHandle, NULL, MenuAction, NULL);

lbAdminend:
    NWSDestroyList(pHandle);
    NWSRestoreNut(pHandle);
    DestroyScreen(iScreenID);
    iScreenID = -1;

    return;
}


/*EnglishComment**************************************************************/
/*  ID   = adminUnload                                                       */
/*  name = UNLOAD command input routine                                      */
/*  func = (1) Changes the context.                                          */
/*         (2) If the file information is on the memory, this routine sets   */
/*             the information to the file.                                  */
/*         (3) Frees the allocated memory.                                   */
/*         (4) Frees the NUT structure.                                      */
/*                                                                           */
/*  i/o  = none                                                              */
/*  return = none                                                            */
/*                                                                           */
/*  common = "adminUnload (SIGTERM routine)" stores the data on the memory,  */
/*           and frees the allocated memory.  "adminUnload2 (AtUnload        */
/*           routine)" only clears the scree.  This is because:              */
/*             (1) SIGTERM routine cannot clear the screen,                  */
/*             (2) AtUnload routine cannot issue Blocking API.               */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

void adminUnload(dummy)
int dummy;
{
    int iRc;

    dummy = dummy;
    iRc = SetThreadGroupID(MainThreadGroupID);
    if (iRc == EFAILURE) {
        return;
    }

    if (pVolFp != NULL) {
        fclose(pVolFp);
    }

    if (pAgtFp != NULL) {
        fclose(pAgtFp);
    }

    if (pOthrFp != NULL) {
        fclose(pOthrFp);
    }


    if (pVInfoTop != NULL) {
        iRc = writeVolumeFile();
        delAllVolume();
    }

    if (pAInfo != NULL) {
        iRc = writeAgentFile();
        delAllAgent;
    }

    if (pOther != NULL) {
        iRc = writeOptFile();
        delAllOther();
    }

    if (pHandle != NULL) {
        NWSRestoreNut(pHandle);
    }

    return;
}

/*EnglishComment**************************************************************/
/*  ID   = adminUnload2                                                      */
/*  name = UNLOAD inputing routine 2                                         */
/*  func = (1) Clears the screen.                                            */
/*                                                                           */
/*  i/o  = none                                                              */
/*  return =                                                                 */
/*                                                                           */
/*  common = "adminUnload (SIGTERM routine)" stores the data on the memory,  */
/*           and frees the allocated memory.  "adminUnload2 (AtUnload        */
/*           routine)" only clears the scree.  This is because:              */
/*             (1) SIGTERM routine cannot clear the screen,                  */
/*             (2) AtUnload routine cannot issue Blocking API.               */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

void adminUnload2(void)
{
    if (iScreenID != -1) {
        DestroyScreen(iScreenID);
    }

    return;
}


/*EnglishComment**************************************************************/
/*  ID   = AdminLogin                                                        */
/*  name = Admin Utility login routine                                       */
/*  func = (1) Gets the user name and password from the console.             */
/*         (2) Checks if the inputed user name is a NetWare user.            */
/*         (3) Checks if the inputed user is allowed to execute Admin        */
/*             Utility.                                                      */
/*         (4) Checks if the inputed password is correct.                    */
/*         (5) Displays an error message to the console in the case of error */
/*                                                                           */
/*  i/o  = none                                                              */
/*  return = -1 : login failed.                                              */
/*            0 : login successful                                           */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int AdminLogin()
{
    char *pszBuf;
    long iObjID;
    int iRc;
    char aszUserName[MAX_NWUSERNAME];
    char aszPasswd[MAX_NWPASSWORD];
    BYTE aszSegment[SEGMENTNUM];
    BYTE byFlags;
    BYTE byMore;
    char aszMessage[128];

    aszUserName[0] = '\0';
    aszPasswd[0] = '\0';

    printf("%s",pMsgTable[USERNAME]);

    iRc = ConsoleInput(aszUserName, INPUTUSER);
    if (iRc != 0) {

        printf("\n%s\n",pMsgTable[LOGINFAILED]);
        return -1;
    }

    printf("\n%s",pMsgTable[PASSWORD]);
    iRc = ConsoleInput(aszPasswd, INPUTPASS);
    if (iRc != 0) {
        printf("\n%s\n",pMsgTable[LOGINFAILED]);
        return -1;
    }

    if (strcmp(aszUserName, "SUPERVISOR")) {
        iRc = GetBinderyObjectID(aszUserName, OT_USER, &iObjID);

        switch (iRc) {
          case ESUCCESS :
            break;

          case ERR_INVALID_NAME :
          case ERR_WILDCARD_NOT_ALLOWED :
          case ERR_NO_SUCH_OBJECT :
            printf("\n%s\n",pMsgTable[LOGINFAILED]);
            return -1;
            break;

          case ERR_SERVER_OUT_OF_MEMORY :
            printf("\n%s\n",pMsgTable[OUTOFMEMORY]);
            return -1;
            break;

          default :
            sprintf(aszError, "%d", iRc);
            strcpy(aszMessage, pMsgTable[BINDERYERROR]);
            strcat(aszMessage, aszError);
            printf("\n%s\n",aszMessage);
            return -1;
            break;
        }

        iRc = ReadPropertyValue(aszUserName, OT_USER, ADMIN_EXEC_PROPERTY, 1, aszSegment,
                                &byMore, &byFlags);

        switch (iRc) {
          case ESUCCESS :
            if (aszSegment[0] != 'Y') {
                printf("\n%s\n",pMsgTable[LOGINFAILED]);
                return -1;
                break;
            }
            break;

          case ERR_NO_SUCH_SEGMENT :
          case ERR_NO_SUCH_PROPERTY :
          case ERR_WILDCARD_NOT_ALLOWED :
            printf("\n%s\n",pMsgTable[LOGINFAILED]);
            return -1;
            break;

          case ERR_SERVER_OUT_OF_MEMORY :
            printf("\n%s\n",pMsgTable[OUTOFMEMORY]);
            return -1;
            break;

          default :
            sprintf(aszError, "%d", iRc);
            strcpy(aszMessage, pMsgTable[BINDERYERROR]);
            strcat(aszMessage, aszError);
            printf("\n%s\n",aszMessage);
            return -1;
            break;
        }
    }

    iRc = VerifyBinderyObjectPassword(aszUserName, OT_USER, aszPasswd);
    pszBuf = aszPasswd;
    while (*pszBuf != '\0') {
        *pszBuf = '\0';
        pszBuf++;
    }

    switch (iRc) {
      case ESUCCESS :
        break;

      case ERR_NO_SUCH_OBJECT_OR_BAD_PASSWORD :
      case ERR_NO_SUCH_PROPERTY :
      case ERR_WILDCARD_NOT_ALLOWED :
        printf("\n%s\n",pMsgTable[LOGINFAILED]);
        return -1;
        break;

      case ERR_SERVER_OUT_OF_MEMORY :
        printf("\n%s\n",pMsgTable[OUTOFMEMORY]);
        return -1;
        break;

      default :
        sprintf(aszError, "%d", iRc);
        strcpy(aszMessage, pMsgTable[BINDERYERROR]);
        strcat(aszMessage, aszError);
        printf("\n%s\n",aszMessage);
        return -1;
        break;
    }

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = MenuAction                                                        */
/*  name = "Menu Screen" processing routine                                  */
/*  func = (1) Terminates Admin Utility if the Esc. key was pressed.         */
/*         (2) Calls one of the following routines (depends on the value of  */
/*             "uItemNum") if the Enter key was pressed:                     */
/*                - Gateway Volume Options                                   */
/*                - Agent Options                                            */
/*                - Gateway User Options                                     */
/*                - Gateway Group Options                                    */
/*                - Ohter Options                                            */
/*                                                                           */
/*  i/o  = uiKey     : LONG    :: pressed key                                */
/*         pSelect   : LIST ** :: selected element                           */
/*         uiItemNum : LONG *  :: item number                                */
/*         pPar      : void *  :: parameter                                  */
/*                                                                           */
/*  return = -2 : Shortage of memory                                         */
/*           -1 : Goes back to Menu Box.                                     */
/*            2 : Terminates Admin Utility.                                  */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int MenuAction (uiKey, pSelect, puiItemNum, pPar)
LONG uiKey;
LIST **pSelect;
LONG *puiItemNum;
void *pPar;
{
    LONG uiCcode;
    int iRc;

    pSelect = pSelect;
    pPar = pPar;

    if (uiKey == M_ESCAPE) {
        iRc = NWSConfirm(EXITADMIN2, EXITROW, EXITCOL, EXITHEIGHT, NULL, pHandle, NULL);
        if (iRc == 1) {
            return 2;
        }
        return -1;
    }

    switch ((*puiItemNum) + 1) {

      case 1 :
        uiCcode = NWSPushList(pHandle);
        if (uiCcode == (LONG)0) {
            NWSAlert(0, 0, pHandle, SCREENPREFAILED);
            return -1;
        }

        DFAMVolumeList();

        NWSPopList(pHandle);
        break;

      case 2 :
        uiCcode = NWSPushList(pHandle);
        if (uiCcode == (LONG)0) {
            NWSAlert(0, 0, pHandle, SCREENPREFAILED);
            return -1;
        }

        DFAMAgent();

        NWSPopList(pHandle);
        break;

      case 3 :
        uiCcode = NWSPushList(pHandle);
        if (uiCcode == (LONG)0) {
            NWSAlert(0, 0, pHandle, SCREENPREFAILED);
            return -1;
        }

        DFAMUserList();

        NWSPopList(pHandle);
        break;

      case 4 :
        uiCcode = NWSPushList(pHandle);
        if (uiCcode == (LONG)0) {
            NWSAlert(0, 0, pHandle, SCREENPREFAILED);
            return -1;
        }

        DFAMGroupList();

        NWSPopList(pHandle);
        break;

      case 5 :
        uiCcode = NWSPushList(pHandle);
        if (uiCcode == (LONG)0) {
            NWSAlert(0, 0, pHandle, SCREENPREFAILED);
            return -1;
        }

        otherOption();

        NWSPopList(pHandle);
        break;

      default :
        uiCcode = NWSConfirm(EXITADMIN2, EXITROW, EXITCOL, EXITHEIGHT, NULL, pHandle, NULL);

        if (uiCcode == (LONG)1) {
            return 2;
        }
        break;
    }
    return -1;
}


/*EnglishComment**************************************************************/
/*  ID   = getVerInfo                                                        */
/*  name = NetWare version getting routine                                   */
/*  func = Gets the NetWare version, and sets the data onto the global area. */
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

void getVerInfo(void)
{
    FILE_SERV_INFO sbuf;

    GetServerInformation(128, &sbuf);
    sprintf(aszNWver,"%u.%u",sbuf.netwareVersion,sbuf.netwareSubVersion);
    strcpy(aszNWserver, sbuf.serverName);

    return;
}


/*EnglishComment**************************************************************/
/*  ID   = CFGVerCheck                                                       */
/*  name = .CFG file version checking routine                                */
/*  func = Checks the 1st line (the version of .CFG file).                   */
/*                                                                           */
/*  i/o  = pFileP    : FILE * : i : file pointer                             */
/*         aszCFGVer : char * : i : version of .CFG file                     */
/*                                                                           */
/*  return = -2 : version information is destroyed.                          */
/*           -1 : invalid version                                            */
/*            0 : normal end                                                 */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int CFGVerCheck(pFileP, aszCFGVer)
FILE *pFileP;
char *aszCFGVer;
{
    int tmLen;
    int iRc;
    char aszBuf[8];
    char *pszBufp;

    pszBufp = fgets(aszBuf,sizeof(aszBuf),pFileP);

    if (pszBufp == NULL) {
        return -2;

    }

    tmLen = strlen(aszBuf);
    if (aszBuf[tmLen-1] == '\n') {
        aszBuf[tmLen-1] = '\0';
        tmLen--;
    }
    else {
        iRc = SkipReadCFGFile(pFileP);
        return -2;
    }

    if (tmLen != 5) {
        return -2;
    }

    if ((isdigit(aszBuf[0]) == 0)
      ||(isdigit(aszBuf[1]) == 0)
      ||(aszBuf[2] != '.')
      ||(isdigit(aszBuf[3]) == 0)
      ||(isdigit(aszBuf[4]) == 0)) {
        return -2;
    }

    if (strcmp(aszBuf,aszCFGVer) != 0) {
        if (strcmp(aszBuf,CFGVER_0100) != 0) {
            return -1;
        };
    }

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = ConsoleInput                                                      */
/*  name = Inputing characters from the console (the characters are used     */
/*         when logged in)                                                   */
/*  func =                                                                   */
/*                                                                           */
/*  i/o  = pszBuf : char * : i : input buffer                                */
/*         iMode  : int    : i : NetWare user name (47 bytes) which is       */
/*                               echoed back.                                */
/*                               Password (128 bytes) which is not echoed    */
/*                               back.                                       */
/*                                                                           */
/*  return = -1 : character I/O error                                        */
/*            0 : normal end                                                 */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int ConsoleInput(pszBuf, iMode)
char *pszBuf;
int iMode;

{
    int iChr;
    int iRc;
    int tmLen;
    int tmMaxLen;

    tmLen = 0;
    tmMaxLen = MAX_NWUSERNAME;
    if (iMode == INPUTPASS) {
        tmMaxLen = MAX_NWPASSWORD;
    }

    while (1) {

        iChr = getch();

        iChr = toupper(iChr);

        if (iChr == EOF) {
            return -1;
        }

        if (iChr == 0x00) {
            iChr = getch();
            if (iChr == EOF) {
                return -1;
            }
            RingTheBell();
        }

        else if (iChr == 0x08) {
            if (tmLen == 0) {
                RingTheBell();
            }
            else {
                tmLen--;
                pszBuf[tmLen] = '\0';

                if (iMode == INPUTUSER) {
                    printf("\x08\x20\x08");
                }
            }
        }

        else if (iChr == 0x0d) {
            pszBuf[tmLen] = '\0';
            break;
        }

        else if ( isgraph(iChr) == 0) {
            RingTheBell();
        }

        else if (tmLen >= tmMaxLen - 1) {
            RingTheBell();
        }

        else {
            pszBuf[tmLen] = iChr;
            tmLen++;
            if (iMode == INPUTUSER) {
                iRc = putch(iChr);
                if (iRc == EOF) {
                    return -1;
                }
            }
        }
    }
    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = isDigitString                                                     */
/*  name = Checking if the designated character string is numeric            */
/*  func =                                                                   */
/*                                                                           */
/*  i/o  = pszBuf : char * : i : input buffer                                */
/*                                                                           */
/*  return = 1 or greater: numeric (string length)                           */
/*           0           : contains non-numeric code.                        */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int isDigitString(pszBuf)
char *pszBuf;
{
    int iCnt;

    for (iCnt = 0; pszBuf[iCnt] != NULL && isdigit(pszBuf[iCnt]) != 0; ++iCnt);
    if (pszBuf[iCnt] != NULL) {
        iCnt = 0;
    }
    return iCnt;
}


/*EnglishComment**************************************************************/
/*  ID   = SkipReadCFGFile                                                   */
/*  name = Read and discard routine                                          */
/*  func = If the data are longer than the read buffer when .CFG file is     */
/*         read, this routine reads and discards the excess.                 */
/*                                                                           */
/*  i/o  = pFp : FILE * : i : file pointer                                   */
/*                                                                           */
/*  return = -2 : EOF                                                        */
/*           -1 : failed to read file                                        */
/*            0 : successfully read and discarded                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int SkipReadCFGFile(pFp)
FILE *pFp;
{
    char *pszBuf;
    char aszReadBuf[64];

    while (1) {
        if (fgets(aszReadBuf, sizeof(aszReadBuf), pFp) == NULL) {
            if (ferror(pFp) != 0) {
                return -1;
            }
            else {
                return -2;
            }
        }

        pszBuf = strrchr(aszReadBuf,'\n');
        if (pszBuf != NULL) {
            break;
        }
    }

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = AlertErrorCode                                                    */
/*  name = Detailed code addition routine                                    */
/*  func = Adds a detailed code to the message displayed by NWSAlert()       */
/*                                                                           */
/*  i/o  = uiMessage : LONG   : i : message ID                               */
/*         pszError  : char * : i : error detailed information               */
/*                                                                           */
/*  return =                                                                 */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
void AlertErrorCode(uiMessage, pszError)
LONG uiMessage;
char *pszError;
{
    BYTE *pszGetMsg;
    char aszMessage[128];

    pszGetMsg = NWSGetMessage(uiMessage, &(pHandle->messages));
    strcpy(aszMessage, pszGetMsg);
    strcat(aszMessage, pszError);
    strcat(aszMessage, ".");

    NWSSetDynamicMessage(DYNAMIC_MESSAGE_ONE, aszMessage, &(pHandle->messages));

    NWSAlert(0, 0, pHandle, DYNAMIC_MESSAGE_ONE);

    return;
}
