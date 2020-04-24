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
# ifndef RCS_agt_acs_h_Included
# define RCS_agt_acs_h_Included
 static char *RCSID_agt_acs_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_acs.h,v 1.1.2.2 1996/03/11 14:05:56 marty Exp $";
# endif /* Not RCS_agt_acs_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_acs.h,v $
 * Revision 1.1.2.2  1996/03/11  14:05:56  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:55:44  marty]
 *
 * Revision 1.1.2.1  1996/02/19  10:10:26  dce-sdc
 * 	First Submit
 * 	[1996/02/19  10:10:05  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  05:20:47  05:20:47  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:20:38  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  01:39:00  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:38:51  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:43:54  kishi
 * 	Initial revision
 * 	[1995/09/09  07:14:15  kishi]
 * 
 * $EndLog$
 */

#ifndef _ACS_H
#define _ACS_H

#include <sys/errno.h>

#define SUCCESS 0
#define SSIZE 2
#define SIZE 4
#define HSIZE 8
#define MAX_PATH_LEN 1024
#define MAX_NAME_LEN 1024

extern int errno;
int d_errno;

#endif
