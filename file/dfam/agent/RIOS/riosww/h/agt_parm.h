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
# ifndef RCS_agt_parm_h_Included
# define RCS_agt_parm_h_Included
 static char *RCSID_agt_parm_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_parm.h,v 1.1.2.2 1996/03/11 14:06:13 marty Exp $";
# endif /* Not RCS_agt_parm_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_parm.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:13  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:55:57  marty]
 *
 * Revision 1.1.2.1  1996/02/19  09:32:53  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:32:31  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  05:51:43  05:51:43  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:51:34  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  02:27:14  kishi
 * 	Change Copyright.
 * 	[1995/10/26  02:27:03  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:39:04  kishi
 * 	Initial revision
 * 	[1995/09/09  07:20:06  kishi]
 * 
 * $EndLog$
 */

#ifndef _AGT_PARM_H
#define _AGT_PARM_H

#define D_OPT_INVALID       0
#define D_OPT_STOP          1
#define D_OPT_ABORT         2
#define D_OPT_CFGFILE       3
#define D_OPT_DFAMDBG       4
#define D_OPT_DFAMFG        5
#define D_OPT_MSG           6
#define D_OPT_TO            7
#define D_OPT_DFAMID        8
#define D_OPT_prmemtr       9
#define D_OPT_TRACEFLAG    10

#ifdef _DEBUG
  #define D_OPT_waitsignal  99
  #define D_OPT_loop_interval 98
  #define D_OPT_noexit      97
#endif

#define D_DFLT_STOP_TIME         60

#define D_CFG_MAXLEN        4095
#define D_CFG_FS            " \t"
#define D_CFG_31DAYS        2678400
#define D_CFG_MEG           1048576
#define D_CFG_KILO          1024

#define D_CFG_INVALID       0
#define D_CFG_HCI           1
#define D_CFG_HCT           2
#define D_CFG_TCPSDT        3
#define D_CFG_GW            4
#define D_CFG_AGTPORT       5
#define D_CFG_IPCID         6
#define D_CFG_MEMTRSZ       7
#define D_CFG_EVTRSZ        8
#define D_CFG_EVTRFL        9
#define D_CFG_PRTRSZ        10
#define D_CFG_TCPRDT        11
#define D_CFG_TGTRT         12

#define D_DFLT_PORTNO       9000
#define D_DFLT_IPCPATH 	    "/opt/dcelocal/var/dfa/.dfa"
#define D_DFLT_MEMTRSIZE    64
#define D_DFLT_EVTTRSIZE    512
#define D_DFLT_EVTTRFILES   2
#define D_DFLT_PRCTRSIZE    64
#define D_DFLT_HCINTERVAL   60
#define D_DFLT_HCTIMEOUT    20
#define D_DFLT_TCPSDT       20
#define D_DFLT_TCPRDT       60
#define D_DFLT_TGTRT        600

#define D_CFG_ERR_MISOPT    1
#define D_CFG_ERR_OFRANGE   2
#define D_CFG_ERR_INVCHAR   3
#define D_CFG_ERR_INTERNAL  4
#define D_CFG_ERR_GATEWAY   5



extern int agt_parm_cmd_line_opt(int argc, char *argv[]);
extern void agt_parm_config_file(void);

#endif

