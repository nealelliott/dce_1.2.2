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
 * $Log: uadmothr.c,v $
 * Revision 1.1.2.2  1996/03/11  14:09:16  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:22  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:20:49  takano
 * 	First Submit
 * 	[1996/02/21  08:20:44  takano]
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
#include <sys\stat.h>

#include "uadmmlh.h"
#include "uadmdef.h"
#include "uadmext.h"


/*EnglishComment**************************************************************/
/*  ID   = otherOption                                                       */
/*  name = "Other Options Screen" processing routine                         */
/*  func = (1) Displays the information of the DFAOPT.CFG file registration  */
/*             on "Other Options Screen."                                    */
/*         (2) Writes the information on "Other Options Screen" onto         */
/*             DFAOPT.CFG file when the Esc. or Enter key was pressed.       */
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

void otherOption(void)
{
    FIELD *pPtr;
    int iLine;
    int iRc;
    int bFlag;

    int iNWSEnd;


    bFlag = 0;
    iNWSEnd = 0;
    NWSInitForm (pHandle);

    NWSStartWait(WAITOTHERROW, WAITOTHERCOL, pHandle);

    iRc = OtherInit();
    switch (iRc) {
      case 1 :
      case 0 :
        break;

      case -1 :
        NWSEndWait(pHandle);
        NWSAlert(0, 0, pHandle, FILEACCESSERROR);
        NWSDestroyForm (pHandle);
        return;
        break;

      case -2 :
        NWSEndWait(pHandle);
        NWSAlert(0, 0, pHandle, OUTOFMEMORY);
        NWSDestroyForm (pHandle);
        return;
        break;

      case -3 :
        NWSEndWait(pHandle);
        NWSAlert(0, 0, pHandle, OPTIONFILENOTUSE);
        NWSDestroyForm (pHandle);
        return;
        break;

      case -4 :
        NWSEndWait(pHandle);
        iNWSEnd = 1;
        NWSAlert(0, 0, pHandle, OPTIONFILEINVALID);
        break;

      default :
        NWSEndWait(pHandle);
        sprintf(aszError, "%d", iRc);
        AlertErrorCode(ADMINERROR, aszError);
        NWSDestroyForm (pHandle);
        return;
        break;
    }

    iLine = 0;
    pPtr = NWSAppendCommentField(iLine, 1, NWSGetMessage(TIMESYNC, &(pHandle->messages)), pHandle);
    if (pPtr == NULL) {
        bFlag = -1;
    }
    pPtr = NWSAppendBoolField(iLine, OTHERFIELD, NORMAL_FIELD, &(pOther->byTimeSync), NULL, pHandle);
    if (pPtr == NULL) {
        bFlag = -1;
    }

    iLine += 2;
    pPtr = NWSAppendCommentField(iLine, 1, NWSGetMessage(MOUNTWARNING,&(pHandle->messages)), pHandle);
    if (pPtr == NULL) {
        bFlag = -1;
    }
    pPtr = NWSAppendBoolField(iLine, OTHERFIELD, NORMAL_FIELD, &(pOther->byMountWarning), NULL, pHandle);
    if (pPtr == NULL) {
        bFlag = -1;
    }

    iLine += 2;
    pPtr = NWSAppendCommentField(iLine, 1, NWSGetMessage(DELAYTIME, &(pHandle->messages)), pHandle);
    if (pPtr == NULL) {
        bFlag = -1;
    }
    pPtr = NWSAppendUnsignedIntegerField(iLine, OTHERFIELD, NORMAL_FIELD, &(pOther->uiCopyback),
                                          MIN_COPYBACKDELAYTIME, MAX_COPYBACKDELAYTIME, NULL, pHandle);
    if (pPtr == NULL) {
        bFlag = -1;
    }

    iLine += 2;
    pPtr = NWSAppendCommentField(iLine, 1, NWSGetMessage(DIRSYNC, &(pHandle->messages)), pHandle);
    if (pPtr == NULL) {
        bFlag = -1;
    }
    pPtr = NWSAppendUnsignedIntegerField(iLine, OTHERFIELD, NORMAL_FIELD, &(pOther->uiDirSync),
                                          MIN_DIRSYNCINTERVAL, MAX_DIRSYNCINTERVAL, NULL, pHandle);
    if (pPtr == NULL) {
        bFlag = -1;
    }

    iLine += 2;
    pPtr = NWSAppendCommentField(iLine, 1, NWSGetMessage(TIMEOUT, &(pHandle->messages)), pHandle);
    if (pPtr == NULL) {
        bFlag = -1;
    }
    pPtr = NWSAppendUnsignedIntegerField(iLine, OTHERFIELD, NORMAL_FIELD, &(pOther->uiTimeOut),
                                          MIN_TIMEOUT_TIME, MAX_TIMEOUT_TIME, NULL, pHandle);
    if (pPtr == NULL) {
        bFlag = -1;
    }

    iLine += 2;
    pPtr = NWSAppendCommentField(iLine, 1, NWSGetMessage(CONNECT, &(pHandle->messages)), pHandle);
    if (pPtr == NULL) {
        bFlag = -1;
    }
    pPtr = NWSAppendUnsignedIntegerField(iLine, OTHERFIELD, NORMAL_FIELD, &(pOther->uiConnect),
                                          MIN_CONNECT_TIME, MAX_CONNECT_TIME, NULL, pHandle);
    if (pPtr == NULL) {
        bFlag = -1;
    }

    iLine += 2;
    pPtr = NWSAppendCommentField(iLine, 1, NWSGetMessage(FILESPERS, &(pHandle->messages)), pHandle);
    if (pPtr == NULL) {
        bFlag = -1;
    }
    pPtr = NWSAppendUnsignedIntegerField(iLine, OTHERFIELD, NORMAL_FIELD, &(pOther->uiFileSpers),
                                          MIN_SPERSDELAYTIME, MAX_SPERSDELAYTIME, NULL, pHandle);
    if (pPtr == NULL) {
        bFlag = -1;
    }




    if (bFlag != 0) {
        NWSEndWait(pHandle);
        NWSAlert(0, 0, pHandle, SCREENPREFAILED);
        delAllOther();
        return;
    }

    if (iNWSEnd == 0) {
        NWSEndWait(pHandle);
    }

    iRc = NWSEditPortalForm(OTHEROPTION, OTHERROW, OTHERCOL, OTHERHEIGHT, OTHERWIDTH, F_NOVERIFY,
                            F_NO_HELP, NULL, pHandle);

    if (iRc < 0) {
        delAllOther();
        return;
    }

    NWSStartWait(WAITOTHERROW, WAITOTHERCOL, pHandle);
    iRc = writeOptFile();
    NWSEndWait(pHandle);

    if (iRc != 0) {
        NWSAlert(0, 0, pHandle, FILEACCESSERROR);
    }

    delAllOther();
    NWSDestroyForm (pHandle);

    return;
}


