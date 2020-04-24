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
# ifndef RCS_agt_debug_h_Included
# define RCS_agt_debug_h_Included
 static char *RCSID_agt_debug_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_debug.h,v 1.1.2.2 1996/03/11 14:06:00 marty Exp $";
# endif /* Not RCS_agt_debug_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_debug.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:00  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:55:48  marty]
 *
 * Revision 1.1.2.1  1996/02/19  10:03:27  dce-sdc
 * 	First Submit
 * 	[1996/02/19  10:02:53  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  05:27:22  05:27:22  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:27:13  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  01:52:19  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:51:41  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:42:44  kishi
 * 	Initial revision
 * 	[1995/09/09  07:15:28  kishi]
 * 
 * $EndLog$
 */

void *abcdefgp;

#define malloc(x)   (syslog(LOG_ERR, "%s(%i) malloc size %i", __FILE__, __LINE__, (x)), abcdefgp=malloc((x)), syslog(LOG_ERR, "%s(%i) malloced 0x%08x", __FILE__, __LINE__, abcdefgp), (void *)abcdefgp)
#define free(x)     (syslog(LOG_ERR, "%s(%i) free 0x%08x", (x)), __FILE__, __LINE__, free((x)))
#define realloc(x, y)  (syslog(LOG_ERR, "%s(%i) realloc  0x%08x size %i, ", __FILE__, __LINE__, (x), (y)), abcdefgp=realloc((x),(y)), syslog(LOG_ERR, "%s(%i) realloced 0x%08x", __FILE__, __LINE__, abcdefgp), (void *)abcdefgp)


