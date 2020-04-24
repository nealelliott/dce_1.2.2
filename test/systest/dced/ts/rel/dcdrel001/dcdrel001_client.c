/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dcdrel001_client.c,v $
 * Revision 1.1.4.2  1996/02/17  23:26:38  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:23  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:55:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:55:22  root]
 * 
 * Revision 1.1.2.1  1994/07/21  21:27:03  fadden
 * 	Initial version after renaming
 * 	Added use of a second server i/f
 * 	[1994/07/21  20:37:32  fadden]
 * 
 * $EndLog$
 */
/* 
 * NAME
 * 
 *   dcdrel001_client.c
 *
 * COMPONENT
 *   
 *   DCE dced system test
 *   
 * ABSTRACT
 *
 *   This module provides the client code for DCDREL001.
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <dce/dce.h>
#include <dce/uuid.h>
#include <dce/binding.h>
#include <unistd.h>		/* for sleep */
#include <dce/tst_comm_api.h>	/* for common test functions */
#include <dcd_st_utils.h>
#include <dcdrel001a.h>
#include <dcdrel001b.h>

/* ------------------- Forward define functions -----------------------------*/

void dcdrel_test1(
    rpc_binding_handle_t bh, 
    unsigned32    	ucount,  
    unsigned8           if_number,
    error_status_t    	*st);


/* ------------------- Global values and constants --------------------------*/

#define   MAX_COMMAND_LENGTH 	500 
#define	  CLIENT_ACCT  		"cell_admin"
#define   CLIENT_PWD   		"-dce-"
#define   SERVER_DELAY 		30
#define   UUID_VEC_COUNT	10
/* note DCDREL001_PASS_COUNT must match value used in dcdrel001 shell script */
#define   DCDREL001_PASS_COUNT   5   
#define   TCDIR         "/dcetest/dcelocal/test/tet/system/dced/ts/rel"

    const char *		tcdir_string=TCDIR; 

/* ------------------- Main test functions ----------------------------------*/

/*
 *
 * ROUTINE NAME: main
 *
 * DESCRIPTION:
 *    Demonstrate that dced can provide endpoint information reliably for
 *    objects which are registered using any of the available forms of
 *    endpoint registration, while concurrently handling credentials
 *    certification requests.
 *
 * PROCEDURE:
 *   1) 
 *   2) 
 */
