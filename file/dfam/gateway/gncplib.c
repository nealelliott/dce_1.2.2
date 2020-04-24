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
 * $Log: gncplib.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:37  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:52  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:06:44  takano
 * 	First Submit
 * 	[1996/02/21  08:06:39  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <ncpext.h>
#include <malloc.h>
#include <process.h>

#include "dfamdef.h"
#include "dfamext.h"
#include "dfamstr.h"
#include "dfamprt.h"


/*EnglishComment**************************************************************/
/*  ID   = AddTGrpID                                                         */
/*  name = User Thread Group Table processing routine                        */
/*  func = Makes a User Thread Group Table corresponding to a connect number.*/
/*         (1) Makes a user thread group table.                              */
/*         (2) Initializes the user thread group table.                      */
/*                                                                           */
/*  i/o  = uiConNum: LONG                : inp : connection number           */
/*         pTGroup : struct GTGroup_t ** : out : created User Thread Group   */
/*                                               Table                       */
/*                                                                           */
/*  return = -3 : ResumeAllUserThread() is called (unload or invalid Admin   */
/*                Connection).                                               */
/*           -2 : there is a user thread group table that has a same         */
/*                connection number.  Or DCE login is being processed.       */
/*           -1 : failed to get memory.                                      */
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


int AddTGrpID(uiConNum, pTGroup)
LONG uiConNum;
struct GTGroup_t **pTGroup;
{
    struct GTGroup_t *pGroupWp;
	struct GTGroup_t *pGroupPre;
    struct GTGroup_t *pGroupLoopP;
    void *pTrace;

    GW_tsh_functrace(FUNC_ADDTGRPID, GW_FUNC_START, 0);

    pGroupWp = (struct GTGroup_t *)GW_malloc(sizeof(struct GTGroup_t));

    if (pGroupWp == (struct GTGroup_t *)NULL) {
        GW_tsh_functrace(FUNC_ADDTGRPID, GW_FUNC_END, -1);
        return -1;
    }

    pTrace = GW_malloc(TRACEAREASIZE);
    if (pTrace == NULL) {
        GW_free(pGroupWp, 0);
        GW_tsh_functrace(FUNC_ADDTGRPID, GW_FUNC_END, -1);
        return -1;
    }

    pGroupWp->uiConnNum         = uiConNum;
    pGroupWp->iTGrpID           = 0;
    pGroupWp->iThreadID         = 0;
    pGroupWp->iReqThreadID      = 0;
    pGroupWp->iExistThread      = 0;
    pGroupWp->iExistCommand     = 0;
    pGroupWp->iLogoutStatus     = 0;
    pGroupWp->uiCConn_sh        = (LONG)0;
    pGroupWp->uiFHandle_sh      = (LONG)0;
    pGroupWp->iLoginStatus      = 1;
    pGroupWp->iResumeThreadMode = 0;
    pGroupWp->iSockFd           = 0;
    pGroupWp->pTblKeep_sp       = (struct TableKeep *)NULL;
    pGroupWp->pOFile_sp         = (struct OpenFile_t *)NULL;
    pGroupWp->pTsh_Trace        = pTrace;
    pGroupWp->pLgin             = (struct LginReturn_t *)NULL;
    pGroupWp->pNext             = (struct GTGroup_t *)NULL;

    if (iResumeAll != 0) {
        GW_free(pTrace, 0);
        GW_free(pGroupWp, 0);
        GW_tsh_functrace(FUNC_ADDTGRPID, GW_FUNC_END, -3);
        return -3;
    }

    if (pTGroup_sp == (struct GTGroup_t *)NULL) {
        pTGroup_sp = pGroupWp;
    }
    else {
        pGroupLoopP = pTGroup_sp;
		pGroupPre = pTGroup_sp;
        do {
            if (pGroupLoopP->uiConnNum == uiConNum) {
                GW_free(pTrace, 0);
                GW_free(pGroupWp, 0);
                GW_tsh_functrace(FUNC_ADDTGRPID, GW_FUNC_END, -2);
                return -2;
            }
			pGroupPre = pGroupLoopP;
           	pGroupLoopP = pGroupLoopP->pNext;
        } while (pGroupLoopP != (struct GTGroup_t *)NULL);

        pGroupPre->pNext = pGroupWp;
    }

    *pTGroup = pGroupWp;
    GW_tsh_functrace(FUNC_ADDTGRPID, GW_FUNC_END, 0);
    return (0);
}


/*EnglishComment**************************************************************/
/*  ID   = DelTGrpID                                                         */
/*  name = User Thread Group Table deletion routine                          */
/*  func = Deletes the User Thread Group Table corresponding to a connect    */
/*         number.                                                           */
/*                                                                           */
/*  i/o  = uiConNum: LONG                : inp : connection number           */
/*                                                                           */
/*  return = -1 : there in no user thread group table corresponding to the   */
/*                connection numer.                                          */
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


int DelTGrpID(uiConNum)
LONG uiConNum;
{
    struct GTGroup_t *pGroupWp;
    struct GTGroup_t *pGroupPre;
    int	bExist;


    GW_tsh_functrace(FUNC_DELTGRPID, GW_FUNC_START, 0);
    bExist = 0;

    if (pTGroup_sp == (struct GTGroup_t *)NULL) {
        GW_tsh_functrace(FUNC_DELTGRPID, GW_FUNC_END, -1);
        return -1;
    }
    else {
        pGroupWp = pTGroup_sp;
        pGroupPre = pTGroup_sp;
        do {
            if (pGroupWp->uiConnNum == uiConNum) {

                bExist = 1;
                break;
            }
            pGroupPre = pGroupWp;
            pGroupWp = pGroupWp->pNext;
        } while(pGroupWp != (struct GTGroup_t *)NULL);

        if(bExist == 0) {
            GW_tsh_functrace(FUNC_DELTGRPID, GW_FUNC_END, -1);
            return -1;
        }

        if (pGroupWp == pTGroup_sp) {
            pTGroup_sp = pTGroup_sp->pNext;
        }
        else {
            pGroupPre->pNext = pGroupWp->pNext;
        }

        GW_free(pGroupWp->pTsh_Trace, 0);
        GW_free((struct GTGroup_t *)pGroupWp, 0);
        GW_tsh_functrace(FUNC_DELTGRPID, GW_FUNC_END, 0);
        return 0;
    }
}


/*EnglishComment**************************************************************/
/*  ID   = GetTGrpID                                                         */
/*  name = User Thread Group Table getting routine                           */
/*  func = Gets a User Thread Group Table corresponding to a connect number. */
/*         (1) Makes a user thread group table.                              */
/*         (2) Initializes the user thread group table.                      */
/*                                                                           */
/*  i/o  = uiConNum: LONG                : inp : connection number           */
/*                                                                           */
/*  return = NULL : there in no user thread group table corresponding to the */
/*                  connection number.                                       */
/*           pointer to user thread group table: normal end                  */
/*                                                                           */
/*  common = none                                                            */
/*  note   = This routine may be invoked even if there is no context.  Thus  */
/*           the blocking API cannot be issued (no function trace, either).  */
/*                                                                           */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/


struct GTGroup_t *GetTGrp(uiConNum)
LONG uiConNum;
{
    struct GTGroup_t *pTGroup_wp;

    if (pTGroup_sp == (struct GTGroup_t *)NULL) {
        return ((struct GTGroup_t *)NULL);
    }
    else {
        pTGroup_wp = pTGroup_sp;
        do {
            if (pTGroup_wp->uiConnNum == uiConNum) {
                return (pTGroup_wp);
            }
            pTGroup_wp = pTGroup_wp->pNext;
        } while (pTGroup_wp != (struct GTGroup_t *)NULL);

        return ((struct GTGroup_t *)NULL);
    }
}
