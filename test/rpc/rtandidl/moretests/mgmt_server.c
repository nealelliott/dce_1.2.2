/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mgmt_server.c,v $
 * Revision 1.1.6.2  1996/02/17  23:20:51  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:39:48  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:06:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:37  root]
 * 
 * Revision 1.1.4.4  1993/01/25  18:05:22  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:37:29  bbelch]
 * 
 * Revision 1.1.4.3  1993/01/11  23:35:28  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:07:49  bbelch]
 * 
 * Revision 1.1.4.2  1992/10/12  16:48:26  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/12  14:43:40  cuti]
 * 
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  18:00:40  cuti]
 * 
 * Revision 1.1.2.2  1992/07/12  19:34:52  cuti
 * 	In hoohah() rutine, modified to print *status.
 * 	[1992/07/10  17:30:58  cuti]
 * 
 * Revision 1.1  1992/01/19  03:39:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
#
#
# Copyright (c) 1991 by
#    Hewlett-Packard Company, Palo Alto, Ca.
#
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Hewlett-Packard Company not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Hewlett-Packard Company makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
#
#
# DCE API Test
#
*/



#include <stdio.h>
#include <signal.h>
#include <tsh_common.h>
#include <tsh_mgmt.h>
#include <tsh_util.h>
#include <my_mgmt.h>
#include <dce/dce_error.h>


struct if_reg {
    rpc_if_handle_t *s_ifspec;
    char *name;
} If[] =  {
	{ &mgmttest_v1_0_s_ifspec,	"Mgmt_Tests"	},
	{0,0}
	};

unsigned long MaxCalls;



/*
 * R E G I S T E R _ I F S
 *
 * Register the interfaces.
 */

register_ifs()
{
    error_status_t	status;
    unsigned long	interface;
    struct if_reg *	if_reg;
    dce_error_string_t	errorbuf;
    rpc_binding_vector_t  *bindings; 
 

/*****************************************************************************/
/*  Register the interfaces.  Don't have to specify manager epvs, use the    */
/*  NIDL generated defaults.  Also, don't have to specify the uuid_type      */
/*  because only a single implementation of each manager is being used.      */
/*****************************************************************************/

    for (if_reg = If; if_reg->s_ifspec; if_reg++) 
    {
        rpc_server_register_if ( *if_reg->s_ifspec, (uuid_p_t)0,
            (rpc_mgr_epv_t) 0,       /* use default, in ifspec */
            &status);
        if ( status != rpc_s_ok) 
        {
            dce_error_inq_text(status, errorbuf, (int *) &status);
/*            if(status!= rpc_s_ok)
                errorbuf=(unsigned_char_p_t)"Unknown error"; */
            fprintf(stderr,"Can't register %s test manager - %s\n", 
                if_reg->name, errorbuf);
        }

       rpc_server_use_all_protseqs( 1, &status);

        if( status != rpc_s_ok) 
        {
            dce_error_inq_text(status, errorbuf, (int *) &status);
/*            if(status!= rpc_s_ok)
                errorbuf=(unsigned_char_p_t)"Unknown error";     */
            fprintf(stderr,"Cant use protocols sequence for %s: %s\n",
                    if_reg->name, errorbuf);
        } 

	    rpc_server_inq_bindings(&bindings, &status);
        if ( status != rpc_s_ok) 
        {
            dce_error_inq_text(status, errorbuf, (int *) &status);
            fprintf(stderr,"Can't inquire binding for %s test manager - %s\n", 
                if_reg->name, errorbuf);
        }

		rpc_ep_register( *if_reg->s_ifspec, bindings, (uuid_vector_t *) NULL, 
                          (unsigned_char_p_t) if_reg->name, &status);
        if ( status != rpc_s_ok) 
        {
            dce_error_inq_text(status, errorbuf, (int *) &status);
            fprintf(stderr,"Can't register %s end point - %s\n", 
                if_reg->name, errorbuf);
        }


    }

}

static void usage()
{
    fprintf(stderr, "usage: server [-dv] <max calls> <family name> ... \n");
    fprintf(stderr, "  -d: Enables NCK debugging trace output\n");
    fprintf(stderr, "  -v: Turns on verbose mode\n");
}
   
/*  

idl_boolean shut_ok = true;
static boolean32 mgmt_auth_fn (h, op, st)

handle_t            h;
unsigned32          op;
unsigned32          *st;

{
    boolean32 b;

    b = (op == rpc_c_mgmt_stop_server_listen) ? shut_ok : true;

    printf (1, ("+ In management authorization function: op=%lu, returning \"%s\"\n",
                op, b ? "true" : "false"));

    *st = b ? rpc_s_ok : rpc_s_mgmt_op_disallowed;

    return (b);
}  
*/

 
void hoohah(h, status)
handle_t h;
error_status_t *status;
{
    rpc_mgmt_stop_server_listening(NULL, status);
printf("rpc_mgmt_stop_server_listening status: %d\n", *status); 

}
  

void fake (h)
handle_t	h;
{
  /* no op */
}

/*
 * M A I N
 *
 * Main program.
 */

main(argc, argv)
int argc;
char *argv[];
{
    error_status_t status;
    int c; 
    int arg_count, i, j;
    unsigned long	interface;
    struct if_reg *	if_reg;
    dce_error_string_t	errorbuf;

    arg_count = argc;
    for (i = 1, j = 1; i < arg_count; i++) {
        argv[j] = argv[i];
        if (strncmp(argv[i], "-d", 2) == 0) {
            if (*(argv[i]+2) == '\0') {
                rpc__dbg_set_switches("0-4.10", &status);
            } else {
                rpc__dbg_set_switches(argv[i]+2, &status);
            }
            argc--;
        } else if (strcmp(argv[i], "-help") == 0) {
            usage();
            argc--; 
			exit(1);
        }  else {
            j++;
        }
    }


/*
    while((c = getopt(argc,argv,"d:eI:l:L:sS")) != -1) {
        switch(c) {
	case '?':
	    usage();
	    break;
        }
    }     

    rpc_mgmt_set_authorization_fn(mgmt_auth_fn, &status);
*/

    register_ifs();
        

/*   Fork is not working correctly on pthread.  5/15

#ifndef DEBUG
    switch (fork())
    {
        case 0:         /* If successful the child will come here /
            close(0);   /* try to close all open descriptors here /
            close(1);
            close(2);
			setpgrp();
            break;
        case -1:        /* If unsuccessful the parent will come here /
            exit(1);
            break;
        default:        /* If successful the parent will come here /
            exit(0);
            break;
    }
#endif     

*/
/***************************************************************************/
/*  Using known ports, don't have to export interfaces to the name server. */
/*  Later, maybe export bindings so that we can test the name server.      */
/***************************************************************************/

    rpc_server_listen(1, &status);
/*****************************************************************************/
/*  Maybe figure out when to unregister_ifs.  Or simply wait to be killed?   */
/*  That should be ok in this case cause we haven't registered with the name */
/*  server and the runtime will disappear when we kill the server.           */
/*****************************************************************************/
}

