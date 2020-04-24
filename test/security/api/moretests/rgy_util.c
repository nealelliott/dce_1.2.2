/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rgy_util.c,v $
 * Revision 1.2.4.2  1996/02/17  23:25:44  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:50  marty]
 *
 * Revision 1.2.4.1  1995/12/11  21:50:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:55  root]
 * 
 * Revision 1.2.2.3  1993/02/25  22:08:18  cuti
 * 	Change sec_rgy_site_open() to sec_rgy_site_open_update() for using site update operation.
 * 	[1993/02/25  16:33:30  cuti]
 * 
 * Revision 1.2.2.2  1992/12/31  17:32:52  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:58:06  htf]
 * 
 * Revision 1.2  1992/01/19  22:14:40  devrcs
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
 *
 *   
 *	Extracted	11/22/91 12:53:19
 *	from delta on	11/13/91 14:18:55
 *
 *  $LOG$
 *
 *
*/

#ifndef LINT
static char sccs_id[] = "src/test/security/api/moretests/rgy_util.c, , dce_osf, dce.70d";
#endif

/* Include files */

#include <tsh_common.h>
#include <dce/rgybase.h>
#include <dce/acct.h>
#include <dce/passwd.h>


#ifndef NULL
#define NULL 0
#endif


extern CtxBlock  *lCtx;

/******************************************************************
**
** Open the registry
**
*/

ndr_long_int RegistryOpen
/* Formal parameters */
#ifdef __STDC__
(
sec_rgy_handle_t   *context 	/* sec_rgy_site_open_update */
)
#else
( context ) 
sec_rgy_handle_t   *context;
#endif
{


/* Local variables */
   error_status_t	tstatus;	/* test status */
   sec_rgy_pname_t	node_name;	/* gethostname */
   sec_rgy_pname_t	site_name;	/* sec_rgy_site_open_update */


/*
   gethostname (node_name, sec_rgy_pname_t_size);
   sprintf ((char *)site_name, "ip:%s", node_name);
*/
   sprintf ((char *)site_name, "");

   sec_rgy_site_open_update (site_name,
                      context,
                      &tstatus);

   TSHMessage (lCtx, MOTRACE,
               "sec_rgy_site_open_update status: %d\n", tstatus);

   TSHStatus (lCtx, SETUP,
                  "sec_rgy_site_open_update failed\n", rpc_s_ok, tstatus);
     return (tstatus);

}

/******************************************************************
**
** Close the registry
**
*/
ndr_long_int RegistryClose
/* Formal parameters */
#ifdef __STDC__
(
sec_rgy_handle_t   context
)
#else
( context ) 
sec_rgy_handle_t   context;
#endif
{

/* Local variables */
   error_status_t	tstatus;	/* test status */

   sec_rgy_site_close (context, &tstatus);

   TSHMessage (lCtx, MOTRACE,
               "sec_rgy_site_close status: %d\n", tstatus);

   TSHStatus (lCtx, CLNUP,
                  "sec_rgy_site_close failed\n", rpc_s_ok, tstatus);

   return tstatus;

}


/******************************************************************
**
** Add a pgo item to the registry
**
*/

ndr_long_int AddPGOItem
/* Formal parameters */
#ifdef __STDC__
(
sec_rgy_handle_t context,	/* rgy context */
sec_rgy_domain_t itemDomain,
sec_rgy_name_t *itemName,
uuid_t *itemId,
signed32 itemUnixId,
signed32 itemQuota,
sec_rgy_pgo_flags_t itemFlags,
sec_rgy_pname_t *itemFullname
)
#else
( context,  itemDomain, itemName,  itemId,  itemUnixId,  itemQuota,  itemFlags,  itemFullname )
sec_rgy_handle_t  context;
sec_rgy_domain_t itemDomain;
sec_rgy_name_t *itemName;
uuid_t *itemId;
signed32 itemUnixId;
signed32 itemQuota;
sec_rgy_pgo_flags_t itemFlags;
sec_rgy_pname_t *itemFullname;
#endif
{


/* Local variables */
   error_status_t	tstatus;	/* test status */
   sec_rgy_pgo_item_t	pgo_item;	/* sec_rgy_pgo_add */


   bzero ((char *)&pgo_item, sizeof (pgo_item));

   if ( itemId == NULL )
      uuid_create_nil(&pgo_item.id, &tstatus);
   else
      memcpy((char *) &pgo_item.id, (char *) itemId, sizeof(pgo_item.id) );

   pgo_item.unix_num = itemUnixId; 
   pgo_item.quota = itemQuota;
   pgo_item.flags = itemFlags;

   memcpy ((char *) pgo_item.fullname, (char *) itemFullname, strlen(itemFullname) );
                                                 

   sec_rgy_pgo_add (context, itemDomain,itemName,&pgo_item,&tstatus);

   TSHMessage (lCtx, MOTRACE,
               "sec_rgy_pgo_add status: %d\n", tstatus);

   TSHStatus (lCtx, SETUP,
                  "sec_rgy_pgo_add failed\n", rpc_s_ok, tstatus);

   return tstatus;

   }


