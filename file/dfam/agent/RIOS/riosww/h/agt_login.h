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
# ifndef RCS_agt_login_h_Included
# define RCS_agt_login_h_Included
 static char *RCSID_agt_login_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_login.h,v 1.1.2.2 1996/03/11 14:06:09 marty Exp $";
# endif /* Not RCS_agt_login_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_login.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:09  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:55:54  marty]
 *
 * Revision 1.1.2.1  1996/02/19  09:42:04  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:41:31  dce-sdc]
 * 
 * Revision 1.1.2.6  96/01/12  05:40:25  05:40:25  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:40:11  kishi]
 * 
 * Revision 1.1.2.5  1996/01/12  05:39:05  kishi
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:38:58  kishi]
 * 
 * Revision 1.1.2.4  1995/10/26  02:13:25  kishi
 * 	Change Copyright.
 * 	[1995/10/26  02:13:15  kishi]
 * 
 * Revision 1.1.2.3  1995/09/28  13:29:29  kishi
 * 	Change D_KEY_SIZE 4 -> 8 , D_KEY_NUM 8 -> 4. by takano
 * 	[1995/09/28  13:29:15  kishi]
 * 
 * Revision 1.1.2.2  1995/09/19  09:30:33  kishi
 * 	Change D_KEY_SIZE Value 8 -> 4
 * 	[1995/09/19  09:30:08  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:40:12  kishi
 * 	Initial revision
 * 	[1995/09/09  07:18:51  kishi]
 * 
 * $EndLog$
 */

#ifndef _AGT_LOGIN_H
#define _AGT_LOGIN_H

#include<sys/types.h>
#include"agt_errno.h"

#define D_NWSRV_NAME_MAX 48

#define D_UNAME_MAX 	1025
#define D_PASSWD_MAX 	1025
#define D_AL_TRC_SIZE	1056
#define D_AL_UXMSG_MAX	2048
#define D_RESP_LEN 	200
#define D_MKF_NAME_MAX	1024
#define D_GWMSG_MAX	1024

#define D_KEY_SIZE 	8
#define D_EKEY_NUM 	4

#define D_LOGIN_ERROR 	-1
#define D_LOGIN_OK 	0

#define D_MASTERKEY_FILE "/opt/dcelocal/var/dfa/dfakey"

struct d_login_info{
	char 		uname[D_UNAME_MAX];
	int 		uid;
	int		linflag;
	unsigned long	nextkey[2];
	unsigned long 	randval[2];
	char 		nwsrvname[D_NWSRV_NAME_MAX];

	char 		passwd[D_PASSWD_MAX];
	int		pswdlen;
	int		conn;
	long		TGTlimit;
};

struct d_passwd {
	char 	*pw_name;
	char 	*pw_passwd;
	int  	pw_uid;
	int  	pw_gid;
	time_t 	pw_change;
	char 	*pw_class;
	char 	*gecos;
	char 	*pw_dir;
	char 	pw_shell;
	time_t  pw_expire;
};

extern int LinGetId(char *data);
extern int LinGetPasswd(char *data);
extern int LinResetKey(void);
extern int Lout(void);

extern long TGTRefreshTime;

#endif
