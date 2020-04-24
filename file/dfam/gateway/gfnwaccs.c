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
 * $Log: gfnwaccs.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:22  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:39  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:01:22  takano
 * 	First Submit
 * 	[1996/02/21  08:01:17  takano]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <stdlib.h>
#include <nwdir.h>
#include <errno.h>
#include <niterror.h>
#include <string.h>
#include <nwtypes.h>
#include <nwsemaph.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "gfnw.h"


BYTE *pszDeviceName[] = {
	"CON",
	"PRN",
	"AUX",
	"NUL",
	"CLOCK$",
	"COM1",
	"COM2",
	"COM3",
	"COM4",
	"LPT1",
	"LPT2",
	"LPT3",
	NULL,
};



/*EnglishComment**************************************************************/
/*  ID   = EraseDFAMFile                                                     */
/*  name = File deletion routine                                             */
/*  func = Deletes the designated file.  The file can be either that of DFAM */
/*         Volume or not.  If the file is on DFAM Volume, and the file name  */
/*         is a converted one, the matched member on the name conversion     */
/*         table will be deleted.                                            */
/*  i/o  = pszFileName:char *:inp: name to be deleted                        */
/*         piErrp     :int * :out: area for errno (when an error occurred)   */
/*  return = GWERR_NORMAL(0) : normal end                                    */
/*           GWERR_ABEND (-1): error return.  See piErrp for detail.         */
/*           GWERR_BUG (-100): internal error (this function does not        */
/*                             display an error message)                     */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 22, 1994 by Isao Ichiyoshi                                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
int EraseDFAMFile ( pszFileName, piErrp )
char *pszFileName;
int  *piErrp;
{
	int		iCcode;
	struct GName_t	*pNamep;
	struct GVolume_t *pVolp;



	if ( pszFileName == NULL  || *pszFileName == '\0') {
		return( GWERR_BUG );
	};

	SetDefaultAttr( pszFileName );


	iCcode = remove( pszFileName );
	*piErrp = errno;

	if ( iCcode != 0 ) {
		return( GWERR_ABEND );
	};


	iCcode = isDFAMVolume( pszFileName, &pVolp);

	if (iCcode == YES) {
		WaitOnLocalSemaphore( pVolp->uiVolAndName_sh );


		pNamep = FindName_t(pszFileName);

		if( pNamep != NULL) {

			iCcode = AddToDelList(pNamep);

			if ( iCcode != GWERR_NORMAL ) {
				SignalLocalSemaphore( pVolp->uiVolAndName_sh );
				return (GWERR_BUG);
			};
		};
		SignalLocalSemaphore( pVolp->uiVolAndName_sh );
	};
	return(GWERR_NORMAL);
};




/*EnglishComment**************************************************************/
/*  ID   = EraseDFAMDir                                                      */
/*  name = Directory deletion routine                                        */
/*  func = Deletes the designated directory.  The directory can be either    */
/*         that of DFAM Volume or not.  If the directory is on DFAM Volume,  */
/*         and the directory name is a converted one, the matched member on  */
/*         the name conversion table will be deleted.                        */
/*  i/o  = pszDirName:char *:inp: directory to be deleted                    */
/*         piErrp    :int * :out: area for errno (if an error occurred)      */
/*         piNerrp   :int * :out: area for NetWareErrno (if an error occurrd)*/
/*  return = GWERR_NORMAL(0) : normal end                                    */
/*           GWERR_ABEND (-1): abnormal end (See piErrp and piNerrp for      */
/*                             detailed error information)                   */
/*           GWERR_BUG (-100): internal error                                */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 22, 1994 by Isao Ichiyoshi                                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
int EraseDFAMDir ( pszDirName, piErrp, piNerrp )
char	*pszDirName;
int		*piErrp;
int		*piNerrp;
{
	int iCcode;
	struct GName_t	*pNamep;
	struct GVolume_t *pVolp;



	if ( pszDirName == NULL  || *pszDirName == '\0') {
		return( GWERR_BUG );
	};

	SetDefaultAttr( pszDirName );


	iCcode = rmdir( pszDirName );
	*piErrp = errno;
	*piNerrp = NetWareErrno;

	if ( iCcode != 0 ) {
		return( GWERR_ABEND );
	};


	iCcode = isDFAMVolume( pszDirName, &pVolp);

	if (iCcode == YES) {
		WaitOnLocalSemaphore( pVolp->uiVolAndName_sh );


		pNamep = FindName_t(pszDirName);

		if( pNamep != NULL) {

			iCcode = AddToDelList(pNamep);

			if ( iCcode != GWERR_NORMAL ) {
				SignalLocalSemaphore( pVolp->uiVolAndName_sh );
				return(GWERR_BUG);
			};
		};
		SignalLocalSemaphore( pVolp->uiVolAndName_sh );
	};
	return(GWERR_NORMAL);
}




