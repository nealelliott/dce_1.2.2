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
# ifndef RCS_agt_ipc_h_Included
# define RCS_agt_ipc_h_Included
 static char *RCSID_agt_ipc_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_ipc.h,v 1.1.2.2 1996/03/11 14:06:07 marty Exp $";
# endif /* Not RCS_agt_ipc_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_ipc.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:07  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:55:52  marty]
 *
 * Revision 1.1.2.1  1996/02/19  09:44:55  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:44:34  dce-sdc]
 * 
 * Revision 1.1.2.6  96/01/12  05:36:34  05:36:34  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:36:27  kishi]
 * 
 * Revision 1.1.2.5  1995/10/26  02:08:33  kishi
 * 	Delete merge log.
 * 	[1995/10/26  02:08:11  kishi]
 * 
 * Revision 1.1.2.4  1995/10/26  02:04:59  kishi
 * 	Change Copyright.
 * 	[1995/10/26  02:04:04  kishi]
 * 
 * Revision 1.1.2.3  1995/09/19  07:40:01  kishi
 * 	Merged with changes from 1.1.2.2
 * 	[1995/09/19  07:39:58  kishi]
 * 
 * 	Change prot-version 0100->0200.
 * 	[1995/09/19  07:39:01  kishi]
 * 
 * Revision 1.1.2.2  1995/09/19  07:38:33  ichikawa
 * 	D_GW_PROTVER change from 0100 to 0200.
 * 	[1995/09/19  07:38:19  ichikawa]
 * 
 * Revision 1.1.2.1  1995/09/09  07:41:04  kishi
 * 	Initial revision
 * 	[1995/09/09  07:18:10  kishi]
 * 
 * $EndLog$
 */

#ifndef _AGT_IPC_H
#define _AGT_IPC_H

#define D_UXMSG_MAX         2048
#define D_UXMSG_REPLY_MAX   12

#define D_UXSOC_PATH        "/opt/dcelocal/var/dfa/.dfa"
#define D_UXSOC_CMD_PATH    "/opt/dcelocal/var/dfa"

#define D_UXSOC_MAIN        "dfM"
#define D_UXSOC_SYSC        "dfS"
#define D_UXSOC_TRAC        "dfT"
#define D_UXSOC_CHLD        "dfa"
#define D_UXSOC_CMND        "dfa"

#define D_AG_STOP           0x00000001
#define D_AG_CSTOP          0x00080001
#define D_AG_ABORT          0x00000002
#define D_AG_ABORT_CMD               0
#define D_AG_ABORT_TERM              1
#define D_AG_CABORT         0x00080002
#define D_AG_MSG            0x00000003
#define D_AG_WMSG           0x00010003
#define D_AG_CMSG           0x00080003
#define D_AG_STPTRC         0x00000004
#define D_AG_TRACE          0x00000005
#define D_AG_TRDY           0x00020006
#define D_AG_SRDY           0x00010006
#define D_AG_GWDOWN         0x00000007
#define D_AG_COMRTN         0x00000008
#define D_AG_CONNEST        0x00000009
#define D_AG_LOGIN          0x0000000a

#define D_AG_RT_OK          0
#define D_AG_RT_INIT        1
#define D_AG_RT_EPRM        2
#define D_AG_RT_STOP        3
#define D_AG_RT_ABORT       4

#define D_GW_CONN           0x0011
#define D_GW_CON_CTRL       1
#define D_GW_CON_WCHD       2
#define D_GW_CON_USER       3
#define D_GW_PROTVER        0x0200
#define D_GW_CONNR          0x8011

#define D_GW_HCHK           0x0012
#define D_GW_HCHKR          0x8012

#define D_GW_MSG            0x0013
#define D_GW_MSG_STOP       1
#define D_GW_MSG_ABRT       2
#define D_GW_MSG_CNSL       3
#define D_GW_MSG_CLIE       4

#endif
