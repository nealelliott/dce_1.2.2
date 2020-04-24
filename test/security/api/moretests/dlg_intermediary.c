/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dlg_intermediary.c,v $
 * Revision 1.1.7.2  1996/02/17  23:25:21  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:30  marty]
 *
 * Revision 1.1.7.1  1995/12/11  21:48:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:13  root]
 * 
 * Revision 1.1.3.8  1994/10/03  18:05:16  rsarbo
 * 	add -exec switch
 * 	[1994/10/03  17:19:08  rsarbo]
 * 
 * Revision 1.1.3.7  1994/09/21  15:45:07  rsarbo
 * 	handle DFS acl's by establishing default delegated context
 * 	in dlg_test_access()
 * 	[1994/09/20  19:51:55  rsarbo]
 * 
 * Revision 1.1.3.6  1994/08/17  19:28:27  bhaim
 * 	fix for 11717
 * 	[1994/08/17  19:27:55  bhaim]
 * 
 * Revision 1.1.3.5  1994/08/12  17:37:56  mdf
 * 	Hewlett Packard Security Drop
 * 	[1994/08/12  14:16:06  mdf]
 * 
 * 	Add session_id and passing server delegation type from client
 * 
 * 	Intermediary will take the delegation type from dlg_test_access()'s parameter, instead of command line.
 * 
 * 	     Add test for rock.
 * 
 * 	merge impersonation support from .../greg_dlg_imp
 * 
 * 	add delegation type command line option so we can
 * 	force mismatches bewteen the client and intermediary.
 * 
 * 	add impersonation test
 * 
 * 	Add name_based check on sec_cred call.
 * 
 * 	merge from dlg_bl2
 * 
 * 	More work
 * 
 * 	More dlg bl2 work
 * 
 * 	Merge from ODESSA_1
 * 
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1993
** Unpublished work. All Rights Reserved.
*/

#include <stdio.h>
#include <locale.h>
#include <dlg_test.h>

#include <string.h>
#include <macros.h>
#include <dce/dce_error.h>
#include <dce/uuid.h>
#include <dce/sec_login.h>
#include <dce/sec_cred.h>
#include <dce/passwd.h>
#include <dce/daclif.h>

#define DLG_SCRIPT "./dlg_script"
#define uuida_str "62198f0c-7248-11cd-8120-080009352589"
#define uuidb_str "651bd962-7248-11cd-af3e-080009352589"
#define ROCKS1 935258 
#define ROCKS2 7248

#define EXIT_ON_ERROR(st) \
    if (BAD_STATUS(&st)) { \
        exit(st); \
    }

static sec_login_handle_t        lc;
static unsigned_char_t           entry[256];
static boolean32                 debug = false;
static boolean32                 exec_prog = false;

/* for dce_error_inq_text */
int                     err_st;
dce_error_string_t      error_buf;


boolean32 test_session_info(
    handle_t                 h,
    uuid_t                   *session_id,
    sec_timeval_sec_t        *expiration,
    error_status_t           *st
)
{
    rpc_authz_cred_handle_t     caller;
    unsigned_char_p_t           pname;
    unsigned32                  plvl, an, az;
    uuid_t                      session_id_1;
    sec_timeval_sec_t           expiration_1;
    boolean32                   rst = false;
    error_status_t              dummy;


    CLEAR_STATUS(st);
    if (debug) {
        fprintf(stderr, "DEBUG: Entered test_session_id\n");
    }

    rpc_binding_inq_auth_caller(h, &caller, &pname, &plvl, &an, &az, st);
    if (debug && BAD_STATUS(st)) {
        dce_error_inq_text((unsigned long)*st, error_buf, &err_st);
        fprintf(stderr, "DEBUG: Error from rpc_binding_inq_auth_caller: %s\n", error_buf);
    }

    if (GOOD_STATUS(st)) {
	sec_cred_get_authz_session_info(caller, session_id, expiration, st);
	if (GOOD_STATUS(st)) {
	    sec_cred_get_authz_session_info(caller, &session_id_1, &expiration_1, st);
	    if (GOOD_STATUS(st)) {
		if (uuid_equal(session_id, &session_id_1, &dummy) && (*expiration == expiration_1)) {
		    rst = true;
		}
		else {
		    rst = false;
		}
	    }
        }
    }

    return rst;


}



