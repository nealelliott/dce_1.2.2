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
# ifndef RCS_agt_usrc_h_Included
# define RCS_agt_usrc_h_Included
 static char *RCSID_agt_usrc_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_usrc.h,v 1.1.2.2 1996/03/11 14:06:28 marty Exp $";
# endif /* Not RCS_agt_usrc_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_usrc.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:28  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:08  marty]
 *
 * Revision 1.1.2.1  1996/02/19  09:10:48  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:09:09  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  06:42:16  06:42:16  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  06:42:05  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  03:03:17  kishi
 * 	Change Copyright.
 * 	[1995/10/26  03:03:06  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:35:10  kishi
 * 	Initial revision
 * 	[1995/09/09  07:24:15  kishi]
 * 
 * $EndLog$
 */
#ifndef _USRC_H
#define _USRC_H

#define DFAM_OPEN 0x00000001
#define DFAM_CLOSE 0x00000002
#define DFAM_CREAT 0x00000003
#define DFAM_REMOVE 0x00000004
#define DFAM_RENAME 0x00000005
#define DFAM_READ 0x00000006
#define DFAM_WRITE 0x00000007
#define DFAM_MKDIR 0x00000008
#define DFAM_RMDIR 0x00000009
#define DFAM_SYSCALL 0x0000000a
#define DFAM_RIGHTS 0x0000000b
#define DFAM_OPENDIR 0x0000000c
#define DFAM_READSTAT 0x0000000d
#define DFAM_CLOSEDIR 0x0000000e
#define DFAM_TIME 0x0000000f
#define DFAM_DIRSYNC 0x00000010
#define DFAM_STAT 0x00000014
#define DFAM_UTIME 0x00000015
#define DFAM_CHOWN 0x00000016
#define DFAM_LingetId 0x00000017
#define DFAM_LinGetPasswd 0x00000018
#define DFAM_LinResetKey 0x00000019
#define DFAM_Lout 0x0000001a
#define DFAM_LSEEK 0x0000001b
#define DFAM_CPREAD 0x0000001c
#define DFAM_CPWRITE 0x0000001d
#define DFAM_CPWRITEEND 0x0000001e

#define DFAM_OPEN_R 0x00008001
#define DFAM_CLOSE_R 0x00008002
#define DFAM_CREAT_R 0x00008003
#define DFAM_REMOVE_R 0x00008004
#define DFAM_RENAME_R 0x00008005
#define DFAM_READ_R 0x00008006
#define DFAM_WRITE_R 0x00008007
#define DFAM_MKDIR_R 0x00008008
#define DFAM_RMDIR_R 0x00008009
#define DFAM_SYSCALL_R 0x0000800a
#define DFAM_RIGHTS_R 0x0000800b
#define DFAM_OPENDIR_R 0x0000800c
#define DFAM_READSTAT_R 0x0000800d
#define DFAM_CLOSEDIR_R 0x0000800e
#define DFAM_TIME_R 0x0000800f
#define DFAM_DIRSYNC_R 0x00008010
#define DFAM_STAT_R 0x00008014
#define DFAM_UTIME_R 0x00008015
#define DFAM_CHOWN_R 0x00008016
#define DFAM_LinGetId_R 0x00008017
#define DFAM_LinGetPasswd_R 0x00008018
#define DFAM_LinResetKey_R 0x00008019
#define DFAM_Lout_R 0x0000801a
#define DFAM_LSEEK_R 0x0000801b
#define DFAM_CPREAD_R 0x0000801c
#define DFAM_CPWRITE_R 0x0000801d
#define DFAM_CPWRITEEND_R 0x0000801e
#define AGT_GWMSG_ERRMSG 0x0000ffff

#define USRC_DOWN 0
#define MAX_SEND_LEN 4096
#define USRC_WAIT 1

struct osinf{
	int syscall;
	int derrno;
};
extern struct osinf oserr;

extern void agt_usrc_send_emsg(char *senddata,unsigned short command);
extern void agt_usrc_send_data(char *sendbuf,int senddatalen,unsigned short command);
extern void agt_usrc_main(void);
extern void agt_usrc_oserr_trace(int osinf,unsigned int derrno);
extern int ttraceflag;

#endif
