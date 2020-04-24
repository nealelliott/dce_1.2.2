/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tsh_misc.h,v $
 * Revision 1.2.4.2  1996/02/18  23:07:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:48  marty]
 *
 * Revision 1.2.4.1  1995/12/11  21:50:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:53:09  root]
 * 
 * Revision 1.2.2.3  1993/01/29  21:12:56  cuti
 * 	Add LOGIN_GET_EFFECTIVE_OK & LOGIN_GET_EFFECTIVE_BAD_ACCT
 * 	[1993/01/28  16:07:28  cuti]
 * 
 * Revision 1.2.2.2  1992/12/31  17:33:56  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:58:50  htf]
 * 
 * Revision 1.2  1992/01/19  22:14:41  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
*/
/*
 *  OSF DCE Version 1.0 
*/
/* 
 * Copyright 1991 by Apollo Computer, Inc.
 * 
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of Apollo Computer, Inc not
 * be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 * Apollo Computer, Inc. makes no representations about the
 * suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 * 
 * src/test/security/api/moretests/tsh_misc.h, , dce_osf, dce.74c
 *   
 *	Extracted	12/19/91 12:39:47
 *	from delta on	11/13/91 14:19:26
 *
 *  $LOG$
 *
 *
*/

#include <tsh_common.h>
#include <dce/rgybase.h>
#include <dce/acct.h>

#ifndef NULL
#define NULL 0
#endif

/* %*START*% Start the body of code that will get translated into the FILE.tshrc */

#define LOGIN_GET_INFO_OK		    	100
#define LOGIN_GET_INFO_BAD_ACCT	    	101
#define LOGIN_GET_EFFECTIVE_OK	    	102
#define LOGIN_GET_EFFECTIVE_BAD_ACCT	103
#define RGY_SET_RESOLVE_MODE_ON     	200
#define RGY_SET_RESOLVE_MODE_OFF    	202
#define RGY_WAIT_UNTIL_CONSISTENT		300
#define RGY_SET_CACHE_HANDLER_ON    	400
#define RGY_SET_CACHE_HANDLER_OFF   	401
#define RGY_CURSOR_RESET            500



/* %*STOP*%  Stop translating for FILE.tshrc  */

ndr_long_int RegistryOpen
(
#ifdef __STDC__
sec_rgy_handle_t   *context 	/* sec_rgy_site_open */
#endif
);
ndr_long_int RegistryClose
/* Formal parameters */
(
#ifdef __STDC__
sec_rgy_handle_t   context
#endif
);

ndr_long_int AddPGOItem
/* Formal parameters */
(              
#ifdef __STDC__
sec_rgy_handle_t context,	/* rgy context */
sec_rgy_domain_t itemDomain,
sec_rgy_name_t *itemName,
uuid_t *itemId,
signed32 itemUnixId,
signed32 itemQuota,
sec_rgy_pgo_flags_t itemFlags,
sec_rgy_pname_t *itemFullname
#endif
);

#define AddPerson(context,  itemName,  itemId,  itemUnixId,  itemQuota,  itemFlags,  itemFullname ) AddPGOItem(context, sec_rgy_domain_person, itemName,  itemId,  itemUnixId,  itemQuota,  itemFlags,  itemFullname )
#define AddGroup(context,   itemName,  itemId,  itemUnixId,  itemQuota,  itemFlags,  itemFullname ) AddPGOItem(context, sec_rgy_domain_group, itemName,  itemId,  itemUnixId,  itemQuota,  itemFlags,  itemFullname )
#define AddOrg(context,   itemName,  itemId,  itemUnixId,  itemQuota,  itemFlags,  itemFullname ) AddPGOItem(context, sec_rgy_domain_org, itemName,  itemId,  itemUnixId,  itemQuota,  itemFlags,  itemFullname )


ndr_long_int DeletePGOItem
/* Formal parameters */
(
#ifdef __STDC__
sec_rgy_handle_t context,	/* rgy context */
sec_rgy_domain_t itemDomain,
sec_rgy_name_t *itemName
#endif
);

