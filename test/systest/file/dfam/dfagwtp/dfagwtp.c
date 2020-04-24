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
 * $Log: dfagwtp.c,v $
 * Revision 1.1.2.2  1996/03/11  14:11:23  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:59:31  marty]
 *
 * Revision 1.1.2.1  1996/02/29  09:59:11  takano
 * 	Submit DFAM-client test
 * 	[1996/02/29  09:56:07  takano]
 * 
 * Revision 1.1.2.1  1996/02/22  10:39:16  takano
 * 	First Submit
 * 	[1996/02/22  10:39:05  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <conio.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <process.h>
#include <time.h>
#include <errno.h>
#include <niterror.h>

#include "dfagwtp.h"

FILE *pLogFp;
FILE *pConfigFp;
FILE *pVolFp;
FILE *pFVTFp;
FILE *pTXTFp;
struct ConfigInfo *pConInfo;
char *pszReadBuf;
int iLogError;
int iLogMode;

char aszMessage[MAX_MSGLEN];


/*EnglishComment**************************************************************/
/*  ID   = main                                                              */
/*                                                                           */
/*  name = Gateway test suite (main routine)                                 */
/*  func = (1) Registers a Unload handler.                                   */
/*         (2) Suppresses Cntl+c and Cntl+s                                  */
/*         (3) Initialization                                                */
/*         (4) Checks the Gateway environment.                               */
/*         (5) Executes the tests.                                           */
/*                                                                           */
/*  i/o  = none                                                              */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
void main(void)
{
    int iRc;

    pLogFp = NULL;
    pConfigFp = NULL;
    pVolFp = NULL;
    pFVTFp = NULL;
    pTXTFp = NULL;
    pConInfo = NULL;
    pszReadBuf = NULL;

    iLogError = 0;
    iLogMode = 0;

    signal(SIGTERM, fnGWFVTTerm);
    SetCtrlCharCheckMode(FALSE);

    iRc = GWFVTInit();
    if (iRc != 0) {
        GWFVTTerm();
        return;
    }

    iRc = CheckGWEnv();
    if (iRc != 0) {
        GWFVTTerm();
        return;
    }

    ExecFVT();

    GWFVTTerm();
    return;
}


/*EnglishComment**************************************************************/
/*  ID   = fnGWFVTTerm                                                       */
/*                                                                           */
/*  name = UNLAOD handler                                                    */
/*  func = (1) Writes the test suite completion to the logging file.         */
/*         (2) Closes the still-opened files.                                */
/*         (3) Releases the un-released memory.                              */
/*                                                                           */
/*  i/o  = iDummy: int:inp: dummy (a SIGTERM handler has an int parameter)   */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
void fnGWFVTTerm(int iDummy)
{
    int iRc;
    int iErrnoSave;
    int iNWErrnoSave;

    iDummy = iDummy;

    if (pLogFp != NULL) {

        if (iLogMode != 0) {
            PutLog("\n");
        }
        PutStartLog();
        PutLog("END\n");

        iRc = fclose(pLogFp);
        iErrnoSave = errno;
        iNWErrnoSave = NetWareErrno;
        pLogFp = NULL;

        if (iRc != 0) {
            FVTMessage(LOG_NO, KDDS32200D, LOGFILE, FUNC_FCLOSE, iRc, iErrnoSave, iNWErrnoSave);

            iLogError = 1;
        }
    }

    if (pConfigFp != NULL) {
        fclose(pConfigFp);
        pConfigFp = NULL;
    }

    if (pVolFp != NULL) {
        fclose(pVolFp);
        pVolFp = NULL;
    }

    if (pFVTFp != NULL) {
        fclose(pFVTFp);
        pFVTFp = NULL;
    }

    if (pTXTFp != NULL) {
        fclose(pTXTFp);
        pTXTFp = NULL;
    }

    if (pConInfo != NULL) {

        if (pConInfo->pszDCEGroup[0] != NULL) {
            free(pConInfo->pszDCEGroup[0]);
            pConInfo->pszDCEGroup[0] = NULL;
        }

        if (pConInfo->pszNWGroup[0] != NULL) {
            free(pConInfo->pszNWGroup[0]);
            pConInfo->pszNWGroup[0] = NULL;
        }

        if (pConInfo->pszNWUser[0] != NULL) {
            free(pConInfo->pszNWUser[0]);
            pConInfo->pszNWUser[0] = NULL;
        }

        free(pConInfo);
        pConInfo = NULL;
    }

    if (pszReadBuf != NULL) {
        free(pszReadBuf);
        pszReadBuf = NULL;
    }

    return;
}


/*EnglishComment**************************************************************/
/*  ID   = GWFVTTerm                                                         */
/*                                                                           */
/*  name = Termination                                                       */
/*  func = (1) Writes the test suite completion to the logging file.         */
/*         (2) Closes the files.                                             */
/*         (3) Releases the memory.                                          */
/*                                                                           */
/*  i/o    = none                                                            */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
void GWFVTTerm(void)
{
    int iRc;
    int iErrnoSave;
    int iNWErrnoSave;

    if (pLogFp != NULL) {
        PutStartLog();
        PutLog("END\n");

        iRc = fclose(pLogFp);
        iErrnoSave = errno;
        iNWErrnoSave = NetWareErrno;
        pLogFp = NULL;

        if (iRc != 0) {
            FVTMessage(LOG_NO, KDDS32200D, LOGFILE, FUNC_FCLOSE, iRc, iErrnoSave, iNWErrnoSave);

            iLogError = 1;
        }
    }

    if (pConInfo != NULL) {

        if (pConInfo->pszDCEGroup[0] != NULL) {
            free(pConInfo->pszDCEGroup[0]);
            pConInfo->pszDCEGroup[0] = NULL;
        }

        if (pConInfo->pszNWGroup[0] != NULL) {
            free(pConInfo->pszNWGroup[0]);
            pConInfo->pszNWGroup[0] = NULL;
        }

        if (pConInfo->pszNWUser[0] != NULL) {
            free(pConInfo->pszNWUser[0]);
            pConInfo->pszNWUser[0] = NULL;
        }

        free(pConInfo);
        pConInfo = NULL;
    }

    if (pszReadBuf != NULL) {
        free(pszReadBuf);
        pszReadBuf = NULL;
    }

    return;
}


/*EnglishComment**************************************************************/
/*  ID   = InputYN                                                           */
/*                                                                           */
/*  name = Inputting Yes/No                                                  */
/*  func = Inputs Yes/No.                                                    */
/*                                                                           */
/*  i/o    = none                                                            */
/*  return = 0: "N" was inputted                                             */
/*           1: "Y" was inputted                                             */
/*  common = none                                                            */
/*  note   = none                                                            */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
int InputYN(void)
{
    int iKey;
    int iResult;

    iResult = -1;

    while (iResult == -1){
        iKey = getch();

        switch(toupper(iKey)) {
          case 'Y' :
            iResult = 1;
            break;

          case 'N' :
            iResult = 0;
            break;

          case 0x00 :
            iKey = getch();
            break;

          default :
            break;
        }
    }

    printf("%s\n",(iResult == 1) ? "Y" : "N");
    return (iResult);
}


/*EnglishComment**************************************************************/
/*  ID   = PutStartLog                                                       */
/*                                                                           */
/*  name = Getting the logging of the starting information                   */
/*  func = Writes the starting time and date to the logging file.            */
/*                                                                           */
/*  i/o    = none                                                            */
/*  return =  0: normal end                                                  */
/*           -1: failed to write                                             */
/*  common = none                                                            */
/*  note   = none                                                            */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
int PutStartLog(void)
{
    time_t uiTime;
    int iRc;
    char aszBuf[26];

    uiTime = time(NULL);
    strcpy(aszBuf, ctime(&uiTime));

    aszBuf[24] = ' ';

    iRc = PutLog(aszBuf);
    if (iRc != 0) {
        return -1;
    }

    iLogMode = 1;
    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = PutEndLog                                                         */
/*                                                                           */
/*  name = Setting the completion time and date to the logging file          */
/*  func = Writes the following information to the logging file:             */
/*         (1) completed time and date                                       */
/*         (2) test item file name (key word)                                */
/*         (2) status                                                        */
/*                                                                           */
/*  i/o    = pszTestID : char * :inp: test item ID or key word               */
/*           pszStatus : char * :inp: status                                 */
/*  return =  0: normal end                                                  */
/*           -1: failed to write                                             */
/*  common = none                                                            */
/*  note   = PutLog() handles the actual writing to the logging file.        */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
int PutEndLog(char *pszTestID, char *pszStatus)
{
    time_t uiTime;
    int iRc;
    char aszBuf[53];

    uiTime = time(NULL);

    sprintf(aszBuf, "%s%s %s\n", ctime(&uiTime), pszTestID, pszStatus);

    aszBuf[24] = ' ';

    iRc = PutLog(aszBuf);
    if (iRc != 0) {
        return -1;
    }

    iLogMode = 0;
    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = PutLog                                                            */
/*                                                                           */
/*  name = Writing to the logging file                                       */
/*  func = (1) Writes the designated character string onto the logging file. */
/*         (2) Flushes the file.                                             */
/*                                                                           */
/*  i/o    = pszLogText: char * :inp: texts to be written to the logging file*/
/*                                                                           */
/*  return =  0: normal end                                                  */
/*           -1: failed to write, or an access file has already occurred     */
/*               to the logging file                                         */
/*  common = none                                                            */
/*  note   = none                                                            */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
int PutLog(char *pszLogText)
{
    int iRc;
    int iErrnoSave;
    int iNWErrnoSave;

    if (iLogError != 0) {
        return -1;
    }

    iRc = fputs(pszLogText, pLogFp);

    iErrnoSave = errno;
    iNWErrnoSave = NetWareErrno;

    if (iRc == EOF) {
        FVTMessage(LOG_NO, KDDS32200D, LOGFILE, FUNC_FPUTS, iRc, iErrnoSave, iNWErrnoSave);

        iLogError = 1;
        return -1;
    }

    iRc = fflush(pLogFp);

    iErrnoSave = errno;
    iNWErrnoSave = NetWareErrno;

    if (iRc == EOF) {
        FVTMessage(LOG_NO, KDDS32200D, LOGFILE, FUNC_FFLUSH, iRc, iErrnoSave, iNWErrnoSave);

        iLogError = 1;
        return -1;
    }

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = FVTMessage                                                        */
/*                                                                           */
/*  name = Outputting the message to the console and/or logging file         */
/*  func = (1) Displays the message on the console.                          */
/*         (2) Writes the message to the logging file (optional).            */
/*                                                                           */
/*  i/o    = iMode  : int    :inp: message write/no-write indicator          */
/*                                    LOG_YES: writes the message to the     */
/*                                             logging file                  */
/*                                    LOG_NO : does not write                */
/*           pszLog : char * :inp: message text                              */
/*                                                                           */
/*  return =  0: normal end                                                  */
/*           -1: failed to write to the logging file                         */
/*                                                                           */
/*  common = none                                                            */
/*  note   = (1) The size of the message buffer is 1,280 bytes.  However,    */
/*               the size of the message is not checked.                     */
/*           (2) If iMode = LOG_NO, this function always returns with 0      */
/*               (no need to check the return value).                        */
/*           (3) The variable-length parameters allowed to pszLog are:       */
/*                   %d : int                                                */
/*                   %s : string                                             */
/*                   %ld: unsigned long                                      */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
int FVTMessage(int iMode, char *pszLog, ... )
{
    va_list pArg;
    char *pChar;
    char *pCharLog;
    char *pszValue;
    LONG uiValue;
    int iValue;
    int iNum;
    int iRc;

    va_start(pArg, pszLog);
    pCharLog = aszMessage;

    for (pChar = pszLog; *pChar != '\0'; pChar++) {
        if (*pChar != '%') {
            *pCharLog = *pChar;
            pCharLog++;
            continue;
        }

        pChar++;

        switch (*pChar) {
          case 'd' :
            iValue = va_arg(pArg, int);
            iNum = sprintf(pCharLog, "%d", iValue);
            pCharLog += iNum;
            break;

          case 's' :
            pszValue = va_arg(pArg, char *);
            iNum = sprintf(pCharLog, "%s", pszValue);
            pCharLog += iNum;
            break;

          case 'l' :
            pChar++;
            if (*pChar == 'd') {
                uiValue = va_arg(pArg, unsigned long);
                iNum = sprintf(pCharLog, "%ld", uiValue);
                pCharLog += iNum;
            }
            else {
                *pCharLog = *pChar;
                pCharLog++;
            }
            break;

          default :
            *pCharLog = *pChar;
            pCharLog++;
            break;
        }
    }
    *pCharLog = '\0';
    va_end(pArg);

    printf ("%s", aszMessage);

    if (iMode == LOG_YES) {
        iRc = PutLog(aszMessage);
        if (iRc != 0) {
            return -1;
        }
    }
    return 0;
}
