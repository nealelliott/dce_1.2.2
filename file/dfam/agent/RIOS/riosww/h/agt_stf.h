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
# ifndef RCS_agt_stf_h_Included
# define RCS_agt_stf_h_Included
 static char *RCSID_agt_stf_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_stf.h,v 1.1.2.4 1996/04/17 17:28:06 root Exp $";
# endif /* Not RCS_agt_stf_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_stf.h,v $
 * Revision 1.1.2.4  1996/04/17  17:28:06  root
 * 	OT 13430 Submit
 * 	[1996/04/17  17:21:32  root]
 *
 * Revision 1.1.2.2  1996/03/11  14:06:19  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:01  marty]
 * 
 * Revision 1.1.2.1  1996/02/19  09:23:37  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:23:08  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  06:16:11  06:16:11  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  06:15:52  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  02:44:26  kishi
 * 	Change Copyright.
 * 	[1995/10/26  02:44:21  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:37:37  kishi
 * 	Initial revision
 * 	[1995/09/09  07:21:46  kishi]
 * 
 * $EndLog$
 */

#ifndef _AGT_STF_H
#define _AGT_STF_H

#include <fcntl.h>
#include <unistd.h>


#define D_STAT_PATH         "/opt/dcelocal/var/dfa"
#define D_STAT_FILE         "dfastat"
#define D_STAT_LINESZ       2048
#define D_STAT_FS           ":\n"


extern int agt_stat_create_file(char *fn, int omode, mode_t access);
extern int agt_stat_open_file(char *fn, int omode);
extern void agt_stat_add_entry(int fd, char *id, pid_t pid,
	       char *date, char *time, char *remote, char *user);
extern void agt_stat_delete_entry(int fd, pid_t pid);
extern void agt_stat_update_entry(int fd, pid_t pid, char *id,
				  char *remote, char *user);


#endif
