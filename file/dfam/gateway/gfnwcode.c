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
 * $Log: gfnwcode.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:23  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:40  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:01:49  takano
 * 	First Submit
 * 	[1996/02/21  08:01:40  takano]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamprt.h"
#include "gfnw.h"


/*EnglishComment**************************************************************
 * ID = D_strupr                                                             *
 *                                                                           *
 * Except for the Hitachi version, this function is idendical to strupr().   *
 *                                                                           *
 * date = Jan. 20, 1995 by Isao Ichiyoshi                                    *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
char *D_strupr(pszStr)
char *pszStr;
{
	return(strupr(pszStr));
}




/*EnglishComment**************************************************************
 * ID = D_strlwr                                                             *
 *                                                                           *
 * Except for the Hitachi version, this function is idendical to strlwr().   *
 *                                                                           *
 * date = Jan. 20, 1995 by Isao Ichiyoshi                                    *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
char *D_strlwr(pszStr)
char *pszStr;
{
	return(strlwr(pszStr));
}




/*EnglishComment**************************************************************
 * ID = D_strchr                                                             *
 *                                                                           *
 * Except for the Hitachi version, this function is idendical to strchr().   *
 *                                                                           *
 * date = Jan. 20, 1995 by Isao Ichiyoshi                                    *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
char * D_strchr( pszString, iChr )
char *pszString;
int  iChr;
{

	return(strchr(pszString, iChr));
}




/*EnglishComment**************************************************************
 * ID = D_strpbrk                                                            *
 *                                                                           *
 * Except for the Hitachi version, this function is idendical to strpbrk().  *
 *                                                                           *
 * date = Jan. 20, 1995 by Isao Ichiyoshi                                    *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
char * D_strpbrk ( pszString, pszTarget )
char *pszString;
char *pszTarget;
{

	return(strpbrk(pszString, pszTarget));

}




/*EnglishComment**************************************************************
 * ID = D_strrchr                                                            *
 *                                                                           *
 * Except for the Hitachi version, this function is idendical to strrchr().  *
 *                                                                           *
 * date = May 6, 1995 by Isao Ichiyoshi                                      *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
char * D_strrchr( pszStr, iTarget )
char	*pszStr;
int		iTarget;
{
	return(strrchr( pszStr, iTarget ) );
}




/*EnglishComment**************************************************************
 * ID = D_strcspn                                                            *
 *                                                                           *
 * Except for the Hitachi version, this function is idendical to strcspn().  *
 *                                                                           *
 * date = May 6, 1995 by Isao Ichiyoshi                                      *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
size_t D_strcspn( pszStr, pszTarget )
char *pszStr;
char *pszTarget;
{
	return( strcspn( pszStr, pszTarget ) );
}












/*EnglishComment**************************************************************/
/*  ID   = GetEntry                                                          */
/*                                                                           */
/*  name = Getting an entry name from a path name                            */
/*  func = Parses a path name, and gets an entry name with considering a     */
/*         2-byte-code character.                                            */
/*                                                                           */
/*  i/o  = pszPath:char *  :inp: a path name (this function decomposed       */
/*                               the path name into entries by removing a    */
/*                               volume name, \ and /)                       */
/*         pszEnt :char *  :out: a pointer to the entry extracted from the   */
/*                               path name                                   */
/*         pszNext:char ** :out: a pointer to the next entry name            */
/*                                                                           */
/*  return = GWERR_NORMAL: normal                                            */
/*           GWERR_LONG  : entry name too long (the output information       */
/*                         cannot be stored to _MAX_NAME)                    */
/*           GWERR_INVAL : entry name invalid (a delimiter came after a      */
/*                         delimiter)                                        */
/*           GWERR_BUG   : bug (pszPath is null or a null string)            */
/*  common = none                                                            */
/*                                                                           */
/*  note = (1) pszEnt has only the path name itself, and \ and / are taken   */
/*             off from the original path name.  If the original path name   */
/*             contains \\ or //, null will be set to pszEnt (this is        */
/*             assumed as an invalid entry name, and this function makes     */
/*             an error return of GWERR_INVAL).                              */
/*         (2) The caller of this function must have a buffer, _MAX_NAME,    */
/*             as a real body of pszEnt.  If the actual buffer is larger     */
/*             than _MAX_NAME, the surplus area is not used.  If the actual  */
/*             buffer size is smaller, memory may be destroyed.              */
/*         (3) If you do not need the information of pszNext, set null.      */
/*         (4) This function does not check the validity of the data         */
/*             contents, but only the length (A.AAAA will not be an error).  */
/*         (5) Except for the Hitachi version, this function does not        */
/*             consider about 2-byte-code character.                         */
/*                                                                           */
/*  date = April 20, 1995  by Isao Ichiyoshi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*EnglishComment**************************************************************/
int GetEntry( pszPath, pszEnt, pszNext )
	char *pszPath;
	char *pszEnt;
	char **pszNext;
{
	int		iRtnc = GWERR_NORMAL;
	int		iMojicnt = 0;
	char	*pszWkp = pszPath;
	char	*pszRtnEntp = pszEnt;
	char	*pszEndp;


	GW_tsh_functrace( FUNC_GETENTRY, GW_FUNC_START, NULL);

	if ( (pszPath == NULL) || (*pszPath == '\0') ) {
		iRtnc = GWERR_BUG;
		goto EndOfFunc;
	};

	if ( pszEnt == NULL ) {
		iRtnc = GWERR_BUG;
		goto EndOfFunc;
	}


	*pszEnt = '\0';


	if (*pszWkp == '\\' || *pszWkp == '\/') {
		pszWkp++;
	};


	pszEndp = D_strpbrk( pszWkp , NW_DELIMITER);
	if ( pszEndp == NULL ) {
		pszEndp = pszPath + strlen( pszPath );
	};


	while ( pszWkp < pszEndp ) {
		*pszRtnEntp++ = *pszWkp++;
		if ( ++iMojicnt >= _MAX_NAME ) {
			iRtnc = GWERR_LONG;
			goto EndOfFunc;
		};
	}
	*pszRtnEntp = '\0';


	if ( *pszWkp == '\\' || *pszWkp == '\/' ) {
		pszWkp++;
	};


	if ( *pszWkp == '\\' || *pszWkp == '\/' ) {
		iRtnc = GWERR_INVAL;
		goto EndOfFunc;
	};


	if ( (pszNext != NULL) && (*pszWkp != '\0') ) {
		*pszNext = pszWkp;
	} else {
		*pszNext = NULL;
	}

EndOfFunc:

	if ( (iRtnc != GWERR_NORMAL) && (pszEnt != NULL) && (*pszEnt != '\0') ) {
		*pszEnt = '\0';
	};


	GW_tsh_functrace( FUNC_GETENTRY, GW_FUNC_END, (int)iRtnc );

	return( iRtnc );
}


/*EnglishComment**************************************************************
 * ID = StrCut                                                               *
 *                                                                           *
 * Except for the Hitachi version, this function cut the string simply.      *
 *                                                                           *
 * date = Nov. 23, 1995 by Isao Ichiyoshi                                    *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
void StrCut( pszStr, uiCutsize )
char *pszStr;
size_t uiCutsize;
{


	if ( strlen(pszStr) > uiCutsize ) {
		pszStr[uiCutsize] = '\0';
	};

	return;
};


