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
# ifndef RCS_agt_dsync_h_Included
# define RCS_agt_dsync_h_Included
 static char *RCSID_agt_dsync_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_dsync.h,v 1.1.2.2 1996/03/11 14:06:01 marty Exp $";
# endif /* Not RCS_agt_dsync_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_dsync.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:01  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:55:48  marty]
 *
 * Revision 1.1.2.1  1996/02/19  10:01:53  dce-sdc
 * 	First Submit
 * 	[1996/02/19  10:01:34  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  05:28:25  05:28:25  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:28:15  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  01:54:27  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:53:58  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:42:28  kishi
 * 	Initial revision
 * 	[1995/09/09  07:15:46  kishi]
 * 
 * $EndLog$
 */

#ifndef _DSYNC_H
#define _DSYNC_H
#include <dirent.h>

#define MIDDLE 0xffffffff

extern size_t dirsyncfilelen;
struct stat filebuf;
extern time_t checktime;
DIR *dirsyncp;
extern struct stat filebuf;
extern time_t cktime;
extern int rflag;

struct fullpathinf{
	char *fullpath;
	unsigned int count;
};
extern struct fullpathinf *fullpathp;

struct fullpathlist{
	char *dirname;
	struct fullpathinf *addressinf;
	struct fullpathlist *next;
};
extern struct fullpathlist *pathinf,*old,*head,*refer;

struct symlist{
	ino_t inode_num;
	struct symlist *next;
};
extern struct symlist *symhead;


#endif
