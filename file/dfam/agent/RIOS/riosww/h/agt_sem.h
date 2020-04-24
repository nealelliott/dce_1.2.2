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
# ifndef RCS_agt_sem_h_Included
# define RCS_agt_sem_h_Included
 static char *RCSID_agt_sem_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_sem.h,v 1.1.2.2 1996/03/11 14:06:15 marty Exp $";
# endif /* Not RCS_agt_sem_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_sem.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:15  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:55:59  marty]
 *
 * Revision 1.1.2.1  1996/02/19  09:30:10  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:29:37  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  05:54:51  05:54:51  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:54:41  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  02:36:37  kishi
 * 	Change Copyright.
 * 	[1995/10/26  02:36:25  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:38:33  kishi
 * 	Initial revision
 * 	[1995/09/09  07:20:53  kishi]
 * 
 * $EndLog$
 */

#ifndef _AGT_SEM_H
#define _AGT_SEM_H

#define D_ID_SEM            'A'
#define D_NUM_SEM           2
#define D_SEM_UP            0
#define D_SEM_TRC           1
#define D_ID_SHM_M          'B'
#define D_SZ_SHM_M          32768
#define D_ID_SHM_T          'C'
#define D_ID_SHM_M2         'D'

extern void agt_sem_open_semaphore(void);
extern void agt_sem_release_semaphore(void);
extern int agt_sem_check_if_agent_is_up(int semid);
extern int agt_sem_set_agent_up_flg(int semid);
extern int agt_sem_set_lock(int semid, int semnum);
extern int agt_sem_release_lock(int semid, int semnum);
extern int agt_sem_wait_agent_gets_down(int semid);

#endif
