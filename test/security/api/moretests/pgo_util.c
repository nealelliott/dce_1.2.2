/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pgo_util.c,v $
 * Revision 1.1.4.2  1996/02/17  23:25:38  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:46  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:49:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:39  root]
 * 
 * Revision 1.1.2.3  1993/02/25  22:07:08  cuti
 * 	Change sec_rgy_site_open() to sec_rgy_site_open_update() for using site update operation.
 * 	[1993/02/25  16:31:52  cuti]
 * 
 * Revision 1.1.2.2  1992/12/31  17:31:12  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:56:33  htf]
 * 
 * Revision 1.1  1992/01/19  04:07:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

#include <sec_rgy.h>
 
do_site_open
#ifdef __STDC__
(    
CtxBlock           *ctx,
error_status_t	   estatus,
sec_rgy_handle_t   *rgy_context,
error_status_t     *tstatus	
)
#else
(ctx, estatus, rgy_context, tstatus)
CtxBlock           *ctx;
error_status_t     estatus;
sec_rgy_handle_t   *rgy_context;
error_status_t     *tstatus;	
#endif
{
    sec_rgy_pname_t    node_name, site_name;/* site_name = ip:node_name for 3/15/91
                                  and rgyd should be on the same node as test node */

    sec_rgy_site_open_update(NULL, rgy_context, tstatus);
    TSHMessage (ctx, MOTRACE, "sec_rgy_site_open_update for site: %s, status: %d\n", NULL, *tstatus);

   if (TSHStatus (ctx, SETUP,
		  "sec_rgy_site_open_update failed", *tstatus, estatus))
     return;
    
}
  
void get_pgo
#ifdef __STDC__
(    
CtxBlock           *ctx,
error_status_t	   estatus,
sec_rgy_domain_t   domain,
sec_rgy_name_t     pgo_name,
sec_rgy_pgo_item_t *pgo,
error_status_t     *tstatus	
)
#else
(ctx, estatus, domain, pgo_name, pgo, tstatus)
CtxBlock           *ctx;
error_status_t     estatus;
sec_rgy_domain_t   domain;
sec_rgy_name_t     pgo_name;
sec_rgy_pgo_item_t *pgo;
error_status_t     *tstatus;	
#endif
{
    sec_rgy_cursor_t    cursor;  

    sec_rgy_cursor_reset(&cursor);
    CALL_RGY(sec_rgy_pgo_get_by_name)(CONTEXT, domain, pgo_name, &cursor, pgo, tstatus);
    TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_get_by_name(%s..%s), status: %d\n", domain_text[domain], pgo_name, *tstatus);
    if (!TSHStatus (ctx, SETUP,
	        	  "sec_rgy_pgo_get_by_name failed", *tstatus, estatus))
	*tstatus = error_status_ok;

}     
  

void reset_tstatus
#ifdef __STDC__
(
error_status_t      *tstatus
) 
#else
( tstatus )
error_status_t      *tstatus;
#endif
{   

	*tstatus = error_status_ok;

}

void compare_pgo
#ifdef __STDC__
(
    CtxBlock            *ctx,
	char                *api_name,  
    sec_rgy_name_t      pname1,
    sec_rgy_pgo_item_t  *p1,
    sec_rgy_name_t      pname2,
    sec_rgy_pgo_item_t  *p2,
    error_status_t      *tstatus
)
#else
    ( ctx, api_name, pname1, p1, pname2, p2, tstatus )
    CtxBlock            *ctx;
	char                *api_name;  
    sec_rgy_name_t      pname1;
    sec_rgy_pgo_item_t  *p1;
    sec_rgy_name_t      pname2;
    sec_rgy_pgo_item_t  *p2;
    error_status_t      *tstatus;
