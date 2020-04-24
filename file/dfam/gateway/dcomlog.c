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
 * $Log: dcomlog.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:18  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:47  marty]
 *
 * Revision 1.1.2.1  1996/02/21  06:44:30  takano
 * 	First Submit
 * 	[1996/02/21  06:44:25  takano]
 * 
 * $EndLog$
 */
#ifdef DCELOG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <malloc.h>
#include <nwsemaph.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>
#include <process.h>
#include <errno.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "dfammsg.h"
#include "dceapi.h"

#define OUTHEADSIZE         20
#define OUTERRSIZE          100
#define OUTMAXFILESIZE      655360
#define BOOTNOLOG           0xFFFF0000
#define ERRNOLOG            0x0000FFFF
#define MAINSOCK            0
#define WATCHSOCK           1
#define EYECHACHSIZE        8
#define FILEHEDDER          (EYECHACHSIZE+sizeof(time_t)+sizeof(clock_t))


static char *pszDCELogFileHed[2]    = {
            "DFADCE          ",
            "DFADCE E        "};


static int iWriteErrFlag;



static const char *pszTraceFileName[2] = {
            "SYS:\\DFADCE1.LOG",
            "SYS:\\DFADCE2.LOG"};

static int iWriteFileFlag;



static long iAmariWriteCtr;

static unsigned long uiLogGetMode;






static int aiSockfd[2];



static FILE *pLogFp;

static int iLogSh;


static int DCELogWrite(char *pHead,int tmDataLen,char *pData,int *piErrno);
static int DCELogFileWrite(char *pWriteBuff,int tmLength,int *piErrno);
static int DCELogFileChange(int *piErrno);
static void DCELogErrMessage(int iWhere,int iReason,int iErrno);

// EnglishComment
/*****************************************************************************/
/*  ID   = DCELogInit                                                        */
/*  name = Communication logging initialization routine                      */
/*  func = Is invoked when DFADCE.NLM is started, and sets up the            */
/*         environment needed for taking the communication logging.          */
/*         If there are old files, deletes them, and makes a new             */
/*         communication logging file.  Outputs a file header.               */
/*                                                                           */
/*  i/o  =  bFlag : BOOL :inp: taking/stopping communication logging         */
/*                        TRUE : takes the logging                           */
/*                        FALSE: stops taking the logging                    */
/*  return =                                                                 */
/*          DLNOERR     (0): successful (including the case of "communication*/
/*                           logging is not taken")                          */
/*          DLREMOVEERR (1): file deletion error (in the case that "Read     */
/*                           Only" is indicated, the target is a directory)  */
/*          DLOPENERR   (2): error when a file is newly made (?)             */
/*          DLWRITEERR  (3): error when writing to a file (disk is full, etc)*/
/*  common =                                                                 */
/*      iWriteErrFlag   :int    :out: error indicator for write processing   */
/*      iWriteFileFlag  :int    :out: log file ID                            */
/*      aiSockfd        :int *  :out: socket descriptor setting area         */
/*      uiLogGetMode    :unsigned long:out: mode of "taking/stopping the     */
/*                                          communication logging"           */
/*      pszTraceFileName:char **:inp: log file name                          */
/*      iLogSh          :int    :out: semaphore handle for commnction logging*/
/*      pLogFp          :FILE * :out: file handle for communication logging  */
/*      pszDCELogFileHed:char **:i/o: file header information                */
/*      iAmariWriteCtr  :long   :out: remaining bytes in the file            */
/*      timezone        :long   :inp: time difference: system global variable*/
/*  note = none                                                              */
/*  date =  May. 22, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DCELogInit(BOOL bFlag)
{
    int iCode;
    int iCtr;
    int iRetCode;
    int iErrno;


    iRetCode = DLNOERR;
    iWriteErrFlag = 0;
    aiSockfd[MAINSOCK] = 0xFFFFFFFF;
    aiSockfd[WATCHSOCK] = 0xFFFFFFFF;
    uiLogGetMode = 0;
    iErrno = 0;
    pLogFp = NULL;

    tzset();

    iLogSh = OpenLocalSemaphore(1);

    if (bFlag == FALSE){
        uiLogGetMode = BOOTNOLOG | ERRNOLOG;
        goto ErrExit;
    }

    for (iCtr = 0; iCtr <= 1; iCtr++){
        iCode = access(pszTraceFileName[iCtr],0);
        if (iCode == 0){
            iCode = remove(pszTraceFileName[iCtr]);
            if (iCode != 0){
                iErrno = errno;
                iRetCode = DLREMOVEERR;
                goto ErrExit;
            }
        }
    }

    iWriteFileFlag = 0;
    iWriteErrFlag = 0;

    iAmariWriteCtr = OUTMAXFILESIZE;

    pLogFp = fopen(pszTraceFileName[iWriteFileFlag],"wb");
    if (pLogFp == NULL){
        iErrno = errno;
        iRetCode = DLOPENERR;
        goto ErrExit;
    }

    *(time_t *)&pszDCELogFileHed[iWriteErrFlag][EYECHACHSIZE] = time(NULL) - timezone;

    *(clock_t *)&pszDCELogFileHed[iWriteErrFlag][EYECHACHSIZE+sizeof(time_t)] = clock();


    iCode = DCELogFileWrite(pszDCELogFileHed[iWriteErrFlag],FILEHEDDER,&iErrno);
    if (iCode != DLNOERR || iWriteErrFlag != 0){
        if (iCode == DLNOERR && iWriteErrFlag != 0){
            iRetCode = DLWRITEERR;
        } else {
            iRetCode = iCode;
        }
        fclose(pLogFp);
        goto ErrExit;
    }

    iAmariWriteCtr -= FILEHEDDER;

ErrExit:
    if (iRetCode != DLNOERR){
        DCELogErrMessage(1,iRetCode,iErrno);
        uiLogGetMode = BOOTNOLOG | ERRNOLOG;
    }
    return(iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = DCELogTerm                                                        */
