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
# ifndef RCS_agt_acl_h_Included
# define RCS_agt_acl_h_Included
 static char *RCSID_agt_acl_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_acl.h,v 1.1.2.2 1996/03/11 14:05:55 marty Exp $";
# endif /* Not RCS_agt_acl_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_acl.h,v $
 * Revision 1.1.2.2  1996/03/11  14:05:55  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:55:44  marty]
 *
 * Revision 1.1.2.1  1996/02/19  10:12:00  dce-sdc
 * 	First Submit
 * 	[1996/02/19  10:11:38  dce-sdc]
 * 
 * Revision 1.1.2.4  96/01/12  05:19:42  05:19:42  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:19:32  kishi]
 * 
 * Revision 1.1.2.3  1995/11/10  06:19:54  ichikawa
 * 	Length of pathname is changed.
 * 	[1995/11/10  06:19:02  ichikawa]
 * 
 * Revision 1.1.2.2  1995/10/26  01:36:58  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:36:51  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:44:11  kishi
 * 	Initial revision
 * 	[1995/09/09  07:13:53  kishi]
 * 
 * $EndLog$
 */

#ifndef _ACL_H
#define _ACL_H

#include "agt_acs.h"
#include "agt_login.h"
#include "agt_agent.h"

#define VNX_OPCODE_DELACL 2
#define ACL_SIZE 8*1024
#define USIZE 16
#define USER 0x00001
#define GROUP 0x0002
#define BOTH 0x0003
#define DFA_OTHER_GOURP "DFA_OTHER_GOURP"
#define DFA_MASK_OBJ "DFA_MASK_OBJ"
#define DFA_OTHER_GROUP "DFA_OTHER_GROUP"
#define VNX_OPCODE_GETACL 0
#define VNX_OPCODE_SETACL 1
#define VNX_OPCODE_DELACL 2

error_status_t status;
sec_login_handle_t      login_context;
sec_rgy_handle_t context;
extern unsigned short user_type;
extern unsigned short group_type;
extern int whichacl;
char pathname[PATH_MAX];
char *sendacl;
char *begin_sendacl;
extern int call_type;
extern unsigned int length;
extern unsigned short option;
extern int overflag;
extern int setentry_type;
char *cellname;
extern sec_rgy_name_t globalname;
extern int acl_size;
char *acl;
char *begin_acl;
char *dfauserobj;
char *dfausername;
char *dfagroupobj;
char *dfagroupname;
extern int dfauseroflag;
extern int dfagroupoflag;
extern unsigned int trusty_bit;
extern uuid_t owner_uuid;
extern uuid_t ownerg_uuid;
extern uuid_t cell_uuid;
extern sec_acl_handle_t h;

extern int agt_acl_main(char *str);
extern int agt_acl_rights(char *str);
#endif
