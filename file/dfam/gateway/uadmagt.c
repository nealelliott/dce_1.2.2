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
 * $Log: uadmagt.c,v $
 * Revision 1.1.2.2  1996/03/11  14:09:09  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:16  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:19:20  takano
 * 	First Submit
 * 	[1996/02/21  08:19:14  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <process.h>
#include <ctype.h>
#include <nwsnut.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <niterror.h>
#include <sys/types.h>
#include <netdb.h>

#include "uadmmlh.h"
#include "uadmdef.h"
#include "uadmext.h"
#include "encrypt.h"


#define SHUFFLENUM 8



NETDB_DEFINE_CONTEXT
char aszGDFAMAgent[MAX_DFAMAGENTNAME];
char aszGMasterKey[MAX_MASTERKEYLEN + 1];
char aszGReMasterKey[MAX_MASTERKEYLEN + 1];


/*EnglishComment**************************************************************/
/*  ID   = DFAMAgent                                                         */
/*  name = Agent screen creating routine                                     */
/*  func = (1) Checks if DFA.NLM is running.                                 */
/*         (2) Initializes the Agent Information.                            */
/*         (3) Cheks if the Agent name is in the HOST file.                  */
/*         (4) Makes the Agent screen.                                       */
/*                                                                           */
/*  io     = none                                                            */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

void DFAMAgent(void)
{
    LIST *pPtr;
    LONG uiCcode;
    int iRc;
    int iWidth;
    int iNWSEnd;


    *aszGDFAMAgent = '\0';
    *aszGMasterKey = '\0';
    *aszGReMasterKey = '\0';
    iNWSEnd = 0;

    NWSInitList(pHandle, free);

    iRc = DFAMLoadCheck();
    if (iRc != 0) {
        NWSAlert(0, 0, pHandle, DFAMLOADED);
        NWSDestroyList(pHandle);
        return;
    }

    NWSStartWait(WAITAGENTROW, WAITAGENTCOL, pHandle);

    pAInfo = (struct AgentInfo *)NULL;

    iRc = AgentInit();
    switch (iRc) {
      case 1 :
      case 0 :
        break;

      case -1 :
        NWSEndWait(pHandle);
        NWSAlert(0, 0, pHandle, FILEACCESSERROR);
        NWSDestroyList(pHandle);
        return;
        break;

      case -2 :
        NWSEndWait(pHandle);
        NWSAlert(0, 0, pHandle, OUTOFMEMORY);
        NWSDestroyList(pHandle);
        return;
        break;

      case -3 :
        NWSEndWait(pHandle);
        NWSAlert(0, 0, pHandle, AGENTFILENOTUSE);
        NWSDestroyList(pHandle);
        return;
        break;

      case -4 :
        NWSEndWait(pHandle);
        iNWSEnd = 1;
        NWSAlert(0, 0, pHandle, AGENTFILEINVALID);
        break;

      case -5 :
        NWSEndWait(pHandle);
        iNWSEnd = 1;
        NWSAlert(0, 0, pHandle, DFAMAGENTNOTFOUND);
        break;

      default :
        NWSEndWait(pHandle);
        sprintf(aszError, "%d", iRc);
        AlertErrorCode(ADMINERROR, aszError);
        NWSDestroyList(pHandle);
        return;
        break;

    }

    if (pAInfo != (struct AgentInfo *)NULL) {

        pPtr = NWSAppendToList(pAInfo->aszDFAMAgent, (void *)0, pHandle);
        if (pPtr == (LIST *)NULL) {
            NWSEndWait(pHandle);
            NWSAlert(0, 0, pHandle, SCREENPREFAILED);
            goto lbAgentEnd;
        }
    }

    if (iNWSEnd == 0) {
        NWSEndWait(pHandle);
    }

    do {

        iWidth = MIN_AGENTNAMEWIDTH;

        if (pAInfo != NULL) {
            if (strlen(pAInfo->aszDFAMAgent) >MIN_AGENTNAMEWIDTH) {
                iWidth = MAX_AGENTNAMEWIDTH;
            }
        }

        uiCcode = NWSList(AGENTNAME, AGENTNAMEROW, AGENTNAMECOL, AGENTNAMEHEIGHT, iWidth,
                          M_ESCAPE | M_SELECT | M_INSERT, NULL, pHandle, NULL,
                          DFAMAgentAction, NULL);

        if (uiCcode == 2) {

            NWSDestroyList(pHandle);
            NWSInitList(pHandle, free);

            pPtr = NWSAppendToList(pAInfo->aszDFAMAgent , (void *)0, pHandle);
            if (pPtr == (LIST *)NULL) {
                NWSAlert(0, 0, pHandle, SCREENPREFAILED);
                goto lbAgentEnd;
            }
        }
    } while (uiCcode > 1);

    NWSStartWait(WAITAGENTROW, WAITAGENTCOL, pHandle);
    iRc = writeAgentFile();
    NWSEndWait(pHandle);

    if (iRc != 0) {
        NWSAlert(0, 0, pHandle, FILEACCESSERROR);
    }

lbAgentEnd:
    delAllAgent();
    NWSDestroyList(pHandle);
    return;
}


/*EnglishComment**************************************************************/
/*  ID   = DFAMAgentAction                                                   */
/*  name = Agent screen processing routine                                   */
/*                                                                           */
/*  func = (1) Goes back to "Menu Screen" if the Esc. key was pressed.       */
/*         (2) Makes "Not Agents Screen" with getting the host name from     */
/*             the HOST file if the Ins. key was pressed.                    */
/*         (3) Displays "Enter Master Key Screen" if the Enter key was       */
/*             pressed.                                                      */
/*                                                                           */
/*  i/o  = uiKey      : LONG    :: pressed key                               */
/*         pSelect    : LIST ** :: selected element                          */
/*         puiItemNum : LONG    :: item member (not used)                    */
/*         pPar       : void *  :: parameter (not used)                      */
/*                                                                           */
/*  return =  2 : Goes back to "Agent Screen" (Agent was modified)           */
/*           -1 : Goes back to "Agent Screen"                                */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int DFAMAgentAction(uiKey, pSelect, puiItemNum, pPar)
LONG uiKey;
LIST **pSelect;
LONG *puiItemNum;
void *pPar;
{
    LIST *pPtr;
    struct hostent *pHost;
    LONG uiCcode;
    int iRc;
    int iWidth;

    puiItemNum = puiItemNum;
    pPar = pPar;

    if (uiKey == M_ESCAPE) {
        return 0;
    }

    if (uiKey == M_INSERT) {

        uiCcode = NWSPushList(pHandle);
        if (uiCcode == (LONG)0) {
            NWSAlert(0, 0, pHandle, SCREENPREFAILED);
            return -1;
        }

        iWidth = MIN_NOTAGENTLISTWIDTH;

        NWSInitList(pHandle, free);

        NWSStartWait(WAITNOTAGENTROW, WAITNOTAGENTCOL, pHandle);
        sethostent(0);

        while(1) {
            pHost = gethostent();
            if (pHost == (struct hostent *)NULL) {
                break;
            }

            if (pAInfo != NULL) {
                if (strcmp(pHost->h_name,pAInfo->aszDFAMAgent) == 0) {
                    continue;
                }
            }

            pPtr = NWSAppendToList(pHost->h_name, (void *)0, pHandle);
            if (pPtr == (LIST *)NULL) {
                NWSEndWait(pHandle);
                NWSAlert(0, 0, pHandle, SCREENPREFAILED);
                NWSDestroyList (pHandle);
                NWSPopList(pHandle);
                endhostent();
                return -1;
            }

            if (strlen(pHost->h_name) > MIN_NOTAGENTLISTWIDTH) {
                iWidth = MAX_NOTAGENTLISTWIDTH;
            }
        }

        endhostent();
        NWSEndWait(pHandle);

        uiCcode = NWSList(NOTDFAMAGENTLIST, NOTAGENTLISTROW, NOTAGENTLISTCOL, NOTAGENTLISTHEIGHT,
                          iWidth, M_ESCAPE | M_SELECT, NULL, pHandle, NULL,
                          NOTDFAMAgentAction, NULL);

        if (uiCcode == 2) {

            NWSDestroyList(pHandle);
            NWSPopList(pHandle);
            return 2;
        }

        NWSDestroyList(pHandle);
        NWSPopList(pHandle);
        return -1;
    }

    else {

        strcpy(aszGDFAMAgent, (*pSelect)->text);
        *aszGMasterKey = '\0';

        iRc = NWSEditString(MASTERKEYROW, MASTERKEYCOL, MASTERKEYHEIGHT, MASTERKEYWIDTH, MASTERKEY,
                            NOTHING, aszGMasterKey, MAX_MASTERKEYLEN, EF_SET | EF_NOECHO, pHandle,
                            NULL, MasterKeyAction, CHAR_OF_STRING);

        return -1;
    }
}


/*EnglishComment**************************************************************/
/*  ID   = NOTDFAMAgentAction                                                */
/*  name = "Not Agents Screen" processing routine                            */
/*  func = (1) Goes back to "AGENT Screen" if the Esc. key was pressed.      */
/*         (2) Registers Agent, and goes back to "Agent Screen" if the       */
/*             Enter key was pressed.                                        */
/*                                                                           */
/*  i/o  = uiKey      : LONG    :: pressed key                               */
/*         pSelect    : LIST ** :: selected element                          */
/*         puiItemNum : LONG    :: item member (not used)                    */
/*         pPar       : void *  :: parameter (not used)                      */
/*                                                                           */
/*  return = -1 : Goes back to "Not Agents Screen"                           */
/*            0 : Goes back to "Agent Screen"                                */
/*            2 : Agent was registered ("Agent Screen" is modified)          */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int NOTDFAMAgentAction(uiKey, pSelect, puiItemNum, pPar)
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

    strcpy(aszGDFAMAgent, (*pSelect)->text);
    *aszGMasterKey = '\0';

    iRc = NWSEditString(MASTERKEYROW, MASTERKEYCOL, MASTERKEYHEIGHT, MASTERKEYWIDTH, MASTERKEY,
                        NOTHING, aszGMasterKey, MAX_MASTERKEYLEN, EF_SET | EF_NOECHO, pHandle,
                        NULL, MasterKeyAction, CHAR_OF_STRING);

    if (iRc == 2) {
        return 2;
    }
    return -1;
}