/*  name = Communication logging termination routine                         */
/*  func = Terminates the communication logging used by DFADCE.NLM.          */
/*         And closes the files.                                             */
/*  i/o  = none                                                              */
/*  return = DLNOERR    (0): successful (including the case of "communication*/
/*                           logging is not taken")                          */
/*           DLCLOSEERR (4): file closing error (?)                          */
/*  common =                                                                 */
/*      uiLogGetMode    :unsigned long:inp: mode of "taking/stopping the     */
/*                                          communication logging"           */
/*      iLogSh          :int    :inp: semaphore handle for commnction logging*/
/*      pLogFp          :FILE * :inp: file handle for communication logging  */
/*                                                                           */
/*  note =  none                                                             */
/*  date =  May. 22, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DCELogTerm(void)
{
    int iCode;
    int iErrno = 0;
    int iRetCode = DLNOERR;

    if (!uiLogGetMode){
        if (pLogFp){
            iCode = fclose(pLogFp);
            if (iCode == -1){
                iErrno = errno;
                iRetCode = DLCLOSEERR;
                DCELogErrMessage(3,iRetCode,iErrno);
            }
            pLogFp = NULL;
        }
    }

    CloseLocalSemaphore(iLogSh);
    return (iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = DCELogSocketSet                                                   */