/******************************************************************
**
** delete a pgo item from the registry
**
*/

ndr_long_int DeletePGOItem
/* Formal parameters */
#ifdef __STDC__
(
sec_rgy_handle_t context,	/* rgy context */
sec_rgy_domain_t itemDomain,
sec_rgy_name_t *itemName
)
#else
( context,  itemDomain, itemName,  itemId,  itemUnixId,  itemQuota,  itemFlags,  itemFullname )
sec_rgy_handle_t  context;
sec_rgy_domain_t itemDomain;
sec_rgy_name_t *itemName;
#endif
{

/* Local variables */
   error_status_t	tstatus;	/* test status */


   sec_rgy_pgo_delete (context, itemDomain, itemName, &tstatus);

   TSHMessage (lCtx, MOTRACE,
               "sec_rgy_pgo_delete status: %d\n", tstatus);

   TSHStatus (lCtx, SETUP,
                  "sec_rgy_pgo_delete failed\n", rpc_s_ok, tstatus);

   return tstatus;

   }

/******************************************************************
**
** Add a member to a group or org
**
*/
ndr_long_int AddMember
#ifdef __STDC__
(
sec_rgy_handle_t context,
sec_rgy_domain_t name_domain,
sec_rgy_name_t *go_name,
sec_rgy_name_t *person_name
)
#else
( context,  name_domain,  go_name,  person_name)
sec_rgy_handle_t context;
sec_rgy_domain_t name_domain;
sec_rgy_name_t *go_name;
sec_rgy_name_t *person_name;
#endif
   {
/* Local variables */
   error_status_t	tstatus;	/* test status */

   sec_rgy_pgo_add_member(context, name_domain, go_name, person_name, &tstatus );

   TSHMessage (lCtx, MOTRACE,
               "sec_rgy_pgo_add_member status: %d\n", tstatus);

   TSHStatus (lCtx, SETUP,
                  "sec_rgy_pgo_add_member failed\n", rpc_s_ok, tstatus);

   return tstatus;
   }
/******************************************************************
**
** delete a member of a group or org
**
*/
ndr_long_int DeleteMember
#ifdef __STDC__
(
sec_rgy_handle_t context,
sec_rgy_domain_t name_domain,
sec_rgy_name_t go_name,
sec_rgy_name_t person_name
)
#else
( context,  name_domain,  go_name,  person_name)
sec_rgy_handle_t context;
sec_rgy_domain_t name_domain;
sec_rgy_name_t go_name;
sec_rgy_name_t person_name;
#endif
    {
/* Local variables */
   error_status_t	tstatus;	/* test status */
          
   sec_rgy_pgo_delete_member(context, name_domain, go_name, person_name, &tstatus );

   TSHMessage (lCtx, MOTRACE,
               "sec_rgy_pgo_delete_member status: %d\n", tstatus);

   TSHStatus (lCtx, SETUP,
                  "sec_rgy_pgo_delete_member failed\n", rpc_s_ok, tstatus);

   return tstatus;
   }

/******************************************************************
**
** Create an account
**
*/

ndr_long_int CreateAccount
/* Formal parameters */
#ifdef __STDC__
(
sec_rgy_handle_t context,	/* rgy context */
sec_rgy_login_name_t *login_name, 
sec_rgy_pname_t *gecos,
sec_rgy_pname_t *homedir,
sec_rgy_pname_t *shell,
idl_char        *passwd
)
#else
( context,  login_name, gecos,  homedir,  shell,  passwd )
sec_rgy_handle_t   context;
sec_rgy_login_name_t *login_name;
sec_rgy_pname_t *gecos;
sec_rgy_pname_t *homedir;
sec_rgy_pname_t *shell;
idl_char        *passwd;
#endif

