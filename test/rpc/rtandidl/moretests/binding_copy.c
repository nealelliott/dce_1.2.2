/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY 
 * $Log: binding_copy.c,v $
 * Revision 1.1.8.2  1996/02/17  23:20:04  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:38:42  marty]
 *
 * Revision 1.1.8.1  1995/12/11  20:03:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:42:48  root]
 * 
 * Revision 1.1.6.1  1993/10/14  21:59:46  tom
 * 	Bug 8300 - Include stdlib.h.
 * 	[1993/10/14  21:57:46  tom]
 * 
 * Revision 1.1.4.2  1993/01/11  22:11:33  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:03:47  bbelch]
 * 
 * Revision 1.1.2.2  1992/07/12  20:08:04  cuti
 * 	Change MOFAIL to MOINFO after rpc_binding_copy() to report message correctly.
 * 	[1992/07/12  20:05:43  cuti]
 * 
 * Revision 1.1  1992/01/19  03:34:44  devrcs
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
*/



#include <tsh_string.h>
#include <bindtest.h>
#include <stdlib.h>

#define ARGS_REQUIRED        7          /* This routine should have 4 args */

static unsigned_char_t	nil_string = '\0';

unsigned_char_p_t get_args(args)
unsigned_char_p_t 	args;
{
	if(!strcmp(NULL_TOKEN,(char *)args))
		return((unsigned_char_p_t)0);
	else if(!strcmp(NIL_TOKEN,(char *)args))
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
	rpc_binding_handle_t	src_binding;
	rpc_binding_rep_p_t	src_binding_p;
	rpc_binding_handle_t	dst_binding;
	unsigned long		count;
	unsigned_char_p_t	memory_holder;
	int					flag=0;


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

	/* Chew up memory if the memory test is required. */

	LoopIndex = FALSE;

	if (SubCase == MEMORY_ERROR)
	  {
	    memory_holder = malloc(GOBBLE_MEM_SIZE);
	    LoopIndex = TRUE;
	  }

	string_object_uuid = get_args(argv[2]);
	protseq = get_args(argv[3]);
	netaddr = get_args(argv[4]);
	endpoint = get_args(argv[5]);
	network_options = get_args(argv[6]);

    TSHMessage(ctx, MOINFO, "Object UUID %s, Protocol Sequence = %s\n",
		string_object_uuid,protseq);
    TSHMessage(ctx, MOINFO, "Network Address %s, Endpoint %s, Options %s\n",
		netaddr,endpoint,network_options);

	count = 0;
	do {
		rpc_string_binding_compose(string_object_uuid,protseq,netaddr,endpoint,
			network_options,&string_binding,&status);
		if(status != rpc_s_ok) {
    		TSHMessage(ctx, MOFAILURE, "String binding compose %s\n",
				string_binding);
			flag = 0;
			break;
		}
		rpc_binding_from_string_binding(string_binding,&src_binding,&status);
		if(status != rpc_s_ok) {
    		TSHMessage(ctx, MOFAILURE, "Binding from string binding %s\n",
				string_binding);
			flag = 1;
			break;
		}

	     /* Force rpc_s_invalid_binding or other errors. */

		src_binding_p = (rpc_binding_rep_p_t) src_binding;

		switch (SubCase)
		  {
		    case SRC_HANDLE_ZERO:
		      src_binding = (rpc_binding_handle_t) 0;
		      break;

		    case SRC_HANDLE_MINUS_ONE:
		      src_binding = (rpc_binding_handle_t) -1;
		      break;

		    case DATA_OFFSET_ERR:
/*		      src_binding_p->data_offset = (unsigned16) -1; */
		      src_binding_p->link.next = (pointer_t) -1; 
		      break;

		    case PROTOCOL_ID_ERR:
		      src_binding_p->protocol_id = (rpc_protocol_id_t) -1;
		      break;

		    case REFCNT_ERR:
		      src_binding_p->refcnt = (signed8) -1;
		      break;

		    case OBJ_ERR:
		      bzero ((char *)&(src_binding_p->obj), sizeof(uuid_t));
		      break;

		    case RPC_ADDR_ERR:
		      src_binding_p->rpc_addr = (rpc_addr_p_t) 0;
		      break;

		    case RPC_ADDR_RPC_PROTSEQ_ID_ERR:
		      src_binding_p->rpc_addr->rpc_protseq_id =
					(rpc_protseq_id_t) 0;
		      break;

		    case RPC_ADDR_LEN_ERR:
		      src_binding_p->rpc_addr->len = (unsigned32) 0;
		      break;

		    case RPC_ADDR_SA_FAMILY_ERR:
		      src_binding_p->rpc_addr->sa.family = (unsigned16) 0;
		      break;

		    case RPC_ADDR_SA_DATA_ERR:
		      bzero ((char *)(src_binding_p->rpc_addr->sa.data),
		             sizeof(unsigned8));
		      break;

		    case TIMEOUT_ERR:
		      src_binding_p->timeout = (signed32) -1;
		      break;

		    case CALLS_IN_PROGRESS_ERR:
		      src_binding_p->calls_in_progress = (signed8) -1;
		      break;

		    case NS_SPECIFIC_ERR:
		      src_binding_p->ns_specific = (pointer_t) 0;
		      break;

		  }

		rpc_binding_copy(src_binding,&dst_binding,&status);
		if(status != rpc_s_ok) {
    		TSHMessage(ctx, MOINFO, "Binding copy %s status: %d\n",string_binding, status);
			flag = 2;
			break;
		}

		if(++count%1000 == 0)
    		TSHMessage(ctx, MOINFO, "Created %d Bindings\n",count);
			
	}while(LoopIndex);

     if (SubCase == MEMORY_ERROR)
       free(memory_holder);

    TSHMessage(ctx, MOINFO, "Binding String %s\n",string_binding);
    if (TSHStatus(ctx, TEST, "rpc_string_copy", status, 
		EStatus))
        return(status);
    return(rpc_s_ok);
}



