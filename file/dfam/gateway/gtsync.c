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
 * $Log: gtsync.c,v $
 * Revision 1.1.2.2  1996/03/11  14:09:08  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:15  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:18:51  takano
 * 	First Submit
 * 	[1996/02/21  08:18:45  takano]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <nwtypes.h>
#include <nwdir.h>
#include <errno.h>
#include <niterror.h>
#include <advanced.h>
#include <malloc.h>
#include <fshooks.h>
#include <signal.h>
#include <fcntl.h>
#include <process.h>
#include <nwsemaph.h>
#include <fileengd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/utime.h>
#include <time.h>
#include <ctype.h>
#include <nwenvrn.h>
#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dceapi.h"
#include "dfamprt.h"
#include "cmnprt.h"




extern int errno;

/*EnglishComment**************************************************************/
/* ID   = Timesync                                                           */
/* name = Time synchronization                                               */
/* func = Synchronizes the time of DFAM.                                     */
/* i/o  = pCur_TG :void *:inp: the address of user thread group table        */
/* return = none                                                             */
/*                                                                           */
/* common = none                                                             */
/* note   = blocking                                                         */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/



void            Timesync(void *pCur_TG)
{
	WORD	ushYear;
	WORD	ushMonth;
	WORD	ushDay;
	WORD	ushHour;
	WORD	ushMinute;
	WORD	ushSecond;
	int		iCcode = 0;
	int		iDFSerrno = 0;
	int		iNWerrno = 0;
	time_t	time_w;
	time_t	s_time;
	time_t	e_time;
	dtime_t dfs_time;
	struct	tm		*new_time;

	if (pCur_TG == NULL)
		return;

	s_time = time(NULL);
	dfs_time = DFStimeReq(NULL,&iDFSerrno,(struct ComTGroup_t *) pCur_TG);
	if (dfs_time == -1) {
		ConvertErrno(GW_FILEERASE,iDFSerrno,&iNWerrno);
		GW_tsh_log(iNWerrno,"Timesync");
		return;
	}

	tzset();

	e_time = time(NULL);
	time_w = (LONG)dfs_time + (e_time - s_time) / 2;
	time_w = time_w - timezone;
	new_time 	= gmtime(&time_w);
	ushYear	 	= new_time->tm_year;
	ushMonth 	= new_time->tm_mon + 1;
	ushDay	 	= new_time->tm_mday;
	ushHour	 	= new_time->tm_hour;
	ushMinute	= new_time->tm_min;
	ushSecond	= new_time->tm_sec;

	iCcode = SetFileServerDateAndTime(	ushYear,
										ushMonth,
										ushDay,
										ushHour,
										ushMinute,
										ushSecond);

	if (iCcode != 0) {
		GW_tsh_log(iNWerrno,"Timesync");
	}

	return;
}
