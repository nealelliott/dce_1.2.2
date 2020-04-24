/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: string_binding_compose.c,v $
 * Revision 1.1.6.2  1996/02/17  23:22:22  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:15  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:10:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:20  root]
 * 
 * Revision 1.1.4.1  1993/10/14  21:59:52  tom
 * 	Bug 8300 - Include stdlib.h.
 * 	[1993/10/14  21:57:56  tom]
 * 
 * Revision 1.1.2.3  1993/01/25  19:28:22  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:44:48  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  23:41:05  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:14:31  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:37:14  devrcs
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



#include <tsh_string.h>
#include <stdlib.h>

#define ARGS_REQUIRED        7          /* This routine should have 4 args */

static unsigned_char_t	nil_string = '\0';

unsigned_char_p_t
get_args(args)
unsigned_char_p_t 	args;
{
	if(!strcmp(NULL_TOKEN,args))
		return((unsigned_char_p_t)0);
	else if(!strcmp(NIL_TOKEN,args))
		return(&nil_string);
	else
		return(args);
}


ndr_long_int 
TestFunction(ctx, argc, argv)
CtxBlock *ctx;
ndr_long_int argc;
unsigned_char_p_t argv[];

{

    ndr_long_int    SubCase;    /* SubCAse for this test modulee */
    ndr_long_int    LoopIndex;  /* generic loop counter */
    error_status_t  status;     /* status from rpc_* and uuid_* calls */
    error_status_t  EStatus;    /* expected status, compared against status */
	unsigned_char_p_t string_object_uuid;
	unsigned_char_p_t protseq;
	unsigned_char_p_t netaddr;
	unsigned_char_p_t endpoint;
	unsigned_char_p_t network_options;
	unsigned_char_p_t string_binding;
	unsigned long		count;
	unsigned_char_p_t	memory_holder;


#ifdef DEBUG
    printf("-------------------------------\nTestFunction:\nInput args are:\n");
    PRINT_ARGS(argc, argv);
    printf("    Program Name is %s\n", ctx->ProgramName);
#endif

    if(argc != ARGS_REQUIRED)
    {
        TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d.\n",
            ARGS_REQUIRED, argc);
        return(FAILURE);
    }
    /*  atol() the subcase and expected return value to numbers.  */
    SubCase = atol(argv[0]);
    EStatus = atol(argv[1]);
	/* Eat enough memory to allow subsequent printfs etc... */
#ifndef OSF
	cma_init();
#endif
	memory_holder = malloc(GOBBLE_MEM_SIZE);

	string_object_uuid = get_args(argv[2]);
	protseq = get_args(argv[3]);
	netaddr = get_args(argv[4]);
	endpoint = get_args(argv[5]);
	network_options = get_args(argv[6]);

    TSHMessage(ctx, MOINFO, "Object UUID %s, Protocol Sequence = %s\n",
		string_object_uuid,protseq);
    TSHMessage(ctx, MOINFO, "Network Address %s, Endpoint %s, Options %s\n",
		netaddr,endpoint,network_options);

	if(SubCase == MEMORY_ERROR)
		LoopIndex = TRUE;
	else
		LoopIndex = FALSE;

	
	count = 0;
	do {
		rpc_string_binding_compose(string_object_uuid,protseq,netaddr,endpoint,
			network_options,&string_binding,&status);
		if(status != rpc_s_ok)
			break;
		if(++count%1000 == 0)
    		TSHMessage(ctx, MOINFO, "Created %d String Bindings\n",count);
			
	}while(LoopIndex);
	free(memory_holder);
    TSHMessage(ctx, MOINFO, "Binding String %s\n",string_binding);
    TSHStatus(ctx, TEST, "rpc_string_binding_compose", status, 
		EStatus);
    return(status == EStatus?0:status);
}



