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
 * $Log: gfilftbl.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:07  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:28  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:56:57  takano
 * 	First Submit
 * 	[1996/02/21  07:56:52  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "dfammsg.h"
#include "err.h"
#include "gfiler.h"


/*EnglishComment**************************************************************/
/* ID   = fnDelTask                                                          */
/*                                                                           */
/* name = Releasing task table (main routine)                                */
/* func = Searching for the task tables to be freed.                         */
/*                                                                           */
/* i/o  = plUtbl :struct GTGroup_t * :inp: pointer to user thread table      */
/*        pTable :struct TableKeep * :inp: pointer to thread table           */
/*                                                                           */
/* return = none                                                             */
/*                                                                           */
/* common = none                                                             */
/*          :                                                                */
/* note   = none                                                             */
/*  date = February 21, 1995 by Jun Nakagawa                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/


void fnDelTask (struct GTGroup_t        *plUtbl,
				struct TableKeep        *pTable)
{
	struct TableKeep    *pTaskTb;

	pTaskTb = plUtbl->pTblKeep_sp;

	if (plUtbl->iLogoutStatus == ON )
	{
		return ;
	}
	if(pTaskTb == pTable)
	{
		if(pTaskTb->pNext == NULL)
		{
			Free_Task(pTable);
			plUtbl->pTblKeep_sp = NULL;
		}

		else
		{
			plUtbl->pTblKeep_sp = pTable->pNext;
			Free_Task(pTable);
		}
	}

	else
	{
		if(	pTaskTb == 0 )
		{
			return;
		}
		while(pTaskTb->pNext != pTable)
		{
			pTaskTb = pTaskTb->pNext;
		}
		pTaskTb->pNext = pTable->pNext;
		Free_Task(pTable);
	}
}


/*EnglishComment**************************************************************/
/* ID   = Free_Task                                                          */
/*                                                                           */
/* name = Releasing task table                                               */
/* func = Frees the designated task table.                                   */
/*                                                                           */
/* i/o  = pTable :struct TableKeep * :inp: pointer to thread table           */
/*                                                                           */
/* return = none                                                             */
/*                                                                           */
/* common = none                                                             */
/*          :                                                                */
/* note   = none                                                             */
/*  date = February 21, 1995 by Jun Nakagawa                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/


void Free_Task(struct TableKeep *pTable)
{
	struct RetComData   *pKeep;
	struct RetComData   *pUpdate;

	if( pTable == 0 )
	{
		return;
	}
	pKeep = pTable->pData;
	while(pKeep != NULL)
	{
		pUpdate = pKeep->pNext;
		GW_free(pKeep,0);
		pKeep = NULL;
		pKeep = pUpdate;
	}
	GW_free(pTable,0);
	pTable = NULL;
}
