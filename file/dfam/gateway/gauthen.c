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
 * $Log: gauthen.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:42  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:07  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:51:40  takano
 * 	First Submit
 * 	[1996/02/21  06:48:03  takano]
 * 
 * $EndLog$
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<io.h>
#include<sys\types.h>
#include<time.h>
#include<nwtypes.h>
#include<nwsemaph.h>
#include<nwbindry.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "dfammsg.h"




/*EnglishComment************************************************************/
/*  ID   = DCE_Login                                                       */
/*  name = DCE login module (DFAM Gateway)                                 */
/*  func = DCE login                                                       */
/*                                                                         */
/*  i/o  = pComTbl  : void *:i   : user thread group table                 */
/*     pszUid   : BYTE *:i   : DCE user ID                                 */
/*     pszPasswd: BYTE *:i   : DCE password                                */
/*     iErrno   : int  *:o   : a pointer to the error number (in           */
/*                                 error case)                             */
/*                                                                         */
/*  return =  0 : DCE login successfully completed                         */
/*       -1 : DCE login failed                                             */
/*  common = none                                                          */
/*  note   = none                                                          */
/*                                                                         */
/*  data = Dec. 26, 1994 by Masato Arai                                    */
/*                                                                         */
/* Trade Secret of Hitachi, Ltd.                                           */
/* Do not disclose without written concession from Hitachi, Ltd.           */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.*/
/*EnglishComment************************************************************/
int DCE_Login(void *pComTbl, BYTE *pszUid, BYTE *pszPasswd, int *iErrno)
{
	int		iCcode;
	int		iRc;
	int		iErr;
	short 	tmLen1;
	short	tmLen2;
	short	tmEnc_len;
	short	tmSend_len;
	short	shSwint;
	LONG	uiSwlong;
	LONG	auiMkey[2];
	LONG	auiRkey1[2];
	LONG	auiRkey2[2];
	long	iStatus;
	BYTE	*pbySend_data;
	BYTE	*pbyRecv_data;
	BYTE	*pbyPdata;
	BYTE	*pbyEdata;

	iRc = 0;
	*iErrno = 0;

	pbySend_data = (BYTE *)malloc(BUFF_SIZE);
	if(pbySend_data == (BYTE *)NULL)
	{
		*iErrno = DENOMEM;
		return(-1);
	}
	pbyRecv_data = (BYTE *)malloc(BUFF_SIZE);
	if(pbyRecv_data == (BYTE *)NULL)
	{
		free(pbySend_data);
		*iErrno = DENOMEM;
		return(-1);
	}
	pbyPdata = (BYTE *)malloc(BUFF_SIZE);
	if(pbyPdata == (BYTE *)NULL)
	{
		free(pbySend_data);
		free(pbyRecv_data);
		*iErrno = DENOMEM;
		return(-1);
	}
	pbyEdata = (BYTE *)malloc(BUFF_SIZE);
	if(pbyEdata == (BYTE *)NULL)
	{
		free(pbySend_data);
		free(pbyRecv_data);
		free(pbyPdata);
		*iErrno = DENOMEM;
		return(-1);
	}


	WaitOnLocalSemaphore(uiKeyFile_sh);

	iCcode = fnGetMasterKey( pAgent_sp->aszAgentName, auiMkey );
	if ( iCcode != 0 )
	{
		SignalLocalSemaphore(uiKeyFile_sh);
		*iErrno = iCcode;
		iRc = -1;
		goto lbExitp;
	}

	tmLen2 = strlen(pszUid) + 8;
	fnCreatRandKey( auiRkey1 );

	memset(pbyPdata, '\0', BUFF_SIZE);
	memset(pbyEdata, '\0', BUFF_SIZE);
	memcpy(pbyPdata, pszUid, strlen(pszUid));
	uiSwlong = fnLongSwap(auiRkey1[0]);
	memcpy(pbyPdata+strlen(pszUid), (BYTE *)&uiSwlong, 4);
	uiSwlong = fnLongSwap(auiRkey1[1]);
	memcpy(pbyPdata+strlen(pszUid)+4, (BYTE *)&uiSwlong, 4);

	tmEnc_len = tmLen2;

	fnSetEncryptData( auiMkey, pbyPdata, tmLen2, pbyEdata );

	memset(pbySend_data, '\0', BUFF_SIZE);
	swab( (BYTE *)&tmLen2, (BYTE *)&shSwint, 2);
	memcpy(pbySend_data, (BYTE *)&shSwint, 2);
	memcpy(pbySend_data + sizeof(short), pbyEdata, tmEnc_len);
	tmSend_len = tmEnc_len + sizeof(short);

	iStatus = DFSLinGetIdReq(	pbySend_data,
								(long)tmSend_len,
								pbyRecv_data,
								&iErr,
								pComTbl  );

	SignalLocalSemaphore(uiKeyFile_sh);

	if ( iStatus != 0L )
	{
		*iErrno = iErr;
		iRc = -1;
		goto lbExitp;
	}

	memcpy(pbyEdata, pbyRecv_data, 16);
	fnGetDecryptData( auiRkey1, pbyEdata, 16, pbyPdata );

	memset(pbyRecv_data, '\0', BUFF_SIZE);

	memcpy( pbyPdata, pbyPdata+8, 8 );
	memcpy( (BYTE *)&uiSwlong, pbyPdata, 4 );
	auiRkey2[0] = fnLongSwap(uiSwlong);
	memcpy( (BYTE *)&uiSwlong, pbyPdata+4, 4 );
	auiRkey2[1] = fnLongSwap(uiSwlong);

	memset(pbyPdata+8, '\0', BUFF_SIZE-8);

	tmLen1 = strlen(pszPasswd);
	swab( (BYTE *)&tmLen1, (BYTE *)&shSwint, 2);
	memcpy(pbyPdata+8, (BYTE *)&shSwint, 2);
	memcpy(pbyPdata+10, pszPasswd, tmLen1);

	tmLen2 = tmLen1 + 10;

	tmEnc_len = tmLen2;

	fnSetEncryptData( auiRkey2, pbyPdata, tmLen2, pbyEdata );

	iStatus = DFSLinGetPasswdReq( pbyEdata,
 								 (long)tmEnc_len,
								  &iErr,
								  pComTbl );

	if ( iStatus != 0L )
	{
		*iErrno = iErr;
		iRc = -1;
		goto lbExitp;
	}

	WaitOnLocalSemaphore(uiKeyFile_sh);
	iCcode = fnSetMasterKey(pAgent_sp->aszAgentName, auiRkey1);
	if ( iCcode != 0 )
	{
		SignalLocalSemaphore(uiKeyFile_sh);
		*iErrno = iCcode;
		iRc = -1;
		goto lbExitp;
	}

	iStatus = DFSLinResetKeyReq( &iErr, pComTbl );

	if ( iStatus != 0L )
	{
		iCcode = fnSetMasterKey(pAgent_sp->aszAgentName, auiMkey);

		if ( iCcode != 0 )
		{
			*iErrno = iCcode;
		}
		else
		{
			*iErrno = iErr;
		}

		SignalLocalSemaphore(uiKeyFile_sh);
		iRc = -1;
		goto lbExitp;
	}
	SignalLocalSemaphore(uiKeyFile_sh);

lbExitp:
	free(pbySend_data);
	free(pbyRecv_data);
	free(pbyPdata);
	free(pbyEdata);

	return(iRc);
}