/*EnglishComment**************************************************************/
/*  ID   = RenameDFAMEntry                                                   */
/*  name = Entry rename routine                                              */
/*  func = Converts an entry name.  If the entry name was a converted one    */
/*         on DFAM Volume, the name conversion table will be modified.  The  */
/*         entry name can be either in DFAM Volume or not.                   */
/*  i/o  = pszOld:char *:inp: entry name to be converted                     */
/*         pszNew:char *:inp: entry name to store the converted name         */
/*         piErr     :int * :out: area for errno (if an error occurred)      */
/*         piNerr    :int * :out: area for NetWareErrno (if an error occurrd)*/
/*  return = GWERR_NORMAL(0) : normal end                                    */
/*           GWERR_ABEND (-1): abnormal end (See piErrp and piNerrp for      */
/*                             detailed error information)                   */
/*           GWERR_BUG (-100): internal error                                */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 13, 1994 by Isao Ichiyoshi                                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
int RenameDFAMEntry( pszOld, pszNew, piErr, piNerr )
char	*pszOld;
char	*pszNew;
int		*piErr;
int		*piNerr;
{
	int iCcode;
	int iRtnc = GWERR_NORMAL;
	BOOL bSemaph = FALSE;
	struct GName_t	*pOldNamep;
	struct GVolume_t *pOldVol;



	if ( pszOld == NULL  || *pszOld == '\0') {
		return(GWERR_BUG);
	};


	if ( pszNew == NULL  || *pszNew == '\0') {
		return(GWERR_BUG);
	};


	iCcode = rename(pszOld, pszNew);
	*piErr = errno;
	*piNerr = NetWareErrno;

	if ( iCcode != 0 ) {
		return(iCcode);
	};


	iCcode = isDFAMVolume( pszOld, &pOldVol );

	if (iCcode == NO) {
		return(GWERR_NORMAL);
	};

	WaitOnLocalSemaphore( pOldVol->uiVolAndName_sh );
	bSemaph = TRUE;

	pOldNamep = FindName_t(pszOld);
	if (pOldNamep == NULL) {
		goto EndOfFunc;
	};


	iCcode = AddToDelList(pOldNamep);
	if (iCcode != GWERR_NORMAL) {
		iRtnc = GWERR_BUG;
	}

EndOfFunc:
	if (bSemaph == TRUE) {
		SignalLocalSemaphore( pOldVol->uiVolAndName_sh );
	};
	return(iRtnc);
}




/*EnglishComment**************************************************************
 *  ID   = isDevice                                                          *
 *  name = Device file checking routine                                      *
 *  func = Checks if the designated entry matches with the name that DFAM    *
 *         recognizes as a device name.  The parameter can be either a full  *
 *         path name or an entry name.                                       *
 *  i/o  = pszEntryName: BYTE *:inp: entry name                              *
 *                                                                           *
 *  return = GWERR_NORMAL  : normal                                          *
 *           GWERR_ISDEVICE: accepted as a device name                       *
 *           GWERR_BUG     : internal error                                  *
 *                                                                           *
 *  note   = (1) This function does not check all the devices: only the      *
 *           fundamental parts.  Thus never assume that the device name is   *
 *           100% perfect.  Users must be cautious for the devices out of    *
 *           scope.                                                          *
 *           (2) Due to the specification of _splitpath, the entry name is   *
 *           recognized as 8 even though the length illegally exceeds the    *
 *           limit.  This function does not check such an invalid entry.     *
 *                                                                           *
 *  common = none                                                            *
 *  note   = none                                                            *
 *  date   = Feb. 23, 1995 by Isao Ichiyoshi                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/


int isDevice(pszEntryName)
BYTE *pszEntryName;
{
	int		iCnt;
	BYTE	uiWbuf[_MAX_FNAME];

	uiWbuf[0]='\0';

	if ( pszEntryName == NULL || *pszEntryName == '\0' ) {
		return (GWERR_BUG);
	};

	_splitpath( (char *)pszEntryName, NULL, NULL, uiWbuf, NULL);


	if ( *uiWbuf == '\0' ) {
		return(GWERR_BUG);
	};

	for (iCnt=0; pszDeviceName[iCnt] != NULL; iCnt++){
		if ( !stricmp(uiWbuf, pszDeviceName[iCnt]) ) {
			return(GWERR_ISDEVICE);
		};
	}

	return(GWERR_NORMAL);

}




/*EnglishComment**************************************************************/
/*  ID   = SetDefaultAttr                                                    */
/*  name = Attribute change routine                                          */
/*  func = Changes the designated path attribute into Read/Write + Archive   */
/*                                                                           */
/*  i/o  = pszPath : char * :inp: path name                                  */
/*                                                                           */
/*  return = GWERR_FAILURE : failed to change the attribute                  */
/*           GWERR_NORMAL  : attribute was successfully changed.             */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 13, 1994 by Isao Ichiyoshi                                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/

int SetDefaultAttr(pszPath)
char *pszPath;
{
    struct ModifyStructure Modify;
    int iRc;

    Modify.MFileAttributes = DEFAULTATTR;
    Modify.MFileAttributesMask = -1;

    iRc = ChangeDirectoryEntry(pszPath, &Modify, MFileAttributesBit, 0L);
    if (iRc != 0) {
        return GWERR_FAILURE;
    }

    return GWERR_NORMAL;
}