void dlg_test_attrs(
    handle_t                 h,
    error_status_t           *stp
)
{
    rpc_authz_cred_handle_t     caller;
    unsigned_char_p_t           pname;
    unsigned32                  plvl, an, az;
    sec_cred_pa_handle_t        pa_h;
    sec_cred_attr_cursor_t      acursor;
    sec_attr_t                  attr;
    error_status_t              tmp_st;

    CLEAR_STATUS(stp);
    if (debug) {
        fprintf(stderr, "DEBUG: Entered dlg_test_attrs\n");
    }

    rpc_binding_inq_auth_caller(h, &caller, &pname, &plvl, &an, &az, stp);
    if (debug && BAD_STATUS(stp)) {
        dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
        fprintf(stderr, "DEBUG: Error from rpc_binding_inq_auth_caller: %s\n", error_buf);
    }

    if (GOOD_STATUS(stp)) {
        pa_h = sec_cred_get_initiator(caller, stp);
        if (GOOD_STATUS(stp)) {
            sec_cred_initialize_attr_cursor(&acursor, stp);
            if (debug && BAD_STATUS(stp)) {
                dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
                fprintf(stderr, "DEBUG: Error from sec_cred_initialize_attr_cursor: %s\n", error_buf);
            }
            if (GOOD_STATUS(stp)) {
                sec_cred_get_extended_attrs(pa_h, &acursor, &attr, stp);
                sec_cred_free_attr_cursor(&acursor, &tmp_st);
            }
        }
    }
}


sec_cred_destruct_fn_t rock_crasher (
    unsigned32 *data
)
{
    if (data != NULL) {
	free(data);
    }
	
}

boolean32 dlg_test_rock(
    handle_t             h,
    error_status_t       *stp
)
{
    uuid_t                      uuida, uuidb;
    unsigned32                  rock_key=0;
    unsigned32                  *rock_data1r;
    unsigned32                  *rock_data2r;
static unsigned32                *rock_data1 = NULL;
static unsigned32                *rock_data2 = NULL;
    rpc_authz_cred_handle_t     caller;
    unsigned_char_p_t           sname, s1name, cname;
    unsigned32                  plvl, an, az, an1, az1;
    boolean32                   test=false;
    unsigned32                  dummy;



    CLEAR_STATUS(stp);   
    if (debug) {
        fprintf(stderr, "DEBUG: Entered dlg_test_rock\n");
    }   

    rpc_binding_inq_auth_caller(h, &caller, &sname, &plvl, &an, &az, stp);

    if (BAD_STATUS(stp)) {
	if (debug) {
	    dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
	    fprintf(stderr, "DEBUG: Error from rpc_binding_inq_auth_caller: %s\n", error_buf);
	}
	return(false);
    }

    rock_crasher(rock_data1);
    rock_crasher(rock_data2);
    
    uuid_from_string((unsigned_char_t *)uuida_str, &uuida, &dummy);
    uuid_from_string((unsigned_char_t *)uuidb_str, &uuidb, &dummy);

    /* ORIGINAL_ROCK */ 
    sec_cred_get_app_private_data(caller, rock_key, (void *)&rock_data1r, stp);
    if ((rock_data1r == NULL) && (*stp == sec_cred_s_key_not_found)) {
	/* no rock set yet */
	rock_key = sec_cred_get_key(&uuida, (sec_cred_destruct_fn_t)rock_crasher, stp);
	if (debug) {
	    fprintf(stderr, "DEBUG: rock key : %d\n", rock_key);
	}
	if (BAD_STATUS(stp)) {
	    if (debug) {
		dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
		fprintf(stderr, "DEBUG: Error from sec_cred_get_key: %s\n", error_buf);
	    }
	    return(false);
	}
    }

    if (rock_data1 == NULL) {
	rock_data1 = (unsigned32 *)malloc(sizeof(unsigned32));
    }
    *rock_data1 = ROCKS1;
    sec_cred_set_app_private_data(caller, rock_key, rock_data1, stp);
    if (BAD_STATUS(stp)) {
	if (debug) {
	    dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
	    fprintf(stderr, "DEBUG: Error from sec_cred_set_app_private_data: %s\n", error_buf);
	}
	return(false);
    }  

    sec_cred_get_app_private_data(caller, rock_key, (void *)&rock_data1r, stp);
    if (BAD_STATUS(stp)) {
	if (debug) {
	    dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
	    fprintf(stderr, "DEBUG: Error from sec_cred_get_app_private_data: %s\n", error_buf);
	}
	return(false);
    } 
    if (rock_data1r != NULL) {
	if (debug) {
	    fprintf(stderr, "DEBUG: rock data: %d, returnd : %d\n", *rock_data1, *rock_data1r);
	}
        if (*rock_data1r != *rock_data1) {
		fprintf(stderr, "Returned rock is different from it was set to .\n");
		return(false);
	}
    }



      /* case OVERWRITE_ROCK: */

    if (rock_data2 == NULL) {
	rock_data2 = (unsigned32 *)malloc(sizeof(unsigned32));
    }
    *rock_data2 = ROCKS2;
    sec_cred_set_app_private_data(caller, rock_key, rock_data2, stp);
    if (BAD_STATUS(stp)) {
	if (debug) {
	    dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
	    fprintf(stderr, "DEBUG: Error from sec_cred_set_app_private_data: %s\n", error_buf);
	}
	return(false);
    }  

    sec_cred_get_app_private_data(caller, rock_key, (void *)&rock_data2r, stp);
    if (BAD_STATUS(stp)) {
	if (debug) {
	    dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
	    fprintf(stderr, "DEBUG: Error from sec_cred_get_app_private_data: %s\n", error_buf);
	}
	return(false);
    }  
    if (rock_data2r != NULL) {
	if (debug) {
	    fprintf(stderr, "DEBUG: rock data: %d, returnd : %d\n", *rock_data2, *rock_data2r);
	}
        if (*rock_data2r != *rock_data2) {
	    fprintf(stderr, "Returned rock is different from it was set to .\n");
	    return(false);
	}
    }

     /* ONE_MORE_KEY: */
    rock_key = sec_cred_get_key(&uuidb, (sec_cred_destruct_fn_t)rock_crasher, stp);
    if (BAD_STATUS(stp)) {
	if (debug) {
	    dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
	    fprintf(stderr, "DEBUG: Error from sec_cred_get_key: %s\n", error_buf);
	}
	if (STATUS_EQUAL(stp, sec_cred_s_no_more_key)) {
	    return(true);
	}
	else {
	    return(false);
	}
    }
    else {
	return(false);
    }


    return(true);

}



