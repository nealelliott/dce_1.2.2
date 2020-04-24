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
 * $Log: gfilerch.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:06  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:27  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:56:35  takano
 * 	First Submit
 * 	[1996/02/21  07:56:31  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <errno.h>
#include <nwbindry.h>
#include <niterror.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamprt.h"
#include "dfamext.h"
#include "dfamer.h"
#include "dfammsg.h"
#include "err.h"
#include "gfiler.h"



/*EnglishComment**************************************************************/
/* ID   = fnErrChange                                                        */
/*                                                                           */
/* name = Mapping from DFS error code into DFAM error code                   */
/* func = Converts the DFS error code returned from Agent into DFAM          */
/*        error code.                                                        */
/*                                                                           */
/* i/o  = DFS_Errno:int                :inp: DFS error code                  */
/*        plUtbl   :struct GTGroup_t  *:inp: pointer to user thread group    */
/*                                           table                           */
/*                                                                           */
/* return = DFAM_errno:unsigned short:out: DFAM error code                   */
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


unsigned short fnErrChange(int DFS_Errno , struct GTGroup_t *plUtbl )
{
	unsigned short 		DFAM_errno;
	struct TableKeep 	*wtablep;

	if( DFS_Errno == DENOERR )
	{
		return 0;
	}

	switch(DFS_Errno)
	{
		case(DEPERM):
							DFAM_errno = NORIGHT;
							return(DFAM_errno);
		case(DENOENT):
							DFAM_errno = PATHERR;
							return(DFAM_errno);
		case(DEIO):
		case(DENXIO):
		case(DETIMEDOUT):
							DFAM_errno = IOERR;
							break;
		case(DENOMEM):
							DFAM_errno = GWMEMERR;
							break;
		case(DEACCES):
							DFAM_errno = NORIGHT;
							return(DFAM_errno);
		case(DEFAULT):
							DFAM_errno = ABNORMAL;
							break;
		case(DENOTDIR):
							DFAM_errno = PATHERR;
							return(DFAM_errno);
		case(DEINVAL):
							DFAM_errno = ABNORMAL;
							break;
		case(DEMFILE):
							DFAM_errno = INTERE;
							break;
		case(DEFBIG):
							DFAM_errno = FSYSBIGERR;
							break;
		case(DENOSPC):
		case(DEDQUOT):
							DFAM_errno = EDSPC;
							break;
		case(DESPIPE):
							DFAM_errno = ABNORMAL;
							break;
		case(DEROFS):
							DFAM_errno = ROFSERR;
							break;
		case(DERANGE):
							DFAM_errno = RANGEERR;
							break;
		case(DEOPNOTSUPP):
							DFAM_errno = ENOTLFS;
							break;
		case(DENAMETOOLONG):
							DFAM_errno = TOOLONG;
							break;
		case(DELOOP):
							DFAM_errno = LOOPERR;
							break;
		case(DECOM):
							DFAM_errno = ABNORMAL;
							break;
		case(DENOTDEL):
							DFAM_errno = NOTDEL;
							return(DFAM_errno);
		case(DEUNAVAILSRV):
		case(DEUNKNOWN):
							DFAM_errno = EUNAVAILSRV;
							break;
		case(DEUNNOWNPRI):
							DFAM_errno = NODCEENT;
							break;
		case(DENOMASK):
							DFAM_errno = NORIGHT;
							return(DFAM_errno);
		case(DENOACLPRI):
							DFAM_errno = NOTRLST;
							break;
		case(DEREFUSEAGT):
		case(DERESPONCEDATA):
		case(DEUNLOADTCPIP):
		case(DEINVALSOCKET):
		case(DETIMEOUTAGT):
							wtablep = plUtbl->pTblKeep_sp;
							wtablep->agterr = DFS_Errno;
							DFAM_errno = AGCONERR;
							break;
		case(DEDFAMTERM):
							DFAM_errno = ERRBUSY;
							break;
		default:
							DFAM_errno = ABNORMAL;
							break;
	}
	if( DFS_Errno > 999 )
	{
		GW_tsh_log( 2000+(DFS_Errno-1000) , "AGT-ERR" );
	}
	else
	{
		GW_tsh_log( 2000+DFS_Errno , "DFS-ERR" );
	}
	return(DFAM_errno);
}



/*EnglishComment**************************************************************/
/* ID   = fnNW_change_errno                                                  */
/*                                                                           */
/* name = Mapping from NetWare error code into DFAM error code               */
/* func = Converts the NetWare error code into DFAM error code.              */
/*                                                                           */
/* i/o  = NW_err_num :int              :inp: NetWare error code              */
/*                                                                           */
/* return = DFAM_errno:unsigned short  :out: DFAM error code                 */
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


unsigned short fnNW_change_errno(int NW_err_num )
{
	unsigned short DFAM_errno;

	switch(NW_err_num)
	{
		case ERR_INVALID_PATH:
					DFAM_errno = NOENT;
					break;
		case ERR_NO_FILES_FOUND:
					DFAM_errno = HEADONLY;
					break;
		default:
					DFAM_errno = APIERR;
					break;
	}
	return(DFAM_errno);
}


/*EnglishComment**************************************************************/
/* ID   = fnResumeThread                                                     */
/*                                                                           */
/* name = Handling a communication error between Gateway and Agent           */
/* func = Resumes the main thread of user thread table when there occurs     */
/*        a communication error.                                             */
/*                                                                           */
/* i/o  = plUtbl   :struct GTGroup_t  *:inp: pointer to user thread group    */
/*                                           table                           */
/*        DFS_Errno:int                :inp: DFS error code                  */
/*                                                                           */
/* return = none                                                             */
/*                                                                           */
/* common = none                                                             */
/*          :                                                                */
/* note   = none                                                             */
/*  date = May 21, 1995 by Jun Nakagawa                                      */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/


void fnResumeThread(struct GTGroup_t *plUtbl , int DFS_Errno )
{
   long 	uiObjID;
   char 	aszObjName[ MAX_NWUSERNAME ];
   char 	uname[ MAX_NWUSERNAME ];
   WORD 	usObjType;
   BYTE 	byLoginTime[7];



	if (plUtbl->iLogoutStatus == OFF)
	{
        plUtbl->iLogoutStatus = ON;
        plUtbl->iResumeThreadMode = ON;


	    GetConnectionInformation( (WORD)(plUtbl->uiConnNum), aszObjName,
								  &usObjType,  &uiObjID, byLoginTime );

    	if ( usObjType != OT_USER )
		{
			uname[0] = '\0';
    	}
		else
		{
			strcpy( uname , aszObjName );
		}

		GW_Message( DFAM_CON , 0 , LOG_YES , KDDS12002 , uname , DFS_Errno );
		GW_Message( DFAM_NWC , (int *)(&(plUtbl->uiConnNum)) , LOG_YES , KDDS12001 );

		ResumeThread( plUtbl->iThreadID );

	}
}


/*EnglishComment**************************************************************/
/* ID   = fnCommnErrLog                                                      */
/*                                                                           */
/* name = Taking log of Filer modules                                        */
/* func = When an error occurs, takes logging for error analysis.            */
/*                                                                           */
/* i/o  = pucRetbuf   :unsigned char *   :i/o: Gateway send buffer           */
/*                                                                           */
/* return = none                                                             */
/*                                                                           */
/* common = none                                                             */
/*          :                                                                */
/* note   = none                                                             */
/*  date = May 23, 1995 by Jun Nakagawa                                      */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/

void  fnCommnErrLog( unsigned char *pucRetbuf )
{
	unsigned short  	num;
	char				info[12];
	struct RetHeader    *bufp;

	bufp = (struct RetHeader *)pucRetbuf;
	num = bufp->DFAM_errno;
	memset( info , 0x00 , 12 );
	memcpy( info , bufp->command_name , COMMLEN );

	switch( num )
	{
		case NORMAL:

							return;





    	case NOENT:
		case LOCALDRIVE:
    	case ERROPT:
    	case NOUSER:
    	case NOGRP:
    	case MEMERR:
    	case NORIGHT:
    	case NULLPATH:
    	case NOVOL:
    	case NOSERVER:
    	case PATHERR:
    	case NOTDEL:
		case INVALENT:
    	case CABNORM:
    	case HEADONLY:
    	case CTHRESHLD:

							return;





    	case FAILECHG:
    	case MODEERR:
    	case ABNORMAL:
    	case GTHRESHLD:

							strcat( info , "-GW-E" );

							break;





    	case TOOLONG:
    	case APIERR:
    	case ERIGHT:
    	case FSYSBIGERR:
    	case LOOPERR:
    	case EFCREATE:
    	case IOERR:
    	case ENOTLFS:
    	case RANGEERR:
    	case EDSPC:
    	case EUNAVAILSRV:
    	case ROFSERR:
		case AGCONERR:
		case NODCEENT:
		case NOTRLST:
    	case NODFAMUSER:
    	case BINDERR:
    	case GWMEMERR:
    	case UNLOGIN:
    	case AGTDOWN:
    	case ERRBUSY:
    	case INTERE:
    	case CONERR:
    	case SYSTEM:

							strcat( info , "-OS-E" );
							break;
		default:
							if( num > 999 )
							{
								num = 999;
							}
							strcat( info , "-ETC-E" );
							break;
	}

	GW_tsh_log( 2000+num , info );
}
