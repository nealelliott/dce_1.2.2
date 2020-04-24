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
 * $Log: gfilcvol.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:03  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:25  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:55:53  takano
 * 	First Submit
 * 	[1996/02/21  07:55:49  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <string.h>
#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "dfammsg.h"
#include "err.h"
#include "gfiler.h"


/*EnglishComment**************************************************************/
/*  ID   = fnChkvol                                                          */
/*                                                                           */
/* name = Referencing of Volume type                                         */
/* func = Checks if the Volume name designated by Client is DFS or NetWare,  */
/*        then returns the result.                                           */
/*                                                                           */
/* i/o  = pucBuf      :unsigned char *   :inp: Gateway receive buffer        */
/*        pulBuflen   :unsigned long     :inp: size of Gateway receive buffer*/
/*        pucRetbuf   :unsigned char *   :i/o: Gateway send buffer           */
/*        pulRetlength:unsigned long *   :i/o: size of Gateway send buffer   */
/*          :                                                                */
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


void fnChkvol (  unsigned char        *pucBuf ,
                 unsigned long        pulBuflen ,
                 unsigned char        *pucRetbuf ,
                 unsigned long        *pulRetlength  )
{
	struct chkvol_data	  *plChkvol;
	int				      iRet;
	struct RetHeader      *headerp;
	struct RetChkvolData  *chkDatap;
	unsigned long         worklen;

	worklen = pulBuflen;
	memset(pucRetbuf , 0x00 , *pulRetlength);

	headerp  = (struct RetHeader *)pucRetbuf;
	chkDatap = (struct RetChkvolData *)(pucRetbuf+sizeof(struct RetHeader));

	plChkvol = ( struct chkvol_data * )pucBuf;


	memmove( headerp->command_name , plChkvol->command_name , COMMLEN );

	iRet = isDFAMVolume( plChkvol->vol_name,NULL );

	switch( iRet )
	{
		case DFAM:
			headerp->DFAM_errno = NORMAL;
			headerp->ret_value  = TRUE;
			headerp->next_frame = FALSE;
			chkDatap->is_vol = DFAM;
			break;
		case NETWARE:
			headerp->DFAM_errno = NORMAL;
			headerp->ret_value  = TRUE;
			headerp->next_frame = FALSE;
			chkDatap->is_vol = NETWARE;
			break;
		default:
			headerp->DFAM_errno = CABNORM;
			headerp->ret_value  = FALSE;
			headerp->next_frame = FALSE;
			*pulRetlength = sizeof(struct RetHeader);

			fnCommnErrLog( pucRetbuf );

			return;
	}


	*pulRetlength = (sizeof(struct RetHeader))+(sizeof(struct RetChkvolData));
}