/*  name = Socket descriptor setting routine                                 */
/*  func = Sets initial values to the socket descriptor.                     */
/*  i/o  = iSockfd     :int           :inp: socket descriptor                */
/*         ushConAssort:unsigned short:inp: connection type                  */
/*  return = none                                                            */
/*  common =                                                                 */
/*      aiSockfd        :int *  :out: socket descriptor setting area         */
/*      uiLogGetMode    :unsigned long:inp: mode of "taking/stopping the     */
/*                                          communication logging"           */
/*  note =  none                                                             */
/*  date =  May. 22, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void DCELogSoketSet(int iSockfd,unsigned short ushConAssort)
{

    if (!uiLogGetMode){
        switch (ushConAssort){
            case GW_CONTROL_CON :
                aiSockfd[MAINSOCK] = iSockfd;
                break;
            case GW_WATCHDOG_CON :
                aiSockfd[WATCHSOCK] = iSockfd;
                break;
            default :
                break;
        }
    }
}

// EnglishComment
/*****************************************************************************/
/*  ID   = DCELogSocketReset                                                 */
/*  name = Socket descriptor initialization routine                          */
/*  func = Sets initial values to the socket descriptor.                     */
/*  i/o  = iSockfd      : int            :inp: socket descriptor             */
/*  return = none                                                            */
/*  common =                                                                 */
/*      aiSockfd        :int *  :out: socket descriptor setting area         */
/*      uiLogGetMode    :unsigned long:inp: mode of "taking/stopping the     */
/*  note =  none                                                             */
/*  date =  May. 22, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void DCELogSoketReset(int iSockfd)
{
    int iCtr;

    if (!uiLogGetMode && iSockfd != 0xFFFFFFFF){
        for (iCtr = 0; iCtr < 2; iCtr++){
            if (aiSockfd[iCtr] == iSockfd){
                aiSockfd[iCtr] = 0xFFFFFFFF;
            }
        }
    }
}

// EnglishComment
/*****************************************************************************/
/*  ID   = DCELogOutput                                                      */
/*  name = Communication logging outputting routine                          */
/*  func = Makes data of the communication logging, and outputs to a file.   */
/*           1. Makes a header.                                              */
/*           2. Makes data.                                                  */
/*           3. Passes the data to the file output function.                 */
/*  i/o  = uiEvent :LONG  :inp: event information                            */
/*         iLength :int   :inp: output length                                */
/*                              0 when sent/received ---> no logging taken   */
/*                              1 when sent/received ---> pData address      */
/*                                                        output             */
/*         iSockfd :int   :inp: socket descriptor                            */
/*         uiComNum:LONG  :inp: connection number                            */
/*         pData   :void *:inp: output data                                  */
/*         iErr    :int   :inp: added information                            */
/*                                                                           */
/*  return = DLNOERR     (0): successful (including the case of              */
/*                            "communication logging is not taken")          */
/*         * DLREMOVEERR (1): file deletion error (in the case that "Read    */
/*                            Only" is indicated, the target is a directory) */
/*         * DLOPENERR   (2): error when a file is newly made (?)            */
/*         * DLCLOSEERR  (4): file closing error (?)                         */
/*         * DLFSEEKERR  (5): file pointer relocation error (when moved to   */
/*                            the current position)                          */
/*         * DLFFLUSHERR (6): file flush error (?)                           */
/*                                                                           */
/*  common = aiSockfd        :int *  :out: socket descriptor setting area    */
/*           uiLogGetMode    :unsigned long:out: mode of "taking/stopping    */
/*                                          the communication logging"       */
/*           pszTraceFileName:char **:inp: log file name                     */
/*           iLogSh          :int    :out: semaphore handle for communication*/
/*                                         logging                           */
/*  note =  none                                                             */
/*  date =  May. 22, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DCELogOutput(LONG uiEvent,int iLength,int iSockfd,LONG uiComNum,void *pData,int iErr)
{
    char caOutBuff[OUTHEADSIZE];
    char caErrBuff[OUTERRSIZE];
    char *pDataArea;
    int iCtr;
    int iCode;
    int iRetCode;
    int iErrno;
    int iCrntNLMID;
    unsigned long iDataAddr;




    iRetCode = DLNOERR;
    iErrno = 0;
    iCrntNLMID = SetNLMID(idfadceNLMID);

    WaitOnLocalSemaphore(iLogSh);

    if (uiLogGetMode){
        goto ErrExit;
    }

    if (iLength == 0){
        switch (uiEvent){
            case SDDATA :
            case RVDATA :
        goto ErrExit;
            default :
                break;
        }
    }

    iCtr = 0;

    *(LONG *)&caOutBuff[iCtr] = uiEvent;
    iCtr += sizeof(LONG);

    switch (uiEvent){
        case FUNC_E :
            sprintf(caErrBuff,"%s:errno = %d",(char *)pData,iErr);
            iLength = strlen(caErrBuff);
            pDataArea = caErrBuff;
            break;
        case PORT_N :
            sprintf(caErrBuff,"%ld",iErr);
            iLength = strlen(caErrBuff);
            pDataArea = caErrBuff;
            break;
        case DISCON :
            sprintf(caErrBuff,"%08lX",iErr);
            iLength = strlen(caErrBuff);
            pDataArea = caErrBuff;
            break;
        case SDDATA :
        case RVDATA :
            if (iLength != -1){
                pDataArea = (char *)pData;
            } else {
                iLength = sizeof(long);
                iDataAddr = (unsigned long)pData;
                pDataArea = (char *)&iDataAddr;
            }
            break;
        case HLTCHK :
            pDataArea = (char *)pData;
            iLength = 0;
            break;
        case HOST_N :
        case IPADDR :
            iLength = strlen((char *)pData);

        case MESSAG :
        default :
            pDataArea = (char *)pData;
            break;
    }

    *(int *)&caOutBuff[iCtr] = iLength;
    iCtr += sizeof(int);

    *(LONG *)&caOutBuff[iCtr] = clock();
    iCtr += sizeof(LONG);

    *(int *)&caOutBuff[iCtr] = iSockfd;
    iCtr += sizeof(int);

    if (iSockfd == aiSockfd[MAINSOCK]){
        uiComNum = 0x00000000;
    } else if (iSockfd == aiSockfd[WATCHSOCK]){
        uiComNum = 0xFFFFFFFF;
    }

    *(LONG *)&caOutBuff[iCtr] = uiComNum;
    iCtr += sizeof(LONG);

    iCode = DCELogWrite(caOutBuff,iLength,pDataArea,&iErrno);
    if (iCode != 0){
        iRetCode = iCode;
        DCELogErrMessage(2,iRetCode,iErrno);
    }

ErrExit:
    SignalLocalSemaphore(iLogSh);
    SetNLMID(iCrntNLMID);
    return iRetCode;
}

// EnglishComment
/*****************************************************************************/
/*  ID   = DCELogWrite                                                       */
/*  name = Controls outputting to the communication logging                  */
/*  func = Outputs to the communication logging file.                        */
/*  i/o  = pHead    : char *:inp: communication logging header information   */
/*         tmDataLen: int   :inp: data length                                */
/*         pData    : char *:inp: data                                       */
/*         piErrno  : int * :out: error information setting area             */
/*                                                                           */
/*  return = DLNOERR     (0): successful (including the case of              */
/*                            "communication logging is not taken")          */
/*         * DLREMOVEERR (1): file deletion error (in the case that "Read    */
/*                            Only" is indicated, the target is a directory) */
/*         * DLOPENERR   (2): error when a file is newly made (?)            */
/*         * DLCLOSEERR  (4): file closing error (?)                         */
/*           DLFSEEKERR  (5): file pointer relocation error (when moved to   */
/*                            the current position)                          */
/*           DLFFLUSHERR (6): file flush error (?)                           */
/*                                                                           */
/*  common = iWriteErrFlag   : int         :inp: write error indicator       */
/*           uiLogGetMode    :unsigned long:i/o: mode of "taking/stopping    */
/*                                          the communication logging"       */
/*           iLogSh          :int    :out: semaphore handle for communication*/
/*           pszTraceFileName:char **:inp: log file name                     */
/*           iAmariWriteCtr  : long  :i/o: remaining bytes of the file       */
/*  note =  none                                                             */
/*  date =  May. 22, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DCELogWrite(char *pHead,int tmDataLen,char *pData,int *piErrno)
{
    int iCode;
    int iRetCode;


    iRetCode = DLNOERR;


    DCEMemCpComLog(pHead,(long)OUTHEADSIZE);
    DCEMemCpComLog(pData,(long)tmDataLen);
    if (iAmariWriteCtr == OUTMAXFILESIZE){
        if (pLogFp == NULL){
            pLogFp = fopen(pszTraceFileName[iWriteFileFlag],"wb");
            if (pLogFp == NULL){
                *piErrno = errno;
                if (*piErrno != ENOSPC){
                    iRetCode = DLOPENERR;
                    uiLogGetMode |= ERRNOLOG;
                }

                goto ErrExit;
            }
        }
        *(time_t *)&pszDCELogFileHed[iWriteErrFlag][EYECHACHSIZE] = time(NULL) - timezone;

        *(clock_t *)&pszDCELogFileHed[iWriteErrFlag][EYECHACHSIZE+sizeof(time_t)] = clock();

        iCode = DCELogFileWrite(pszDCELogFileHed[iWriteErrFlag],FILEHEDDER,piErrno);
        if (iCode != DLNOERR || iWriteErrFlag != 0){
            iRetCode = iCode;
            goto ErrExit;
        }
        iAmariWriteCtr -= FILEHEDDER;
    }

    if (iAmariWriteCtr - OUTHEADSIZE - tmDataLen < 0){
        iCode = DCELogFileChange(piErrno);
        if (iCode != DLNOERR || iWriteErrFlag != 0 || pLogFp == NULL){
            iRetCode = iCode;
            goto ErrExit;
        }
    }

    iCode = DCELogFileWrite(pHead,OUTHEADSIZE,piErrno);
    if (iCode != DLNOERR){
        iRetCode = iCode;
        goto ErrExit;
    } else if (iWriteErrFlag != 0){

    } else {
        if (tmDataLen > 0){
            iCode = DCELogFileWrite(pData,tmDataLen,piErrno);
            if (iCode != DLNOERR || iWriteErrFlag != 0){
                iCode = fseek(pLogFp,0-OUTHEADSIZE,SEEK_CUR);
                if (iCode != 0){
                    *piErrno = errno;
                    iRetCode = DLFSEEKERR;
                    uiLogGetMode |= ERRNOLOG;
                    fclose(pLogFp);
                    pLogFp = NULL;
                    goto ErrExit;
                }
            }
        }
    }

    if (iWriteErrFlag == 0){
        iAmariWriteCtr -= (OUTHEADSIZE + tmDataLen);
    } else {
        iCode = DCELogFileChange(piErrno);
        if (iCode != DLNOERR || iWriteErrFlag != 0 || pLogFp == NULL){
            iRetCode = iCode;
            goto ErrExit;
        }
    }

    iCode = fflush(pLogFp);
    if (iCode == -1){
        *piErrno = errno;
        if (*piErrno == ENOSPC){
            iWriteErrFlag = 1;
            iCode = fseek(pLogFp,0-(OUTHEADSIZE + tmDataLen),SEEK_CUR);
            if (iCode != 0){
                iRetCode = DLFSEEKERR;
                uiLogGetMode |= ERRNOLOG;
                fclose(pLogFp);
                pLogFp = NULL;
                goto ErrExit;
            }
            iCode = DCELogFileChange(piErrno);
            if (iCode != DLNOERR || iWriteErrFlag != 0 || pLogFp == NULL){
                iRetCode = iCode;
            }
            goto ErrExit;
        }
        iRetCode = DLFFLUSHERR;
        uiLogGetMode |= ERRNOLOG;
        fclose(pLogFp);
        pLogFp = NULL;
        goto ErrExit;

    }

ErrExit:
    return (iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = DCELogFileWrite                                                   */
