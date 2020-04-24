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
 * $Log: dcdrel001a_server.c,v $
 * Revision 1.1.4.2  1996/02/17  23:26:42  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:25  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:55:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:55:28  root]
 * 
 * Revision 1.1.2.1  1994/07/21  21:27:12  fadden
 * 	Initial version after renaming
 * 	[1994/07/21  20:37:39  fadden]
 * 
 * $EndLog$
 */
/* 
 * NAME
 * 
 *   dcdrel001a_server.c
 *
 * COMPONENT
 *   
 *   DCE dced system test
 *   
 * ABSTRACT
 *
 *   This module provides a server which registers its
 *   endpoints using the rpc runtime API rather than the dced calls.
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <dce/dce.h>
#include <dce/uuid.h>

#include <dcd_st_utils.h>
#include <dcdrel001a.h>

/* ------------------- Global values and constants --------------------------*/

unsigned_char_t		srv_name[80];
unsigned_char_t		grp_name[80] ="/.:/test/systest/srv_grp/dcdrel001";

#define	  SERVER_ACCT  "cell_admin"
#define   SERVER_PWD   "-dce-"

boolean32
dcdrel001a_authzn(rpc_binding_handle_t h, unsigned32 m, error_status_t *st);

/* ------------------- Main test functions ----------------------------------*/

int
main(int argc, char **argv)
{
    error_status_t		st;
    rpc_binding_vector_t	*bind_vec;
    char			*myhost;
    int				init_acl;
    uuid_t                      server_obj_uuid;
    
    tst_init_dbg(DBG_WARN);  /* set message output level */
    dce_cf_get_host_name(&myhost, &st);
    TST_CK_DCE_STATUS(st, DBG_FATAL, "dce_cf_get_host_name", CHK_RET);
    sprintf(srv_name, "/.:/%s/dcdrel001a", myhost);
    free(myhost);

    /* append command line arg $1 to name if provided */
    if ( argc > 1)
    {
	sprintf(&srv_name[strlen(srv_name)],".%s", argv[1]);
    }

    TSTDBG((DBG_NOTE, "Using server name %s; group name %s\n",
	    srv_name, grp_name));

    /* perform dce_login */
    tst_dce_login(SERVER_ACCT, SERVER_PWD, &st);
    TST_CK_STATUS(st, DBG_FATAL, "Failed to perform dce login", CHK_RET);

    /* generate new obj UUID for this server instance */
    uuid_create(&server_obj_uuid, &st);
    TST_CK_DCE_STATUS(st, DBG_FATAL, "uuid_create", CHK_RET);
    
    server_setup(srv_name, 
		 grp_name,
		 dcdrel001a_v1_0_s_ifspec, 
		 &server_obj_uuid,
		 (unsigned_char_t *)"DCDREL001a Server",
		 dcdrel001a_authzn,
	         &bind_vec,
		 &st);
    TST_CK_DCE_STATUS(st, DBG_FATAL, "Attempt to setup server failed",CHK_RET);

    TSTDBG((DBG_NOTE, "Server listening...\n"));
    rpc_server_listen(rpc_c_listen_max_calls_default, &st);
    TST_CK_DCE_STATUS(st, DBG_FATAL, "rpc_server_listen", CHK_CONT);
    
    server_takedown(&server_obj_uuid, 
		    &bind_vec, 
		    srv_name,
		    dcdrel001a_v1_0_s_ifspec, 
		    &st);
    
    TSTDBG((DBG_NOTE, "Server done.\n"));

    return 0;
}

boolean32
dcdrel001a_authzn(rpc_binding_handle_t h, unsigned32 m, error_status_t *st)
{
    return TRUE;
}