/*EnglishComment**************************************************************/
/*  ID   = MasterKeyAction                                                   */
/*  name = "Enter Master Key Screen" processing routine                      */
/*  func = (1) Goes back to "Not Agents Screen" if the Esc. key was pressed. */
/*         (2) Checks the inputed master key if the Enter key was pressed.   */
/*             Displays "Re-Enter Master Key Screen" if the master key is    */
/*             correct.                                                      */
/*                                                                           */
/*  i/o  = uiAction     : LONG    :: pressed key                             */
/*         pszMasterKey : BYTE *  :: inputed data (master key)               */
/*         pPar         : void *  :: parameter (not used)                    */
/*                                                                           */
/*  return = -2 : Goes back to "Not Agents Screen"                           */
/*           -1 : Goes back to "Enter Master Key Screen"                     */
/*            2 : Agent was registered ("Agent Screen" is modified)          */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int MasterKeyAction(uiAction, pszMasterKey, pPar)
LONG uiAction;
BYTE *pszMasterKey;
void *pPar;
{
    int iRc;
    int tmLen;
    int iCount;

    pPar = pPar;

    if ((uiAction & E_ESCAPE) != (LONG)0) {
        *pszMasterKey = '\0';
        return -2;
    }

    tmLen = strlen(pszMasterKey);

    if ((tmLen < MIN_MASTERKEYLEN)
      ||(tmLen > MAX_MASTERKEYLEN)) {
        NWSAlert(0, 0, pHandle, MASTERKEYFAILURE);
        *pszMasterKey = '\0';
        return -1;
    }




    for (iCount = 0; iCount < tmLen; iCount++) {
        if (isprint(pszMasterKey[iCount]) == 0) {
            NWSAlert(0, 0, pHandle, MASTERKEYFAILURE);
            *pszMasterKey = '\0';
            return -1;
        }
    }

    *aszGReMasterKey = '\0';

    iRc = NWSEditString(MASTERKEYROW, MASTERKEYCOL, MASTERKEYHEIGHT, MASTERKEYWIDTH, REMASTERKEY,
                        NOTHING, aszGReMasterKey, MAX_MASTERKEYLEN, EF_SET | EF_NOECHO, pHandle,
                        NULL, ReMasterKeyAction, CHAR_OF_STRING);

    *pszMasterKey = '\0';

    if (iRc == -3) {
        iRc = -1;
    }

    return iRc;
}


