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
 * $Log: gdirglbl.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:46  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:11  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:46:34  takano
 * 	First Submit
 * 	[1996/02/21  07:46:29  takano]
 * 
 * $EndLog$
 */
/*EnglishComment**************************************************************
 * GDIRGLBL.C                                                                *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
#include "gdir.h"


struct ComTGroup_t	gComS;

unsigned int		uigAllocatedMem = 0;

int					igThreadID;
int					igThreadGroupID;

time_t				igTimeGap = 0;

int					igSemHandleFlag = 0;
int					igAdmSockfdFlag = 0;
int					igAdmConReqFlag = 0;
int					igDCELoginFlag = 0;

int					igAdmConReqCount = 0;

unsigned int		uigSemHandle;
int					igAdmSockfd;
int					igDelaySlice;
int					igInitErr = 0;
int					igInitAct = 0;

char *apszNWLogFile[] = {	"VOL$LOG.ERR",
							(char *)NULL
};