int
main(int argc, char *argv)
{

    unsigned32 			retval;
    error_status_t		st;
    unsigned_char_t	        *name="/.:/test/systest/srv_grp/dcdrel001";
    unsigned_char_t		*bstring;
    rpc_ns_handle_t		import_context;
    rpc_binding_handle_t	bh;
    int				i;
    unsigned32			ucount; /* number of uuids in uuid_vec */
    unsigned32                  passct; /* number of times to loop thru test */
    unsigned32                  exp_age = 0;
    unsigned_char_t 		command[MAX_COMMAND_LENGTH];
    char			*myhost;
    unsigned_char_t		srv_name[80];
#ifdef NEVER
    rpc_if_id_t                 dcdrel001a_if_id;
#endif
    rpc_ep_inq_handle_t         mgmt_iq_context;
    unsigned8                   ifnum;

    tst_init_dbg(DBG_WARN);

    ucount = UUID_VEC_COUNT;

    TSTDBG((DBG_NOTE, "DCDREL001 Demonstrate that dced can provide endpoint \
information reliably."));
    
    /* perform dce_login */
    tst_dce_login(CLIENT_ACCT, CLIENT_PWD, &st);
    TST_CK_DCE_STATUS(st, DBG_FATAL, "tst_dce_login", CHK_RET);
    
    
    for (passct = 1; passct <= DCDREL001_PASS_COUNT; passct++)
    {
	TSTDBG((DBG_NOTE, "Starting client pass #%d\n", passct));

	for (ifnum = 1; ifnum <= 2; ifnum++)
	{
	    switch (ifnum)
	    {
		/*
		 *  cycle once thru all the bindings associated with 
		 *  the specified group of servers 
		 */
	    case 1:
		rpc_ns_binding_import_begin(rpc_c_ns_syntax_default,
					    name,
					    dcdrel001a_v1_0_c_ifspec, 
					    NULL, 
					    &import_context, 
					    &st);
		TST_CK_DCE_STATUS(st, DBG_FATAL, 
				  "rpc_ns_binding_import_begin, server a", 
				  CHK_RET);
		break;
	    case 2:
		rpc_ns_binding_import_begin(rpc_c_ns_syntax_default,
					    name,
					    dcdrel001b_v1_0_c_ifspec, 
					    NULL, 
					    &import_context, 
					    &st);
		TST_CK_DCE_STATUS(st, DBG_FATAL, 
				  "rpc_ns_binding_import_begin, server a", 
				  CHK_RET);
		break;
	    default:
		TSTDBG((DBG_FATAL,
			"main: Bad server test interface number %d\n",
			ifnum));
		return 0;
	    }
	    
	    /* set a short expiration age for the server binding information */
	    rpc_ns_mgmt_handle_set_exp_age(import_context, exp_age, &st);
	    TST_CK_DCE_STATUS(st, DBG_WARN, "rpc_ns_mgmt_handle_set_exp_age",
			      CHK_CONT);
	    
	    rpc_ns_binding_import_next(import_context, &bh, &st);
	    while (st == error_status_ok)
	    {
		if (rpc_mgmt_is_server_listening(bh, &st)) 
		{
		    /*
		     * register a set of objects with the endpoint mapper;
		     * then bind to each individually and remove it from 
		     * the endpoint map 
		     */
		    dcdrel_test1(bh, ucount, ifnum, &st);
		    if ((st) != error_status_ok)
		    { /* failure already reported; try to report binding used*/
			rpc_binding_to_string_binding(bh, &bstring, &st);       
			if ((st) == error_status_ok)
			{
			    TSTDBG((DBG_WARN,
				    "Test failure or warning occured while using binding: %s\n",
				    bstring));
			    rpc_string_free(&bstring, &st);
			    TST_CK_DCE_STATUS(st, DBG_WARN, "rpc_string_free",
					      CHK_CONT);
			}
			else
			{
			    TST_CK_DCE_STATUS(st, DBG_WARN, 
					      "Failure converting binding used in test to a string binding",
					      CHK_CONT);
			}
		    }	
		}	
		else
		{ /* attempt to report which binding failed */
		    rpc_binding_to_string_binding(bh, &bstring, &st);       
		    if ((st) == error_status_ok)
		    {
			TSTDBG((DBG_WARN,"Server not listening on: %s\n", 
				bstring));
			rpc_string_free(&bstring, &st);
			TST_CK_DCE_STATUS(st, DBG_WARN, "rpc_string_free",
					  CHK_CONT);
		    }
		    else
		    {
			TST_CK_DCE_STATUS(st, DBG_WARN, 
					  "Server not listening; failed to convert rpc binding to string binding",
					  CHK_CONT);
		    }
		}	
		rpc_ns_binding_import_next(import_context, &bh, &st);
	    }
	    
	    if (st == rpc_s_no_more_bindings) 
	    {
		TSTDBG((DBG_NOTE, "No more server bindings\n"));
	    }
	    else
	    {
		TST_CK_DCE_STATUS(st, DBG_WARN, "rpc_ns_binding_import_next",
				  CHK_CONT);
	    }
	    
	    rpc_ns_binding_import_done(&import_context, &st);
	    TST_CK_STATUS(st, DBG_WARN, "rpc_ns_binding_import_done", 
			  CHK_CONT);
	}
    }  /* end of for loop to run multiple passes */

    return 0;
}

