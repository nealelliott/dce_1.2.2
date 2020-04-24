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
# ifndef RCS_agt_lib_h_Included
# define RCS_agt_lib_h_Included
 static char *RCSID_agt_lib_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_lib.h,v 1.1.2.2 1996/03/11 14:06:08 marty Exp $";
# endif /* Not RCS_agt_lib_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_lib.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:08  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:55:53  marty]
 *
 * Revision 1.1.2.1  1996/02/19  09:43:30  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:42:58  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  05:37:43  05:37:43  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:37:31  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  02:11:26  kishi
 * 	Change Copyright.
 * 	[1995/10/26  02:11:17  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:40:47  kishi
 * 	Initial revision
 * 	[1995/09/09  07:18:31  kishi]
 * 
 * $EndLog$
 */

#ifndef _AGT_LIB_H
#define _AGT_LIB_H

extern long char_to_num(char *c, int n);
extern void long_to_char(char *c, long n);
extern void short_to_char(char *c, short n);
extern int GetIPaddr(char *host, unsigned long  *addr);
extern char *agt_get_gwname(struct in_addr ipaddr);
extern int GetLong(char *str, long  *longint);
extern void agt_abort_on_signal(int sig, int code, struct sigcontext *scp);
extern int agt_sleep(unsigned long sec);
extern void agt_do_nothing(void);

#endif