/*EnglishComment*************************************************************/
/*  ID   = fnGetMasterKey                                                   */
/*  name = master key read routinw                                          */
/*  func = reads a master key from the DFAMAGT.CFG file.                    */
/*  i/o  = pszAgent: BYTE *:i   : DFAM Agent name                           */
/*         auiMkey : LONG  :o   : the string to register master keys        */
/*                                                                          */
/*  return =        0 : normal                                              */
/*           DENOKEY  : master key is not registered, or Agent is not       */
/*                        registered.                                       */
/*           DEKEYSIZE: master key length is invalid.                       */
/*           DENOKFILE: cannot open the DFAMAGT.CFG file.                   */
/*                                                                          */
/*  common = AGTFILENAME = "SYS:SYSTEM\\DFAMAGT.CFG                         */
/*  note   = none                                                           */
/*  data = Dec. 26, 1994 by Masato Arai                                     */
/*                                                                          */
/* Trade Secret of Hitachi, Ltd.                                            */
/* Do not disclose without written concession from Hitachi, Ltd.            */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*EnglishComment*************************************************************/
int fnGetMasterKey( pszAgent, auiMkey )
BYTE *pszAgent;
LONG auiMkey[2];
{
	int		iCcode;
	int		iAgtfind;
	FILE	*pKfile;
	BYTE	*pszLbuf;
	BYTE	aszLine[LINE_MAX+1];
	BYTE	aszStr1[20];
	BYTE	aszStr2[20];
	size_t	tmName_len;

	iCcode = 0;
	iAgtfind = 0;

	tmName_len = (size_t)strlen(pszAgent);
	pKfile = fopen(AGTFILENAME, "r");
	if ( pKfile == (FILE *)NULL )
	{
		iCcode = DENOKFILE;
		return(iCcode);
	}

	pszLbuf = fgets( aszLine, LINE_MAX+1, pKfile);
	if ( pszLbuf == NULL )
	{
		goto errorout;
	}

	for ( ; ; )
	{
		pszLbuf = fgets( aszLine, LINE_MAX+1, pKfile);

		if ( pszLbuf == NULL )
		{
			break;
		}

		if ( strlen(aszLine) >= LINE_MAX )
		{
			goto errorout;
		}

		if ( iAgtfind )
		{
			sscanf(aszLine, "%s %s", aszStr1, aszStr2);
			if( ( strlen(aszStr1) != 8 ) ||
				( strlen(aszStr2) != 8 ) 	)
			{
				iCcode = DEKEYSIZE;
				break;
			}
			sscanf(aszLine, "%lx %lx",&auiMkey[0], &auiMkey[1]);
			break;
		}

		if ( !strnicmp(aszLine, pszAgent, tmName_len) )
		{
			iAgtfind = 1;
		}
	}

errorout:
	if ( !iAgtfind )
	{
		iCcode = DENOKEY;
	}

	fclose(pKfile);
	return(iCcode);
}