/*  name = Communication logging file outputting routine                     */
/*  func = Outputs to the communication logging file.                        */
/*           1. Outputs to a file.                                           */
/*           2. If failed, sets back the file pointer to the last successful */
/*              location.                                                    */
/*                                                                           */
/*  i/o  = pWriteBuff: char *:inp: output data setting area                  */
/*         tmLength  : int   :inp: output data length                        */
/*         piErrno   : int * :out: error information setting area            */
/*  return = DLNOERR     (0): successful (including the case of              */
/*                            "communication logging is not taken")          */
/*           DLFSEEKERR  (5): file pointer relocation error (when moved to   */
/*                            the current position)                          */
/*                                                                           */
/*  common = iWriteErrFlag   : int         :out: write error indicator       */
/*           uiLogGetMode    :unsigned long:out: mode of "taking/stopping    */
/*                                               the communication logging"  */
/*           pLogFp          : FILE *      :out: communication logging file  */
/*                                               handle.                     */
/*                                                                           */
/*  note =  none                                                             */
/*  date =  May. 22, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DCELogFileWrite(char *pWriteBuff,int tmLength,int *piErrno)
{
    int iCode;
    int iRetCode = DLNOERR;



    iCode = fwrite(pWriteBuff,sizeof(char),tmLength,pLogFp);
    if (iCode < sizeof(char) * tmLength){
        *piErrno = errno;
        if (iCode > 0){
            iCode = fseek(pLogFp,0-iCode,SEEK_CUR);
            if (iCode != 0){
                *piErrno = errno;
                iRetCode = DLFSEEKERR;
                uiLogGetMode |= ERRNOLOG;
            }
        }
        iWriteErrFlag = 1;
    } else {
        iWriteErrFlag = 0;
    }

    return (iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = DCELogFileChange                                                  */
