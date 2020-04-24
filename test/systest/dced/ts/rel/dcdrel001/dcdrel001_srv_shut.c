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
 * $Log: dcdrel001_srv_shut.c,v $
 * Revision 1.1.4.2  1996/02/17  23:26:40  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:24  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:55:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:55:24  root]
 * 
 * Revision 1.1.2.1  1994/07/21  21:27:07  fadden
 * 	Initial version after renaming
 * 	Shutdown two dcdrel servers with different interface specs
 * 	Modify to try all server endpoint bindings found instead of only
 * 	  the first one
 * 	[1994/07/21  20:37:35  fadden]
 * 
 * $EndLog$
 */
/* 
 * NAME
 * 
 *   dcdrel001_srv_shut.c
 *
 * COMPONENT
 *   
 *   DCE dced system test
 *   
 * ABSTRACT
 *
 *   This module shuts down the DCDREL servers on the local node.
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <dce/dce.h>
#include <dce/uuid.h>
#include <dce/binding.h>
#include <dce/tst_comm_api.h>	/* for common test functions */
#include <dcd_st_utils.h>
#include <dcdrel001a.h>
#include <dcdrel001b.h>

/* ------------------- Global values and constants --------------------------*/

#define	  CLIENT_ACCT  		"cell_admin"
#define   CLIENT_PWD   		"-dce-"

/* ----------------------- Prototypes --------------------------------------*/ 
void 
shut_server(rpc_if_handle_t);

/* -------------------------Main routine  ----------------------------------*/

/*
 *
 * ROUTINE NAME: main
 *
 * DESCRIPTION:
 *    Shut down the local DCDREL servers
 *
 */
int
main(int argc, char **argv)
{

    tst_init_dbg(DBG_WARN); /* set message output level */

    shut_server(dcdrel001a_v1_0_c_ifspec);

    shut_server(dcdrel001b_v1_0_c_ifspec);

    return(0);
}

/*
 *
 * ROUTINE NAME: shut_server
 *
 * DESCRIPTION:
 *    Shuts down the local server specified by the interface spec
 *
 */
void 
shut_server(rpc_if_handle_t if_handle)
{
    unsigned32 			retval;
    error_status_t		st;
    unsigned_char_t		*bstring;
    rpc_binding_handle_t	bh;
    rpc_if_id_t                 dcdrel001_if_id;
    rpc_ep_inq_handle_t         mgmt_iq_context;

    rpc_if_inq_id(if_handle, &dcdrel001_if_id, &st);
    TST_CK_DCE_STATUS(st, DBG_FATAL, "rpc_if_inq_id", CHK_RET);
    rpc_mgmt_ep_elt_inq_begin(NULL, rpc_c_ep_match_by_if, &dcdrel001_if_id,
			      rpc_c_vers_exact, NULL, &mgmt_iq_context,
			      &st);
    TST_CK_DCE_STATUS(st, DBG_FATAL, "rpc_mgmt_ep_elt_inq_begin", CHK_RET);

    rpc_mgmt_ep_elt_inq_next(mgmt_iq_context, &dcdrel001_if_id, &bh, NULL,
			     NULL, &st);
    while (st == error_status_ok)
    {    
	if (rpc_mgmt_is_server_listening(bh, &st)) 
	{
	    rpc_mgmt_stop_server_listening(bh, &st);
	    TST_CK_DCE_STATUS(st, DBG_FATAL, "rpc_mgmt_stop_server_listening",
			      CHK_CONT);
	    if ((st) != error_status_ok)
	    {
		rpc_binding_to_string_binding(bh, &bstring, &st);       
		TST_CK_DCE_STATUS(st, DBG_WARN, 
				  "rpc_binding_to_string_binding",
				  CHK_CONT);
		if ((st) == error_status_ok)
		{
		    TSTDBG((DBG_WARN,
			    "Failure occured while using binding: %s\n",
			    bstring));
		    rpc_string_free(&bstring, &st);
		    TST_CK_DCE_STATUS(st, DBG_WARN, "rpc_string_free",
				      CHK_CONT);
		}
	    }
	}	
	rpc_mgmt_ep_elt_inq_next(mgmt_iq_context, &dcdrel001_if_id, &bh, NULL,
				 NULL, &st);
    }

    if (st == rpc_s_no_more_bindings) 
    {
	TSTDBG((DBG_NOTE, "No more server bindings\n"));
    }
    else
    {
	TST_CK_DCE_STATUS(st, DBG_WARN, "rpc_mgmt_ep_elt_inq_next",
			  CHK_CONT);
    }
	
    rpc_mgmt_ep_elt_inq_done(&mgmt_iq_context, &st);
    TST_CK_DCE_STATUS(st, DBG_WARN, "rpc_mgmt_ep_elt_inq_done",CHK_CONT);
    
    return;
}