/*EnglishComment*************************************************************/
/*  ID   = fnSetMasterKey                                                   */
/*  name = master key modification routine                                  */
/*  func = writes a master key to the DFAMAGT.CFG file.                     */
/*  i/o  = pszAgent: BYTE *:i  : DFAM Agent name                            */
/*         auiMkey : LONG  :i  : the string where the master key is stored  */
/*                                                                          */
/*  return =  0         : normal                                            */
/*           DENOKFILE  : DFAMAGT.CFG file does not exist.                  */
/*           DEUPDATEKEY: failed to write the master key.                   */
/*           -1         : failed to write (modify) the master key.          */
/*                                                                          */
/*  common = AGTFILENAME = "SYS:SYSTEM\\DFAMAGT.CFG                         */
/*  note   = none                                                           */
/*  data = Dec. 26, 1994 by Masato Arai                                     */
/*                                                                          */
/* Trade Secret of Hitachi, Ltd.                                            */
/* Do not disclose without written concession from Hitachi, Ltd.            */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*EnglishComment*************************************************************/
int fnSetMasterKey( pszAgent, auiMkey )
BYTE *pszAgent;
LONG auiMkey[2];
{
	int		iCcode;
	int		iAgtfind;
	FILE	*pKfile;
	FILE	*pNew;
	BYTE	*pszLbuf;
	BYTE	aszLine[LINE_MAX+1];
	size_t	tmName_len;

	iCcode = 0;
	iAgtfind = 0;

	tmName_len = (size_t)strlen(pszAgent);

	pKfile = fopen(AGTFILENAME, "r");
	if ( pKfile == (FILE *)NULL )
	{
		iCcode = DENOKFILE;
		return(iCcode);
	}

	pNew = fopen(AGTFILENAME, "rt+");
	if ( pNew == (FILE *)NULL )
	{
		iCcode = DENOKFILE;
		fclose(pKfile);
		return(iCcode);
	}

	pszLbuf = fgets( aszLine, LINE_MAX+1, pKfile);

	if ( fseek(pNew, 0L, SEEK_SET) != 0)
	{
		iCcode = DEUPDATEKEY;
		return(iCcode);
	}
	if ( pszLbuf == NULL )
	{
		goto errorout;
	}
	else if ( fputs(aszLine, pNew) == EOF )
	{
		goto errorout;
	}

	for (;;)
	{
		pszLbuf = fgets( aszLine, LINE_MAX+1, pKfile);

		if ( pszLbuf == NULL )
		{
			break;
		}

		if ( strlen(aszLine) >= LINE_MAX )
		{
			goto errorout;
		}
		if ( iAgtfind == 1 )
		{
			sprintf(aszLine, "%08lx %08lx\n",
					auiMkey[0], auiMkey[1]);
			iAgtfind = -1;
		}
		if ( !strnicmp(aszLine, pszAgent, tmName_len) )
		{
			iAgtfind = 1;
		}
		if ( EOF == fputs(aszLine, pNew) )
		{
			iCcode = DEUPDATEKEY;
		}
	}

errorout:
	if ( !iAgtfind )
	{
		sprintf(aszLine, "%s\n%08lx %08lx\n",
				pszAgent, auiMkey[0], auiMkey[1]);
		if ( EOF == fputs(aszLine, pNew) )
		{
			iCcode = DEUPDATEKEY;
		}
	}

	fclose(pKfile);
	fclose(pNew);

	return(iCcode);
}

/*EnglishComment*************************************************************/
/*  ID   = fnCreatRandKey                                                   */
/*  name = random key generation routine                                    */
/*  func = generates a random numebr, and creates an encryption key.        */
/*         Use millitm of ftime as a seed for srand (each time, the random  */
/*         number generation system is changed).                            */
/*                                                                          */
/*  i/o  = auiKey  : LONG *:i/o : the string to store the random key        */
/*  return = none                                                           */
/*  common = none                                                           */
/*  note   = none                                                           */
/*  data = Dec. 26, 1994 by Masato Arai                                     */
/*                                                                          */
/* Trade Secret of Hitachi, Ltd.                                            */
/* Do not disclose without written concession from Hitachi, Ltd.            */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*EnglishComment*************************************************************/
void fnCreatRandKey( auiKey )
LONG auiKey[2];
{
	int		i;
	WORD	ushRand1;
	WORD	ushRand0;
	BYTE	aszKeybuf[9];
	LONG	iSeed;

	iSeed = (LONG)clock();
	srand(iSeed);

	for ( i=0 ; i<=1 ; i++ )
	{
		ushRand1 = (WORD)rand() + (WORD)rand();
		ushRand0 = (WORD)rand() + (WORD)rand();

		sprintf( aszKeybuf, "%04x%04x", ushRand1, ushRand0 );
		sscanf( aszKeybuf, "%lx", &auiKey[i] );
	}

	return;
}

