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
 * $Log: gfilrmap.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:15  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:34  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:59:33  takano
 * 	First Submit
 * 	[1996/02/21  07:59:25  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <string.h>
#include <nwtypes.h>
#include <nwalias.h>
#include <time.h>

#define NW_rbit  0x0001
#define NW_wbit  0x0002
#define NW_cbit  0x0008
#define NW_ebit  0x0010
#define NW_abit  0x0020
#define NW_fbit  0x0040
#define NW_mbit  0x0080
#define NW_sbit  0x0100
#define NW_qbit  0x0200

#define NW_trustee "rwceafmsq"

static WORD value[9] = { NW_rbit,NW_wbit,NW_cbit,NW_ebit,NW_abit,NW_fbit,NW_mbit,NW_sbit,NW_qbit };
static unsigned char mozi[] =  NW_trustee;
int cl_num[] = {1,2,3,4,5,6,7,0,8};


/*EnglishComment**************************************************************/
/* ID   = fnKenriMap                                                         */
/*                                                                           */
/* name = Mapping of trustee (from numeric to character)                     */
/* func = Converts trustee (from a numeric value into a character)           */
/*                                                                           */
/* i/o  = ushKyokabit :WORD           :inp: trustee information (bit)        */
/*        ucKenri     :unsigned char *:out: trustee information (character)  */
/*                                                                           */
/* return = i         :int                :out: normal (0)                   */
/*                                                                           */
/* common = none                                                             */
/*          :                                                                */
/* note   = none                                                             */
/*  date = February 3, 1995 by Jun Nakagawa                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/


int fnKenriMap(WORD    ushKyokaBit , unsigned char    *ucKenri)
{
	int i;

	memset(ucKenri,0x00,10);

	for (i=0;i<9;i++)
	{
		if ((ushKyokaBit & value[i]) != 0)
		{
			*(ucKenri+cl_num[i]) = mozi[i];
		}
		else
		{
			*(ucKenri+cl_num[i]) = ' ';
		}
	}
	return(0);
}


/*EnglishComment**************************************************************/
/* ID   = fnKenriBitmap                                                      */
/*                                                                           */
/* name = Mapping of trustee (from character to numeric)                     */
/* func = Converts trustee (from a character into a numeric value)           */
/*                                                                           */
/* i/o  = pTlist    :unsigned char *:inp: trustee information (character)    */
/*        ucKenriBit:WORD           :out: trustee information (bit)          */
/*                                                                           */
/* return = i       :int            :out: normal (0)                         */
/*                                                                           */
/* common = none                                                             */
/*          :                                                                */
/* note   = none                                                             */
/*  date = February 3, 1995 by Jun Nakagawa                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/


int fnKenriBitmap(unsigned char   *pTlist , WORD   *ushKenriBit)
{
	int i;

	*ushKenriBit = 0;

	for (i=0 ; (i<9)&&((*(pTlist+i)) != '\0') ; i++)
	{
		switch(*(pTlist+i))
		{
			case 'r':
				*ushKenriBit = NW_rbit | *ushKenriBit;
				break;
			case 'w':
				*ushKenriBit = NW_wbit | *ushKenriBit;
				break;
			case 'c':
				*ushKenriBit = NW_cbit | *ushKenriBit;
				break;
			case 'e':
				*ushKenriBit = NW_ebit | *ushKenriBit;
				break;
			case 'a':
				*ushKenriBit = NW_abit | *ushKenriBit;
				break;
			case 'f':
				*ushKenriBit = NW_fbit | *ushKenriBit;
				break;
			case 'm':
				*ushKenriBit = NW_mbit | *ushKenriBit;
				break;
			case 'q':
				*ushKenriBit = NW_qbit | *ushKenriBit;
				break;
			case 's':
			default:
				break;
		}
	}

	return(0);
}