ndr_long_int AddMember
(
#ifdef __STDC__
sec_rgy_handle_t context,
sec_rgy_domain_t name_domain,
sec_rgy_name_t *go_name,
sec_rgy_name_t *person_name
#endif
);

#define AddGroupMember(context,goName,personName)  AddMember(context, sec_rgy_domain_group, goName,  personName )
#define AddOrgMember(context,goName,personName)    AddMember(context, sec_rgy_domain_org, goName,  personName )


ndr_long_int DeleteMember
(
#ifdef __STDC__
sec_rgy_handle_t context,
sec_rgy_domain_t name_domain,
sec_rgy_name_t *go_name,
sec_rgy_name_t *person_name
#endif
);

#define DeleteGroupMember(context,  goName,  personName) DeleteMember(context, sec_rgy_domain_group, goName,  personName )
#define DeleteOrgMember(context,  goName,  personName) DeleteMember(context, sec_rgy_domain_org, goName,  personName )


ndr_long_int CreateAccount
(
#ifdef __STDC__
sec_rgy_handle_t context,	/* rgy context */
sec_rgy_login_name_t *login_name,
sec_rgy_pname_t *gecos,
sec_rgy_pname_t *homedir,
sec_rgy_pname_t *shell,
idl_char        *passwd
#endif
);

ndr_long_int LookupAccount
(
#ifdef __STDC__
sec_rgy_handle_t context,	/* rgy context */
sec_rgy_login_name_t *name_key,
sec_rgy_cursor_t *account_cursor,
sec_rgy_login_name_t *name_result,
sec_rgy_sid_t *id_sid,
sec_rgy_unix_sid_t *unix_sid,
sec_rgy_acct_key_t *key_parts,
sec_rgy_acct_user_t *user_part,
sec_rgy_acct_admin_t *admin_part
#endif
);

ndr_long_int DeleteAccount
(
#ifdef __STDC__
sec_rgy_handle_t context,	/* rgy context */
sec_rgy_login_name_t *name_key
#endif
);


/* Constants for use with sec_rgy rpc's */

#define ACCT_PERSON_NAME "finger\0"

#define ACCT_PERSON_FULLNAME "alan finger\0"

#define ACCT_PERSON_PASSWD "squirrel\0"

#define ACCT_PERSON_REPL_PASSWD "replpswd\0"

#define ACCT_GROUP_NAME	"ccd_e\0"

#define ACCT_GROUP_FULLNAME	"ccd east\0"

#define ACCT_ORG_NAME	"testing\0"

#define ACCT_ORG_FULLNAME	"quality R Us\0"

#define ACCT_ALT_ORG_NAME "unique\0"

#define ACCT_KEY_ORG_NAME "newkey\0"

#define ACCT_GECOS            "This space for rent\0"

#define ACCT_HOMEDIR          "/usr\0"

#define ACCT_SHELL            "/bin/csh\0"

#define ACCT_PERSON_PASSWORD  "1q2w3e4r5t6t"

sec_rgy_login_name_t ACCT_LOGIN_NAME = {"finger\0", "ccd_e\0", "testing\0"};

sec_rgy_login_name_t ACCT_NEW_LOGIN_NAME = {"marsha\0", "locus\0", "quality\0"};

sec_rgy_login_name_t ACCT_ALT_LOGIN_NAME = {"finger\0", "locus\0", "unique\0"};

sec_rgy_login_name_t ACCT_NON_LOGIN_NAME = {"marsha\0", "none\0", "none\0"};

sec_rgy_login_name_t ACCT_KEY_LOGIN_NAME = {"finger\0", "ccd_e\0", "newkey\0"};

#define AUTH_USERNAME	(unsigned_char_p_t)"tester\0"

#define AUTH_PASSWD	(unsigned_char_p_t)"qualjob1\0"

#define AUTH_REALM	(unsigned_char_p_t)"testcell\0"