boolean32 dlg_test_authz_name(
    handle_t             h,
    error_status_t       *stp
)
{
    rpc_authz_cred_handle_t     caller;
    unsigned_char_p_t           sname, s1name, cname;
    unsigned32                  plvl, an, az, an1, az1;
    boolean32                   retval=false;
    sec_cred_cursor_t           cursor;


    CLEAR_STATUS(stp);   
    if (debug) {
        fprintf(stderr, "DEBUG: Entered dlg_test_authz_name\n");
    }   

    rpc_binding_inq_auth_caller(h, &caller, &sname, &plvl, &an, &az, stp);

    if (BAD_STATUS(stp)) {
	if (debug) {
	    dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
	    fprintf(stderr, "DEBUG: Error from rpc_binding_inq_auth_caller: %s\n", error_buf);
	}
	return retval;
    }

    retval = sec_cred_is_authenticated(caller, stp);


    if (BAD_STATUS(stp)) {
	if (debug) {
	    dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
	    fprintf(stderr, "DEBUG: Error from sec_cred_is_authenticated: %s\n", error_buf);
	}
    }

    if ((retval == false) && debug) {
        fprintf(stderr, "DEBUG: Caller is not authenticated.\n");
        retval = true;
      }

    sec_cred_inq_auth_service_info(caller, &s1name, &an1, &az1, stp);
    if (debug && BAD_STATUS(stp)) {
        dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
        fprintf(stderr, "DEBUG: Error from sec_cred_inquire_auth_service_info: %s\n", error_buf);
      } 

    if (strcmp((char *)sname, (char *)s1name)){
       fprintf(stderr, "Server name returnd differently\n");
       fprintf(stderr, " from rpc_binding_inq_auth_caller (%s)\n ", sname);
       fprintf(stderr, " from sec_cred_inquire_auth_service_info (%s)\n", s1name);
       if (retval == true)
          retval = false;
     }

    if (an != an1) {
        fprintf(stderr, "authentication service returned differently",        
           " from rpc_binding_inq_auth_caller (%d), ",
           "from sec_cred_inquire_auth_service_info (%d)\n",  an, an1);
       if (retval == true)
          retval = false;
    }   

    if (az != az1) {
        fprintf(stderr, "authroization service returned differently",        
           " from rpc_binding_inq_auth_caller (%d), ",
           "from sec_cred_inquire_auth_service_info (%d)\n",  az, az1);
       if (retval == true)
          retval = false;
      }
 

    sec_cred_get_client_princ_name(caller, &cname, stp);
    if (debug && BAD_STATUS(stp)) {
        dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
        fprintf(stderr, "DEBUG: Error from sec_cred_get_client_princ_namrpc_binding_inq_auth_caller: %s\n", error_buf);
    } else {
        if (debug) {
            fprintf(stderr, "DEBUG: client principal name: %s\n", cname);
        }
   
      }

    sec_cred_get_initiator(caller, stp);
    if (*stp != sec_cred_s_authz_cannot_comply) {
        if (debug) {
            dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
            fprintf(stderr, "DEBUG: Error from sec_cred_get_initiator: %s, expected: \
                    sec_cred_s_authz_cannot_comply.\n", error_buf);
        }
      } 
    else
	{
        CLEAR_STATUS(stp);
    }

    sec_cred_get_delegate(caller, &cursor, stp);
    if (*stp != sec_cred_s_authz_cannot_comply) {
        if (debug) {
            dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
            fprintf(stderr, "DEBUG: Error from sec_cred_get_delegate: %s, expected: \
                    sec_cred_s_authz_cannot_comply.\n", error_buf);
        }
      }    else
	{
        CLEAR_STATUS(stp);
    }


    return(retval);

}    

    