/*EnglishComment**************************************************************/
/*  ID   = OtherInit                                                         */
/*  name = Other Options information initializing routine                    */
/*  func = (1) Gets memory for Other Options                                 */
/*         (2) Reads Other Options from DFAOPT.CFG file, and stores Other    */
/*             Options to the buffer.                                        */
/*                                                                           */
/*  i/o  = none                                                              */
/*  return = -4 : invalid data in the option file (the data ignored)         */
/*           -3 : option file invalid version                                */
/*           -2 : shortage of memory                                         */
/*           -1 : failed to access DFAOPT.CFG file                           */
/*            0 : normally completed                                         */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int OtherInit(void)
{
    char *pszBufp;
    struct OptFileData flags;
    int iLen;
    int iRc;
    int bSkipFlag;

    char aszBuf[MAX_COPYBACKDIRNAME + 19];

    flags.bIsTimeSync  = OFF;
    flags.bIsMountWarn = OFF;
    flags.bIsCopyBack  = OFF;
    flags.bIsDirSync   = OFF;
    flags.bIsTimeOut   = OFF;
    flags.bIsConnect   = OFF;
    flags.bIsFileSpers = OFF;
    flags.bIsBackup    = OFF;
    flags.bIsSendBuf   = OFF;
    flags.bIsMesLog    = OFF;
    flags.bIsErrLog    = OFF;
    flags.bIsWatchdog  = OFF;

    pOther = (struct OtherInfo *)NWSAlloc(sizeof(struct OtherInfo), pHandle);

    if (pOther == (struct OtherInfo *)NULL) {
        return -2;
    }

    pOther->uiCopyback = DEF_COPYBACKDELAYTIME;
    pOther->uiDirSync = DEF_DIRSYNCINTERVAL;
    pOther->uiTimeOut = DEF_TIMEOUT_TIME;
    pOther->uiConnect = DEF_CONNECT_TIME;
    pOther->uiFileSpers = DEF_SPERSDELAYTIME;
    pOther->byTimeSync      = 0x00;
    pOther->byMountWarning  = 0x00;
    *(pOther->aszCopyDir)   = '\0';
    *(pOther->aszSendBuf)   = '\0';
    *(pOther->aszMesFile)   = '\0';
    *(pOther->aszLogFile)   = '\0';
    *(pOther->aszWatchdog)  = '\0';

    pOthrFp = fopen(OPTFILE,"r");
    if (pOthrFp == NULL) {
        if (errno == ENOENT) {
            return 1;
        }
        delAllOther();
        return -1;
    }

    bSkipFlag = 0;

    iRc = CFGVerCheck(pOthrFp, OPTFILEVER);
    if (iRc == 0) {
        ;
    }
    else if (iRc == -1) {
        fclose(pOthrFp);
        pOthrFp = NULL;
        delAllOther();
        return -3;
    }
    else {
        bSkipFlag = 1;
    }

    while (1) {
        if (fgets(aszBuf, sizeof(aszBuf), pOthrFp) == NULL) {

            if (ferror(pOthrFp) != 0) {
                fclose(pOthrFp);
                pOthrFp = NULL;
                delAllOther();
                return -1;
             }
             break;
        }

        pszBufp = strrchr(aszBuf,'\n');
        if (pszBufp != NULL) {
            *pszBufp = '\0';
        }
        else {
        	if (aszBuf[0] == 0x1a)
        		break;

            if (strlen(aszBuf) > MAX_COPYBACKDIRNAME + 18 ) {

                iRc = SkipReadCFGFile(pOthrFp);
                switch (iRc) {
                  case  0 :
                    break;

                  case -2 :
                    fclose(pOthrFp);
                    pOthrFp = NULL;
                    return -4;
                    break;

                  default :
                    fclose(pOthrFp);
                    pOthrFp = NULL;
                    delAllOther();
                    return -1;
                    break;
                }

                bSkipFlag = 1;
                continue;
            }
        }

        if (strnicmp(aszBuf,"TIMESYNC=",9) == 0) {

            if (flags.bIsTimeSync == ON) {
                bSkipFlag = 1;
                continue;
            }

            flags.bIsTimeSync = ON;

            if (stricmp(aszBuf+9,"YES") == 0) {
                pOther->byTimeSync = 0x01;
            }
            else if (stricmp(aszBuf+9,"NO") != 0) {
                bSkipFlag = 1;
            }

        }

        else if (strnicmp(aszBuf,"COPYBACKDELAYTIME=",18) == 0) {

            if (flags.bIsCopyBack == ON) {
                bSkipFlag = 1;
                continue;
            }

            flags.bIsCopyBack = ON;

            iRc = isDigitString(aszBuf+18);
            if (iRc == 0) {
                bSkipFlag = 1;
                continue;
            }

            iLen = strlen(aszBuf) - 18;
            if (iLen > MAX_COPYBACKBUFLEN - 1) {
                bSkipFlag = 1;
                continue;
            }

            pOther->uiCopyback = atol(aszBuf+18);
            if (pOther->uiCopyback > MAX_COPYBACKDELAYTIME) {
                pOther->uiCopyback = DEF_COPYBACKDELAYTIME;
                bSkipFlag = 1;
            }
        }

        else if (strnicmp(aszBuf,"DIRSYNCINTERVAL=",16) == 0) {

            if (flags.bIsDirSync == ON) {
                bSkipFlag = 1;
                continue;
            }

            flags.bIsDirSync = ON;

            iRc = isDigitString(aszBuf+16);
            if (iRc == 0) {
                bSkipFlag = 1;
                continue;
            }

            iLen = strlen(aszBuf) - 16;
            if (iLen > MAX_DIRSYNCBUFLEN - 1) {
                bSkipFlag = 1;
                continue;
            }

            pOther->uiDirSync = atol(aszBuf+16);
            if ((pOther->uiDirSync < MIN_DIRSYNCINTERVAL)
              ||(pOther->uiDirSync > MAX_DIRSYNCINTERVAL)) {
                pOther->uiDirSync = DEF_DIRSYNCINTERVAL;
                bSkipFlag = 1;
            }
        }

        else if (strnicmp(aszBuf,"MOUNTWARNING=",13) == 0) {

            if (flags.bIsMountWarn == ON) {
                bSkipFlag = 1;
                continue;
            }

            flags.bIsMountWarn = ON;

            if (stricmp(aszBuf+13,"YES") == 0) {
                pOther->byMountWarning = 0x01;
            }
            else if (stricmp(aszBuf+13,"NO") != 0) {
                bSkipFlag = 1;
            }
        }

        else if (strnicmp(aszBuf,"COPYBACKDIRECTORY=",18) == 0) {

            if (flags.bIsBackup == ON) {
                bSkipFlag = 1;
                continue;
            }

            flags.bIsBackup = ON;

            iRc = EscapeDirNameCheck(aszBuf + 18);
            if (iRc != 0) {
                bSkipFlag = 1;
                continue;
            }

            strcpy(pOther->aszCopyDir,aszBuf+18);
        }

        else if (strnicmp(aszBuf,"RESPONSEWAITTIME=",17) == 0) {

            if (flags.bIsTimeOut == ON) {
                bSkipFlag = 1;
                continue;
            }

            flags.bIsTimeOut = ON;

            iRc = isDigitString(aszBuf+17);
            if (iRc == 0) {
                bSkipFlag = 1;
                continue;
            }

            iLen = strlen(aszBuf) - 17;
            if (iLen > MAX_TIMEOUTBUFLEN - 1) {
                bSkipFlag = 1;
                continue;
            }

            pOther->uiTimeOut = atol(aszBuf+17);
            if (pOther->uiTimeOut > MAX_TIMEOUT_TIME) {
                pOther->uiTimeOut = DEF_TIMEOUT_TIME;
                bSkipFlag = 1;
            }
        }
        else if (strnicmp(aszBuf,"CONNECTTIME=",12) == 0) {

            if (flags.bIsConnect == ON) {
                bSkipFlag = 1;
                continue;
            }

            flags.bIsConnect = ON;

            iRc = isDigitString(aszBuf+12);
            if (iRc == 0) {
                bSkipFlag = 1;
                continue;
            }

            iLen = strlen(aszBuf) - 12;
            if (iLen > MAX_CONNECTBUFLEN - 1) {
                bSkipFlag = 1;
                continue;
            }

            pOther->uiConnect = atol(aszBuf+12);
            if (pOther->uiConnect > MAX_CONNECT_TIME) {
                pOther->uiConnect = DEF_CONNECT_TIME;
                bSkipFlag = 1;
            }
        }
        else if (strnicmp(aszBuf,"FILESPARSEDELAYTIME=",20) == 0) {

            if (flags.bIsFileSpers == ON) {
                bSkipFlag = 1;
                continue;
            }

            flags.bIsFileSpers = ON;

            iRc = isDigitString(aszBuf+20);
            if (iRc == 0) {
                bSkipFlag = 1;
                continue;
            }

            iLen = strlen(aszBuf) - 20;
            if (iLen > MAX_FILESPERSBUFLEN - 1) {
                bSkipFlag = 1;
                continue;
            }

            pOther->uiFileSpers = atol(aszBuf+20);
            if (pOther->uiFileSpers > MAX_SPERSDELAYTIME) {
                pOther->uiFileSpers = DEF_SPERSDELAYTIME;
                bSkipFlag = 1;
            }
        }


        else if (strnicmp(aszBuf,"SENDBUFFERLENGTH=",17) == 0) {

            if (flags.bIsSendBuf == ON) {
                bSkipFlag = 1;
                continue;
            }

            flags.bIsSendBuf = ON;

            iLen = strlen(aszBuf) - 17;

            if ((iLen < MAX_SENDBUFLEN)
              &&(iLen > 0)) {
                strcpy(pOther->aszSendBuf,aszBuf + 17);
            }
            else {
                bSkipFlag = 1;
            }
        }

        else if (strnicmp(aszBuf,"MESSAGELOGFILE=",15) == 0) {

            if (flags.bIsMesLog == ON) {
                bSkipFlag = 1;
                continue;
            }

            flags.bIsMesLog = ON;

            iLen = strlen(aszBuf) - 15;

            if ((iLen < MAX_MESBUFLEN)
              &&(iLen > 0)) {
                strcpy(pOther->aszMesFile,aszBuf+15);
            }
            else {
                bSkipFlag = 1;
            }
        }

        else if (strnicmp(aszBuf,"ERRORLOGFILE=",13) == 0) {

            if (flags.bIsErrLog == ON) {
                bSkipFlag = 1;
                continue;
            }

            flags.bIsErrLog = ON;

            iLen = strlen(aszBuf) - 13;

            if ((iLen < MAX_ERRBUFLEN)
              &&(iLen > 0)) {
                strcpy(pOther->aszLogFile,aszBuf+13);
            }
            else {
                bSkipFlag = 1;
            }
        }

        else if (strnicmp(aszBuf,"WATCHDOGWAITTIME=",17) == 0) {

            if (flags.bIsWatchdog == ON) {
                bSkipFlag = 1;
                continue;
            }

            flags.bIsWatchdog = ON;

            iLen = strlen(aszBuf) - 17;

            if ((iLen < MAX_WATCHDOGBUFLEN)
              &&(iLen > 0)) {
                strcpy(pOther->aszWatchdog,aszBuf+17);
            }
            else {
                bSkipFlag = 1;
            }
        }
		else if (aszBuf[0] == 0x1a){
			break;
		}
        else {
            bSkipFlag = 1;
        }
    }

    fclose(pOthrFp);
    pOthrFp = NULL;
    if (bSkipFlag != 0) {
        return -4;
    }
    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = writeOptFile                                                      */
/*  name = Other Options Information writing routine                         */
/*  func = (1) Writes Other Options Information onto the temporary file.     */
/*         (2) Deletes DFAOPT.CFG                                            */
/*         (3) Renames the temporary file to DFAOPT.CFG file.                */
/*                                                                           */
/*  i/o  = none                                                              */
/*  return = -2 : failed to rename DFAOPT.TMP file (no DFAOPT.CFG file)      */
/*           -1 : failed to create DFAOPT.CFG file                           */
/*            0 : normally completed                                         */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int writeOptFile(void)
{
    int iRc;
    char aszBuf[MAX_COPYBACKDIRNAME + 19];
    char aszLbuf[6];

    pOthrFp = fopen(TMPOPTFILE,"w");
    if (pOthrFp != NULL) {

        strcpy(aszBuf,OPTFILEVER);
        strcat(aszBuf,"\n");
        iRc = fputs(aszBuf,pOthrFp);

        if (iRc != EOF) {
            strcpy(aszBuf,"TIMESYNC=");
            if (pOther->byTimeSync == 0x01) {
                strcat(aszBuf,"YES");
            }
            else {
                strcat(aszBuf,"NO");
            }
            strcat(aszBuf,"\n");
            iRc = fputs(aszBuf,pOthrFp);
        }

        if (iRc != EOF) {
            strcpy(aszBuf,"MOUNTWARNING=");
            if (pOther->byMountWarning == 0x01) {
                strcat(aszBuf,"YES");
            }
            else {
                strcat(aszBuf,"NO");
            }
            strcat(aszBuf,"\n");
            iRc = fputs(aszBuf,pOthrFp);
        }

        if (iRc != EOF) {
            ltoa(pOther->uiCopyback, aszLbuf, 10);
            strcpy(aszBuf,"COPYBACKDELAYTIME=");
            strcat(aszBuf,aszLbuf);
            strcat(aszBuf,"\n");
            iRc = fputs(aszBuf,pOthrFp);
        }

        if (iRc != EOF) {
            ltoa(pOther->uiDirSync,aszLbuf,10);
            strcpy(aszBuf,"DIRSYNCINTERVAL=");
            strcat(aszBuf,aszLbuf);
            strcat(aszBuf,"\n");
            iRc = fputs(aszBuf,pOthrFp);
        }

        if (iRc != EOF) {
            ltoa(pOther->uiTimeOut,aszLbuf,10);
            strcpy(aszBuf,"RESPONSEWAITTIME=");
            strcat(aszBuf,aszLbuf);
            strcat(aszBuf,"\n");
            iRc = fputs(aszBuf,pOthrFp);
        }

        if (iRc != EOF) {
            ltoa(pOther->uiConnect,aszLbuf,10);
            strcpy(aszBuf,"CONNECTTIME=");
            strcat(aszBuf,aszLbuf);
            strcat(aszBuf,"\n");
            iRc = fputs(aszBuf,pOthrFp);
        }

        if (iRc != EOF) {
            ltoa(pOther->uiFileSpers,aszLbuf,10);
            strcpy(aszBuf,"FILESPARSEDELAYTIME=");
            strcat(aszBuf,aszLbuf);
            strcat(aszBuf,"\n");
            iRc = fputs(aszBuf,pOthrFp);
        }

        if (iRc != EOF) {
            if ( *(pOther->aszSendBuf) != '\0') {
                strcpy(aszBuf,"SENDBUFFERLENGTH=");
                strcat(aszBuf,pOther->aszSendBuf);
                strcat(aszBuf,"\n");
                iRc = fputs(aszBuf,pOthrFp);
            }
        }

        if (iRc != EOF) {
            if ( *(pOther->aszMesFile) != '\0') {
                strcpy(aszBuf,"MESSAGELOGFILE=");
                strcat(aszBuf,pOther->aszMesFile);
                strcat(aszBuf,"\n");
                iRc = fputs(aszBuf,pOthrFp);
            }
        }

        if (iRc != EOF) {
            if ( *(pOther->aszLogFile) != '\0') {
                strcpy(aszBuf,"ERRORLOGFILE=");
                strcat(aszBuf,pOther->aszLogFile);
                strcat(aszBuf,"\n");
                iRc = fputs(aszBuf,pOthrFp);
            }
        }

        if (iRc != EOF) {
            if ( *(pOther->aszWatchdog) != '\0') {
                strcpy(aszBuf,"WATCHDOGWAITTIME=");
                strcat(aszBuf,pOther->aszWatchdog);
                strcat(aszBuf,"\n");
                iRc = fputs(aszBuf,pOthrFp);
            }
        }

        if (iRc == EOF) {
            fclose(pOthrFp);
            unlink(TMPOPTFILE);
            return -1;
        }

		iRc = fputc(0x1a,pOthrFp);
        if (iRc == EOF) {
            fclose(pOthrFp);
            unlink(TMPOPTFILE);
            return -1;
        }

        fclose(pOthrFp);
        pOthrFp = NULL;

        SetDefaultFileAttribute(OPTFILE);
        unlink(OPTFILE);

        iRc = rename(TMPOPTFILE,OPTFILE);
        if (iRc != 0) {
            return -2;
        }

    }
    else {
        return -1;
    }
    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = OtherOptAction                                                    */
/*  name = Enter key processing of Gateway Backup Directory of "Other        */
/*         Options Screen"                                                   */
/*  func = Processings when the Enter key was pressed at "Gateway Backup     */
/*         Directory" of "Other Options Screen"                              */
/*         (1) Checks if the volume of the already-registered Backup         */
/*             Directory is a Gateway volume.                                */
/*         (2) Displays "Gateway Backup Directory Screen."                   */
/*                                                                           */
/*  i/o  = none                                                              */
/*  return = 0        : Esc. key was pressed                                 */
/*           K_SELECT : Backup Directory was registered.                     */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

LONG OtherOptAction(pFld, iKey, piField, pNUTInfo)

FIELD *pFld;
int iKey;
int *piField;
NUTInfo *pNUTInfo;
{
    int iRc;
    char aszBackupDir[MAX_COPYBACKDIRNAME];

    iKey = iKey;
    pFld = pFld;
    piField = piField;
    pNUTInfo = pNUTInfo;

    aszBackupDir[0] = '\0';

    if (*(pOther->aszCopyDir) != '\0') {

        iRc = DeleteServerNameFromBackupDir(pOther->aszCopyDir);
        if (iRc != 0) {
            NWSAlert(0, 0, pHandle, COPYDIRNOTEXIST);
            *(pOther->aszCopyDir) = '\0';
        }
    }

    if (*(pOther->aszCopyDir) != '\0') {

        iRc = EscapeDirExistCheck(pOther->aszCopyDir);
        if (iRc != 0) {
            NWSAlert(0, 0, pHandle, COPYDIRNOTEXIST);
            *(pOther->aszCopyDir) = '\0';
        }
    }

    if (*(pOther->aszCopyDir) != '\0') {

        iRc = CompareEscapeDirNameToDFAMVolume(pOther->aszCopyDir);

        switch (iRc) {

          case 0 :
            break;

          case -2 :
            NWSAlert(0, 0, pHandle, OUTOFMEMORY);
            return 0;
            break;

          case -3 :
            NWSAlert(0, 0, pHandle, OPTIONFILEINVALID);

            *(pOther->aszCopyDir) = '\0';
            break;

          default :
            sprintf(aszError, "%d", iRc);
            AlertErrorCode(ADMINERROR, aszError);
            return 0;
            break;

        }
    }

    strcpy(aszBackupDir, pOther->aszCopyDir);


    iRc = NWSEditString(COPYDIRROW, COPYDIRCOL, COPYDIRHEIGHT, COPYDIRWIDTH, COPYBACKDIRPATH ,
                        NOTHING, aszBackupDir, MAX_COPYBACKDIRNAMESTRING,
                        EF_SET | EF_UPPER , pHandle, NULL, CopyDirAction, CHAR_OF_BACKUPDIR);

    if (iRc == K_SELECT) {
        strcpy(pOther->aszCopyDir, aszBackupDir);
    }

    return (LONG)iRc;
}


/*EnglishComment**************************************************************/
/*  ID   = CopyDirAction                                                     */
/*  name = "Gateway Backup Directory Screen" processing routine              */
/*  func = (1) Goes back to "Other Options Screen" when the Esc key was      */
/*             pressed.                                                      */
/*         (2) Checks if the entered directory name for Backup Directory     */
/*             exists.                                                       */
/*                                                                           */
/*  i/o  = none                                                              */
/*  return = -1       : the name of the inputed Backup Directory is invalid. */
/*            0       : Esc. key was pressed.                                */
/*           K_SELECT : Backup Directory was registered.                     */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int CopyDirAction(uiAction, pszCopyDir, pPar)
LONG uiAction;
BYTE *pszCopyDir;
void *pPar;
{
    int iRc;
    char aszBackupDir[MAX_COPYBACKDIRNAME];

    pPar = pPar;

    if ((uiAction & E_ESCAPE) != (LONG)0) {
        return 0;
    }

    if (pszCopyDir[0] == '\0') {
        *(pOther->aszCopyDir) = '\0';
        return K_SELECT;
    }

    strcpy(aszBackupDir, pszCopyDir);


    iRc = DeleteServerNameFromBackupDir(aszBackupDir);
    if (iRc != 0) {
        NWSAlert(0, 0, pHandle, COPYDIRNOTEXIST2);
        return -1;
    }

    iRc = EscapeDirExistCheck(aszBackupDir);
    if (iRc != 0) {
        NWSAlert(0, 0, pHandle, COPYDIRNOTEXIST2);
        return -1;
    }


    iRc = CompareEscapeDirNameToDFAMVolume(aszBackupDir);

    switch (iRc) {

      case 0 :
        break;

      case -2 :
        NWSAlert(0, 0, pHandle, OUTOFMEMORY);
        return -1;
        break;

      case -3 :
        NWSAlert(0, 0, pHandle, DEFINEVOLUME);
        return -1;
        break;

      default :
        sprintf(aszError, "%d", iRc);
        AlertErrorCode(ADMINERROR, aszError);
        return -1;
        break;

    }

    strcpy(pOther->aszCopyDir, aszBackupDir);

    return K_SELECT;
}


/*EnglishComment**************************************************************/
/*  ID   = delAllOther                                                       */
/*  name = Information deletion routine                                      */
/*  func = Deletes Other Information on the memory.                          */
/*         (1) Frees the memory registering Other Information.               */
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

void delAllOther(void)
{
    if (pOther != NULL) {
        NWSFree((struct OtherInfo *)pOther, pHandle);
        pOther = (struct OtherInfo *)NULL;
    }
    return;
}


/*EnglishComment**************************************************************/
/*  ID   = EscapeDirNameCheck                                                */
/*  name = Backup Directory syntax checking routine                          */
/*  func =                                                                   */
/*                                                                           */
/*  i/o  = pszEscDir : char * : i : Backup Directory name                    */
/*                                                                           */
/*  return = -1 : Backup Directory name invalid                              */
/*            0 : Backup Directory name syntactically correct                */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int EscapeDirNameCheck(pszEscDir)
char *pszEscDir;
{
    char *pszBuf;

    pszBuf = pszEscDir;
    while (*pszBuf != '\0') {

        if (isgraph(*pszBuf) == 0) {
            return -1;
        }

        if (islower(*pszBuf) != 0) {
            *pszBuf = (char)toupper(*pszBuf);
        }

        pszBuf++;
    }
    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = CompareEscapeDirNameToDFAMVolume                                  */
/*  name = Backup Directory checking routine                                 */
/*  func = Checks if the volume of Backup Directory has been already         */
/*         registered as a Gateway Volume.                                   */
/*                                                                           */
/*  i/o  = pszEscapeDir : char * : i : Backup Directory name                 */
/*                                                                           */
/*  return = 0 : not registered as a Gateway Volume                          */
/*          -2 : shortage of memory                                          */
/*          -3 : already registered as a Gateway Volume                      */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int CompareEscapeDirNameToDFAMVolume(pszEscapeDir)
char *pszEscapeDir;
{
    struct VolumeInfo *pVol;
    int iRc;
    int iCnt;
    char aszVol[_MAX_VOLUME];

    iRc = VolumeInit(FROM_OTHER_INIT);
    switch (iRc) {
      case  0 :
      case -4 :
      case -6 :
        break;

      case -2 :
        delAllVolume();
        return -2;
        break;

      default :
        delAllVolume();
        return 0;
        break;
    }

    iCnt = 0;
    while (pszEscapeDir[iCnt] != ':') {
        aszVol[iCnt] = pszEscapeDir[iCnt];
        iCnt++;
    }

    aszVol[iCnt] = '\0';

    pVol = getVolume(aszVol);

    if (pVol != (struct VolumeInfo *)NULL) {
        delAllVolume();
        return -3;
    }

    delAllVolume();
    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = EscapeDirExistCheck                                               */
/*  name = Backup Directory checking routine                                 */
/*  func = Checks if Backup Directory exists.                                */
/*                                                                           */
/*  i/o  = pszEscapeDir : char * : i : Backup Directory name                 */
/*                                                                           */
/*  return = -1 : Backup Directory does nor exists.  Or the directory name   */
/*                is not an absolute path name.                              */
/*            0 : Backup Directory exists.                                   */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int EscapeDirExistCheck(pszEscapeDir)
char *pszEscapeDir;
{
    struct stat buf;
    char *pChr;
    int iOfset;
    int iRc;
    int iCnt;

    iCnt = 0;

    pChr = strstr(pszEscapeDir, "\\.");
    if (pChr != NULL) {
        return -1;
    }

    while (pszEscapeDir[iCnt] != '\0') {
        pChr = strchr("*?", pszEscapeDir[iCnt]);
        if (pChr != NULL) {
            return -1;
        }
        iCnt++;
    }

    pChr = strchr(pszEscapeDir, ':');
    if (pChr == NULL) {
        return -1;
    }

    iOfset = pChr - pszEscapeDir;

    if ((iOfset >= _MAX_VOLUME)
      ||(iOfset < 2)) {
        return -1;
    }

    iRc = stat(pszEscapeDir, &buf);
    if (iRc != 0) {
        return -1;
    }

    if ( (buf.st_mode & S_IFDIR) != S_IFDIR) {
        return -1;
    }
    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = DeleteServerNameFromBackupDir                                     */
/*  name = Server name deleting routine                                      */
/*  func = Deletes a server name if the Backup Directory name includes the   */
/*         server name.                                                      */
/*                                                                           */
/*  i/o  = pszBackupDir : char * : i : Backup Directory name                 */
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

int DeleteServerNameFromBackupDir(pszBackupDir)
char *pszBackupDir;
{
    char *pszBuf;
    int tmLen;
    char aszDrive[_MAX_SERVER + _MAX_VOLUME];

    if (*pszBackupDir == '\0') {
        return 0;
    }

    _splitpath(pszBackupDir, aszDrive, NULL, NULL, NULL);

    pszBuf = strchr(aszDrive, '\\');
    if (pszBuf == NULL) {
        pszBuf = strchr(aszDrive, '/');
        if (pszBuf == NULL) {
            return 0;
        }
    }

    *pszBuf = '\0';
    tmLen = strlen(aszDrive);

    if (strcmpi(aszNWserver, aszDrive) != 0) {
        return -1;
    }

    strcpy(pszBackupDir, pszBackupDir + tmLen + 1);

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = SetDefaultFileAttribute                                           */
/*  name = file attributes changing routine                                  */
/*  func = Changes the .CFG file attributes into Read/Write + Archive.       */
/*                                                                           */
/*  i/o  = pszPath : char * : i : path name (.CFG file name)                 */
/*                                                                           */
/*  return = -1: failed to change the attributes                             */
/*            0: successfully changed the attributes                         */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int SetDefaultFileAttribute(pszPath)
char *pszPath;
{
    struct ModifyStructure sModify;
    int iRc;

    sModify.MFileAttributes = DEFAULTATTRIBUTE;
    sModify.MFileAttributesMask = -1;

    iRc = ChangeDirectoryEntry(pszPath, &sModify, MFileAttributesBit, 0L);
    if (iRc != 0) {
        return -1;
    }

    return 0;
}
