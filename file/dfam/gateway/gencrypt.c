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
 * $Log: gencrypt.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:59  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:21  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:54:04  takano
 * 	First Submit
 * 	[1996/02/21  07:54:00  takano]
 * 
 * $EndLog$
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "encrypt.h"



#define SHUFFLENUM 8





/*EnglishComment*************************************************************/
/*  ID   = fnSetEncryptData                                                 */
/*  name = character string encryption routine                              */
/*  func = encrypts the designated character string, and returns an         */
/*         encrypted character string.                                      */
/*  i/o  = auiSkey : unsigned long *:i   : an encryption key                */
/*         pbyData : unsigned char *:i   : data to be encrypted (plain text)*/
/*         iLen    : int            :i   : data length (byte)               */
/*         pbyEdata: unsigned char *:i/o : the pointer to the encrypted data*/
/* return = encrypted data (unsigned char *)                                */
/* common = :::                                                             */
/* note   = The caller of this routine should have an enough data space     */
/*          for the encrypted data.                                         */
/*  data = Dec. 26, 1994 by Masato Arai                                     */
/*                                                                          */
/* Trade Secret of Hitachi, Ltd.                                            */
/* Do not disclose without written concession from Hitachi, Ltd.            */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*EnglishComment*************************************************************/
void fnSetEncryptData( auiSkey, pbyData, iLen, pbyEdata )
unsigned long	auiSkey[2];
unsigned char	*pbyData;
int		iLen;
unsigned char	*pbyEdata;
{

	auiSkey[0] = auiSkey[0];
	memcpy( pbyEdata, pbyData, iLen );
	return;
}

/*EnglishComment*************************************************************/
/*  ID   = fnGetDecryptData                                                 */
/*  name = decryption routine                                               */
/*  func = decrypts the encrypted string, and returns a plain text.         */
/*                                                                          */
/*  i/o  = auiSkey : unsigned long *:i : decryption key                     */
/*         pbyData : unsigned char *:i : the data to be decrypted           */
/*         iLength : int            :i : length of the decrypted text (byte)*/
/*         pbyPdata: unsigned char *:i/o : the pointer to the decrypted data*/
/*                                                                          */
/*  return = none                                                           */
/*  common = :::                                                            */
/*  note   = The caller of this routine should calculate the length of the  */
/*           decrypted data.                                                */
/*  data = Dec. 26, 1994 by Masato Arai                                     */
/*                                                                          */
/* Trade Secret of Hitachi, Ltd.                                            */
/* Do not disclose without written concession from Hitachi, Ltd.            */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*EnglishComment*************************************************************/
void fnGetDecryptData( auiSkey, pbyData, iLength, pbyPdata )
unsigned long	auiSkey[2];
unsigned char	*pbyData;
int		iLength;
unsigned char	*pbyPdata;
{

	auiSkey[0] = auiSkey[0];
	memcpy( pbyPdata, pbyData, iLength );
	return;
}

/*EnglishComment*************************************************************/
/*  ID   = fnLongSwap                                                       */
/*  name = byte order change routine                                        */
/*  func = changes the byte order of the unsigned long type.                */
/*  i/o  = uiSrc   : unsigned long  :i   : the target data                  */
/*  return = the data whose byte order has been changed                     */
/*  common = :::                                                            */
/*  note   =                                                                */
/*  data = Dec. 26, 1994 by Masato Arai                                     */
/*                                                                          */
/* Trade Secret of Hitachi, Ltd.                                            */
/* Do not disclose without written concession from Hitachi, Ltd.            */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*EnglishComment*************************************************************/
unsigned long fnLongSwap( unsigned long uiSrc )
{
	unsigned long	uiDst;

	uiDst = uiSrc;

	swab( (unsigned char *)&uiDst+1, (unsigned char *)&uiDst+1, 2 );
	swab( (unsigned char *)&uiDst, (unsigned char *)&uiDst, 2 );
	swab( (unsigned char *)&uiDst+2, (unsigned char *)&uiDst+2, 2 );

	swab( (unsigned char *)&uiDst+1, (unsigned char *)&uiDst+1, 2 );
	swab( (unsigned char *)&uiDst, (unsigned char *)&uiDst, 2 );
	swab( (unsigned char *)&uiDst+2, (unsigned char *)&uiDst+2, 2 );

	return(uiDst);
}

