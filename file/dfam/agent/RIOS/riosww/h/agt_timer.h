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
# ifndef RCS_agt_timer_h_Included
# define RCS_agt_timer_h_Included
 static char *RCSID_agt_timer_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_timer.h,v 1.1.2.2 1996/03/11 14:06:24 marty Exp $";
# endif /* Not RCS_agt_timer_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_timer.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:24  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:05  marty]
 *
 * Revision 1.1.2.1  1996/02/19  09:16:18  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:15:56  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  06:25:49  06:25:49  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  06:25:08  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  02:54:30  kishi
 * 	Change Copyright.
 * 	[1995/10/26  02:54:22  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:36:17  kishi
 * 	Initial revision
 * 	[1995/09/09  07:23:13  kishi]
 * 
 * $EndLog$
 */

#ifndef _AGT_TIMER_H
#define _AGT_TIMER_H

#define D_ALARM_INTERVAL    1

struct agt_timerinfo {
    long dummy;
};

struct agt_timer {
    struct agt_timer *next;
    unsigned long seconds;
    void (*timer_func)();
    struct agt_timerinfo *info;
};

struct agt_timer_chkup {
    struct agt_timer_chkup *next;
    int (*func)();

};

struct d_timer_table {
    struct agt_timer *wait_list;
    struct agt_timer *done_list;
    unsigned long cur_timer;
    int itimer_on;
    struct agt_timer_chkup *chkup_list;
};


extern struct d_timer_table AgentTimer;


extern void AgentInitTimer(void);
extern void AgentSetTimer(struct agt_timer *timep);
extern struct agt_timer *AgentGetTimer(void);
extern int AgentResetTimer(struct agt_timer *timep);


#endif