boolean32 dlg_test_access(
    handle_t                 h,
    sec_id_delegation_type_t deleg_type,
    sec_acl_component_name_t component_name,
    sec_acl_permset_t        *permset,
    error_status_t           *stp
)
{
    rpc_authz_cred_handle_t     caller;
    unsigned_char_p_t           pname;
    boolean32                   test;
    unsigned32                  plvl, an, az, su, nt;
    boolean32                   is_anonymous=false;
    error_status_t              tmp_st;
    uuid_t                      mgr_type;
    sec_acl_handle_t            aclh;
    sec_acl_bind_auth_info_t    auth_info;
    sec_login_handle_t          dlh, def_login_context;
    sec_cred_pa_handle_t        pa_h, pa1_h;
    sec_id_pa_t                 *pa_dataP;
    sec_acl_permset_t           perms =
      (sec_acl_perm_read|sec_acl_perm_write|sec_acl_perm_execute);
    
    CLEAR_STATUS(stp);
    test = false;
    if (debug) {
        fprintf(stderr, "DEBUG: Entered dlg_test_access\n");
    }

    rpc_binding_inq_auth_caller(h, &caller, &pname, &plvl, &an, &az, stp);
    if (BAD_STATUS(stp)) {
        if (debug) {
          dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
          fprintf(stderr, "DEBUG: Error from rpc_binding_inq_auth_caller: %s\n", error_buf);
      }
        return test;
    }

    pa_h = sec_cred_get_initiator(caller, stp);

    if (BAD_STATUS(stp)) {
	if (debug) {
	    dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
	    fprintf(stderr, "DEBUG: Error from sec_cred_get_initiator: %s\n",error_buf);
	}
	return test;
    }		
    

    pa_dataP = sec_cred_get_pa_data (pa_h, stp);
    if (BAD_STATUS(stp)) {
	if (debug) {
	    dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
	    fprintf(stderr, "DEBUG: Error from sec_cred_get_pa_data: %s\n",error_buf);
	}
	return test;
    }		

    is_anonymous = sec_id_is_anonymous_pa(pa_dataP, stp);

    if (debug) {
	printf("is anonymous? %s\n", (is_anonymous) ? "y" : "n");
    }

    if (debug) {
	fprintf(stderr, "DEBUG: delegation type = %d\n", deleg_type);
    }

    switch (deleg_type) {
    case sec_id_deleg_type_none:  /* test "none" by calling become_delegate () */
    case sec_id_deleg_type_traced:
	dlh = sec_login_become_delegate(caller, lc, sec_id_deleg_type_traced,
					NULL, NULL, NULL, NULL,
					sec_id_compat_mode_initiator, stp);
	if (BAD_STATUS(stp)) {
	    if (debug) {
		dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
		fprintf(stderr, "DEBUG: Error from sec_login_become_delegate: %s\n",error_buf);
	    }
	}
	break;
    case sec_id_deleg_type_impersonation:
	dlh = sec_login_become_impersonator(caller, lc, 
					    sec_id_deleg_type_impersonation,
					    NULL, NULL, NULL, NULL,
					    stp);
	if (BAD_STATUS(stp)) {
	    if (debug) {
		dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
		fprintf(stderr, "DEBUG: Error from sec_login_become_impersonator: %s\n",error_buf);
	    }
	}
	break;
    }

    if (BAD_STATUS(stp)) {
        return test;
    }

    pa1_h = sec_login_cred_get_initiator(dlh, stp);

    if (BAD_STATUS(stp)) {
	if (debug) {
	    dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
	    fprintf(stderr, "DEBUG: Error from sec_login__cred_get_initiator: %s\n",error_buf);
	}
	return test;
    }

    pa_dataP = sec_cred_get_pa_data (pa1_h, stp);
    if (BAD_STATUS(stp)) {
	if (debug) {
	    dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
	    fprintf(stderr, "DEBUG: Error from sec_cred_get_pa_data: %s\n",error_buf);
	}
	return test;

    }

    is_anonymous = sec_id_is_anonymous_pa(pa_dataP, stp);

    if (debug) {
	printf("is anonymous? %s\n", (is_anonymous) ? "y" : "n");
    }

    /* Get a binding to the sample DCE ACL server with the delegated context */
    auth_info.info_type = sec_acl_bind_auth_dce;
    auth_info.tagged_union.dce_info.authn_lvl = rpc_c_authn_level_default;
    auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
    auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity  = dlh;
    sec_acl_bind_auth(entry, false, &auth_info, &aclh, stp);
    if (BAD_STATUS(stp)) {
        if (debug){
            dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
            fprintf(stderr, "DEBUG: Error from sec_acl_bind: %s\n", error_buf);
	}
        goto test_access_return;
    }
    sec_acl_get_manager_types(aclh, sec_acl_type_object, 1, &su, &nt,
                                &mgr_type, stp);
    if (BAD_STATUS(stp)) {
        if (debug) {
            dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
            fprintf(stderr, "DEBUG: Error from sec_acl_get_mgr_types: %s\n", error_buf);
	}
    } else {
	int dfs_acl = 0;

	/*
	 * If DFS acl, need to set the delegated context 
	 * as the default.  sec_acl_bind_auth() on DFS doesn't 
	 * set up auth_info for subsequent sec_acl calls.
	 *
	 * We already have the info on whether this is a DFS
	 * acl in the aclh, but that's supposed to be opaque
	 * at this layer, so we have to find out again the long
	 * way.
	 */
	*stp = dfs_dce_acl_lookup(entry, sec_acl_type_object, NULL);
	if (GOOD_STATUS(stp)) {
	    dfs_acl++;
    	    sec_login_get_current_context(&def_login_context, stp);
    	    if (BAD_STATUS(stp)) {
                if (debug) {
            	    dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
            	    fprintf(stderr, "DEBUG: Error from sec_login_get_current_context: %s\n", error_buf);
	        }
    	    }
    	    sec_login_set_context(dlh, stp);
    	    if (BAD_STATUS(stp)) {
                if (debug) {
            	    dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
            	    fprintf(stderr, "DEBUG: Error from sec_login_set_context: %s\n", error_buf);
	        }
            }
	} else {
	    CLEAR_STATUS(stp);
	}
        test = sec_acl_test_access(aclh, &mgr_type, perms, stp);
        if (debug) {
            if (BAD_STATUS(stp)) {
                dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
                fprintf(stderr, "DEBUG: Error from sec_acl_test_access: %s\n", error_buf);
            } else {
                fprintf(stderr, "DEBUG: test_access returned %s\n",
                        ((test) ? "true" : "false"));
            }
        }
	if (exec_prog) {
            system(DLG_SCRIPT);
	}
	if (dfs_acl) {
    	    sec_login_set_context(def_login_context, stp);
    	    if (BAD_STATUS(stp)) {
                if (debug) {
            	    dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
            	    fprintf(stderr, "DEBUG: Error from sec_login_set_context: %s\n", error_buf);
	        }
            }
	}
    }
    CLEAR_STATUS(&tmp_st);
    sec_acl_release_handle(&aclh, &tmp_st);
    if (debug && BAD_STATUS(&tmp_st)) {
        dce_error_inq_text((unsigned long)tmp_st, error_buf, &err_st);
        fprintf(stderr,"DEBUG: Error from sec_acl_release_handle %s\n", error_buf);
    }

test_access_return:
    sec_login_purge_context(&dlh, &tmp_st);
    if (debug && BAD_STATUS(&tmp_st)) {
        dce_error_inq_text((unsigned long)tmp_st, error_buf, &err_st);
        fprintf(stderr, "DEBUG: Error purging delegation context: %s\n", error_buf);
    }
    return test;
}


