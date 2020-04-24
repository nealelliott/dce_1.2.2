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
 * $Log: dcommain.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:20  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:48  marty]
 *
 * Revision 1.1.2.1  1996/02/21  06:45:34  takano
 * 	First Submit
 * 	[1996/02/21  06:44:49  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <process.h>
#include <errno.h>
#include <conio.h>
#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "dceapi.h"

static char aszDCEAPI_Ver[] = "01.01";
char aszDFAMServName[] = "dfa";
char aszDFAMProtcol[] = "tcp";
unsigned short ushDFAMGWTranceVer=0x0200;
char aszEyeChach[DCOMEYEC+1] = "DCOM";
int idfadceNLMID;
int idfadceTGID;
BOOL bComTermFlag;

char *pszResumeMainT         = "ResumeMainT          ";

char *pszAgtConnectReq       = "AgtConnectReq        ";
char *pszAgentDisConnectReq  = "AgentDisConnectReq   ";
char *pszConnectDiscrimReq   = "ConnectDiscrimReq    ";
char *pszDustRecvReq         = "DustRecvReq          ";
char *pszAgtSendHedReq       = "AgtSendHedReq        ";
char *pszAgtSendReq          = "AgtSendReq           ";
char *pszAgtRecvHedReq       = "AgtRecvHedReq        ";
char *pszAgtRecvDefData      = "AgtRecvDefData       ";
char *pszAgtRecvReq          = "AgtRecvReq           ";
char *pszWatchDogStatRecvReq = "WatchDogStatRecvReq  ";


char *pszDFSopenReq          = "DFSopenReq           ";
char *pszDFScloseReq         = "DFScloseReq          ";
char *pszDFScreatReq         = "DFScreatReq          ";
char *pszDFSremoveReq        = "DFSremoveReq         ";
char *pszDFSrenameReq        = "DFSrenameReq         ";
char *pszDFSreadReq          = "DFSreadReq           ";
char *pszDFSwriteReq         = "DFSwriteReq          ";
char *pszDFSmkdirReq         = "DFSmkdirReq          ";
char *pszDFSrmdirReq         = "DFSrmdirReq          ";
char *pszDFSopendirReq       = "DFSopendirReq        ";
char *pszDFSreadstatReq      = "DFSreadstatReq       ";
char *pszDFSclosedirReq      = "DFSclosedirReq       ";
char *pszDFSutimeReq         = "DFSutimeReq          ";
char *pszDFSchownReq         = "DFSchownReq          ";
char *pszDFSstatReq          = "DFSstatReq           ";
char *pszDFSCpFirstReadReq   = "DFSCpFirstReadReq    ";
char *pszDFSCpMiddleReadReq  = "DFSCpMiddleReadReq   ";
char *pszDFSCpLastReadReq    = "DFSCpLastReadReq     ";
char *pszDFSCpbkFirstWriteReq= "DFSCpFirstWriteReq   ";
char *pszDFSCpbkMiddleWriteReq="DFSCpMiddleWriteReq  ";
char *pszDFSCpbkLastWriteReq = "DFSCpLastWriteReq    ";

char *pszDFSdirsyncReq       = "DFSdirsyncReq        ";

char *pszDFStimeReq          = "DFStimeReq           ";

char *pszDFSgetacl_syscallReq= "DFSgetacl_syscallReq ";
char *pszDFSsetacl_syscallReq= "DFSsetacl_syscallReq ";
char *pszDFSdelacl_syscallReq= "DFSdelacl_syscallReq ";
char *pszDFSrightsReq        = "DFSrightsReq         ";

char *pszDFSLinGetIdReq      = "DFSLinGetIdReq       ";
char *pszDFSLinGetPasswdReq  = "DFSLinGetPasswdReq   ";
char *pszDFSLinResetKeyReq   = "DFSLinResetKeyReq    ";
char *pszDFSLoutReq          = "DFSLoutReq           ";

char *pszDFSlseekReq         = "DFSlseekReq          ";

// EnglishComment
/*****************************************************************************/
/*  ID   =  main                                                             */
/*  name =  DFADCE.NLM initiation routine                                    */
/*  func =  Does the resident processing for DFADCE.NLM                      */
/*          1. Registers callback routines for unloading.                    */
/*          2. Resident processing                                           */
/*  i/o  = argc : int     :inp: number of parameters                         */
/*         argv : char ** :inp: parameter string                             */
/*  return = ESUCCESS : resident processing successful                       */
/*           EFAILURE : resident processing failed                           */
/*  common = idfadceNLMID  : int           :out: NLM ID                      */
/*           aszDFADCE_Ver : BYTE *        :out: DFAMDCE.NLM version         */
/*           aszDCEAPI_Ver : static char * :inp: DFADCE.NLM internal version */
/*  note   = none                                                            */
/*  date   = Jan. 09, 1995 by Masakazu Mitsuhashi                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
#ifdef DCELOG
void main(int argc,char **argv)
#else
void main(void)
#endif
{
    void (*pCode)(int);
#ifdef DCELOG
    int iOptFlag;
#endif

    idfadceNLMID = GetNLMID();
    idfadceTGID = GetThreadGroupID();
    bComTermFlag = FALSE;
    pCode = signal(SIGTERM,fnDCEAPICleanUp);

    if (pCode == SIG_ERR){
        bComTermFlag = TRUE;
        ExitThread(EXIT_NLM,EFAILURE);
    }

    strcpy(aszDFADCE_Ver,aszDCEAPI_Ver);

    if (EnterCritSec()){
        bComTermFlag = TRUE;
        ExitThread(EXIT_NLM,EFAILURE);
    }
    DCEMemInit();

#ifdef DCELOG
    iOptFlag = DCELogOptChk(argc,argv);
    if (iOptFlag){
        DCELogInit(FALSE);
    } else {
        DCELogInit(TRUE);
    }
#endif

    if (ExitCritSec()){
        bComTermFlag = TRUE;
        ExitThread(EXIT_NLM,EFAILURE);
    }

    bComTermFlag = TRUE;
    ExitThread(TSR_THREAD,ESUCCESS);
}

// EnglishComment
/*****************************************************************************/
/*  ID   =  fnDCEAPICleanUp                                                  */
/*  name =  Library termination routine                                      */
/*  func =  Frees a resident library.                                        */
/*          1. Frees the registration of a library.                          */
/*  i/o  = iSignalCode : int :inp: signal code                               */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Jan. 09, 1995 by Masakazu Mitsuhashi                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void fnDCEAPICleanUp(int iSignalCode)
{

    iSignalCode = iSignalCode;

    SetThreadGroupID(idfadceTGID);
    SetNLMID(idfadceNLMID);

    while (bComTermFlag == FALSE){
        delay(SHORTWAITTIME);
        ThreadSwitch();
    }

    DCEMemTerm();
#ifdef DCELOG
    DCELogTerm();
#endif

}

// EnglishComment
/*****************************************************************************/
/*  ID   = ResumeMainT                                                       */
/*  name = Main thread resume routine                                        */
/*  func = Resumes the main thread.                                          */
/*  i/o  = none                                                              */
/*  return = none                                                            */
/*  common = iUnloadTcpipFlag : int :i/o: TCP/IP unload flag                 */
/*           iSystem_ccode    : int :out: system condition code              */
/*           iMain_tid        : int :inp: MainThread thread ID               */
/*  note   = none                                                            */
/*  date   = Jan. 09, 1995 by Masakazu Mitsuhashi                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.   Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void ResumeMainT(void)
{
#ifdef DCETRC
    GW_tsh_functrace(pszResumeMainT,GW_FUNC_START,0);
#endif
    switch(iUnloadTcpipFlag){
        case LOAD_TCPIP :
            iUnloadTcpipFlag = UNLOAD_TCPIP;

            iSystem_ccode = GWERR_ABEND;
            ResumeThread(iMain_tid);


            break;
        case UNLOAD_TCPIP :

            break;
        default :

            break;
    }
#ifdef DCETRC
    GW_tsh_functrace(pszResumeMainT,GW_FUNC_END,1);
#endif

}
