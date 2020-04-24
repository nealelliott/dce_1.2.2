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
 * $Log: dfagwtst.c,v $
 * Revision 1.1.2.2  1996/03/11  14:11:29  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:59:34  marty]
 *
 * Revision 1.1.2.1  1996/02/29  10:03:45  takano
 * 	Submit DFAM-client test
 * 	[1996/02/29  09:57:14  takano]
 * 
 * Revision 1.1.2.1  1996/02/22  10:39:56  takano
 * 	First Submit
 * 	[1996/02/22  10:39:51  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <ctype.h>
#include <nwtypes.h>
#include <errno.h>
#include <niterror.h>
#include <sys\stat.h>

#include <conio.h>

#include "dfagwtp.h"

extern struct ConfigInfo *pConInfo;
extern char *pszReadBuf;
extern FILE *pFVTFp;
extern FILE *pTXTFp;


/*EnglishComment**************************************************************/
/*  ID   = ExecFVT                                                           */
/*                                                                           */
/*  name = Checking the test item registration file                          */
/*  func = (1) Sequentially reads a test item from the test item             */
/*             registration file.                                            */
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
void ExecFVT(void)
{
    char *pszRc;
    LONG uiTestSuccess;
    LONG uiTestFailure;
    LONG uiLine;
    int iRc;
    int iErrnoSave;
    int iNWErrnoSave;
    int tmLen;
    int iResult;
    char aszFVTFile[_MAX_PATH];
    char aszTXTFile[_MAX_NAME];

    FVTMessage(LOG_NO, KDDS32100);

    iResult = 0;
    uiTestSuccess = 0L;
    uiTestFailure = 0L;

    sprintf(aszFVTFile, "%s%s", DFAGWTPDIR, pConInfo->aszFVTFile);

    pFVTFp = fopen(aszFVTFile, "r");
    iErrnoSave = errno;
    iNWErrnoSave = NetWareErrno;

    if (pFVTFp == NULL) {

        if (iErrnoSave == ENOENT) {
            FVTMessage(LOG_YES, KDDS32107);
        }
        else {
            FVTMessage(LOG_YES, KDDS32200S, aszFVTFile, FUNC_FOPEN, LOG_NULL, iErrnoSave, iNWErrnoSave);

        }
        iResult = -1;
        goto lbOpenErr;
    }

    uiLine = 1L;

    while (1) {

        pszRc = fgets(pszReadBuf, READBUFLEN, pFVTFp);

        iErrnoSave = errno;
        iNWErrnoSave = NetWareErrno;

        if (pszRc == NULL) {
            if (ferror(pFVTFp) != 0) {

                FVTMessage(LOG_YES, KDDS32200S, aszFVTFile, FUNC_FGETS, LOG_NULL, iErrnoSave, iNWErrnoSave);

                iResult = -1;
                goto lbExecFVTEnd;
            }

            break;
        }

        tmLen = strlen(pszReadBuf);
        if (pszReadBuf[tmLen-1] == '\n') {
            pszReadBuf[tmLen-1] = '\0';
            tmLen--;
        }
        else {
            FVTMessage(LOG_YES, KDDS32106, uiLine);
            iResult = -1;
            goto lbExecFVTEnd;
        }

        if (strnicmp(pszReadBuf, "REM ", 4) == 0) {
            uiLine++;
            continue;
        }


        iRc = CheckTXTFile(pszReadBuf, uiLine);
        if (iRc != 0) {
            iResult = -1;
            goto lbExecFVTEnd;
        }

        strcpy(aszTXTFile, pszReadBuf);

        iRc = ExecTest(aszTXTFile);
        switch (iRc) {
          case 0 :
            uiTestSuccess++;
            break;

          case 1 :
            uiTestFailure++;
            break;

          case 2 :
            uiTestFailure++;
            iResult = 1;
            goto lbExecFVTEnd;

          case 3 :
            break;

          default :
            iResult = -1;
            goto lbExecFVTEnd;
        }

        uiLine++;
        ThreadSwitch();
    }


lbExecFVTEnd:

    fclose(pFVTFp);
    pFVTFp = NULL;

lbOpenErr:

    if (iResult != -1) {
        if (uiTestFailure == 0) {
            FVTMessage(LOG_NO, KDDS32110, uiTestSuccess);
        }
        else {
            FVTMessage(LOG_NO, KDDS32111, uiTestSuccess, uiTestFailure);
        }
    }

    FVTMessage(LOG_NO, KDDS32101);

    return;
}


/*EnglishComment**************************************************************/
/*  ID   = CheckTXTfile                                                      */
/*                                                                           */
/*  name = Syntax checking of the test item file name                        */
/*  func = (1) Checks if the format of the designated test item file name    */
/*             is Gxxxxxxxx.TXT (x is 0 - 9).                                */
/*                                                                           */
/*  i/o    = pszTXT : char * :i/o: test item file name                       */
/*           uiLine : int    :inp: the number of line of the test item       */
/*                                 registration file that has read the test  */
/*                                 item file name                            */
/*                                                                           */
/*  return =  0: the file name is syntactically correct                      */
/*           -1: the file name is syntactically wrong                        */
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
int CheckTXTFile(pszTXT,  uiLine)
char *pszTXT;
LONG uiLine;
{
    int tmLen;
    int iLoop;

    tmLen = strlen(pszTXT);
    if (tmLen != 12) {
        FVTMessage(LOG_YES, KDDS32106, uiLine);
        return -1;
    }

    strupr(pszTXT);

    if (pszTXT[0] != 'G') {
        FVTMessage(LOG_YES, KDDS32106, uiLine);
        return -1;
    }

    for (iLoop = 1; iLoop < 8; iLoop++) {
        if (isdigit(pszTXT[iLoop]) == 0) {
            FVTMessage(LOG_YES, KDDS32106, uiLine);
            return -1;
        }
    }

    if (strncmp(&(pszTXT[1]), "00", 2) == 0) {
        FVTMessage(LOG_YES, KDDS32106, uiLine);
        return -1;
    }

    if (strncmp(&(pszTXT[3]), "00", 2) == 0) {
        FVTMessage(LOG_YES, KDDS32106, uiLine);
        return -1;
    }

    if (strncmp(&(pszTXT[5]), "000", 3) == 0) {
        FVTMessage(LOG_YES, KDDS32106, uiLine);
        return -1;
    }

    if (strcmp(&(pszTXT[8]), ".TXT") != 0) {
        FVTMessage(LOG_YES, KDDS32106, uiLine);
        return -1;
    }

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = ExecTest                                                          */
/*                                                                           */
/*  name = Checking the test item file                                       */
/*  func = (1) Reads a test target directory/file from the test item file.   */
/*                                                                           */
/*  i/o    = pszTXT : char * :inp: test item file name                       */
/*                                                                           */
/*  return =  3: test not executed (the test item file is empty, or only     */
/*               REM statement)                                              */
/*            2: test result invalid + testing is killed                     */
/*            1: test result invalid + testing is continued                  */
/*            0: test result normal                                          */
/*           -1: error: testing is killed                                    */
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
int ExecTest(pszTXTFile)
char *pszTXTFile;
{
    char *pszRc;
    LONG uiLine;
    int iRc;
    int iErrnoSave;
    int iNWErrnoSave;
    int tmLen;
    int iResult;
    int iPosition;
    char aszFull[_MAX_PATH];
    char aszTestName[_MAX_VOLUME + _MAX_PATH + 1];

    iResult = 0;

    sprintf(aszFull, "%s%s", DFAGWTPTESTDIR, pszTXTFile);

    pTXTFp = fopen(aszFull, "r");
    iErrnoSave = errno;
    iNWErrnoSave = NetWareErrno;

    if (pTXTFp == NULL) {

        if (iErrnoSave == ENOENT) {
            FVTMessage(LOG_YES, KDDS32109, aszFull);
        }
        else {
            FVTMessage(LOG_YES, KDDS32200S, aszFull, FUNC_FOPEN, LOG_NULL, iErrnoSave, iNWErrnoSave);

        }
        return -1;
    }

    uiLine = 1L;
    aszTestName[0] = '\0';

    while (1) {

        pszRc = fgets(pszReadBuf, READBUFLEN, pTXTFp);

        iErrnoSave = errno;
        iNWErrnoSave = NetWareErrno;

        if (pszRc == NULL) {
            if (ferror(pTXTFp) != 0) {
                FVTMessage(LOG_YES, KDDS32200S, aszFull, FUNC_FGETS, LOG_NULL, iErrnoSave, iNWErrnoSave);

                iResult = -1;
                goto lbExecTestEnd;
            }

            break;
        }

        tmLen = strlen(pszReadBuf);
        if (pszReadBuf[tmLen-1] == '\n') {
            pszReadBuf[tmLen-1] = '\0';
            tmLen--;
        }
        else {
            FVTMessage(LOG_YES, KDDS32108, aszFull, uiLine);
            iResult = -1;
            goto lbExecTestEnd;
        }

        if (strnicmp(pszReadBuf, "REM ", 4) == 0) {
            ;
        }

        else if(strnicmp(pszReadBuf, "MAP ", 4) == 0) {

            if (aszTestName[0] != '\0') {
                FVTMessage(LOG_YES, KDDS32108, aszFull, uiLine);
                iResult = -1;
                goto lbExecTestEnd;
            }

            iRc = CheckTestName(pszReadBuf + 4, aszFull, uiLine);
            if (iRc != 0) {
                iResult = -1;
                goto lbExecTestEnd;
            }

            iPosition = 4;

            if (pszReadBuf[4] == '\\') {
                iPosition = 5;
            }

            sprintf (aszTestName, "%s:\\%s", pConInfo->aszGWVolume, strupr(pszReadBuf + iPosition));

        }
        else {
            FVTMessage(LOG_YES, KDDS32108, aszFull, uiLine);
            iResult = -1;
            goto lbExecTestEnd;
        }
        uiLine++;
        ThreadSwitch();
    }

    if (aszTestName[0] == '\0') {
        iResult = 3;
        goto lbExecTestEnd;
    }

    iRc = MapTest(aszTestName, pszTXTFile);
    switch (iRc) {
      case 0 :
        break;

      case 1 :
        if (pConInfo->iErrorMode == 1) {

            FVTMessage(LOG_NO, KDDS32105);
            iRc = InputYN();
            if (iRc != 0) {
                iResult = 2;
                goto lbExecTestEnd;
            }
        }
        iResult = 1;
        break;

      default :
        iResult = -1;
        break;
    }

lbExecTestEnd:

    fclose(pTXTFp);
    pTXTFp = NULL;

    return iResult;
}


/*EnglishComment**************************************************************/
/*  ID   = CheckTestName                                                     */
/*                                                                           */
/*  name = Syntax check of the test target file/directory                    */
/*  func = (1) Checks if the length of the test item file/directory name     */
/*             is 255 bytes or shorter.                                      */
/*         (2) Checks if the test item file/directory name contains a space. */
/*                                                                           */
/*  i/o    = pszTest: char * :inp: test target file/directory                */
/*           pszTXT : char * :inp: test item file name                       */
/*           uiLine : int    :inp: number of lines of the test item read     */
/*                                 from the test target file/directory       */
/*                                                                           */
/*  return =  0: normal end                                                  */
/*           -1: invalid file/directory name                                 */
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
int CheckTestName(pszTest, pszTXT, uiLine)
char *pszTest;
char *pszTXT;
LONG uiLine;
{
    char *pszSpace;
    int tmLen;

    tmLen = strlen(pszTest);
    if ((tmLen > _MAX_PATH)
      ||(tmLen < 1)) {
        FVTMessage(LOG_YES, KDDS32108, pszTXT, uiLine);
        return -1;
    }

    pszSpace = strchr(pszTest, ' ');
    if (pszSpace != NULL) {
        FVTMessage(LOG_YES, KDDS32108, pszTXT, uiLine);
        return -1;
    }

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = MapTest                                                           */
/*                                                                           */
/*  name = Mapping test                                                      */
/*  func = Uses "stat" to check if the designated file/directory exists.     */
/*                                                                           */
/*  i/o    = pszTestName: char * :inp: file/directory name to be tested of   */
/*                                     mapping                               */
/*           pszTXT     : char * :inp: test item file name                   */
/*                                                                           */
/*  return =  1: error in the checking                                       */
/*            0: normal end                                                  */
/*           -1: abnormal end                                                */
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
int MapTest(pszTestName, pszTXT)
char *pszTestName;
char *pszTXT;
{
    struct stat buf;
    int iResult;
    int iErrnoSave;
    int iNWErrnoSave;
    int iStatRc;
    int iRc;

    iResult = 0;

    FVTMessage(LOG_NO ,KDDS32102, pszTXT);

    iRc = PutStartLog();
    if (iRc != 0) {
        return -1;
    }

    iStatRc = stat(pszTestName, &buf);
    iErrnoSave = errno;
    iNWErrnoSave = NetWareErrno;

    if (iStatRc != 0) {
        switch (iErrnoSave) {

          case ENOENT :
            iResult = 1;
            break;

          default :
            iResult = -1;
            break;
        }
    }

    switch (iResult) {
      case 0 :
        FVTMessage(LOG_NO, KDDS32103, pszTXT);
        iRc = PutEndLog(pszTXT, LOG_SUCCESS);
        if (iRc != 0) {
            iResult = -1;
        }
        break;

      case 1 :
        iRc = PutEndLog(pszTXT, LOG_FAILED);
        if (iRc != 0) {
            iResult = -1;
            break;
        }
        iRc = FVTMessage(LOG_YES, KDDS32104, pszTXT, FUNC_STAT, iStatRc, iErrnoSave, iNWErrnoSave);
        if (iRc != 0) {
            iResult = -1;
        }
        break;

      default :
        iRc = PutEndLog(pszTXT, LOG_FAILED);
        FVTMessage(LOG_YES, KDDS32200D, pszTestName, FUNC_STAT, iStatRc,iErrnoSave, iNWErrnoSave);

        break;
    }

    return iResult;
}