/*EnglishComment**************************************************************/
/*  ID   = ReMasterKeyAction                                                 */
/*  name = "Re-Enter Master Key Screen" processing routine                   */
/*  func = (1) Goes back to "Enter Master Key Screen" if the Esc. key was    */
/*             pressed.                                                      */
/*         (2) Compares the master key inputed at the "Enter Master Key      */
/*             Screen" with the previous master key if the Enter key was     */
/*             pressed.                                                      */
/*         (3) If the re-entered master key matches with the previous master */
/*             key:                                                          */
/*               - Encrypts the master key.                                  */
/*               - Registers the host name (selected at the "Agent Screen")  */
/*                 with the Agent table.                                     */
/*               - Registers the master key with the Agent table.            */
/*                                                                           */
/*  i/o  = uiAction       : LONG    :: pressed key                           */
/*         pszReMasterKey : BYTE *  :: inputed data (re-entered master key)  */
/*         pPar           : void *  :: parameter (not used)                  */
/*                                                                           */
/*  return = -3 : Goes back to "Enter Master Key Screen."                    */
/*           -2 : Goes back to "Not Agents Screen."                          */
/*           -1 : Goes back to "Re-Enter Master Key Screen" (shortage of     */
/*                memory)                                                    */
/*            2 : Agent was registered ("Agent Screen" is modified)          */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int ReMasterKeyAction(uiAction, pszReMasterKey, pPar)
LONG uiAction;
BYTE *pszReMasterKey;
void *pPar;
{
    int iRc;
    char *pszMkey;
    char aszMkey[MAX_MASTERKEY];

    pPar = pPar;

    if ((uiAction & E_ESCAPE) != (LONG)0) {
        *pszReMasterKey = '\0';
        return -2;
    }

    if (strcmp(pszReMasterKey, aszGMasterKey) != 0) {
        NWSAlert(0, 0, pHandle, MASTERKEYNOTSAME);
        *pszReMasterKey = '\0';
        return -3;
    }

    pszMkey = aszMkey;
    MakeMasterKey(aszGMasterKey,pszMkey);
    if (pszMkey == NULL) {
        sprintf(aszError, "%d", iRc);
        AlertErrorCode(ADMINERROR, aszError);
        *pszReMasterKey = '\0';
        return -2;
    }

    if (pAInfo != NULL) {
        strcpy(pAInfo->aszDFAMAgent, aszGDFAMAgent);
        strcpy(pAInfo->aszMasterKey, aszMkey);
    }
    else {
        iRc = addAgent(aszGDFAMAgent, aszMkey);
        if (iRc != 0) {
            NWSAlert(0, 0, pHandle, OUTOFMEMORY);
            return -1;
        }
    }

    *pszReMasterKey = '\0';

    return 2;
}


