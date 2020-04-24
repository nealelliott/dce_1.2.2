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
 * $Log: gdirmsg.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:55  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:17  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:52:23  takano
 * 	First Submit
 * 	[1996/02/21  07:52:17  takano]
 * 
 * $EndLog$
 */
#include "gdir.h"

/*********************************************************** EnglishComment **/
/* ID = GetDMLogMsg                                                          */
/* name = Conversion of log ID into message chacaters                        */
/* func = Converts the DM-based log ID into a character string.              */
/* i/o  = iDMErr  :int   :inp: log message code                              */
/* return = message characetr string                                         */
/* common = none                                                             */
/* :::                                                                       */
/* note = Messages can be any order.                                         */
/* date = December 29, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
char *GetDMLogMsg( int iDMErr )
{
	int			i;
	static char	acErrcode[8];

	static struct LogMsg	aDMLogMsg[] = {
		{DMLOGnoadmin, "no dfam administorator." },
		{DMLOGnodfamvolume, "no dfam volume is setuped." },
		{DMLOGvolumenotmapped, "Volume %s is not mapped." },
		{DMLOGvolumenotexist, "Volume %s does not exist."},
		{DMLOGvolumeinfo, "fail to get Volume %s's information."},
		{DMLOGvolumeinit, "fail to initialize Volume %s."},
		{DMLOGreflect, "fail to sync %s."},
		{DMLOGtimereq, "fail to get time from DFAM Agent."},
		{DMLOGkeepfile, "file %s is preserved." },
		{DMLOGsystemfile, "file %s is systemfile." },
		{DMLOGusingfile, "file %s is opend in Volume %s." },
		{DMLOGinitializeabort, "DFAM G/W initialization error." },
		{DMLOGconnection, "connection error." },
		{DMLOGlogin, "DCE Login failed." },
		{DMLOGlogout, "DCE Logout failed." },
		{DMLOGdisconnection, "fail to disconnection." },
		{DMLOGagentdown, "Agent is down." },
		{DMLOGdfsclosedir, "erro in DFSopandirReq()." },

		{DMLOGdeletefile, "error with deleting file %s." },
		{DMLOGdeletedir, "error with deleting directory %s." },
		{DMLOGfileinfo, "error with getting info file %s." },
		{DMLOGdirinfo, "error with getting stat directory %s." },
		{DMLOGstat, "error with getting stat %s." },
		{DMLOGcreat, "error with creating or truncating file %s." },
		{DMLOGwrite, "error with writing file %s." },
		{DMLOGseek, "error with seeking file %s." },
		{DMLOGclose, "error with closing file %s." },
		{DMLOGutime, "error with changing time stamp of file %s." },
		{DMLOGmkdir, "error with making directory %s." },
		{DMLOGgetsemhandle, "error with gettig semaphere handle." },
		{DMLOGrelsemhandle, "error with releaseing semaphere handle." },
		{DMLOGtimereq, "error with getting time of DCE." },
		{DMLOGmalloc, "memory allocation error." },

		{DMLOGbug, "bug in %s." },
		{DMLOGnullptr, "null pointer is assigned in %s." },
		{-1, NULL}
	};

	for ( i = 0; aDMLogMsg[i].iMsgCode != -1; i++ ) {
		if ( iDMErr == aDMLogMsg[i].iMsgCode ) {
			return aDMLogMsg[i].pszMsg;
		}
	}

	sprintf( acErrcode, "Err: %d\n", iDMErr );
	return acErrcode;


}
