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
 * $Log: err.h,v $
 * Revision 1.1.2.2  1996/03/11  14:07:41  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:06  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:34:26  takano
 * 	First Submit
 * 	[1996/02/21  07:34:22  takano]
 * 
 * $EndLog$
 */
/*EnglishComment**************************************************************
 * ERR.H                                                                     *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
#ifndef ERR_H
enum error {
    NORMAL = 0,


    NOENT       = 2,
    LOCALDRIVE  = 3,
    ERROPT      = 4,
    NOUSER      = 5,
    NOGRP       = 6,
    MEMERR      = 7,
    NORIGHT     = 8,
    NULLPATH    = 9,
    NOVOL       = 10,
    NOSERVER    = 11,
    PATHERR     = 12,
    NOTDEL      = 13,
	INVALENT	= 14,

    CABNORM     = 97,
    HEADONLY    = 98,


    CTHRESHLD   = 99,


    FAILECHG    = 100,
    MODEERR     = 101,

    ABNORMAL    = 198,


    GTHRESHLD   = 199,


    TOOLONG     = 200,
    APIERR      = 201,
    ERIGHT      = 202,
    FSYSBIGERR  = 203,
    LOOPERR     = 204,
    EFCREATE    = 205,
    IOERR       = 206,
    ENOTLFS     = 207,
    RANGEERR    = 208,
    EDSPC       = 209,
    EUNAVAILSRV = 210,
    ROFSERR     = 211,
	AGCONERR	= 212,
	NODCEENT	= 213,
	NOTRLST		= 214,


    NODFAMUSER  = 247,
    BINDERR     = 248,
    GWMEMERR    = 249,
    UNLOGIN     = 250,
    AGTDOWN     = 251,
    ERRBUSY     = 252,
    INTERE      = 253,
    CONERR      = 254,
    SYSTEM      = 255

};
#endif
