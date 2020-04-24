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
 * $Log: drasmain.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:39  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:05  marty]
 *
 * Revision 1.1.2.1  1996/02/21  06:47:41  takano
 * 	First Submit
 * 	[1996/02/21  06:47:25  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <conio.h>
#include <process.h>
#include <ctype.h>
#include <errno.h>
#include <nwsemaph.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "dceapi.h"
#include "dfammsg.h"

#define DFARAS_MAX_OPTION 5

struct dfaras_t {
    char cOpt;
    char *pMessage;
    int (*pfnFunc)(void);
    int iChkNum;
};

struct dfaras_t DFARAS[DFARAS_MAX_OPTION] = {
    {'M',KDDS12401,DFARASmessage,0},
    {'E',KDDS12402,DFARASlog,0},
    {'C',KDDS12403,DCELogFileFlush,0},
    {'T',KDDS12404,DFARAStrace,0},
    {'D',KDDS12405,DFARASdump,0}
};

static int iDfarasNLMID;
static int iDfarasTGID;
static BOOL bRasTermFlag;

static int DFARASInit(void);
static void fnDFARASTerm(int iSignalCode);
static int DFARASFileGet(void);
static int DFARASoptChk(int argc,char **argv);
static int DFARASCmp(const void *pP1,const void *pP2);

// EnglishComment
/*****************************************************************************/
/*  ID   =  main                                                             */
/*                                                                           */
/*  name = DFARAS.NLM main process                                           */
/*  func = Main processing for DFARAS.NLM                                    */
/*          1. Registers UNLOAD handler.                                     */
/*          2. Suppresses Ctrl+C and Ctrl+S (it does not have Screen, but    */
/*             this is for double-checking purpose).                         */
/*          3. Analyzes options.                                             */
/*             /M Taking message logging (DFAERM.001, DFAERM.002)            */
/*             /E Taking error logging   (DFAERL.001, DFAERL.002)            */
/*             /C Taking communication logging (DFADCE1.LOG, DFADCE2.LOG)    */
/*             /T Taking trace file (DFAFUNC.TRC, ADMIN.TRC, MAIN.TRC,       */
/*                WATCHDOG.TRC)                                              */
/*             /D Taking dump file (DFADUMP.001)                             */
/*          4. Takes the logging in the order of the options                 */
/*          5. termination                                                   */
/*                                                                           */
/*  i/o  = argc : int     :inp: the number of parameters                     */
/*         argv : char ** :inp: parameters                                   */
/*  return = none                                                            */
/*      :                                                                    */
/*  common = iDfarasNLMID :int:out: NLM ID of DFARAS.NLM                     */
/*  note = none                                                              */
/*  date =  September 21, 1995  by Masakazu Mitsuhashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
// EnglishComment
void main(int argc,char **argv)
{
    void (*pCode)(int);
    int iRet;

    iDfarasNLMID = GetNLMID();
    iDfarasTGID = GetThreadGroupID();
    bRasTermFlag = FALSE;

    pCode = signal(SIGTERM,fnDFARASTerm);

    if (pCode == SIG_ERR){
        bRasTermFlag = TRUE;
        ExitThread(EXIT_NLM,EFAILURE);
    }

        SetCtrlCharCheckMode( FALSE );

    iRet = DFARASInit();
    switch (iRet){
        case 0 :
            break;
        case 1 :
            ConsolePrintf("%s%c",KDDS12407,'\r');
        default :
            bRasTermFlag = TRUE;
            ExitThread(EXIT_NLM,EFAILURE);
            break;
    }
    DFARASoptChk(argc,argv);

    if (uiRAS_sh){
        WaitOnLocalSemaphore(uiRAS_sh);
        DFARASFileGet();
        SignalLocalSemaphore(uiRAS_sh);
    }

    bRasTermFlag = TRUE;
    ExitThread(EXIT_NLM,ESUCCESS);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = DFARASInit                                                        */
/*                                                                           */
/*  name = DFARAS.NLM initialization                                         */
/*  func = Does initialization needed to start DFARAS.NLM                    */
/*          1. Checks the interface version of DFA.NLM.                      */
/*                                                                           */
/*  i/o  = none                                                              */
/*      :                                                                    */
/*  return = 0: successfully initialized                                     */
/*           1: failed to initialize                                         */
/*      :                                                                    */
/*  common = aszDFAGW_Ver : char * :inp: interface version of DFA.NLM        */
/*  note = none                                                              */
/*  date =  September 4, 1995  by Masakazu Mitsuhashi                        */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
// EnglishComment
int DFARASInit(void)
{
    int iRet = 1;


    if ( strcmp( DFAM_NLM_INTERFACE_VER, aszDFAGW_Ver) == 0 ) {
        iRet = 0;
    }
    return iRet;
}

// EnglishComment
/*****************************************************************************/
/*  ID   = fnDFARASTerm                                                      */
/*                                                                           */
/*  name = DFARAS UNLOAD handler                                             */
/*  func = Suppresses UNLOAD of DFARAS.NLM.                                  */
/*          1. Waits until the processing is completed even when DFA.NLM     */
/*             attempts to terminate.                                        */
/*                                                                           */
/*  i/o  = iSignalCode :int :inp: signal code                                */
/*      :                                                                    */
/*  return = none                                                            */
/*      :                                                                    */
/*  common = bRasTermFlag :BOOL :inp: termination indicator                  */
/*                                    (terminated/not yet)                   */
/*  note = none                                                              */
/*  date =  September 1, 1995  by Masakazu Mitsuhashi                        */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
// EnglishComment
void fnDFARASTerm(int iSignalCode)
{
    SetThreadGroupID(iDfarasTGID);
    SetNLMID(iDfarasNLMID);

    iSignalCode = iSignalCode;

    while (bRasTermFlag == FALSE){
        delay(SHORTWAITTIME);
        ThreadSwitch();
    }
}

// EnglishComment
/*****************************************************************************/
/*  ID   = DFARASoptChk                                                      */
/*                                                                           */
/*  name = Checking DFARAS.NLM options                                       */
/*  func = Analyzes the DFARAS.NLM options, and sets information to the      */
/*         structure for the options.                                        */
/*                                                                           */
/*  i/o  = argc : int     :inp: the number of parameters                     */
/*         argv : char ** :inp: parameters                                   */
/*                                                                           */
/*  return = 0: normal end (since this function does not check the contents  */
/*                     of the parameters, this always makes a normal return) */
/*      :                                                                    */
/*  common = DFARAS :struct dfaras_t :inp: structure of DFARAS.NLM options   */
/*           -> cOpt     :char          :inp:                                */
/*           -> pMessage :char *        : x : (x denotes "not used")         */
/*           -> pfnFunc  :int (*)(void) : x : (x denotes "not used")         */
/*           -> iChkNum  :int           :out:                                */
/*                                                                           */
/*  note = none                                                              */
/*  date =  September 4, 1995  by Masakazu Mitsuhashi                        */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
// EnglishComment
int DFARASoptChk(int argc,char **argv)
{
    int iArgcCtr;
    char *pCtr;
    char cTmp;
    int iOptCtr;
    int iOutNum;

    iOutNum = 1;
    for (iArgcCtr = 1; iArgcCtr < argc; iArgcCtr++){
        for (pCtr = argv[iArgcCtr]; *pCtr != '\0'; pCtr++){
            if (*pCtr == '/' && *(pCtr+1) != '/' && *(pCtr+1) != '\0'){
                pCtr++;
                cTmp = *pCtr;
                cTmp = toupper(cTmp);
                for (iOptCtr = 0; iOptCtr < DFARAS_MAX_OPTION; iOptCtr++){
                    if (cTmp == DFARAS[iOptCtr].cOpt){
                        if (DFARAS[iOptCtr].iChkNum == 0){
                            DFARAS[iOptCtr].iChkNum = iOutNum;
                            iOutNum++;
                        }
                        break;
                    }
                }
            }
        }
    }
    return 0;
}

// EnglishComment
/*****************************************************************************/
/*  ID   = DFARASFileGet                                                     */
/*                                                                           */
/*  name = Taking RAS file                                                   */
/*  func = Takes RAS file in the order designated at the startingtime.       */
/*          1. Determines  the order of the file taking.                     */
/*          2. Takes RAS file.                                               */
/*          3. Displays a message on the console if the file successfully    */
/*             taken.                                                        */
/*                                                                           */
/*  i/o  = none                                                              */
/*                                                                           */
/*  return = 0: normal end (since an error, if any, is not reported to a     */
/*                          caller, this always makes a normal return)       */
/*      :                                                                    */
/*  common = DFARAS :struct dfaras_t :inp: structure of DFARAS.NLM options   */
/*           -> cOpt     :char          : x : (x denotes "not used")         */
/*           -> pMessage :char *        :inp:                                */
/*           -> pfnFunc  :int (*)(void) :inp:                                */
/*           -> iChkNum  :int           :inp:                                */
/*                                                                           */
/*  note = none                                                              */
/*  date =  September 4, 1995  by Masakazu Mitsuhashi                        */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
// EnglishComment
int DFARASFileGet(void)
{
    int i;
    int iResult;

    qsort(DFARAS,DFARAS_MAX_OPTION,sizeof(struct dfaras_t),DFARASCmp);

    for (i = 0; i < DFARAS_MAX_OPTION; i++){
        if (DFARAS[i].iChkNum != 0){
            iResult = (DFARAS[i].pfnFunc)();
            switch (iResult){
                case 0 :
                    ConsolePrintf("%s%c",DFARAS[i].pMessage,'\r');
                    break;
                default :
                    break;
            }
        }
    }
    return 0;
}

// EnglishComment
/*****************************************************************************/
/*  ID   = DFARASCmp                                                         */
/*                                                                           */
/*  name = Comparing the file taking orders                                  */
/*  func = qsort() invokes this function to determine which RAS file should  */
/*         be taken first.                                                   */
/*                                                                           */
/*  i/o  = pP1 : const void * :inp:                                          */
/*         pP2 : const void * :inp:                                          */
/*                                                                           */
/*  return =  < 0: (left hand side < right hand side)                        */
/*            = 0: (left hand side = right hand side)                        */
/*            > 0: (left hand side > right hand side)                        */
/*  common = none                                                            */
/*                                                                           */
/*  note = none                                                              */
/*  date =  September 4, 1995  by Masakazu Mitsuhashi                        */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
// EnglishComment
int DFARASCmp(const void *pP1,const void *pP2)
{
    return ((((struct dfaras_t *)pP1)->iChkNum - ((struct dfaras_t *)pP2)->iChkNum));
}
