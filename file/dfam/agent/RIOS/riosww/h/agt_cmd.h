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
# ifndef RCS_agt_cmd_h_Included
# define RCS_agt_cmd_h_Included
 static char *RCSID_agt_cmd_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_cmd.h,v 1.1.2.2 1996/03/11 14:05:58 marty Exp $";
# endif /* Not RCS_agt_cmd_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_cmd.h,v $
 * Revision 1.1.2.2  1996/03/11  14:05:58  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:55:46  marty]
 *
 * Revision 1.1.2.1  1996/02/19  10:06:46  dce-sdc
 * 	First Submit
 * 	[1996/02/19  10:06:12  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  05:25:08  05:25:08  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:24:54  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  01:47:23  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:47:10  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:43:20  kishi
 * 	Initial revision
 * 	[1995/09/09  07:14:53  kishi]
 * 
 * $EndLog$
 */

#ifndef _AGT_CMD_H
#define _AGT_CMD_H

struct d_cmd {
    unsigned long stoptime;
    char *mesg;
    unsigned long (*dest);
    long destnum;
};

extern struct d_cmd cmd;

extern void agt_cmd_stop(void);
extern void agt_cmd_abort(void);
extern void agt_cmd_send(void);
extern void agt_cmd_prmemtr(void);

#endif