/*
 * dcdrel_test1()
 *
 * generate a vector of uuids, send the vector to the server requesting
 * that it register endpoints for all the uuids in the vector, 
 * then use and unregister the resulting endpoints one at a time
 */
void
dcdrel_test1(
    rpc_binding_handle_t bh, 
    unsigned32    	ucount,  
    unsigned8           if_number,
    error_status_t    	*st
)
{
    uuid_vector_t *obj_vec;
    int i;
    rpc_binding_handle_t nbh;
    unsigned_char_t    *bstring;

    dcd_st_alloc_uuid_vec(ucount, &obj_vec, st);
    TST_CK_DCE_STATUS(*st, DBG_FATAL, "dcd_st_alloc_uuid_vec", CHK_RET);

    dcd_st_load_uuid_vec(obj_vec, st);
    TST_CK_DCE_STATUS(*st, DBG_FATAL, "dcd_st_load_uuid_vec", CHK_RET);

    /* call the server */
    switch (if_number)
	{
	case 1:
	    register_uuid_vec_all_protseqsa(bh, (uuid_vec_a_t *) obj_vec, st);
	    TST_CK_DCE_STATUS(*st, DBG_FATAL, 
			      "register_uuid_vec_all_protseqsa",
			      CHK_RET);
	    TSTDBG((DBG_NOTE, 
		    "Successfully registered endpoints on server a\n"));
	    break;
	case 2:
	    register_uuid_vec_all_protseqsb(bh, (uuid_vec_b_t *) obj_vec, st);
	    TST_CK_DCE_STATUS(*st, DBG_FATAL, 
			      "register_uuid_vec_all_protseqsb",
			      CHK_RET);
	    TSTDBG((DBG_NOTE, 
		    "Successfully registered endpoints on server b\n"));
	    break;
        default:
            TSTDBG((DBG_FATAL,
		    "dcdrel_test1: Bad server test interface number\n"));
	    *st = -1;
            return;
	}

    rpc_binding_copy(bh, &nbh, st); 
    TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_binding_copy", CHK_RET);
    
    for (i=0; i<ucount; i++)
    {
	rpc_binding_set_object(nbh, obj_vec->uuid[i], st);
	TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_binding_set_object", CHK_RET);
	rpc_binding_reset(nbh, st);
	TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_binding_reset", CHK_RET);

	/* report which binding we're using to request unregistering */
	rpc_binding_to_string_binding(nbh, &bstring, st);       
	TST_CK_STATUS(*st, DBG_WARN, "rpc_binding_to_string_binding",CHK_CONT);
	if (*st == error_status_ok)
	{
	    TSTDBG((DBG_LEV6,
		    "Bind to server to unregister endpoint(s) using %s\n", 
		    bstring));
	    rpc_string_free(&bstring, st);
	    TST_CK_STATUS(*st, DBG_WARN, "rpc_string_free", CHK_CONT);
	}
	/*
         * make the remote call to unregister the object 
	 * associated with this binding on all protocol sequences 
	 */
    switch (if_number)
	{
	case 1:
	    unregister_obj_all_protseqs_a(nbh, st);
	    TST_CK_DCE_STATUS(*st, DBG_FATAL, "unregister_obj_all_protseqs_a", 
			      CHK_CONT);
	    break;
	case 2:
	    unregister_obj_all_protseqs_b(nbh, st);
	    TST_CK_DCE_STATUS(*st, DBG_FATAL, "unregister_obj_all_protseqs_b", 
			      CHK_CONT);
	    break;
        default:
            TSTDBG((DBG_FATAL,
		    "dcdrel_test1: Bad server test interface number\n"));
	}
    }
    rpc_binding_free(&nbh, st);
    TST_CK_DCE_STATUS(*st, DBG_WARN, "rpc_binding_free", CHK_CONT);
    dcd_st_free_uuid_vec(obj_vec, st);
    TST_CK_STATUS(*st, DBG_WARN, "dcd_st_free_uuid_vec", CHK_CONT);

    return;
}
