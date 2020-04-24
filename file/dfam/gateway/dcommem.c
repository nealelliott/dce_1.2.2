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
 * $Log: dcommem.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:21  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:49  marty]
 *
 * Revision 1.1.2.1  1996/02/21  06:46:02  takano
 * 	First Submit
 * 	[1996/02/21  06:45:52  takano]
 * 
 * $EndLog$
 */
#include <malloc.h>
#include <string.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "dceapi.h"

#define LOGRECTBLSIZE               (128*1024)


#define LOGRECTBLEYECATCHSIZE       8
#define LOGRECTBLTERMEYECATCHSIZE   16

static char *pcLogRecHeadTbl;
static char *pcLogRecTbl;
static char aszEyeCatch[LOGRECTBLEYECATCHSIZE+1] = "*DFADCE*";

static char aszDataTermEyeCatch[LOGRECTBLTERMEYECATCHSIZE+1] = "****************";

static long lComLogMemCtr;

// EnglishComment
/*****************************************************************************/
/*  ID   = DCEMemInit                                                        */
/*  name = Memory initialization function for DFADCE.NLM                     */
/*  func = 1. Allocates memory.                                              */
/*         2. Copies eye catchers on the top and bottom.                     */
/*         3. Copies an eye catcher of the logging starting location.        */
/*  i/o  = :::                                                               */
/*  return = 0 : normal end                                                  */
/*           1 : abnormal end (memory assignment error)                      */
/*                                                                           */
/*  common = lComLogMemCtr      :long  :out: outputted data bytes of         */
/*                                           the communication logging       */
/*           pcLogRecHeadTbl    :char *:out: the starting location of        */
/*                                           the communication logging       */
/*           pcLogRecTbl        :char *:out: the current location of         */
/*                                           the communication logging       */
/*           aszEyeCatch        :char *:inp: eye catchers set on the top     */
/*                                           and bottom                      */
/*           aszDataTermEyeCatch:char *:inp: an eye catcher at the end of    */
/*                                           the data                        */
/*  note = none                                                              */
/*  date = August 4, 1995  by Masakazu Mitsuhashi                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
// EnglishComment
int DCEMemInit(void)
{
    int iRet;


    iRet = 0;
    lComLogMemCtr = 0;
    pcLogRecTbl = NULL;

    pcLogRecHeadTbl = (char *)calloc(LOGRECTBLSIZE+LOGRECTBLEYECATCHSIZE*2,sizeof(char));
    if (!pcLogRecHeadTbl){
        iRet = 1;
    } else {
        memcpy(pcLogRecHeadTbl,aszEyeCatch,LOGRECTBLEYECATCHSIZE);
        pcLogRecTbl = pcLogRecHeadTbl + LOGRECTBLEYECATCHSIZE;
        memcpy(pcLogRecTbl+LOGRECTBLSIZE,aszEyeCatch,LOGRECTBLEYECATCHSIZE);
        memcpy(pcLogRecTbl,aszDataTermEyeCatch,LOGRECTBLTERMEYECATCHSIZE);
    }
    return (iRet);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = DCEMemTerm                                                        */
/*  name = Memory termination function for DFADCE.NLM                        */
/*  func = Frees the memory assigned for the communication logging.          */
/*  i/o  = :::                                                               */
/*  return = 0 : normal end                                                  */
/*           1 : abnormal end (removes eye catchers if running with          */
/*                             debugging mode)                               */
/*                                                                           */
/*  common = pcLogRecHeadTbl    :char *:out: the starting location of        */
/*                                           the communication logging       */
/*           pcLogRecTbl        :char *:out: the current location of         */
/*                                           the communication logging       */
/*           aszEyeCatch        :char *:inp: eye catchers set on the top     */
/*                                           and bottom                      */
/*  note = none                                                              */
/*  date = August 4, 1995  by Masakazu Mitsuhashi                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
// EnglishComment
int DCEMemTerm(void)
{
    int iRet;


    iRet = 0;

    if (pcLogRecHeadTbl){
        free(pcLogRecHeadTbl);
        pcLogRecHeadTbl = NULL;
        pcLogRecTbl = NULL;
    }
    return(iRet);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = DCEMemCpLog                                                       */
/*  name = Copying the DFADCE.NLM communication logging to memory            */
/*  func = 1. Copies data to memory.                                         */
/*         2. Copies an eye catcher on the end of the data.                  */
/*                                                                           */
/*  i/o  = pcData: char *:inp: data to be copied to memory                   */
/*         lSize : long  :inp: data length                                   */
/*                                                                           */
/*  return = 0 : normal end                                                  */
/*                                                                           */
/*  common = pcLogRecHeadTbl    :char *:inp: the starting location of        */
/*                                           the communication logging       */
/*           pcLogRecTbl        :char *:i/o: the current location of         */
/*                                           the communication logging       */
/*           lComLogMemCtr      :long  :i/o: outputted data bytes of         */
/*                                           the communication logging       */
/*           aszDataTermEyeCatch:char *:inp: an eye catcher at the end of    */
/*                                           the data                        */
/*  note = none                                                              */
/*  date = August 4, 1995  by Masakazu Mitsuhashi                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
// EnglishComment
int DCEMemCpComLog(char *pcData,long lSize)
{
    if (pcLogRecHeadTbl){
        if (lComLogMemCtr + lSize < LOGRECTBLSIZE){
            memcpy(pcLogRecTbl,pcData,(int)lSize);
            lComLogMemCtr += lSize;
            pcLogRecTbl += lSize;
        } else if (lComLogMemCtr + lSize == LOGRECTBLSIZE){
            memcpy(pcLogRecTbl,pcData,(int)lSize);
            lComLogMemCtr = 0;
            pcLogRecTbl = pcLogRecHeadTbl + LOGRECTBLEYECATCHSIZE;
        } else {
            memcpy(pcLogRecTbl,pcData,(int)(LOGRECTBLSIZE-lComLogMemCtr));
            pcLogRecTbl = pcLogRecHeadTbl + LOGRECTBLEYECATCHSIZE;
            memcpy(pcLogRecTbl,&pcData[LOGRECTBLSIZE-lComLogMemCtr],(int)(lComLogMemCtr+lSize-LOGRECTBLSIZE));
            lComLogMemCtr = lComLogMemCtr+lSize-LOGRECTBLSIZE;
            pcLogRecTbl += lComLogMemCtr;
        }

        if (lComLogMemCtr + LOGRECTBLTERMEYECATCHSIZE <= LOGRECTBLSIZE){
            memcpy(pcLogRecTbl,aszDataTermEyeCatch,LOGRECTBLTERMEYECATCHSIZE);
        } else {
            memcpy(pcLogRecTbl,aszDataTermEyeCatch,(int)(LOGRECTBLSIZE-lComLogMemCtr));
            memcpy(pcLogRecHeadTbl+LOGRECTBLEYECATCHSIZE,aszDataTermEyeCatch,
                    (int)(lComLogMemCtr+LOGRECTBLTERMEYECATCHSIZE-LOGRECTBLSIZE));
        }
    }
    return 0;
}
