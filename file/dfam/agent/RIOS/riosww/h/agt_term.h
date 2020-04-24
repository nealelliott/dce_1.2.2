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
# ifndef RCS_agt_term_h_Included
# define RCS_agt_term_h_Included
 static char *RCSID_agt_term_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_term.h,v 1.1.2.2 1996/03/11 14:06:22 marty Exp $";
# endif /* Not RCS_agt_term_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_term.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:22  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:04  marty]
 *
 * Revision 1.1.2.1  1996/02/19  09:19:17  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:18:56  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  06:21:44  06:21:44  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  06:21:00  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  02:49:44  kishi
 * 	Change Copyright.
 * 	[1995/10/26  02:49:31  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:36:47  kishi
 * 	Initial revision
 * 	[1995/09/09  07:22:40  kishi]
 * 
 * $EndLog$
 */

#ifndef _AGT_TERM_H
#define _AGT_TERM_H


extern void agt_term_remove_statfile(void);
extern void agt_term_close_syslog(void);
extern void agt_term_release_shmem(void);
extern void agt_term_close_unixsocket(void);
extern void agt_term_remove_semaphore(void);

#endif
