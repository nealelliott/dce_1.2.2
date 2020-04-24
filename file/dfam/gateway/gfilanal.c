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
 * $Log: gfilanal.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:01  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:22  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:54:48  takano
 * 	First Submit
 * 	[1996/02/21  07:54:43  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <string.h>

#include"dfamdef.h"
#include"dfamstr.h"
#include"dfamprt.h"
#include"dfamext.h"
#include "err.h"
#include "gfiler.h"

/*EnglishComment**************************************************************/
/* ID   = fnExec_Filer                                                       */
/*          :                                                                */
/* name = File analyzer                                                      */
/* func = Analyzes if the information sent from Client is a Filer command.   */
/*        If a Filer command, passes the command to an appropriate module.   */
/*        If not a Filer command, sets the command name, return value        */
/*        (FALSE), and error number (CONERR) to the Gateway send buffer,     */
/*        and sends the buffer data.                                         */
/*          :                                                                */
/* i/o  = plUtbl     :struct GTGroup_t *:inp: pointer to thread table        */
/*        iTask_num  :int               :inp: task number                    */
/*        pucS_buf   :unsigned char *   :inp: Gateway receive buffer         */
/*        ulS_length :unsigned long     :inp: size of Gateway receive buffer */
/*        pucR_buf   :unsigned char *   :i/o: Gateway send buffer            */
/*        pulR_length:unsigned long *   :i/o: size of Gateway send buffer    */
/*          :                                                                */
/* return = none                                                             */
/*          :                                                                */
/* common = none                                                             */
/*          :                                                                */
/*  date = January 30, 1995 by Jun Nakagawa                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/


void fnExec_Filer (plUtbl,iTask_num,pucS_buf,ulS_length,pucR_buf,pulR_length)
struct GTGroup_t *plUtbl;
int               iTask_num;
unsigned char    *pucS_buf;
unsigned long     ulS_length;
unsigned char    *pucR_buf;
unsigned long    *pulR_length;
{
	if(!strncmp(pucS_buf,NCPGRANT,COMMLEN))
	{
		fnDGrant(plUtbl,iTask_num,pucS_buf,ulS_length,pucR_buf,pulR_length);
	}
	else if(!strncmp(pucS_buf,NCPREVOKE,COMMLEN))
	{
		fnDRevoke(plUtbl,iTask_num,pucS_buf,ulS_length,pucR_buf,pulR_length);
	}
	else if(!strncmp(pucS_buf,NCPREMOVE,COMMLEN))
	{
		fnDRemove(plUtbl,iTask_num,pucS_buf,ulS_length,pucR_buf,pulR_length);
	}
	else if(!strncmp(pucS_buf,NCPTLIST,COMMLEN))
	{
		fnDTlist(plUtbl,iTask_num,pucS_buf,ulS_length,pucR_buf,pulR_length);
	}
	else if(!strncmp(pucS_buf,NCPRIGHT,COMMLEN))
	{
		fnDRights(plUtbl,iTask_num,pucS_buf,ulS_length,pucR_buf,pulR_length);
	}
	else if(!strncmp(pucS_buf,NCPLISTDIR,COMMLEN))
	{
		fnDListdir(plUtbl,iTask_num,pucS_buf,ulS_length,pucR_buf,pulR_length);
	}
	else if(!strncmp(pucS_buf,NCPNDIR,COMMLEN))
	{
		fnDNdir(plUtbl,iTask_num,pucS_buf,ulS_length,pucR_buf,pulR_length);
	}
	else if(!strncmp(pucS_buf,NCPCHGDATE,COMMLEN))
	{
		fnChgdate(plUtbl,iTask_num,pucS_buf,ulS_length,pucR_buf,pulR_length);
	}
	else if(!strncmp(pucS_buf,NCPCHGOWN,COMMLEN))
	{
		fnChgowner(plUtbl,iTask_num,pucS_buf,ulS_length,pucR_buf,pulR_length);
	}
	else
	{
		strncpy(((struct RetHeader *)pucR_buf)->command_name,pucS_buf,COMMLEN);
		((struct RetHeader *)pucR_buf)->ret_value = FALSE;
		((struct RetHeader *)pucR_buf)->DFAM_errno = CONERR;
		*pulR_length = sizeof(struct RetHeader);
	}
}
