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
 * $Log: dfagwini.c,v $
 * Revision 1.1.2.2  1996/03/11  14:11:20  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:59:30  marty]
 *
 * Revision 1.1.2.1  1996/02/29  10:00:05  takano
 * 	Submit DFAM-client test
 * 	[1996/02/29  09:56:49  takano]
 * 
 * Revision 1.1.2.1  1996/02/22  10:38:20  takano
 * 	First Submit
 * 	[1996/02/22  10:36:48  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <process.h>
#include <malloc.h>
#include <errno.h>
#include <niterror.h>
#include <sys\stat.h>

#include "dfagwtp.h"

extern FILE *pLogFp;
extern FILE *pConfigFp;
extern struct ConfigInfo *pConInfo;
extern char *pszReadBuf;
extern iLogMode;



/*EnglishComment**************************************************************/
/*  ID   = GWFVTInit                                                         */
/*                                                                           */
/*  name = Initialization of the Gateway test suite                          */
/*  func = (1) Checks if the logging file exists.                            */
/*         (2) Checks if DFA.NLM is loaded.                                  */
/*         (3) Gets the memory for registering the configuration file        */
/*             information.                                                  */
/*         (4) Reads information from the configuration file.                */
/*                                                                           */
/*  i/o    = none                                                            */
/*                                                                           */
/*  return =  0: initialization normally completed                           */
/*           -1: initialization failed                                       */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
int GWFVTInit(void)
{
    int iRc;

    iRc = LogfileExistCheck();
    if (iRc != 0) {
        return -1;
    }

    iRc = PutStartLog();
    if (iRc != 0) {
        return -1;
    }

    iRc = PutLog("START\n");
    if (iRc != 0) {
        return -1;
    }
    iLogMode = 0;

    iRc = DFALoadCheck();
    if (iRc != 0) {
        return -1;
    }

    iRc = FVTAllocMemory();
    if (iRc != 0) {
        return -1;
    }

    iRc = ReadConfigFile();
    if (iRc != 0) {
        return -1;
    }

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = LogfileExistCheck                                                 */
/*                                                                           */
/*  name = Checking and opening the logging file                             */
/*  func = (1) Checks if the logging file exists.                            */
/*         (2) If the file exists, Ask a user whether it is OK to overwrite. */
/*         (3) If a user selected "overwrite," or if there is no logging     */
/*             file, opens the logging file.                                 */
/*                                                                           */
/*  i/o    = none                                                            */
/*                                                                           */
/*  return =  1: test suite is killed                                        */
/*            0: normal end                                                  */
/*           -1: abnormal end                                                */
/*                                                                           */
/*  common = none                                                            */
/*  note   = No write is made to the logging file if there is an error.      */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
int LogfileExistCheck(void)
{
    struct stat buf;
    int iRc;
    int iErrnoSave;
    int iNWErrnoSave;

    iRc = stat(LOGFILE, &buf);
    iErrnoSave = errno;
    iNWErrnoSave = NetWareErrno;

    if (iRc != 0) {
        switch (iErrnoSave) {

          case ENOENT :
            break;

          default :
            FVTMessage(LOG_NO, KDDS32200D, LOGFILE, FUNC_STAT, iRc, iErrnoSave, iNWErrnoSave);

            return -1;
        }
    }
    else {
        FVTMessage(LOG_NO, KDDS32004);
        iRc = InputYN();
        if (iRc == 0) {
            return 1;
        }
    }

    pLogFp = fopen(LOGFILE, "w");
    iErrnoSave = errno;
    iNWErrnoSave = NetWareErrno;

    if (pLogFp == NULL) {
        FVTMessage(LOG_NO, KDDS32200S, LOGFILE, FUNC_FOPEN, LOG_NULL, iErrnoSave, iNWErrnoSave);

        return -1;
    }

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = FVTAllocMemory                                                    */
/*                                                                           */
/*  name = Memory assignment and initialization                              */
/*  func = (1) Gets the memory to be used by the test suite (mainly used     */
/*             for the data of the configuration file).                      */
/*         (2) Sets default values to the configuration information.         */
/*                                                                           */
/*  i/o    = none                                                            */
/*                                                                           */
/*  return =  0: normal end                                                  */
/*           -1: failed to get memory                                        */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
int FVTAllocMemory(void)
{
    char *pWork;
    int iLoop;


    pConInfo = (struct ConfigInfo *)calloc(1, sizeof(struct ConfigInfo));
    if (pConInfo == NULL) {
        FVTMessage(LOG_YES, KDDS32201);
        return -1;
    }

    strncpy(pConInfo->asEcat, CON_ECAT, ECATCH_LEN);



    pWork = (char *)calloc( (MAX_NWUSERLEN + 1) * MAX_NWUSERNUM, sizeof(char));
    if (pWork == NULL) {
        free(pConInfo);
        pConInfo = NULL;
        FVTMessage(LOG_YES, KDDS32201);
        return -1;
    }

    for (iLoop = 0; iLoop < MAX_NWUSERNUM; iLoop++) {
        pConInfo->pszNWUser[iLoop] = pWork;
        pWork += MAX_NWUSERLEN + 1;
    }


    pWork = (char *)calloc( (MAX_NWGROUPLEN + 1) * MAX_GROUPNUM, sizeof(char));
    if (pWork == NULL) {
        free(pConInfo->pszNWUser[0]);
        pConInfo->pszNWUser[0] = NULL;
        free(pConInfo);
        pConInfo = NULL;
        FVTMessage(LOG_YES, KDDS32201);
        return -1;
    }

    for (iLoop = 0; iLoop < MAX_GROUPNUM; iLoop++) {
        pConInfo->pszNWGroup[iLoop] = pWork;
        pWork += MAX_NWGROUPLEN + 1;
    }


    pWork = (char *)calloc( (MAX_DCEGROUPLEN + 1) * MAX_GROUPNUM, sizeof(char));
    if (pWork == NULL) {
        free(pConInfo->pszNWGroup[0]);
        pConInfo->pszNWGroup[0] = NULL;
        free(pConInfo->pszNWUser[0]);
        pConInfo->pszNWUser[0] = NULL;
        free(pConInfo);
        pConInfo = NULL;
        FVTMessage(LOG_YES, KDDS32201);
        return -1;
    }

    for (iLoop = 0; iLoop < MAX_GROUPNUM; iLoop++) {
        pConInfo->pszDCEGroup[iLoop] = pWork;
        pWork += MAX_DCEGROUPLEN + 1;
    }


    pszReadBuf = (char *)calloc(READBUFLEN, sizeof(char));
    if (pszReadBuf == NULL) {
        free(pConInfo->pszDCEGroup[0]);
        pConInfo->pszDCEGroup[0] = NULL;
        free(pConInfo->pszNWGroup[0]);
        pConInfo->pszNWGroup[0] = NULL;
        free(pConInfo->pszNWUser[0]);
        pConInfo->pszNWUser[0] = NULL;
        free(pConInfo);
        pConInfo = NULL;
        FVTMessage(LOG_YES, KDDS32201);
        return -1;
    }

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = ReadConfigFile                                                    */
/*                                                                           */
/*  name = Reading the configuration file                                    */
/*  func = (1) Gets information from the configuration file, and expands     */
/*             the information on the memory.                                */
/*                                                                           */
/*  i/o    = none                                                            */
/*                                                                           */
/*  return =  0: normal end                                                  */
/*           -1: failed to read                                              */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
int ReadConfigFile(void)
{
    char *pszRc;
    char *pszWork;
    LONG uiLine;
    int iRc;
    int tmLen;
    int iErrnoSave;
    int iNWErrnoSave;
    int iResult;
    int iNum;

    iResult = 0;

    pConfigFp = fopen(CFGFILE, "r");

    iErrnoSave = errno;
    iNWErrnoSave = NetWareErrno;

    if (pConfigFp == NULL) {

        if (iErrnoSave == ENOENT) {
            FVTMessage(LOG_YES, KDDS32015);
        }
        else {
            FVTMessage(LOG_YES, KDDS32200S, CFGFILE, FUNC_FOPEN, LOG_NULL, iErrnoSave, iNWErrnoSave);

        }
        return -1;
    }

    uiLine = 1L;

    while (1) {

        pszRc = fgets(pszReadBuf, READBUFLEN, pConfigFp);

        iErrnoSave = errno;
        iNWErrnoSave = NetWareErrno;

        if (pszRc == NULL) {
            if (ferror(pConfigFp) != 0) {
                FVTMessage(LOG_YES, KDDS32200S, CFGFILE, FUNC_FOPEN, LOG_NULL, iErrnoSave, iNWErrnoSave);

                iResult = -1;
                goto lbReadConfigEnd;
             }
             break;
        }

        pszWork = strrchr(pszReadBuf, '\n');
        if (pszWork != NULL) {
            *pszWork = '\0';
        }
        else {
            iResult = -1;
            goto lbConfigErr;
        }

        if (strnicmp(pszReadBuf, "NWUSER", 6) == 0) {

            if ((isdigit(pszReadBuf[6]) != 0)
              &&(pszReadBuf[7] == '=')) {

                iNum = pszReadBuf[6] - '0';

                if (*(pConInfo->pszNWUser[iNum]) != '\0') {
                    iResult = -1;
                    goto lbConfigErr;
                }

                tmLen = strlen(&pszReadBuf[8]);
                if ((tmLen <= MAX_NWUSERLEN)
                  &&(tmLen >= 1)) {
                    strcpy(pConInfo->pszNWUser[iNum], strupr(&pszReadBuf[8]));
                }
                else {
                    iResult = -1;
                    goto lbConfigErr;
                }
            }
            else {
                iResult = -1;
                goto lbConfigErr;
            }
        }

        else if (strnicmp(pszReadBuf, "NWGROUP", 7) == 0) {

            if ((isdigit(pszReadBuf[7]) != 0)
              &&(pszReadBuf[8] == '=')) {

                iNum = pszReadBuf[7] - '0';

                if (*(pConInfo->pszNWGroup[iNum]) != '\0') {
                    iResult = -1;
                    goto lbConfigErr;
                }

                tmLen = strlen(&pszReadBuf[9]);
                if ((tmLen <= MAX_NWGROUPLEN)
                  &&(tmLen >= 1)) {
                    strcpy(pConInfo->pszNWGroup[iNum], strupr(&pszReadBuf[9]));
                }
                else {
                    iResult = -1;
                    goto lbConfigErr;
                }
            }
            else {
                iResult = -1;
                goto lbConfigErr;
            }
        }

        else if (strnicmp(pszReadBuf, "DCEGROUP", 8) == 0) {

            if ((isdigit(pszReadBuf[8]) != 0)
              &&(pszReadBuf[9] == '=')) {

                iNum = pszReadBuf[8] - '0';

                if (*(pConInfo->pszDCEGroup[iNum]) != '\0') {
                    iResult = -1;
                    goto lbConfigErr;
                }

                tmLen = strlen(&pszReadBuf[10]);
                if ((tmLen <= MAX_DCEGROUPLEN)
                  &&(tmLen >= 1)) {
                    strcpy(pConInfo->pszDCEGroup[iNum], &pszReadBuf[10]);
                }
                else {
                    iResult = -1;
                    goto lbConfigErr;
                }
            }
            else {
                iResult = -1;
                goto lbConfigErr;
            }
        }

        else if (strnicmp(pszReadBuf, "OTHEROBJ=", 9) == 0) {

            if (*(pConInfo->aszOtherobj) != '\0') {
                iResult = -1;
                goto lbConfigErr;
            }

            tmLen = strlen(pszReadBuf) - 9;
            if ((tmLen <= MAX_NWGROUPLEN)
              &&(tmLen >= 1)) {
                strcpy(pConInfo->aszOtherobj, strupr(pszReadBuf + 9));
            }
            else {
                iResult = -1;
                goto lbConfigErr;
            }
        }

        else if (strnicmp(pszReadBuf, "MASKOBJ=", 8) == 0) {

            if (*(pConInfo->aszMaskobj) != '\0') {
                iResult = -1;
                goto lbConfigErr;
            }

            tmLen = strlen(pszReadBuf) - 8;
            if ((tmLen <= MAX_NWGROUPLEN)
              &&(tmLen >= 1)) {
                strcpy(pConInfo->aszMaskobj, strupr(pszReadBuf + 8));
            }
            else {
                iResult = -1;
                goto lbConfigErr;
            }
        }

        else if (strnicmp(pszReadBuf, "GWVOLUME=", 9) == 0) {

            if (*(pConInfo->aszGWVolume) != '\0') {
                iResult = -1;
                goto lbConfigErr;
            }

            tmLen = strlen(pszReadBuf);

            if (pszReadBuf[tmLen - 1] == ':') {
                pszReadBuf[tmLen - 1] = '\0';
                tmLen--;
            }

            tmLen -= 9;

            if ((tmLen <= _MAX_VOLUME - 1)
              &&(tmLen >= MIN_VOLUME)) {
                strcpy(pConInfo->aszGWVolume, strupr(pszReadBuf + 9));
            }
            else {
                iResult = -1;
                goto lbConfigErr;
            }
        }

        else if (strnicmp(pszReadBuf, "TOPDIR=", 7) == 0) {

            if (*(pConInfo->aszTopdir) != '\0') {
                iResult = -1;
                goto lbConfigErr;
            }

            tmLen = strlen(pszReadBuf);

            if (pszReadBuf[tmLen - 1] == '/') {
                pszReadBuf[tmLen - 1] = '\0';
                tmLen--;
            }

            tmLen -= 7;

            if ((tmLen <= MAX_TOPDIRLEN)
              &&(tmLen >= MIN_TOPDIRLEN)) {
                strcpy(pConInfo->aszTopdir, pszReadBuf + 7);
            }
            else {
                iResult = -1;
                goto lbConfigErr;
            }
        }

        else if (strnicmp(pszReadBuf, "FVTFILE=", 8) == 0) {

            if (*(pConInfo->aszFVTFile) != '\0') {
                iResult = -1;
                goto lbConfigErr;
            }

            tmLen = strlen(pszReadBuf) - 8;
            if ((tmLen <= _MAX_NAME - 1)
              &&(tmLen >= 1)) {
                strcpy(pConInfo->aszFVTFile, strupr(pszReadBuf + 8));
            }
            else {
                iResult = -1;
                goto lbConfigErr;
            }
        }

        else if (strnicmp(pszReadBuf, "ERRORMODE=", 10) == 0) {

            if (pConInfo->iErrorMode != 0) {
                iResult = -1;
                goto lbConfigErr;
            }

            if (stricmp(pszReadBuf + 10, MSG_CONT) == 0) {
                pConInfo->iErrorMode = -1;
            }
            else if (stricmp(pszReadBuf + 10, MSG_STOP) == 0) {
                pConInfo->iErrorMode = 1;
            }
            else {
                iResult = -1;
                goto lbConfigErr;
            }
        }

        else if (strnicmp(pszReadBuf, "REM ", 4) == 0) {
            ;
        }

        else {
            iResult = -1;
            goto lbConfigErr;
        }

        uiLine++;
    }

    iRc = SetDefaultValue();
    if (iRc != 0) {
        iResult = -1;
    }
    goto lbReadConfigEnd;

lbConfigErr:

    FVTMessage(LOG_YES, KDDS32016, uiLine);

lbReadConfigEnd:

    fclose(pConfigFp);
    pConfigFp = NULL;

    return iResult;
}


/*EnglishComment**************************************************************/
/*  ID   = SetDefaultValue                                                   */
/*                                                                           */
/*  name = Setting values to the unset information of the configuration file */
/*  func = (1) Sets default values if the information is not set to the      */
/*             configuration file.                                           */
/*         (2) Checks if the NetWare groups and DCE group are properly       */
/*             mapped.                                                       */
/*                                                                           */
/*  i/o    = none                                                            */
/*                                                                           */
/*  return =  0: normal end                                                  */
/*           -1: NetWare group and DCE group are not matched                 */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
int SetDefaultValue(void)
{
    int iLoop;
    int iDefineNW;

    int iDefineDCE;


    for (iLoop = 0; iLoop < MAX_GROUPNUM; iLoop++) {
        iDefineNW = 1;
        iDefineDCE = 1;

        if (*(pConInfo->pszNWGroup[iLoop]) == '\0') {
            iDefineNW = 0;

            switch (iLoop) {
              case 0 :
                strcpy(pConInfo->pszNWGroup[0], DEF_NWGROUP0);
                iDefineNW = 1;
                break;

              case 1 :
                strcpy(pConInfo->pszNWGroup[1], DEF_NWGROUP1);
                iDefineNW = 1;
                break;

              case 2 :
                strcpy(pConInfo->pszNWGroup[2], DEF_NWGROUP2);
                iDefineNW = 1;
                break;

              default :
                break;
            }
        }

        if (*(pConInfo->pszDCEGroup[iLoop]) == '\0') {
            iDefineDCE = 0;

            switch (iLoop) {
              case 0 :
                strcpy(pConInfo->pszDCEGroup[0], DEF_DCEGROUP0);
                iDefineDCE = 1;
                break;

              case 1 :
                strcpy(pConInfo->pszDCEGroup[1], DEF_DCEGROUP1);
                iDefineDCE = 1;
                break;

              case 2 :
                strcpy(pConInfo->pszDCEGroup[2], DEF_DCEGROUP2);
                iDefineDCE = 1;
                break;

              default :
                break;
            }
        }

        if (iDefineNW > iDefineDCE) {
            FVTMessage(LOG_YES, KDDS32010, pConInfo->pszNWGroup[iLoop]);
            return -1;
        }
        else if (iDefineDCE > iDefineNW) {
            FVTMessage(LOG_YES, KDDS32011, pConInfo->pszDCEGroup[iLoop]);
            return -1;
        }
    }

    if (*(pConInfo->pszNWUser[0]) == '\0') {
        strcpy(pConInfo->pszNWUser[0], DEF_NWUSER0);
    }

    if (*(pConInfo->pszNWUser[1]) == '\0') {
        strcpy(pConInfo->pszNWUser[1], DEF_NWUSER1);
    }

    if (*(pConInfo->pszNWUser[2]) == '\0') {
        strcpy(pConInfo->pszNWUser[2], DEF_NWUSER2);
    }

    if (*(pConInfo->aszOtherobj) == '\0') {
        strcpy(pConInfo->aszOtherobj, DEF_OTHEROBJ);
    }

    if (*(pConInfo->aszMaskobj) == '\0') {
        strcpy(pConInfo->aszMaskobj, DEF_MASKOBJ);
    }

    if (*(pConInfo->aszGWVolume) == '\0') {
        strcpy(pConInfo->aszGWVolume, DEF_GWVOLUME);
    }

    if (*(pConInfo->aszTopdir) == '\0') {
        strcpy(pConInfo->aszTopdir, DEF_TOPDIR);
    }

    if (*(pConInfo->aszFVTFile) == '\0') {
        strcpy(pConInfo->aszFVTFile, DEF_FVTFILE);
    }

    if (pConInfo->iErrorMode == 0) {
        pConInfo->iErrorMode = 1;
    }

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = DFALoadCheck                                                      */
/*                                                                           */
/*  name = Checking the loading of DFA.NLM                                   */
/*  func = (1) Checks if DFA.NLM has been loaded.                            */
/*             mapped.                                                       */
/*                                                                           */
/*  i/o    = none                                                            */
/*                                                                           */
/*  return =  0: DFA.NLM already loaded                                      */
/*           -1: DFA.NLM not loaded                                          */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
int DFALoadCheck(void)
{
    unsigned int uiRc;

    uiRc = FindNLMHandle(DFANLM);
    if (uiRc == 0) {
        FVTMessage(LOG_YES, KDDS32003);
        return -1;
    }
    return 0;
}