{

/* Local variables */
   error_status_t	tstatus;	/* test status */
   sec_rgy_acct_key_t	key_parts;
   sec_rgy_acct_user_t  user_part;
   sec_rgy_acct_admin_t admin_part;

   sec_passwd_rec_t        caller_key;
   sec_passwd_rec_t        new_key;
   sec_passwd_version_t    new_key_version;
   unsigned_char_t         plain_buff[sec_passwd_str_t_size];

/* Setup the parameters to the sec_rgy_acct_add function. */

   bzero ((char *)&user_part, sizeof (user_part));

   user_part.passwd_version_number = 0;
   user_part.passwd_dtm = time(NULL);
   user_part.flags = 0;

   strncpy ((char *)user_part.gecos,
	   (char *) gecos,
	   sizeof (user_part.gecos));

   strncpy ((char *)user_part.homedir,
	   (char *) homedir,
	   sizeof (user_part.homedir));

   strncpy ((char *)user_part.shell,
	   (char *) shell,
	   sizeof (user_part.shell));


   bzero ((char *)&admin_part, sizeof (admin_part));

   admin_part.creation_date = time(NULL);
   admin_part.change_date = time(NULL);
   admin_part.expiration_date = time(NULL) + (60*60*24);
   admin_part.good_since_date = time(NULL);
   admin_part.flags = sec_rgy_acct_admin_valid;
/*   admin_part.authentication_flags = sec_rgy_acct_auth_tgt; */

   key_parts = (sec_rgy_acct_key_t)sec_rgy_acct_key_person;


    bzero ((char *)&caller_key, sizeof (caller_key));
    caller_key.version_number=sec_passwd_c_version_none;
    caller_key.key.key_type=sec_passwd_plain;
    caller_key.pepper=NULL;
    caller_key.key.tagged_union.plain= (idl_char *) "-dce-";

    bzero ((char *)&new_key, sizeof (new_key));
    new_key.version_number=sec_passwd_c_version_none;
    new_key.key.key_type=sec_passwd_plain;
    new_key.pepper=NULL;
    new_key.key.tagged_union.plain=(idl_char *) passwd;


/* Add the test account. */

   sec_rgy_acct_add (context,
                     login_name,
  		             &key_parts,
		             &user_part,
		             &admin_part,
                     &caller_key,
                     &new_key,
                     sec_passwd_des,
                     &new_key_version,
		             &tstatus);                     

   TSHMessage (lCtx, MOTRACE,
               "sec_rgy_acct_add status: %d\n", tstatus);

   TSHStatus (lCtx, SETUP,
                  "sec_rgy_acct_add failed\n", rpc_s_ok, tstatus);


   return tstatus;
   }


/******************************************************************
**
** Lookup an account
**
*/

ndr_long_int LookupAccount
/* Formal parameters */
#ifdef __STDC__
(
sec_rgy_handle_t context,	/* rgy context */
sec_rgy_login_name_t *name_key,
sec_rgy_cursor_t *account_cursor,
sec_rgy_login_name_t *name_result,
sec_rgy_sid_t *id_sid,
sec_rgy_unix_sid_t *unix_sid,
sec_rgy_acct_key_t *key_parts,
sec_rgy_acct_user_t *user_part,
sec_rgy_acct_admin_t *admin_part
)
#else
( context, name_key, account_cursor, name_result, id_sid, unix_sid, key_parts, 
  user_part, admin_part )
sec_rgy_handle_t context;
sec_rgy_login_name_t *name_key;
sec_rgy_cursor_t *account_cursor;
sec_rgy_login_name_t *name_result;
sec_rgy_sid_t *id_sid;
sec_rgy_unix_sid_t *unix_sid;
sec_rgy_acct_key_t *key_parts;
sec_rgy_acct_user_t *user_part;
sec_rgy_acct_admin_t *admin_part;
#endif
{

/* Local variables */
   error_status_t	tstatus;	/* test status */


   sec_rgy_acct_lookup (context,
                        name_key,
                        account_cursor,
                        name_result,
                        id_sid,
                        unix_sid,
                        key_parts,
                        user_part,
                        admin_part,
                        &tstatus);

   TSHMessage (lCtx, MOTRACE,
               "sec_rgy_acct_lookup status: %d\n", tstatus);

   TSHStatus (lCtx, SETUP,
                  "sec_rgy_acct_lookup failed\n", rpc_s_ok, tstatus);

   return tstatus;
   }


/******************************************************************
**
** Delete  an account
**
*/

ndr_long_int DeleteAccount
/* Formal parameters */
#ifdef __STDC__
(
sec_rgy_handle_t context,	/* rgy context */
sec_rgy_login_name_t *name_key
)
#else
( context, name_key )
sec_rgy_handle_t context;
sec_rgy_login_name_t *name_key;
#endif

{

/* Local variables */
   error_status_t	tstatus;	/* test status */


   sec_rgy_acct_delete (context,
                        name_key,
                        &tstatus);

   TSHMessage (lCtx, MOTRACE,
              "sec_rgy_acct_delete status: %d\n", tstatus);

   if (TSHStatus (lCtx, CLNUP,
                  "sec_rgy_acct_delete failed\n", rpc_s_ok, tstatus))

   return tstatus;
   }