#endif
{

    if (strcmp((char *) pname1, (char *) pname2)) {
        printf("%s : %s - names differ\n", pname1, pname2);
		TSHMessage (ctx, MOTRACE, "%s failed: name differ, for %s\n", api_name, pname1);
		*tstatus = TSH_DATA_MISSMATCH;
 		return;
	}


    if (!uuid_equal(&p1->id, &p2->id, tstatus)) {
		printf("Time low : %d     %d \n", p1->id.time_low, p2->id.time_low);
		printf("Time mid : %d     %d \n", p1->id.time_mid, p2->id.time_mid);
		printf("Time hi  : %d     %d \n", p1->id.time_hi_and_version, p2->id.time_hi_and_version);
        printf("%s - uuids differ\n", pname1);
		TSHMessage (ctx, MOTRACE, "%s failed: uuid differ for %s, status : %d\n", api_name, pname1, *tstatus);
		*tstatus = TSH_DATA_MISSMATCH;
        return;
    }
   if (TSHStatus (ctx, SETUP,
		  "uuid_equal failed", *tstatus,  error_status_ok))
     return;

    if (p1->unix_num != p2->unix_num) {
        printf("%s unix ids differ\n", pname1);
		TSHMessage (ctx, MOTRACE, "%s failed: unix_num differ, for %s\n", api_name, pname1);
		*tstatus = TSH_DATA_MISSMATCH;
       return;
    }

    if (strcmp( (char *) p1->fullname, (char *) p2->fullname) != 0) {
        printf("%s fullnames differ\n", pname1);
		TSHMessage (ctx, MOTRACE, "%s failed: fullname differ, for %s\n", api_name, pname1);
		*tstatus = TSH_DATA_MISSMATCH;
        return;
    }

    if (p1->flags != p2->flags) {
        printf("%s flags differ\n", pname1);
		TSHMessage (ctx, MOTRACE, "%s failed: flags differ, for %s\n", api_name, pname1);
		*tstatus = TSH_DATA_MISSMATCH;
        return;
    }

    return;
}
        



void add_pgo 
#ifdef __STDC__
( 
CtxBlock          *ctx,
ndr_long_int       subcase,
sec_rgy_domain_t domain, 
sec_rgy_name_t pgo_name, 
sec_rgy_pgo_item_t *pgo,
error_status_t *tstatus,
error_status_t estatus
) 
#else
(ctx, subcase, domain, pgo_name, pgo, tstatus, estatus)
CtxBlock          *ctx;
ndr_long_int      subcase;
sec_rgy_domain_t domain; 
sec_rgy_name_t pgo_name; 
sec_rgy_pgo_item_t *pgo;
error_status_t *tstatus;
error_status_t estatus;
#endif
{
    char  msg[512];

    CALL_RGY(sec_rgy_pgo_add)(CONTEXT, domain, pgo_name, pgo, tstatus); 

    TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_add\n    domain : %s\n", domain_text[domain]);
    TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_add\n    name   : %s\n", pgo_name);
    TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_add\n    unix id: %d\n", pgo->unix_num);
    TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_add\n    fullname: %s\n",pgo->fullname);

    TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_add : %d, for %s %s\n", *tstatus, domain_text[domain], pgo_name);
          
	if (subcase != PGO_ADD && (*tstatus == sec_rgy_name_exists || *tstatus == sec_rgy_object_exists)) {
		*tstatus = error_status_ok; /* ignore the double entry for non_pgo_add */ 
		return;
	}

    if (TSHStatus (ctx, (subcase == PGO_ADD)? TEST: SETUP,
		  "sec_rgy_pgo_add failed", *tstatus, estatus)) 
    	return; 

	*tstatus = error_status_ok;
 
/*
    if (BAD_STATUS(stp)) {
	sprintf(msg, "Unable to add %s '%s'", domain_text[domain], pgo_name);
        print_status(msg, stp);
        switch (STATUS(stp)) {
            case sec_rgy_object_exists:
            case sec_rgy_name_exists:
            case sec_rgy_is_an_alias:
            case sec_rgy_unix_id_changed:
                break;

            default:
                sleep(2);
                goto retry;
        }
    }
*/

}
   

