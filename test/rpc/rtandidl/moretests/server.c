/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: server.c,v $
 * Revision 1.1.4.2  1996/02/17  23:22:04  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:04  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:09:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:55  root]
 * 
 * Revision 1.1.2.4  1993/01/25  19:27:59  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:44:18  bbelch]
 * 
 * Revision 1.1.2.3  1993/01/11  23:40:00  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:13:27  bbelch]
 * 
 * Revision 1.1.2.2  1992/10/12  17:32:59  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  21:20:07  cuti]
 * 
 * Revision 1.1  1992/01/19  03:39:04  devrcs
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



#include <signal.h>
#include <dce/dce_error.h>
            
#include <enum.h>
#include <ih.h>
#include <scal.h>
#include <rec.h>
#include <strz.h>
#include <spec.h>
#include <rattr.h>
#include <ary.h>
#include <tattr.h>
#include <vrec.h>
#include <pipe.h>
#include <outofline.h>
#include <pattr.h>
#include <repas.h>
#include <string.h>
#include <tsh_call.h>


struct if_reg {
    rpc_if_handle_t *s_ifspec;
    char *name;
} If[] =  {
	{ &enumtest_v2_0_s_ifspec,	"Enums"		},
	{ &implicit_test_v2_0_s_ifspec,	"Implicit Handle"},
	{ &scalartest_v2_0_s_ifspec,	"Scalars"	},
	{ &recordtest_v2_0_s_ifspec,	"Records"	},
	{ &string0test_v2_0_s_ifspec,	"String0"	},
	{ &specialtest_v2_0_s_ifspec,	"Specials"	},
	{ &rattrtest_v2_0_s_ifspec,	"Parameter Attributes"},
	{ &arraytest_v2_0_s_ifspec,	"Arrays"	},
	{ &tattrtest_v2_0_s_ifspec,	"Transmit As"	},
	{ &varrectest_v2_0_s_ifspec,	"Variable Records"},
	{ &pipetest_v2_0_s_ifspec,	"Pipes"		},
	{ &outofline_test_v2_0_s_ifspec,"Out of line"		},
	{ &pattrtest_v2_0_s_ifspec,	"Parameter attributes"	},
	{ &repastest_v2_0_s_ifspec,	"Represent As"	},
	{ &stringtest_v2_0_s_ifspec,	"Strings"	},
	{0,0} 
	};

unsigned long MaxCalls;
rpc_binding_vector_t  *bindings; 



/*
 * R E G I S T E R _ I F S
 *
 * Register the interfaces.
 */

static void register_ifs()
{
    unsigned32		status;
    unsigned long	interface;
    struct if_reg *	if_reg;
    dce_error_string_t	errorbuf;
 

/*****************************************************************************/
/*  Register the interfaces.  Don't have to specify manager epvs, use the    */
/*  NIDL generated defaults.  Also, don't have to specify the uuid_type      */
/*  because only a single implementation of each manager is being used.      */
/*****************************************************************************/

    for (if_reg = If; if_reg->s_ifspec; if_reg++) 
    {


        rpc_server_use_all_protseqs(1, &status);
        if( status != rpc_s_ok) 
        {
            dce_error_inq_text(status, errorbuf, (int *)&status);
        /*    if(status!= rpc_s_ok)
                strcpy(errorbuf, "Unknown error");  */
            fprintf(stderr,"Cant use protocols sequence for %s: %s\n",
                    if_reg->name, errorbuf);
        } 



        rpc_server_register_if ( *if_reg->s_ifspec, (uuid_p_t)0,
            (rpc_mgr_epv_t) 0,       /* use default, in ifspec */
            &status);
        if ( status != rpc_s_ok) 
        {
            dce_error_inq_text(status, errorbuf, (int *)&status);
         /*   if(status!= rpc_s_ok)
                strcpy(errorbuf, "Unknown error"); */
            fprintf(stderr,"Can't register %s test manager - %s\n", 
                if_reg->name, errorbuf);
        }   

        
	    rpc_server_inq_bindings(&bindings, &status);
  		if ( status != rpc_s_ok) 
	    {
            dce_error_inq_text(status, errorbuf, &status);
            fprintf(stderr,"Can't inq bindings %s test manager - %s\n", 
                if_reg->name, errorbuf);
        } 


	    rpc_ep_register( *if_reg->s_ifspec,
		    bindings,
		    (uuid_vector_t *) NULL,
		    (unsigned_char_p_t) if_reg->name,
		    &status);

        if ( status != rpc_s_ok) 
        {
            dce_error_inq_text(status, errorbuf, &status);
            fprintf(stderr,"Can't register ep %s test manager - %s\n", 
                if_reg->name, errorbuf);
        }   
    	

	} 

#ifdef NOTDEF
    rpc_server_use_protseq_if( (unsigned char *)"ncadg_dds",
			1, tsh_call_test_v2_0_s_ifspec, &status);
#endif
}

static void usage()
{
    fprintf(stderr, "usage: server [-dv] <max calls> <family name> ... \n");
    fprintf(stderr, "  -d: Enables NCK debugging trace output\n");
    fprintf(stderr, "  -v: Turns on verbose mode\n");
}  

unregister_ifs()
{
    unsigned long	interface;
    struct if_reg *	if_reg;
    dce_error_string_t	errorbuf;
    unsigned32      status;

    for (if_reg = If; if_reg->s_ifspec; if_reg++) 
    { 

	rpc_server_unregister_if(*if_reg->s_ifspec,
		(uuid_p_t) 0,
		&status);
 		if ( status != rpc_s_ok) 
	    {
            dce_error_inq_text(status, errorbuf, (int *)&status);
            fprintf(stderr,"Can't unregister %s test manager - %s\n", 
                if_reg->name, errorbuf);
        }    
/*
	    rpc_server_inq_bindings(&bindings, &status);
  		if ( status != rpc_s_ok) 
	    {
            dce_error_inq_text(status, errorbuf, &status);
            fprintf(stderr,"Can't inq bindings %s test manager - %s\n", 
                if_reg->name, errorbuf);
        } 

     	rpc_ep_unregister( *if_reg->s_ifspec, 
                      bindings,
                      (uuid_vector_t *) NULL,
                      &status);

        if ( status != rpc_s_ok) 
        	{
  	  	    dce_error_inq_text(status, errorbuf, &status);
            fprintf(stderr,"Can't unregister ep %s test manager - %s\n", 
                if_reg->name, errorbuf);
		} 
*/
	}

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
    unsigned32    status;
    int arg_count, i, j;
    int c; 
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
        } else if (strcmp(argv[i], "-rem") == 0) { 
			unregister_ifs();
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
*/
 
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
  
TRY
                         
	register_ifs();  

    rpc_server_listen(1, &status);

CATCH_ALL 

	unregister_ifs();


ENDTRY

}