static void usage
  (
    char *name
  )
{
    fprintf(stderr,
        "Usage: %s [-p server_princ_name][-f key_table_name][-e name][-dbg]\n",
        name);
}


static void register_server(
  char              *keytable,
  unsigned_char_p_t server_princ_name,
  error_status_t    *stp
)
{
    rpc_binding_vector_p_t  bvec;
    error_status_t          tmp_st;
    ndr_char                *string_binding;
    int                     i;
    unsigned_char_t         annotation[64];
    dce_error_string_t      dce_error;
    uuid_t                  uuid_nil, obj;
    uuid_vector_t           test_server_obj_uuids;
    dlg_test_v0_0_epv_t     dlg_test_v0_0_manager_epv =
      { dlg_test_access, dlg_test_attrs, dlg_test_authz_name, dlg_test_rock, test_session_info };

    uuid_create_nil(&uuid_nil, &tmp_st);
    uuid_from_string((unsigned_char_p_t)"7738f506-ecf8-11cc-859d-0800093569b9",
      &obj, &tmp_st);
    rpc_server_use_all_protseqs(5, stp);
    if (BAD_STATUS(stp)) {
        dce_error_inq_text((unsigned long)*stp, dce_error, (int *) &tmp_st);
        fprintf(
            stderr, "Cannot use protocol sequence - %s\n",
            dce_error
        );
        exit(1);
    }
    rpc_server_register_if(dlg_test_v0_0_s_ifspec, &uuid_nil,
                             (rpc_mgr_epv_t)&dlg_test_v0_0_manager_epv, stp);
    if (BAD_STATUS(stp)) {
        dce_error_inq_text((unsigned long)*stp, dce_error, (int *) &tmp_st);
        printf("Cannot register dlg_test interface - %s\n", dce_error);
        exit(1);
    }

    rpc_server_inq_bindings(&bvec, stp);
    if (BAD_STATUS(stp)) {
        dce_error_inq_text((unsigned long)*stp, dce_error, (int *) &tmp_st);
        printf("Cannot inquire bindings - %s\n", dce_error);
        exit(1);
    }

    /* LOCATE  register all interfaces with the local endpoint
     *         map so all interfaces are reachable with
     *         bound-to-host handles.
     */
    strcpy((char *) annotation, "DCE Delegation Test Server");
    test_server_obj_uuids.count = 1;
    test_server_obj_uuids.uuid[0] = &obj;
    rpc_ep_register(dlg_test_v0_0_s_ifspec,
		    bvec,
		    &test_server_obj_uuids,
		    annotation,
		    stp);
    if (BAD_STATUS(stp)) {
        dce_error_inq_text((unsigned long)*stp, dce_error, (int *) &tmp_st);
        fprintf(stderr, "Error registering endpoint: %s\n", dce_error);
    }

    rpc_server_register_auth_info(server_princ_name, rpc_c_authn_dce_secret,
                                  (rpc_auth_key_retrieval_fn_t) NULL,
                                  (void *) keytable, stp);
    if (BAD_STATUS(stp)) {
        dce_error_inq_text((unsigned long)*stp, dce_error, (int *) &tmp_st);
        fprintf(stderr, "Error: %s\n", dce_error);
        fprintf(stderr, "Unable to register authorization info.\n");
    }

    /* Delete previous bindings */
    rpc_ns_mgmt_entry_delete(rpc_c_ns_syntax_default,
                            (unsigned_char_p_t) "/.:/dlg_test",
                            stp);

    if (BAD_STATUS(stp) && (*stp != rpc_s_entry_not_found)) {
        dce_error_inq_text((unsigned long)*stp, dce_error, (int *) &tmp_st);
        fprintf(stderr, "Error  deleting namespace entry: %s\n",
                dce_error);
    }

    /* Establish a name-service database entry with binding handles */
    rpc_ns_binding_export(rpc_c_ns_syntax_default,
                            (unsigned_char_p_t) "/.:/dlg_test",
                            dlg_test_v0_0_s_ifspec, bvec,
                            &test_server_obj_uuids,
                            stp);
    if (BAD_STATUS(stp)) {
        dce_error_inq_text((unsigned long)*stp, dce_error, (int *) &tmp_st);
        fprintf(stderr, "Error exporting interface to namespace: %s\n",
                dce_error);
    }

    printf("Bindings:\n");
    for (i = 0; i < bvec->count; i++) {
        rpc_binding_to_string_binding(
            bvec->binding_h[i], &string_binding, stp
        );
        if (BAD_STATUS(stp)) {
            dce_error_inq_text((unsigned long)*stp, dce_error, (int *) &tmp_st);
            printf("Cannot get string binding - %s\n", dce_error);
            exit(1);
        }
        printf("%s\n", (char *)string_binding);

        rpc_string_free(&string_binding, stp);
        if (BAD_STATUS(stp)) {
            dce_error_inq_text((unsigned long)*stp, dce_error, (int *) &tmp_st);
            fprintf(
                stderr, "Cannot free string binding %s - %s\n",
                string_binding, dce_error
            );
            exit(1);
        }
    }
}


