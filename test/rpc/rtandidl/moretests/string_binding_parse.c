/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: string_binding_parse.c,v $
 * Revision 1.1.4.2  1996/02/17  23:22:24  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:16  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:10:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:22  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:28:26  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:44:53  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  23:41:11  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:14:35  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:37:15  devrcs
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

static unsigned_char_t    nil_string = '\0';

unsigned_char_p_t
get_args(args)
unsigned_char_p_t     args;
{
    if(!strcmp(NULL_TOKEN,args))
        return((unsigned_char_p_t)0);
    else if(!strcmp(NIL_TOKEN,args))
        return(&nil_string);
    else
        return(args);
}


#ifdef apollo
#include <signal.h>
catchapollo()
{
    write(2,"Caught Apollo signal\n",21);
}
#endif
#define ARGS_REQUIRED        7          /* This routine should have 4 args */

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
    unsigned_char_p_t output_uuid=(unsigned_char_p_t)"";
    unsigned_char_p_t output_protseq=(unsigned_char_p_t)"";
    unsigned_char_p_t output_netaddr=(unsigned_char_p_t)"";
    unsigned_char_p_t output_endpoint=(unsigned_char_p_t)"";
    unsigned_char_p_t output_network_options=(unsigned_char_p_t)"";
    unsigned long        count;
    int                    flag=0;


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

#ifdef apollo
    signal(SIGAPOLLO,catchapollo);
#endif

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
        flag = -1;
        rpc_string_binding_compose(string_object_uuid,protseq,netaddr,endpoint,
            network_options,&string_binding,&status);
        if(status != rpc_s_ok) {
            flag = 0;
            break;
        }
        switch (SubCase) {
        case NULL_UUID:
            rpc_string_binding_parse(string_binding,(unsigned_char_p_t *)0,
                &output_protseq, &output_netaddr, &output_endpoint,
                &output_network_options,&status);
                break;
        case NULL_PROTSEQ:
            rpc_string_binding_parse(string_binding,&output_uuid,
                (unsigned_char_p_t *)0, &output_netaddr, &output_endpoint,
                &output_network_options,&status);
                break;
        case NULL_NET_ADDR:
            rpc_string_binding_parse(string_binding,&output_uuid,
                &output_protseq, (unsigned_char_p_t *)0, &output_endpoint,
                &output_network_options,&status);
                break;
        case NULL_ENDPOINT:
            rpc_string_binding_parse(string_binding,&output_uuid,
                &output_protseq, &output_netaddr, (unsigned_char_p_t *)0,
                &output_network_options,&status);
                break;
        case NULL_OPTIONS:
            rpc_string_binding_parse(string_binding,&output_uuid,
                &output_protseq, &output_netaddr, &output_endpoint,
                (unsigned_char_p_t *)0,&status);
                break;
        default:
            rpc_string_binding_parse(string_binding,&output_uuid,
                &output_protseq, &output_netaddr, &output_endpoint,
                &output_network_options,&status);
                break;
        }
        if(status != rpc_s_ok) {
            flag = 1;
            break;
        }
        if(strcmp(output_uuid,string_object_uuid)){
            flag = 2;
            break;
        }
        if(strcmp(output_protseq,protseq)) {
            flag = 3;
            break;
        }
        if(strcmp(output_netaddr,netaddr)) {
            flag = 4;
            break;
        }
        if(strcmp(output_endpoint,endpoint)) {
            flag = 5;
            break;
        }
        if(strcmp(output_network_options,network_options)) {
            flag = 6;
            break;
        }
        if(++count%1000 == 0)
            TSHMessage(ctx, MOINFO, "Created %d String Bindings\n",count);
            
    }while(LoopIndex);
    TSHMessage(ctx, MOINFO, "Binding String %s\n",string_binding);
    switch(flag) {
    case 0:
    case 1:
        TSHMessage(ctx, MOWARNING, 
            flag?"rpc_binding_string_parse wanted %x got %x\n":"rpc_string_binding_compose wanted %x got %x\n", 
            EStatus, status);
        break;
    case 2:
        if(SubCase != NULL_UUID)
            TSHMessage(ctx,MOWARNING,
                "rpc_string_binding_parse, uuid mismatch %s %s\n",
                string_object_uuid,output_uuid);
        else
            flag = 0;
        break;
    case 3:
        if(SubCase != NULL_PROTSEQ)
            TSHMessage(ctx,MOWARNING,
                "rpc_string_binding_parse, Protocol sequence mismatch %s %s\n",
                protseq,output_protseq);
        else
            flag = 0;
        break;
    case 4:
        if(SubCase != NULL_NET_ADDR)
            TSHMessage(ctx,MOWARNING,
                "rpc_string_binding_parse, network address mismatch %s %s\n",
                netaddr,output_netaddr);
        else
            flag = 0;
        break;
    case 5:
        if(SubCase != NULL_ENDPOINT)
            TSHMessage(ctx,MOWARNING,
                "rcp_string_binding_parse, endpoint mismatch %s %s\n",
                endpoint,output_endpoint);
        else
            flag = 0;
        break;
    case 6:
        if(SubCase != NULL_OPTIONS)
            TSHMessage(ctx,MOWARNING,
                "rpc_string_binding_parse, network option mismatch %s\n",
                network_options,output_network_options);
        else
            flag = 0;
        break;
    case -1:
        break;
    default:
        TSHMessage(ctx,MOWARNING,"Unknown return code %d\n",flag);
        break;
    }
        
    TSHStatus(ctx, TEST, "rpc_string_binding_parse failed", flag>2?flag:status, 
        EStatus);
    return(flag>1?flag:status== EStatus?0:status);
}