/*  name = Communication logging switching function                          */
/*  func = Switches to another file when the outputting file is full.        */
/*  i/o  = piErrno : int * :out: error information area                      */
/*                                                                           */
/*  return = DLNOERR     (0): successful (including the case of              */
/*                            "communication logging is not taken")          */
/*           DLREMOVEERR (1): file deletion error (in the case that "Read    */
/*                            Only" is indicated, the target is a directory) */
/*           DLOPENERR   (2): error when a file is newly made (?)            */
/*           DLCLOSEERR  (4): file closing error (?)                         */
/*         * DLFSEEKERR  (5): file pointer relocation error (when moved to   */
/*                            the current position)                          */
/*                                                                           */
/*  common =                                                                 */
/*      iWriteErrFlag   :int    :i/o: error indicator for write processing   */
/*      uiLogGetMode    :unsigned long:out: mode of "taking/stopping the     */
/*                                          communication logging"           */
/*      pszTraceFileName:char **:inp: log file name                          */
/*      pLogFp          :FILE * :out: file handle for communication logging  */
/*      pszDCELogFileHed:char **:i/o: file header information                */
/*      iAmariWriteCtr  :long   :out: remaining bytes in the file            */
/*      timezone        :long   :inp: time difference: system global variable*/
/*  note =  none                                                             */
/*  date =  May. 22, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DCELogFileChange(int *piErrno)
{
    int iCode;
    int iRetCode;
    int iFileNum;



    iRetCode = DLNOERR;
    iFileNum = iWriteFileFlag;

    iCode = fclose(pLogFp);
    if (iCode == -1){
        *piErrno = errno;
        pLogFp = NULL;
        iRetCode = DLCLOSEERR;
        uiLogGetMode |= ERRNOLOG;
        goto ErrExit;
    }

    pLogFp = NULL;

    iFileNum = (iFileNum) ? 0 : 1;

    iCode = access(pszTraceFileName[iFileNum],0);
    if (iCode == 0){
        iCode = remove(pszTraceFileName[iFileNum]);
        if (iCode != 0){
            *piErrno = errno;
            iRetCode = DLREMOVEERR;
            uiLogGetMode |= ERRNOLOG;
            goto ErrExit;
        }
    }

    iWriteFileFlag = iFileNum;
    iAmariWriteCtr = OUTMAXFILESIZE;

    pLogFp = fopen(pszTraceFileName[iFileNum],"wb");
    if (pLogFp == NULL){
        *piErrno = errno;
        iWriteErrFlag = 1;
        if (*piErrno != ENOSPC){
            iRetCode = DLOPENERR;
            uiLogGetMode |= ERRNOLOG;
        }
        goto ErrExit;
    }

    *(time_t *)&pszDCELogFileHed[iWriteErrFlag][EYECHACHSIZE] = time(NULL) - timezone;

    *(clock_t *)&pszDCELogFileHed[iWriteErrFlag][EYECHACHSIZE+sizeof(time_t)] = clock();


    iCode = DCELogFileWrite(pszDCELogFileHed[iWriteErrFlag],FILEHEDDER,piErrno);
    if (iCode != DLNOERR || iWriteErrFlag != 0){
        iRetCode = iCode;
        goto ErrExit;
    }

    iAmariWriteCtr -= FILEHEDDER;

ErrExit:
    return (iRetCode);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = DCELogErrMessage                                                  */
