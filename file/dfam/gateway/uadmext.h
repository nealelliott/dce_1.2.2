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
 * $Log: uadmext.h,v $
 * Revision 1.1.2.2  1996/03/11  14:09:11  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:18  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:41:10  takano
 * 	First Submit
 * 	[1996/02/21  07:40:44  takano]
 * 
 * $EndLog$
 */
/*EnglishComment**************************************************************
 * UADMEXT.H                                                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
extern NUTInfo *pHandle;
extern FILE *pVolFp;
extern FILE *pAgtFp;
extern FILE *pOthrFp;
extern char *pMsgTable[];
extern LONG uiAllocTag;
extern unsigned int uiNLMHandle;
extern int iScreenID;
extern char aszNWver[10];
extern char aszNWserver[_MAX_SERVER];
extern char aszError[16];
extern struct AgentInfo *pAInfo;
extern struct OtherInfo *pOther;
extern struct VolumeInfo *pVInfoTop;
