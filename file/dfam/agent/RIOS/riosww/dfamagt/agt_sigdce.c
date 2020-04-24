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
static char *RCSID_agt_sigdce_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/agt_sigdce.c,v 1.1.2.2 1996/03/09 20:44:15 marty Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_sigdce.c,v $
 * Revision 1.1.2.2  1996/03/09  20:44:15  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:09  marty]
 *
 * Revision 1.1.2.1  1996/02/16  05:48:15  dce-sdc
 * 	First Submit
 * 	[1996/02/16  05:47:46  dce-sdc]
 * 
 * Revision 1.1.2.4  96/01/12  04:54:35  04:54:35  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  04:54:26  kishi]
 * 
 * Revision 1.1.2.3  1995/10/26  01:08:13  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:08:08  kishi]
 * 
 * Revision 1.1.2.2  1995/09/23  09:51:06  kishi
 * 	Add header comment.
 * 	[1995/09/23  09:25:07  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  06:49:57  kishi
 * 	Initial revision
 * 	[1995/09/09  06:43:12  kishi]
 * 
 * $EndLog$
 */
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <syslog.h>

#include "agt_agent.h"
#include "agt_sigdce.h"
#include "agt_errid.h"
#include "agt_msg.h"
#include "agt_lib.h"


/**************************************************************************/
/* ID	= agt_sig_dce_init						  */
/* name = Set function executed during signal transmission		  */
/* func = Set to ignore SIGPIPE signal. Set to transfer execution to      */
/*        agt_abort_on_signal function when transmitting SIGILL,SIGBUS,   */
/*        SIGSEGV,SIGSYS signals					  */
/* io = none								  */
/* return = none							  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_sig_dce_init(void)
{
    struct sigaction newact;
    struct sigaction oldact;
    void (*agt_abort_on_signal)();

    newact.sa_handler = SIG_IGN;
    sigemptyset(&newact.sa_mask);
    sigemptyset(&oldact.sa_mask);
    newact.sa_flags = 0;
    if (sigaction(SIGPIPE, &newact, &oldact) == -1) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_20501, errno);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }

    newact.sa_handler = agt_abort_on_signal;
    if (sigaction(SIGILL, &newact, &oldact) == -1) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_20502, errno);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }
    if (sigaction(SIGBUS, &newact, &oldact) == -1) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_20503, errno);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }
    if (sigaction(SIGSEGV, &newact, &oldact) == -1) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_20504, errno);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }
    if (sigaction(SIGSYS, &newact, &oldact) == -1) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_20505, errno);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }
}
