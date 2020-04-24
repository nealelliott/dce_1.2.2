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
 * $Log: gfildtbl.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:04  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:25  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:56:13  takano
 * 	First Submit
 * 	[1996/02/21  07:56:09  takano]
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
/* ID   = fnDelAllTask                                                       */
/*                                                                           */
/* name = Releasing task table                                               */
/* func = Frees task tables chained to user thread table.                    */
/*                                                                           */
/* i/o  = plUtbl :struct GTGroup_t * :inp: pointer to thread table           */
/*                                                                           */
/* return = none                                                             */
/*                                                                           */
/* common = none                                                             */
/*          :                                                                */
/* note   = none                                                             */
/*  date = February 8, 1995 by Jun Nakagawa                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/


void fnDelAllTask (plUtbl)
struct GTGroup_t  *plUtbl;
{
	struct TableKeep        *pTaskTb;
	struct RetComData       *pKeep;
	struct RetComData       *pUpdate;
	struct TableKeep        *pTable;

	pTaskTb = plUtbl->pTblKeep_sp;

	while(pTaskTb != NULL)
	{
		pTable = pTaskTb;

		pTaskTb = pTable->pNext;

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
	plUtbl->pTblKeep_sp = NULL;
}