void do_add_pgo
#ifdef __STDC__
(    
CtxBlock          *ctx,
ndr_long_int       subcase,
error_status_t     estatus,	
sec_rgy_domain_t   domain,
sec_rgy_name_t     pgo_name,
long			   unix_num,
boolean32          is_an_alias, 
error_status_t     *tstatus	
)
#else
(ctx, subcase, estatus, domain, pgo_name, unix_num, is_an_alias, tstatus)
CtxBlock           *ctx;
ndr_long_int       subcase;
error_status_t     estatus;	
sec_rgy_domain_t   domain;
sec_rgy_name_t     pgo_name;
long			   unix_num;
boolean32          is_an_alias; 
error_status_t     *tstatus;	
#endif
{
	sec_rgy_pgo_item_t pgo, item, item_check;
    sec_rgy_name_t     item_name;
    sec_rgy_cursor_t   cursor;
    sec_rgy_name_t     scope;

 	*scope = '\0';
         
	if (estatus == sec_rgy_object_exists){ 
		if (domain == sec_rgy_domain_person)
			pgo.unix_num = 0;
		else
		    pgo.unix_num = 12;
	}
	else
 		pgo.unix_num = unix_num;
	pgo.flags = (is_an_alias)? sec_rgy_pgo_is_an_alias: sec_rgy_pgo_flags_none;
	sprintf((char *) pgo.fullname, "%s %05.05d", pgo_name, pgo.unix_num); 
/*	if (!is_an_alias) { /* get a new uuid */
    CALL_RGY(uuid_create_nil)(&(pgo.id), tstatus);
    TSHMessage (ctx, MOTRACE, "uuid_create_nil, status code: %d\n", *tstatus);
    if (TSHStatus (ctx, SETUP,
		  "uuid_create_nil failed", *tstatus,  error_status_ok))
	     return;
/*	}
	else { /* get the uuid of the pgo  for alias /
 	    sec_rgy_cursor_reset(&cursor);
 		CALL_RGY(sec_rgy_pgo_get_by_unix_num)(CONTEXT, domain, scope, unix_num, true, &cursor, 
		    &item, item_name, tstatus);
  	   TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_get_by_unix_num (%s..%d), status: %d\n", 
          domain_text[domain], unix_num, *tstatus);
 	   if (TSHStatus (ctx, SETUP,
       	  "sec_rgy_pgo_get_by_unix_num failed", *tstatus, error_status_ok)) 
  	 	return;
		pgo.id = item.id;
	}    */
	add_pgo(ctx, subcase, domain, pgo_name, &pgo, tstatus, estatus); 

	if (subcase == PGO_ADD) {
   		if (estatus != error_status_ok || tstatus != error_status_ok)
 			return;   /* if expected is not ok, no sense to check the result */  
			/* check the result */
		get_pgo(ctx, estatus, domain, pgo_name, &item, tstatus);
   		if (estatus != error_status_ok)
 			return;   /* if expected is not ok, no sense to check the result */  
        compare_pgo(ctx, "sec_rgy_pgo_add", pgo_name, &pgo, pgo_name, &item, tstatus); 

	}  


}  

 
void do_delete_pgo
#ifdef __STDC__
(    
CtxBlock          *ctx,
ndr_long_int       subcase,
error_status_t     estatus,	
sec_rgy_domain_t   domain,
sec_rgy_name_t     pgo_name,
error_status_t     *tstatus	
)
#else
(ctx, subcase, estatus, domain, pgo_name, tstatus)
CtxBlock           *ctx;
ndr_long_int       subcase;
error_status_t     estatus;	
sec_rgy_domain_t   domain;
sec_rgy_name_t     pgo_name;
error_status_t     *tstatus;	
#endif
{

			CALL_RGY(sec_rgy_pgo_delete)(CONTEXT, domain, pgo_name, tstatus);
            TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_delete (%s..%s), status: %d\n", 
                        domain_text[domain], pgo_name, *tstatus);
			if (subcase != PGO_DELETE && *tstatus == sec_rgy_object_not_found) {
				*tstatus = error_status_ok; /* ignore double deletion for non_pgo_delete */
				return;
			}
            if (TSHStatus (ctx, (subcase == PGO_DELETE)? TEST: CLNUP,
	        	  "sec_rgy_pgo_delete failed", *tstatus, estatus)) 
				return;
			else  /* reset tstatus for estatus != error_status_ok */
				*tstatus = error_status_ok; 

}        

 
void do_add_member
#ifdef __STDC__
(    
CtxBlock          *ctx,
ndr_long_int       subcase,
error_status_t     estatus,	
sec_rgy_domain_t   domain,   	/* member owner domain: group or org */
sec_rgy_name_t     go_name,     /* member owner id */
sec_rgy_name_t     p_name,  	/* person id */
error_status_t     *tstatus	
)
#else
(ctx, subcase, estatus, domain, go_name, p_name, tstatus)
CtxBlock           *ctx;
ndr_long_int       subcase;
error_status_t     estatus;	
sec_rgy_domain_t   domain;
sec_rgy_name_t     go_name;
sec_rgy_name_t     p_name;
error_status_t     *tstatus;	
#endif
{

	CALL_RGY(sec_rgy_pgo_add_member)(CONTEXT, domain, go_name, p_name, tstatus);
	TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_add_member, status: %d\n", *tstatus);
	if (TSHStatus (ctx, (subcase == PGO_ADD_MEMBER)? TEST: SETUP,
			"sec_rgy_pgo_add_member failed", *tstatus,  estatus))
		return;
	else
		*tstatus = error_status_ok;

}                        


