/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.
 */
#ifdef HITACHI_REV_CHECK
static char *RCSID_agt_time_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/agt_time.c,v 1.1.2.2 1996/03/09 20:44:22 marty Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_time.c,v $
 * Revision 1.1.2.2  1996/03/09  20:44:22  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:14  marty]
 *
 * Revision 1.1.2.1  1996/02/16  05:36:06  dce-sdc
 * 	First Submit
 * 	[1996/02/16  05:33:47  dce-sdc]
 * 
 * Revision 1.1.2.4  96/01/12  05:00:36  05:00:36  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:00:12  kishi]
 * 
 * Revision 1.1.2.3  1995/10/26  01:20:56  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:20:20  kishi]
 * 
 * Revision 1.1.2.2  1995/09/28  12:25:40  kishi
 * 	Add header comment.
 * 	[1995/09/28  12:25:10  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  06:48:36  kishi
 * 	Initial revision
 * 	[1995/09/09  06:44:45  kishi]
 * 
 * $EndLog$
 */
#include <pthread.h>
#include <syslog.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "agt_agent.h"
#include "agt_msg.h"
#include "agt_usrc.h"
#include "agt_acs.h"
#include "agt_errid.h"
#include "agt_sysid.h"

int agt_time_gettime(void);

/**************************************************************************/
/* ID	= agt_time_gettime						  */
/* name = Get machine time schedule					  */
/* func = Get time schedule of agent machine				  */
/* io = none								  */
/* return =								  */
/* 	0 :normal end							  */
/*  -1:abnormal end							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_time_gettime(void)
{
	time_t tloc;
	char senddata[8];

	if (time(&tloc) == -1){
		d_errno = errno;
		agt_usrc_oserr_trace(DTIME,d_errno);
		goto func_err;
	}



	memset(senddata,0,8);
	tloc = htonl(tloc);
	memcpy(senddata,(char *)&tloc,sizeof(tloc));
	agt_usrc_send_data(senddata,8,DFAM_TIME_R);
	return(0);

func_err:
	agt_usrc_send_emsg(senddata,DFAM_TIME_R);
	return(-1);
}

