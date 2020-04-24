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
 * $Log: encrypt.h,v $
 * Revision 1.1.2.2  1996/03/11  14:07:40  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:05  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:33:58  takano
 * 	First Submit
 * 	[1996/02/21  07:33:53  takano]
 * 
 * $EndLog$
 */
/*EnglishComment**************************************************************
 * ENCRYPT.H                                                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/

void	fnSetEncryptData( unsigned long *, unsigned char *, int, unsigned char * );
void	fnGetDecryptData( unsigned long *, unsigned char *, int, unsigned char * );
unsigned long fnLongSwap( unsigned long);

