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
# ifndef RCS_agt_main_h_Included
# define RCS_agt_main_h_Included
 static char *RCSID_agt_main_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_main.h,v 1.1.2.2 1996/03/11 14:06:10 marty Exp $";
# endif /* Not RCS_agt_main_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_main.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:10  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:55:55  marty]
 *
 * Revision 1.1.2.1  1996/02/19  09:40:25  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:40:01  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  05:41:46  05:41:46  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:41:36  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  02:16:29  kishi
 * 	Change Copyright.
 * 	[1995/10/26  02:16:15  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:39:58  kishi
 * 	Initial revision
 * 	[1995/09/09  07:19:12  kishi]
 * 
 * $EndLog$
 */

#ifndef _AGT_MAIN_H
#define _AGT_MAIN_H


struct d_local_main {
    int foreground;
};

#ifdef _DEBUG
struct d_debug_info {
    int loop_interval;
    struct timeval tmv;
    int waitsignal;
    int debug_loop;
    int noexit;
};

extern struct d_debug_info debug;

#endif

extern struct d_local_main lmain;

extern void agt_main_sig_term(int sig, int code, struct sigcontext *scp);
extern void agt_main_sig_chld(int sig, int code, struct sigcontext *scp);

#endif