void do_delete_member
#ifdef __STDC__
(    
CtxBlock          *ctx,
ndr_long_int       subcase,
error_status_t     estatus,	
sec_rgy_domain_t   domain,
sec_rgy_name_t     go_name,
sec_rgy_name_t     p_name,
error_status_t     *tstatus	
)
#else
(ctx, subcase, estatus, domain, go_name, p_name, tstatus)
CtxBlock           *ctx;
ndr_long_int       subcase;
error_status_t     estatus;	
sec_rgy_domain_t   domain;
sec_rgy_name_t     go_name;
sec_rgy_name_t     p_name;
error_status_t     *tstatus;	
#endif
{

 	CALL_RGY(sec_rgy_pgo_delete_member)(CONTEXT, domain, go_name, p_name, tstatus);
	TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_delete_member, status: %d\n", *tstatus);
	if (TSHStatus (ctx, (subcase == PGO_DELETE_MEMBER)? TEST: CLNUP,
		  "sec_rgy_pgo_delete_member failed", *tstatus,  estatus)) 
		return;
	else  /* reset tstatus for estatus != error_status_ok */
		*tstatus = error_status_ok;


} 

do_add_account
#ifdef __STDC__
(    
CtxBlock          *ctx,
ndr_long_int       subcase,
error_status_t     estatus,	
sec_rgy_name_t     pgo_name,
sec_passwd_str_t   passwd,
sec_passwd_str_t   upasswd,
error_status_t     *tstatus	
)
#else
(ctx, subcase, estatus, pgo_name, passwd, upasswd, tstatus)
CtxBlock           *ctx;
ndr_long_int       subcase;
error_status_t     estatus;	
sec_rgy_name_t     pgo_name;
sec_passwd_str_t   passwd;
sec_passwd_str_t   upasswd;
error_status_t     *tstatus;	
#endif
{
   sec_rgy_login_name_t login_name;
   sec_rgy_acct_key_t	key_parts;	/* sec_rgy_acct_add */
   sec_rgy_acct_user_t  user_part;	/* sec_rgy_acct_add */
   sec_rgy_acct_admin_t admin_part;	/* sec_rgy_acct_add */
   sec_passwd_rec_t	caller_key;	/* sec_rgy_acct_add */
   sec_passwd_rec_t	new_key;	/* sec_rgy_acct_add */
   sec_passwd_type_t	new_keytype;	/* sec_rgy_acct_add */
   sec_passwd_version_t	passwd_ver_no;	/* sec_rgy_acct_add */

                                                

/* Setup the parameters to the sec_rgy_acct_add function. */
                  
   strcpy(login_name.pname, pgo_name);
   strcpy(login_name.gname, "none");
   strcpy(login_name.oname, "none"); 

   key_parts = (sec_rgy_acct_key_t)sec_rgy_acct_key_person;

   bzero ((char *)&user_part, sizeof (user_part));

   user_part.passwd_version_number = sec_passwd_c_version_none;
   user_part.passwd_dtm = time(NULL);
   user_part.flags = sec_rgy_acct_user_flags_none;
       
   strcpy((char *)user_part.gecos, pgo_name);
/*
   memcpy ((char *)user_part.gecos,
	   (char *)"marco's test user account\0",
	   sizeof (user_part.gecos));

   memcpy ((char *)user_part.homedir,
	   (char *)"//node_feb5/marco\0",
	   sizeof (user_part.homedir));

   memcpy ((char *)user_part.shell,
	   (char *)"/bin/seashell\0",
	   sizeof (user_part.shell));
*/
   bzero ((char *)&admin_part, sizeof (admin_part));

   admin_part.creation_date = time(NULL);
   admin_part.change_date = time(NULL);
   admin_part.expiration_date = time(NULL) + (60*60*24);
   admin_part.good_since_date = time(NULL);
   admin_part.flags = sec_rgy_acct_admin_valid;
   admin_part.authentication_flags = sec_rgy_acct_auth_tgt;

   bzero ((char *)&caller_key, sizeof (sec_passwd_rec_t));

   caller_key.version_number = sec_passwd_c_version_none;
   caller_key.key.key_type = sec_passwd_plain;
   caller_key.key.tagged_union.plain = upasswd;
   
   bzero ((char *)&new_key, sizeof (sec_passwd_rec_t));

   new_key.key.key_type = sec_passwd_plain;
   new_key.key.tagged_union.plain = passwd;
   
/* sec_rgy_acct_add is called unconditionally either for its test
   or as setup for sec_rgy_acct_delete. */

   sec_rgy_acct_add (rgy_context,
             &login_name, 
		     &key_parts,
		     &user_part,
		     &admin_part,
		     &caller_key,
		     &new_key,
             sec_passwd_des,
		     &passwd_ver_no,
		     tstatus);

   TSHMessage (ctx, MOTRACE,
               "sec_rgy_acct_add status: %d\n", *tstatus);

   TSHStatus (ctx, SETUP, "sec_rgy_acct_add failed", *tstatus, estatus); 

}  

do_delete_account
#ifdef __STDC__
(    
CtxBlock          *ctx,
ndr_long_int       subcase,
error_status_t     estatus,	
sec_rgy_name_t     pgo_name,
error_status_t     *tstatus	
)
#else
(ctx, subcase, estatus, domain, pgo_name, unix_num, is_an_alias, tstatus)
CtxBlock           *ctx;
ndr_long_int       subcase;
error_status_t     estatus;	
sec_rgy_name_t     pgo_name;
error_status_t     *tstatus;	
#endif
       
{
   sec_rgy_login_name_t login_name;

   strcpy(login_name.pname, pgo_name);
   strcpy(login_name.gname, "none");
   strcpy(login_name.oname, "none"); 


   sec_rgy_acct_delete (rgy_context, &login_name, tstatus);
   TSHMessage (ctx, MOTRACE,
               "sec_rgy_acct_delete status: %d\n", *tstatus);

   TSHStatus (ctx, SETUP, "sec_rgy_acct_delete failed", *tstatus, estatus); 

}
                                
                              


