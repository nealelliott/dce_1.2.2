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
# ifndef RCS_agt_wchd_h_Included
# define RCS_agt_wchd_h_Included
 static char *RCSID_agt_wchd_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_wchd.h,v 1.1.2.2 1996/03/11 14:06:29 marty Exp $";
# endif /* Not RCS_agt_wchd_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_wchd.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:29  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:08  marty]
 *
 * Revision 1.1.2.1  1996/02/19  09:08:07  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:07:41  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  06:43:44  06:43:44  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  06:43:31  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  03:04:59  kishi
 * 	Change Copyright.
 * 	[1995/10/26  03:04:45  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:34:55  kishi
 * 	Initial revision
 * 	[1995/09/09  07:24:32  kishi]
 * 
 * $EndLog$
 */

#ifndef _AGT_WCHD_H
#define _AGT_WCHD_H

#include "agt_timer.h"

struct d_wchd {
    char tableid[4];

    struct agt_timer *timerhcint;
    struct agt_timer *timerhcwait;


    unsigned long hcinterval;
    unsigned long hctimeout;
};

extern struct d_wchd wchd;

extern void agt_wchd(void);

#endif
