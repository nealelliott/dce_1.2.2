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
# ifndef RCS_agt_sdk_h_Included
# define RCS_agt_sdk_h_Included
 static char *RCSID_agt_sdk_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/setdfakey/agt_sdk.h,v 1.1.2.2 1996/03/09 20:44:32 marty Exp $";
# endif /* Not RCS_agt_sdk_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_sdk.h,v $
 * Revision 1.1.2.2  1996/03/09  20:44:32  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:21  marty]
 *
 * Revision 1.1.2.1  1996/02/16  04:54:36  dce-sdc
 * 	First Submit
 * 	[1996/02/16  04:54:02  dce-sdc]
 * 
 * Revision 1.1.2.7  96/01/12  06:58:33  06:58:33  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  06:58:25  kishi]
 * 
 * Revision 1.1.2.6  1995/10/25  12:16:50  kishi
 * 	Change Copyright.
 * 	[1995/10/25  12:16:40  kishi]
 * 
 * Revision 1.1.2.5  1995/09/29  01:21:09  kishi
 * 	Change D_KEY_SIZE 4->8.
 * 	[1995/09/29  01:20:32  kishi]
 * 
 * Revision 1.1.2.4  1995/09/26  11:11:58  kishi
 * 	Delete merge log.
 * 	[1995/09/26  11:09:50  kishi]
 * 
 * Revision 1.1.2.3  1995/09/19  07:44:04  takano
 * 	Merged with changes from 1.1.2.2
 * 	[1995/09/19  07:44:02  takano]
 * 
 * 	Delete #ifdef
 * 	[1995/09/19  07:41:37  takano]
 * 
 * Revision 1.1.2.2  1995/09/19  07:36:12  kishi
 * 	Change D_KEY_SIZE. 8 -> 4
 * 	[1995/09/19  07:36:00  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:55:26  kishi
 * 	Initial revision
 * 	[1995/09/09  07:54:41  kishi]
 * 
 * $EndLog$
 */

#ifndef _AGT_SDK_H
#define _AGT_SDK_H

#define D_MASTERKEY_FILE 	"/opt/dcelocal/var/dfa/dfakey"

#define D_IPC_ACCESS_MODE	0440
#define D_DFLT_IPCPATH		"/opt/dcelocal/var/dfa/.dfa"
#define D_ID_SEM		'A'
#define D_NUM_SEM		2

#define D_KEY_SIZE		8
#define D_NWSRV_NAME_MAX	48
#define D_SDK_MSG_MAX 		1024
#define D_INPUTBUF_SIZE		1024
#define D_LINE_MAX 		128

#define D_ADDKEY_ON		1
#define D_ADDKEY_OFF		0

#define D_TMP_FILE 	"/opt/dcelocal/var/dfa/._dfamkeyp.bak"

#define SDK_ADD 	1
#define SDK_DELETE 	2
#define SDK_LIST 	3
#define NOT_CMD 	4
#define DFAM_ID 	5

#endif