/*  name = Error message outputting function                                 */
/*  func = Outputs an error message in a fixed format to Sytem Console       */
/*  i/o  = iWhere  : int :inp: the processing taken for the error            */
/*                             (error timing code)                           */
/*         iReason : int :inp: the function where error occurred.            */
/*                             (error type code)                             */
/*         iErrno  : int :inp: error information (error code)                */
/*  return = none                                                            */
/*  note =  none                                                             */
/*  date =  May. 22, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void DCELogErrMessage(int iWhere,int iReason,int iErrno)
{
    struct tm *pT;
    time_t uiTime;

    uiTime = time(NULL);
    pT = localtime(&uiTime);
    ConsolePrintf("%s(%02d:%02d:%02d wcode=%02d rcode=%02d ecode=%02d)%c%c",
        KDDS12201,
        pT->tm_hour,pT->tm_min,pT->tm_sec,iWhere,iReason,iErrno,0x0d,0x0a);

}

// EnglishComment
/*****************************************************************************/
/*  ID   = DCELogOptChk                                                      */
/*  name = Parameter checking routine                                        */
/*  func = Checks initialization parameters.                                 */
/*  i/o  = argc : int     :inp: number of initialization parameters          */
/*         argv : char ** :inp: parameters for initialization                */
/*                                                                           */
/*  return = 0 : /X was not designated.                                      */
/*           1 : /X was designated.                                          */
/*  common = none                                                            */
/*  note =  none                                                             */
/*  date =  May. 22, 1995 by Masakazu Mitsuhashi                             */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DCELogOptChk(int argc,char **argv)
{
    char *p;
    int i;
    int flag;




    for (flag = 0,i = 1; i < argc; i++){
        for (p = argv[i]; *p != '\0'; p++){
            if (*p == '/'){
                p++;
                *p = (char)toupper(*p);
                switch(*p){
                    case 'X':
                        flag = 1;
                        break;
                    case '/' :
                        p--;
                        break;
                    default :
                        break;
                }
            }
        }
    }
    return(flag);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = DCELogFileFlush                                                   */
/*  name = Taking communication logging                                      */
/*  func = Takes communication logging (taking while "communication logging  */
/*         active" indicator is on).                                         */
/*         1. Closes the file.                                               */
/*         2. Deletes the file.                                              */
/*         3. Opens the file.                                                */
/*  i/o  =                                                                   */
/*      :::                                                                  */
/*  return = DRNOERR       : (0) : normal end                                */
/*           DRINTERNALERR : (2) : internal error (self-contradiction)       */
/*           DRCLOSEERR    : (3) : close error                               */
/*           DRREMOVEERR   : (4) : delete error                              */
/*           DROPENERR     : (5) : open error                                */
/*           DRWRITEERR    : (6) : write error                               */
/*           DRNOGETMODE   : (7) : "communication logging active" indicator  */
/*                                 is not on                                 */
/*                                                                           */
/*  common = iWriteErrFlag   :int    :inp: write error indicator             */
/*           uiLogGetMode    :unsigned long :i/o: communication logging      */
/*                                         active/inactive indicator         */
/*           iLogSh          :int    :inp: semaphore handle for communication*/
/*                                         logging                           */
/*           pszDCELogFileHed:char **:i/o: file header information           */
/*           iAmariWriteCtr  :long   :i/o: remaining data bytes in the file  */
/*                                                                           */
/*  note = This function is directly invoked when DFARAS.NLM is started      */
/*         with /C.                                                          */
/*  date   = September 18, 1995 by Masakazu Mitsuhashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int DCELogFileFlush(void)
{
    int iCode;
    int iRetCode;
    int iCrntNLMID;
    int iCrntTGID;
    int iErrno;
    int iReason;
    int i;
    unsigned int uiSaveLogGetMode;

    iCrntTGID = SetThreadGroupID(idfadceTGID);
    iCrntNLMID = SetNLMID(idfadceNLMID);

    iReason = DLNOERR;
    iRetCode = DRINTERNALERR;

    WaitOnLocalSemaphore(iLogSh);

    uiSaveLogGetMode = uiLogGetMode;
    if (uiLogGetMode){
        iRetCode = DRNOGETMODE;
        goto ErrExit;
    }

    if (pLogFp != NULL){
        iCode = fclose(pLogFp);
        if (iCode == -1){
            pLogFp = NULL;
            iErrno = errno;
            iReason = DLCLOSEERR;
            iRetCode = DRCLOSEERR;
            uiLogGetMode |= ERRNOLOG;
            goto ErrExit;
        }
    }

    pLogFp = NULL;

    for (i = 0; i <=1; i++){
        iCode = access(pszTraceFileName[i],0);
        if (iCode == 0){
            iCode = remove(pszTraceFileName[i]);
            if (iCode != 0){
                iErrno = errno;
                iReason = DLREMOVEERR;
                iRetCode = DRREMOVEERR;
                uiLogGetMode |= ERRNOLOG;
                goto ErrExit;
            }
        }
    }

    iWriteFileFlag = 0;
    iAmariWriteCtr = OUTMAXFILESIZE;

    pLogFp = fopen(pszTraceFileName[iWriteFileFlag],"wb");
    if (pLogFp == NULL){
        iErrno = errno;
        iWriteErrFlag = 1;
        if (iErrno != ENOSPC){
            iReason = DLOPENERR;
            iRetCode = DROPENERR;
            uiLogGetMode |= ERRNOLOG;
        }
        goto ErrExit;
    }

    *(time_t *)&pszDCELogFileHed[iWriteErrFlag][EYECHACHSIZE] = time(NULL) - timezone;

    *(clock_t *)&pszDCELogFileHed[iWriteErrFlag][EYECHACHSIZE+sizeof(time_t)] = clock();


    iCode = DCELogFileWrite(pszDCELogFileHed[iWriteErrFlag],FILEHEDDER,&iErrno);
    if (iCode != DLNOERR || iWriteErrFlag != 0){
        iReason = iCode;
        iRetCode = DRWRITEERR;
        goto ErrExit;
    }

    iAmariWriteCtr -= FILEHEDDER;

    iRetCode = DRNOERR;
ErrExit:
    if (uiSaveLogGetMode != uiLogGetMode){
        DCELogErrMessage(2,iReason,iErrno);
    }

    SignalLocalSemaphore(iLogSh);
    SetNLMID(iCrntNLMID);
    SetThreadGroupID(iCrntTGID);

    return (iRetCode);
}
#endif
