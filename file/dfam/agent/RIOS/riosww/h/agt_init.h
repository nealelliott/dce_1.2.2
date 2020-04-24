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
# ifndef RCS_agt_init_h_Included
# define RCS_agt_init_h_Included
 static char *RCSID_agt_init_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_init.h,v 1.1.2.2 1996/03/11 14:06:06 marty Exp $";
# endif /* Not RCS_agt_init_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_init.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:06  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:55:52  marty]
 *
 * Revision 1.1.2.1  1996/02/19  09:46:15  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:45:54  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  05:35:14  05:35:14  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:35:05  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  02:02:07  kishi
 * 	Change Copyright.
 * 	[1995/10/26  02:01:34  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:41:21  kishi
 * 	Initial revision
 * 	[1995/09/09  07:17:49  kishi]
 * 
 * $EndLog$
 */

#ifndef _AGT_INIT_H
#define _AGT_INIT_H


extern void agt_init_check_environment(void);
extern void agt_init_alloc_shmem(void);
extern void agt_init_setup_signals(void);
extern void agt_init_go_background(void);
extern void agt_init_open_uxsock(void);
extern int agt_init_create_processes(void);
extern void agt_init_statfile(void);
extern void agt_init_syslog(void);
extern void agt_init_memory(void);
extern void agt_init_once(void);
extern void agt_init_check_agent(void);


#endif