int main
(
    int     argc,
    char    *argv[]
)
{
    error_status_t          st, tmp_st;
    dce_error_string_t      dce_error;
    char                    *keytable;
    char                    keytabname[BUFSIZ];
    unsigned_char_p_t       server_princ_name = (unsigned_char_p_t) "dlg_svr";
    sec_passwd_str_t        pbuf;
    int                     i;

    setlocale(LC_ALL, "");

    keytable = NULL;
    CLEAR_STATUS(&st);
    strcpy((char *) &entry[0], "/.:/myfilesystem/mike");
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            if ((i+1) >= argc) {
                fprintf(stderr,"Option -p requires principal name argument\n");
                usage(argv[0]);
                exit(1);
            }
            i++;
            server_princ_name = (unsigned_char_p_t) argv[i];

        } else if (strcmp(argv[i], "-f") == 0) {
            if ((i+1) >= argc) {
                fprintf(stderr,"Option -f requires keytable name argument\n");
                usage(argv[0]);
                exit(1);
            }
            i++;
            strcpy(keytabname, argv[i]);
            keytable = &keytabname[0];
        } else if (strcmp(argv[i], "-e") == 0) {
            if ((i+1) >= argc) {
                fprintf(stderr,"Option -e requires entry name argument\n");
                usage(argv[0]);
                exit(1);
            }
            i++;
            strcpy((char *)entry, argv[i]);
        } else if (strncmp(argv[i], "-dbg", 4) == 0) {

            debug = true;

        } else if (strncmp(argv[i], "-exec", 4) == 0) {

            exec_prog = true;

        } else if (strncmp(argv[i], "-h", 2) == 0) {

            usage(argv[0]);
            exit(0);

        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            usage(argv[0]);
            exit(1);
        }
    }

    strcpy((char *) pbuf, "dlg_svr");
    register_server(keytable, server_princ_name, &st);
    EXIT_ON_ERROR(st);
    log_in(&lc, "dlg_svr", pbuf, &st);
    EXIT_ON_ERROR(st);
    
    printf("Listening...\n");

    rpc_server_listen((unsigned32)5, &st);

    if (st != error_status_ok) {
        dce_error_inq_text((unsigned long)st, dce_error, (int *) &tmp_st);
        fprintf(stderr, "Error: %s\n", dce_error);
    }
    sec_login_purge_context(&lc, &tmp_st);
}
