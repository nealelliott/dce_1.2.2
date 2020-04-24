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
# ifndef RCS_agt_sig_h_Included
# define RCS_agt_sig_h_Included
 static char *RCSID_agt_sig_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_sig.h,v 1.1.2.2 1996/03/11 14:06:16 marty Exp $";
# endif /* Not RCS_agt_sig_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_sig.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:16  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:55:59  marty]
 *
 * Revision 1.1.2.1  1996/02/19  09:26:44  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:26:18  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  05:57:16  05:57:16  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:56:52  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  02:38:39  kishi
 * 	Change Copyright.
 * 	[1995/10/26  02:38:31  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:38:13  kishi
 * 	Initial revision
 * 	[1995/09/09  07:21:08  kishi]
 * 
 * $EndLog$
 */

#ifndef _AGT_SIG_H
#define _AGT_SIG_H

#include <signal.h>

#define D_SIG_MAX_SIZE      20
struct d_sig_table {
    int sig;
    void (*func)();
};

extern struct d_sig_table sigtable[D_SIG_MAX_SIZE];

extern void agt_sig_action(int sig, void (*func)());
extern void agt_sig_init_signal_threads(void);
extern void agt_sig_ignore(int sig, int code, struct sigcontext *scp);
extern void agt_sig_ok_exit(int sig, int code, struct sigcontext *scp);

#endif
