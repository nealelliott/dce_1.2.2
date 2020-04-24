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
# ifndef RCS_agt_file_h_Included
# define RCS_agt_file_h_Included
 static char *RCSID_agt_file_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_file.h,v 1.1.2.2 1996/03/11 14:06:05 marty Exp $";
# endif /* Not RCS_agt_file_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_file.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:05  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:55:51  marty]
 *
 * Revision 1.1.2.1  1996/02/19  09:47:38  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:47:09  dce-sdc]
 * 
 * Revision 1.1.2.4  96/01/12  05:32:06  05:32:06  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:31:57  kishi]
 * 
 * Revision 1.1.2.3  1995/11/09  06:07:15  ichikawa
 * 	Define DPATH_MAX and DNAME_MAX.
 * 	[1995/11/09  06:07:00  ichikawa]
 * 
 * Revision 1.1.2.2  1995/10/26  01:59:47  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:59:40  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:41:39  kishi
 * 	Initial revision
 * 	[1995/09/09  07:17:30  kishi]
 * 
 * $EndLog$
 */


#ifndef _FILE_H
#define _FILE_H

#include <dirent.h>
#include "agt_usrc.h"

#define NOCHECK 0
#define CHECK   1
#define FILE 0
#define DIRECTORY 1
#define ONLYDIR 0
#define DS_NOMODE  0x0000
#define DS_DENYRD  0x0001
#define DS_DENYWR  0x0002
#define DS_DENYRW  0x0003
#define DS_COMPATI 0x0004
#define DS_DENYRD_COM 0x0005
#define DS_DENYWR_COM 0x0006
#define DS_DENYRW_COM 0x0007
#define GID_NO_CHANGE ((gid_t) -1)
#define DPATH_MAX	255
#define DNAME_MAX	1024

struct pathlist {
	unsigned int pathlen;
	char *fullpath;
	DIR *diraddress;
	struct pathlist *next;
};

struct fdlist {
	int fd;
	struct fdlist *next;
};

extern int agt_file_open(char *str);
extern int agt_file_close(char *str);
extern int agt_file_creat(char *str);
extern int agt_file_remove(char *str);
extern int agt_file_rename(char *str);
extern int agt_file_read(char *str);
extern int agt_file_write(char *str);
extern int agt_file_mkdir(char *str);
extern int agt_file_rmdir(char *str);
extern int agt_file_acl_syscall(char *str);
extern int agt_file_opendir(char *str);
extern int agt_file_readstat(char *str);
extern int agt_file_closedir(char *str);
extern int agt_file_gettime(char *str);
extern int agt_file_dirsync(char *str);
extern int agt_file_rights(char *str);
extern int agt_file_lseek(char *str);
extern int agt_file_cpread(char *str);
extern int agt_file_cpwrite(char *str);
extern int agt_file_cpwrite_end(char *str);


#endif
