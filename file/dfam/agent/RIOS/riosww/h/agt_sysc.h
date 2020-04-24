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
# ifndef RCS_agt_sysc_h_Included
# define RCS_agt_sysc_h_Included
 static char *RCSID_agt_sysc_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_sysc.h,v 1.1.2.2 1996/03/11 14:06:20 marty Exp $";
# endif /* Not RCS_agt_sysc_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_sysc.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:20  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:02  marty]
 *
 * Revision 1.1.2.1  1996/02/19  09:22:18  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:21:56  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  06:17:23  06:17:23  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  06:17:07  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  02:46:10  kishi
 * 	Change Copyright.
 * 	[1995/10/26  02:46:01  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:37:20  kishi
 * 	Initial revision
 * 	[1995/09/09  07:22:05  kishi]
 * 
 * $EndLog$
 */

#ifndef _AGT_SYSC_H
#define _AGT_SYSC_H

#include "agt_timer.h"

#define D_TIMER_STOP       1
#define D_TIMER_ABORT      2
#define D_TIMER_ABORT_TIME 25


#define D_SYSCOM_ST_INIT   0x00010000
#define D_SYSCOM_ST_RUN    0x00040000
#define D_SYSCOM_ST_STOP   0x00080000
#define D_SYSCOM_ST_ABORT  0x00080080

#define D_SYSC_ST_INIT     0x00010000
#define D_SYSC_ST_RUN      0x00040000
#define D_SYSC_ST_TERM     0x00080000
#define D_SYSC_ST_DEAD     0x00080080

struct d_local_sysc {
    struct agt_timer *stoptimer;
};

extern struct d_local_sysc lsysc;


extern void agt_sysc(void);
extern void agt_sysc_add_sysc(struct d_sysc **top,
			      struct d_sysc **last, struct d_sysc *p);


#endif