/*EnglishComment**************************************************************/
/*  ID   = AgentInit                                                         */
/*  name = Agent Information initialization routine                          */
/*  func = (1) Gets Agent Information from the Agent file.                   */
/*         (2) Checks the version of the Agent file.                         */
/*         (3) Stores the information (obtained from the Agent File) to the  */
/*             Agent Table.                                                  */
/*                                                                           */
/*  i/o  = none                                                              */
/*                                                                           */
/*  return = -5: the host name registered with the Agent file does not exist */
/*               in the hosts file.                                          */
/*           -4: the Agent file has invalid data (which were ignored).       */
/*           -3: Agent file has an invalid version.                          */
/*           -2: not enough memory to store the Agent information.           */
/*           -1: failed to access the Agent file.                            */
/*            0: Agent information was successfully initialized.             */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int AgentInit(void)
{
    char *pszBufp;
    int iRc;
    int iReturn;
    int bFlag;

    int bExist;

    int bVer;


    char aszAgentBuf[MAX_DFAMAGENTNAME + 1];
    char aszKeyBuf[MAX_MASTERKEY + 1];

    iReturn = 0;
    bFlag = 0;
    bExist = 0;
    bVer = 0;

    pAgtFp = fopen(AGTFILE,"r");
    if (pAgtFp == NULL) {
        if (errno == ENOENT) {
            return 1;
        }
        return -1;
    }

    iRc = CFGVerCheck(pAgtFp, AGTFILEVER);
    if (iRc == 0) {
        ;
    }
    else if (iRc == -1) {
        iReturn = -3;
        goto lAgtFileRead;
    }
    else {
        bVer = 1;
    }

    if (fgets(aszAgentBuf, sizeof(aszAgentBuf), pAgtFp) == NULL) {

        if (ferror(pAgtFp) != 0) {
            iReturn = -1;
            goto lAgtFileRead;
        }
        iReturn = 0;
        if (bVer != 0) {
            iReturn = -4;
        }
        goto lAgtFileRead;
    }

    pszBufp = strrchr(aszAgentBuf,'\n');
    if (pszBufp != NULL) {
        *pszBufp = '\0';
    }
    else {
        iReturn = -4;
        goto lAgtFileRead;
    }

    iRc = DFAMAgentNameCheck(aszAgentBuf);
    if (iRc != 0) {
        bFlag = 1;
    }

    iRc = AgentExistCheck(aszAgentBuf);
    if (iRc != 0) {
        bFlag = 1;
        bExist = 1;
    }


    if (fgets(aszKeyBuf, sizeof(aszKeyBuf), pAgtFp) == NULL) {

        if (ferror(pAgtFp) != 0) {
            iReturn = -1;
            goto lAgtFileRead;
        }
        iReturn = -4;
        goto lAgtFileRead;
    }

    pszBufp = strrchr(aszKeyBuf,'\n');
    if (pszBufp != NULL) {
        *pszBufp = '\0';
    }
    else {

        if (strlen(aszKeyBuf) > MAX_MASTERKEY - 1) {
            iReturn = -4;
            goto lAgtFileRead;
        }
    }

    iRc = DFAMMasterKeyCheck(aszKeyBuf);
    if (iRc != 0) {
        bFlag = 1;
    }

    if (bFlag == 0) {
        iRc = addAgent(aszAgentBuf, aszKeyBuf);
        if (iRc != 0) {
            iReturn = -2;
            goto lAgtFileRead;
        }
        else {
            if (bVer != 0) {
                iReturn = -4;
                goto lAgtFileRead;
            }
        }
    }
    else {
        if (bExist == 1) {
            iReturn = -5;
        }
        else {
            iReturn = -4;
        }
    }

    if (fgets(aszKeyBuf, sizeof(aszKeyBuf), pAgtFp) != NULL) {
    	if (aszKeyBuf[0] != 0x1a) {
        	if (iReturn == 0) {
           		iReturn = -4;
        	}
        }
    }


lAgtFileRead :
    fclose(pAgtFp);
    pAgtFp = NULL;
    return iReturn;
}


