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
# ifndef RCS_agt_msg_h_Included
# define RCS_agt_msg_h_Included
 static char *RCSID_agt_msg_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_msg.h,v 1.1.2.2 1996/03/11 14:06:10 marty Exp $";
# endif /* Not RCS_agt_msg_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_msg.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:10  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:55:55  marty]
 *
 * Revision 1.1.2.1  1996/02/19  09:38:03  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:37:34  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  05:43:21  05:43:21  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:43:10  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  02:22:51  kishi
 * 	Change Copyright.
 * 	[1995/10/26  02:22:35  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:39:43  kishi
 * 	Initial revision
 * 	[1995/09/09  07:19:28  kishi]
 * 
 * $EndLog$
 */
#include "agt_trc.h"
#include "agt_sck.h"
#include "agt_msgid.h"

#include <unistd.h>
#include <string.h>

#ifndef _AGT_MSG_H
#define _AGT_MSG_H

#define D_CAT_AGTFILE       "dfaagt"
#define D_CAT_SDKFILE       "setdfakey"

#define D_ERMSG_MAX         2048
#define D_SYSLOG_ID         "dfa"
#define D_SYSLOG_ID_LEN     4
#define D_SYSLOG_WAIT_TIME  100000

#define D_SLOG_ST_CLOSE     0
#define D_SLOG_ST_OPEN      1

#define D_ERMSG_NORMAL      0
#define D_ERMSG_EXTRA       10
#define D_ERMSG_DEBUG       20


struct d_msg {
    char tableid[4];

    char syslogid[D_SYSLOG_ID_LEN+1];

    int slog_status;
    int level;

    int buflen;
    char buf[D_ERMSG_MAX];
    char wkbuf[D_ERMSG_MAX];
};

extern struct d_msg msg;

extern void AgentMsg(int priority, char *message);
extern void GatewayMsg(int msglen, char *message, pid_t wdpid);

#endif