/*EnglishComment**************************************************************/
/*  ID   = addAgent                                                          */
/*  name = Agent table addition routine                                      */
/*  func = Get area in the Agent table, and sets the Agent Information.      */
/*                                                                           */
/*  i/o  = pszAgent : char * : i : Agent name                                */
/*         pszKey   : char * : i : master key                                */
/*                                                                           */
/*  return = -1 : failed to get enough memory to store the Agent Information */
/*            0 : Agent Information successfully registered.                 */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int addAgent(pszAgent, pszKey)
char *pszAgent;
char *pszKey;
{

    if (pAInfo == (struct AgentInfo *)NULL) {

        pAInfo = (struct AgentInfo *)NWSAlloc(sizeof(struct AgentInfo), pHandle);
        if (pAInfo == (struct AgentInfo *)NULL) {
            return -1;
        }
    }

    strcpy(pAInfo->aszDFAMAgent, pszAgent);
    strcpy(pAInfo->aszMasterKey, pszKey);
    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = AgentExistCheck                                                   */
/*  name = Agent name checking routine (checks if the Agent name exists in   */
/*         the HOST file)                                                    */
/*  func = (1) Obtains a host name from the HOSTS file, and compares the     */
/*             name with the Agent name.                                     */
/*                                                                           */
/*  i/o  = pszAgent : char * : i : Agent name                                */
/*                                                                           */
/*  return = -1 : the Agent name already registered with the Agnet file      */
/*                does not exist in the HOSTS file.                          */
/*            0 : the Agent name already registered with the Agnet file      */
/*                exists in the HOSTS file.                                  */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int AgentExistCheck(pszAgent)
char *pszAgent;
{
    struct hostent *pHost;
    int iRc;

    sethostent(0);

    while(1) {
        pHost = gethostent();
        if (pHost == (struct hostent *)NULL) {
            iRc = -1;
            break;
        }

        if (strcmp(pHost->h_name, pszAgent) == 0) {
            iRc = 0;
            break;
        }
    }

    endhostent();
    return iRc;
}


/*EnglishComment**************************************************************/
/*  ID   = writeAgentFile                                                    */
/*  name = Agent Information storing routine                                 */
/*  func = (1) Makes a temporary file.                                       */
/*         (2) Writes the version information and Agent information onto     */
/*             the file.                                                     */
/*         (3) Deletes an old Agent file.                                    */
/*         (4) Renames the temporary file to the previous Agent name.        */
/*                                                                           */
/*  i/o  = none                                                              */
/*                                                                           */
/*  return =                                                                 */
/*     -2 : failed to rename ther DFAAGT.TMP file. (no DFAAGT.CFG file)      */
/*     -1 : failed to access the Agent file.                                 */
/*      0 : normal end.                                                      */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int writeAgentFile(void)
{
    int iRc;
    char aszBuf[7];

    pAgtFp = fopen(TMPAGTFILE,"W");
    if (pAgtFp != NULL) {

        strcpy(aszBuf,AGTFILEVER);
        strcat(aszBuf,"\n");
        iRc = fputs(aszBuf,pAgtFp);

        if (iRc == EOF) {
            fclose(pAgtFp);
            pAgtFp = NULL;
            unlink(TMPAGTFILE);
            return -1;
        }

        if (pAInfo == NULL) {
            goto lbAgtWriteEnd;
        }

        if (( *(pAInfo->aszDFAMAgent) == '\0')
          ||( *(pAInfo->aszMasterKey) == '\0')) {

            fclose(pAgtFp);
            pAgtFp = NULL;

            SetDefaultFileAttribute(AGTFILE);
            unlink(AGTFILE);

            iRc = rename(TMPAGTFILE,AGTFILE);
            if (iRc != 0) {
                return -2;
            }
            return 0;
        }

        strcat(pAInfo->aszDFAMAgent,"\n");
        iRc = fputs(pAInfo->aszDFAMAgent, pAgtFp);
        if (iRc == EOF) {
            fclose(pAgtFp);
            pAgtFp = NULL;
            unlink(TMPAGTFILE);
            return -1;
        }

        strcat(pAInfo->aszMasterKey,"\n");
        iRc = fputs(pAInfo->aszMasterKey, pAgtFp);
        if (iRc == EOF) {
            fclose(pAgtFp);
            pAgtFp = NULL;
            unlink(TMPAGTFILE);
            return -1;
        }
		iRc = fputc(0x1a,pAgtFp);
        if (iRc == EOF) {
            fclose(pAgtFp);
            pAgtFp = NULL;
            unlink(TMPAGTFILE);
            return -1;
        }
    }

    else {
        return -1;
    }

lbAgtWriteEnd :

    fclose(pAgtFp);
    pAgtFp = NULL;

    SetDefaultFileAttribute(AGTFILE);
    unlink(AGTFILE);

    iRc = rename(TMPAGTFILE,AGTFILE);
    if (iRc != 0) {
        return -2;
    }

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = DFAMLoadCheck                                                     */
/*  name = Checks if DFA.NLM is already loaded.                              */
/*  func =                                                                   */
/*                                                                           */
/*  i/o  = none                                                              */
/*  return = -1 : DFA.NLM has been loaded.                                   */
/*            0 : DFA.NLM is not yet loaded.                                 */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int DFAMLoadCheck(void)
{
    unsigned int uiRc;

    uiRc = FindNLMHandle(DFAMNLM);
    if (uiRc != 0) {
        return -1;
    }
    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = delAllAgent                                                       */
/*  name = Agent Information deletion routine                                */
/*  func = (1) Frees the memory registering the Agent Information.           */
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

void delAllAgent(void)
{
    if (pAInfo != NULL) {
        NWSFree((struct AgentInfo *)pAInfo, pHandle);
        pAInfo = (struct AgentInfo *)NULL;
    }
    return;
}


/*EnglishComment**************************************************************/
/*  ID   = DFAMAgentNameCheck                                                */
/*  name = Agent name syntax check routine                                   */
/*  func =                                                                   */
/*                                                                           */
/*  i/o  = pszDFAMAgent : char * : i : Agent name                            */
/*                                                                           */
/*  return = -1 : invalid Agent name                                         */
/*            0 : The syntax of the Agent name is correct.                   */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int DFAMAgentNameCheck(pszDFAMAgent)
char *pszDFAMAgent;
{
    char *pszBuf;

    pszBuf = pszDFAMAgent;

    if (*pszBuf == '\0') {
        return -1;
    }

    while (*pszBuf != '\0') {

        if (isprint(*pszBuf) == 0) {
            return -1;
        }

        pszBuf++;
    }
    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = DFAMMasterKeyCheck                                                */
/*  name = Master key syntax checking routine                                */
/*  func =                                                                   */
/*                                                                           */
/*  i/o  = pszDFAMMasterKey : char * : i : Master key                        */
/*                                                                           */
/*  return = -1 : Invalid master key                                         */
/*            0 : The syntax of the Agent name is correct.                   */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int DFAMMasterKeyCheck(pszDFAMMasterKey)
char *pszDFAMMasterKey;
{
    int iCnt;
    char *pszBuf;

    iCnt = 0;
    pszBuf = pszDFAMMasterKey;
    while (*pszBuf != '\0') {

        if (iCnt != 8) {
            if (isxdigit(*pszBuf) == 0) {
                return -1;
            }
        }
        else {
            if (*pszBuf != ' ') {
                return -1;
            }
        }

        iCnt++;
        pszBuf++;
    }

    if (iCnt != MAX_MASTERKEY -1) {
        return -1;
    }

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = MakeMasterKey                                                     */
/*  name = Master key creating routine                                       */
/*  func = Converts the designated character string into an encryption key,  */
/*         and returns the key as a master key character string.             */
/*  i/o  = pszKeystr: BYTE *:i   : character string (1 - 8 characters) to    */
/*                                 be used as a seed for the master key.     */
/*         aszMkeystr:BYTE *:i/o : pointer to the character string (the      */
/*                                 master key in hexadecimal format).        */
/*                                 The first 4 bytes and the last 4 bytes    */
/*                                 are spaces.                               */
/*  return = none                                                            */
/*  common = none                                                            */
/*                                                                           */
/*  note   = NULL is returned if pszKeystr has no characters.                */
/*  data   = Dec. 26, 1994 by Masato Arai                                    */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
void MakeMasterKey( pszKeystr, pszMkeystr )
BYTE *pszKeystr;
BYTE *pszMkeystr;
{
	pszKeystr = pszKeystr;
	sprintf(pszMkeystr, "deadbeef deadbeef");
	return;
}
