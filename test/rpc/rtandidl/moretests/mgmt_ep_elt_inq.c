/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mgmt_ep_elt_inq.c,v $
 * Revision 1.1.6.2  1996/02/17  23:20:44  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:39:33  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:05:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:30  root]
 * 
 * Revision 1.1.4.2  1993/10/15  17:39:54  tom
 * 	Bug 8355 - Make register_ifs a void function.
 * 	[1993/10/15  17:26:48  tom]
 * 
 * Revision 1.1.4.1  1993/09/30  18:50:28  tatsu_s
 * 	Bug 7174 - Ignore the ept_s_not_registered status returned from
 * 	rpc_ep_unregister() in unregister_ifs(). This test does not keep
 * 	track of which endpoints being registered.
 * 	[1993/09/29  18:49:03  tatsu_s]
 * 
 * Revision 1.1.2.6  1993/03/18  17:44:46  weisman
 * 	Fix for OT 7533, don't free bogus binding vector.
 * 	[1993/03/18  17:39:25  weisman]
 * 
 * Revision 1.1.2.5  1993/02/03  18:20:25  cuti
 * 	Add rpc_ep_register_no_replace() test
 * 	[1993/02/01  22:32:31  cuti]
 * 
 * Revision 1.1.2.4  1993/01/25  18:05:06  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:37:12  bbelch]
 * 
 * Revision 1.1.2.3  1993/01/11  22:15:11  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:07:19  bbelch]
 * 
 * Revision 1.1.2.2  1992/10/12  16:42:44  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/12  14:42:37  cuti]
 * 
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  17:59:44  cuti]
 * 
 * Revision 1.1  1992/01/19  03:35:24  devrcs
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

#include <dce/rpc.h>
#include <tsh_mgmt.h>
#include <mgmt_ep_elt_inq.h> 
#include <mgmt_copy.h>
#include <mgmts.h>
#include <dce_error.h>

rpc_if_id_t if_ids[] =  {
	{0},
	{ {0x42c460ba, 0xa000, 0x0000, 0x0d, 0x00, {0x0, 0xc3, 0x66, 0x0, 0x0, 0x0}},
		  1,
		  0
	},
	{	{0x52c460ba, 0xa000, 0x0000, 0x0d, 0x00, {0x0, 0xc3, 0x66, 0x0, 0x0, 0x0}},
     	   1,
		   0
    }
};  

struct if_reg {
    rpc_if_handle_t *s_ifspec;
    char *name;
} If[] =  {
	{ &mgmtcopy_v1_0_s_ifspec,	"Mgmt_Copy"	},
	{ &mgmts_v1_0_s_ifspec,	"Mgmt_Tests"	},
	{0,0}
	};
 
/* taken form ./src/rpc/runtime/com.h. to test invalid_handle */    
#define IF_NUM	2
/*
 * R E G I S T E R _ I F S
 *
 * Register the interfaces.
 */
void
register_ifs(ctx, subcase, estatus, hostname, protocol, tstatus)
CtxBlock                *ctx;
ndr_long_int      		subcase;		/* argv[0], subcase to run */
unsigned32			    estatus;		/* argv[1], expected return from test */ 
char					*hostname;
char					*protocol;
unsigned32				*tstatus; 
{
    error_status_t	cstatus;
    unsigned long	interface;
	rpc_binding_vector_t  *bindings; 
    struct if_reg *	if_reg;
    dce_error_string_t	errorbuf;
     unsigned_char_p_t 		bound_string; 
	rpc_binding_handle_t	ep_binding;


/*****************************************************************************/
/*  Register the interfaces.  Don't have to specify manager epvs, use the    */
/*  NIDL generated defaults.  Also, don't have to specify the uuid_type      */
/*  because only a single implementation of each manager is being used.      */
/*****************************************************************************/

    for (if_reg = If; if_reg->s_ifspec; if_reg++) 
    {
        rpc_server_register_if ( *if_reg->s_ifspec, (uuid_p_t)0,
                                (rpc_mgr_epv_t) 0, /* use default, in ifspec */
                                tstatus);
        dce_error_inq_text(*tstatus, errorbuf, &cstatus);
        TSHMessage(ctx, MOINFO, "rpc_server_register_if status: %s\n", errorbuf);
        TSHStatus(ctx, SETUP, "rpc_server_register_if failed", *tstatus, rpc_s_ok);

        rpc_server_use_all_protseqs(1, tstatus);
        dce_error_inq_text(*tstatus, errorbuf, &cstatus);
        TSHMessage(ctx, MOINFO, "rpc_server_use_all_protseqs status: %s\n", errorbuf);
        TSHStatus(ctx, SETUP, "rpc_server_use_all_protseqs failed", *tstatus, rpc_s_ok);

        rpc_server_inq_bindings(&bindings, tstatus);
        dce_error_inq_text(*tstatus, errorbuf, &cstatus);
        TSHMessage(ctx, MOINFO, "rpc_server_inq_bindings status: %s\n", errorbuf);
        TSHStatus(ctx, SETUP, "rpc_server_inq_bindings failed", *tstatus, rpc_s_ok);

        rpc_ep_register( *if_reg->s_ifspec, bindings, (uuid_vector_t *) NULL, 
                        (unsigned_char_p_t) if_reg->name, tstatus);
        dce_error_inq_text(*tstatus, errorbuf, &cstatus);
        TSHMessage(ctx, MOINFO, "rpc_ep_register status: %s\n", errorbuf);
        TSHStatus(ctx, SETUP, "rpc_ep_register failed", *tstatus, rpc_s_ok);

        if (subcase == EP_REG_NO_REPLACE) 
        { 
            switch(estatus) {
            case rpc_s_no_bindings:
                bindings = NULL;
                break;
            case rpc_s_invalid_binding: /* setup an invalid binding */
                rpc_string_binding_compose(
                                           (unsigned_char_p_t) NULL,
                                           (unsigned_char_p_t)protocol,
                                           hostname, NULL, NULL, &bound_string, tstatus);
                if (TSHStatus(ctx, SETUP, "rpc_string_binding_compose", *tstatus, rpc_s_ok))
                    return;
                rpc_binding_from_string_binding(bound_string, &ep_binding, tstatus);
                TSHMessage(ctx, MOINFO, "rpc_binding_from_string_binding status: %d\n", *tstatus);
                if (TSHStatus(ctx, SETUP, "rpc_binding_from_string_binding", *tstatus, rpc_s_ok))
                    return;
                bindings->binding_h[0] = ep_binding;
                /*	((rpc_binding_rep_p_t)(bindings->binding_h[0]))->addr_has_endpoint = false;  */
                break;							
            }		
            rpc_ep_register_no_replace( *if_reg->s_ifspec, bindings, (uuid_vector_t *) NULL, 
                                       (unsigned_char_p_t) if_reg->name, tstatus);
            dce_error_inq_text(*tstatus, errorbuf, (int *) &cstatus);
            TSHMessage(ctx, MOINFO, "rpc_ep_register_no_replace status: %s\n", errorbuf);
            if (!TSHStatus(ctx, TEST, "rpc_ep_register_no_replace failed", *tstatus, estatus))
            	*tstatus = rpc_s_ok;
			                    
            if (bindings != NULL)
            {
                rpc_binding_vector_free(&bindings, tstatus);
                dce_error_inq_text(*tstatus, errorbuf, &cstatus);
                TSHMessage(ctx, MOINFO, "rpc_binding_vector_free status: %s\n", errorbuf);
                TSHStatus(ctx, CLNUP, "rpc_binding_vector_free failed", *tstatus, rpc_s_ok);
            }                           /* bindings != NULL */

        }
    } 
		if (subcase == EP_REG_NO_REPLACE) 
			unregister_ifs(ctx, subcase, estatus, tstatus); 

}  


unregister_ifs(ctx, subcase, estatus, tstatus)
CtxBlock                *ctx;
ndr_long_int      		subcase;		/* argv[0], subcase to run */
unsigned32			    estatus;		/* argv[1], expected return from test */
unsigned32				*tstatus; 
{
    unsigned long	interface;
	rpc_binding_vector_t  *bindings; 
    struct if_reg *	if_reg;
    dce_error_string_t	errorbuf;
    unsigned32      cstatus;

    for (if_reg = If; if_reg->s_ifspec; if_reg++) 
    { 
	    rpc_server_inq_bindings(&bindings, tstatus);
		dce_error_inq_text(*tstatus, errorbuf, &cstatus);
   		TSHMessage(ctx, MOINFO, "rpc_server_inq_bindings status: %s\n", errorbuf);
        TSHStatus(ctx, CLNUP, "rpc_server_inq_bindings failed", *tstatus, rpc_s_ok);

     	rpc_ep_unregister( *if_reg->s_ifspec, 
                      bindings,
                      (uuid_vector_t *) NULL,
                      tstatus);
		dce_error_inq_text(*tstatus, errorbuf, &cstatus);
   		TSHMessage(ctx, MOINFO, "rpc_ep_unregister status: %s\n", errorbuf);
        if (*tstatus == ept_s_not_registered)
            *tstatus = rpc_s_ok;
        TSHStatus(ctx, CLNUP, "rpc_ep_unregister failed", *tstatus, rpc_s_ok);

		rpc_server_unregister_if(*if_reg->s_ifspec,
			(uuid_p_t) 0,
			tstatus);
		dce_error_inq_text(*tstatus, errorbuf, &cstatus);
   		TSHMessage(ctx, MOINFO, "rpc_server_unregister_if status: %s\n", errorbuf);
        TSHStatus(ctx, CLNUP, "rpc_server_unregister_if failed", *tstatus, rpc_s_ok);
             

        rpc_binding_vector_free(&bindings, tstatus);
		dce_error_inq_text(*tstatus, errorbuf, &cstatus);
  		TSHMessage(ctx, MOINFO, "rpc_binding_vector_free status: %s\n", errorbuf);
       	TSHStatus(ctx, CLNUP, "rpc_binding_vector_free failed", *tstatus, rpc_s_ok);


	}

} 



do_rpc_server_inq_if_test(ctx, subcase, estatus, tstatus)
CtxBlock                *ctx;
ndr_long_int      		subcase;		/* argv[0], subcase to run */
unsigned32			    estatus;		/* argv[1], expected return from test */
unsigned32				*tstatus; 
{ 
	uuid_t	arbitrary_uuid;
	rpc_mgr_epv_t			mgr_epv;

	/* do rpc_server_inq_if test */
	switch (estatus)  
	{  
		case rpc_s_ok:
		case rpc_s_unknown_if: 
        	rpc_server_inq_if(*If->s_ifspec, NULL, &mgr_epv, tstatus); 
        	TSHMessage(ctx, MOINFO, "rpc_server_inq_if status: %d\n", *tstatus);
            if (!TSHStatus(ctx, TEST, "rpc_server_inq_if failed", *tstatus, estatus))
				*tstatus = rpc_s_ok;
            break;

		case rpc_s_unknown_mgr_type: 
			uuid_create(&arbitrary_uuid, tstatus);
       		TSHMessage(ctx, MOINFO, "uuid_create status: %d\n", *tstatus);
            TSHStatus(ctx, SETUP, "uuid_create failed", *tstatus, rpc_s_ok);

        	rpc_server_inq_if(*If->s_ifspec, &arbitrary_uuid, &mgr_epv, tstatus); 
        	TSHMessage(ctx, MOINFO, "rpc_server_inq_if status: %d\n", *tstatus);
            if (!TSHStatus(ctx, TEST, "rpc_server_inq_if failed", *tstatus, estatus))
   				*tstatus = rpc_s_ok;
           break;
	}

	if (estatus != rpc_s_unknown_if)
		unregister_ifs(ctx, subcase, estatus, tstatus);
		
}




/*  Args required by the test program...
 */
#define ARGS_REQUIRED 7


/*  Generic test progrm entry point
 */
ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock          *ctx;
ndr_long_int      argc;
unsigned_char_p_t argv[];
{
	ndr_long_int      		subcase;		/* argv[0], subcase to run */
	unsigned32			    estatus;		/* argv[1], expected return from test */
    
	rpc_binding_handle_t	ep_binding;
	rpc_binding_handle_t	binding;
	unsigned32				inq_type;
	rpc_if_id_t       		if_id; 		/* if_spec for call_handle */
	unsigned32				if_id_choice;
	unsigned32				vers;
	rpc_ep_inq_handle_t		inq_context;
	rpc_ep_inq_handle_t		old_context;
	unsigned32				tstatus;
	unsigned32				cstatus;
	unsigned_char_p_t		annotation;
    unsigned_char_p_t 		bound_string; 
	unsigned32				i;
	unsigned32				estatus_begin;
	unsigned32				estatus_next;
	unsigned32				estatus_unregister;
	unsigned32				estatus_done;
	unsigned32				ttype_begin;
	unsigned32				ttype_next;
	unsigned32				ttype_unregister;
	unsigned32				ttype_done;



/*  check the number of arguments passed in.  They should be exactly the   
 *  required number when you come into this routine. Looking for a subcase,
 *  a bogus expected_status, and a hostname.
 */
if(argc != ARGS_REQUIRED)
    {
    TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
        ARGS_REQUIRED, argc);
    return(TSH_BAD_ARG_COUNT);
    } 

	tstatus = rpc_s_ok;
	cstatus = rpc_s_ok;


/*  atol() the input args...
 */         
	subcase = atol(argv[0]);
	estatus = atol(argv[1]); 
	/* argv[2] is the hostname */
	/* argv[3] is the protocol */
	inq_type = atol(argv[4]);
	if_id_choice = atol(argv[5]);  
	vers	 = atol(argv[6]);  
           

	if (subcase != INQ_IF || estatus != rpc_s_unknown_if)
		register_ifs(ctx, subcase, estatus, argv[2], argv[3], &tstatus);   
          
	switch(subcase)
        { 
		case EP_REG_NO_REPLACE:
			return(tstatus); 
		case INQ_IF:
			do_rpc_server_inq_if_test(ctx, subcase, estatus, &tstatus); 
			return(tstatus);
			break;

        case EP_ELT_INQ_BEGIN:
            TSHMessage(ctx, MOINFO, "running subcase EP_ELT_INQ_BEGIN\n");
            ttype_begin = TEST; 
            estatus_begin = estatus;
    
            ttype_next  = CLNUP;
            estatus_next  = rpc_s_ok;
    
            ttype_done  = CLNUP;
            estatus_done  = rpc_s_ok;
            break;
    
        case EP_ELT_INQ_NEXT:
            TSHMessage(ctx, MOINFO, "running subcase EP_ELT_INQ_NEXT\n");
            ttype_begin = SETUP;
            estatus_begin = rpc_s_ok;
    
            ttype_next  = TEST;
            estatus_next  = estatus;
    
            ttype_done  = CLNUP;
            estatus_done  = rpc_s_ok;
            break;            

        case EP_UNREGISTER:
            TSHMessage(ctx, MOINFO, "running subcase EP_ELT_INQ_UNREGISTER\n");
            ttype_begin = SETUP;
            estatus_begin = rpc_s_ok;
    
            ttype_next  = SETUP;
            estatus_next  = rpc_s_ok;
    
            ttype_unregister  = TEST;
            estatus_unregister  = estatus;

            ttype_done  = CLNUP;
            estatus_done  = rpc_s_ok;
            break;

    
        case EP_ELT_INQ_DONE:
            TSHMessage(ctx, MOINFO, "running subcase EP_ELT_INQ_DONE\n");
            ttype_begin = SETUP;
            estatus_begin = rpc_s_ok;
    
            ttype_next  = SETUP;
            estatus_next  = rpc_s_ok;
    
            ttype_done  = TEST;
            estatus_done  = estatus;
            break;
    
        default:
            TSHMessage(ctx, MOFAILURE, "Unrecognized subcase: %d\n", subcase);
            return(TSH_UNKNOWN_SUBCASE);
        }


    rpc_string_binding_compose(
       (unsigned_char_p_t) NULL,
       (unsigned_char_p_t)argv[3],
       argv[2], NULL, NULL, &bound_string, &tstatus);
    if (TSHStatus(ctx, SETUP, "rpc_string_binding_compose", tstatus, rpc_s_ok))
       return(tstatus);    
    rpc_binding_from_string_binding(bound_string, &ep_binding, &tstatus);
	TSHMessage(ctx, MOINFO, "rpc_binding_from_string_binding status: %d\n", tstatus);
    if (TSHStatus(ctx, SETUP, "rpc_binding_from_string_binding", tstatus, rpc_s_ok))
       return(tstatus);


	rpc_mgmt_ep_elt_inq_begin(ep_binding, inq_type, &if_ids[if_id_choice], vers, NULL, 
		&inq_context, &tstatus);
    TSHMessage(ctx, MOINFO, "rpc_mgmt_ep_elt_inq_begin status: %d\n", tstatus); 
    TSHStatus(ctx, ttype_begin, "rpc_mgmt_ep_elt_inq_begin", tstatus, estatus_begin);
                                
      
	if (tstatus == rpc_s_ok && (subcase == EP_ELT_INQ_NEXT || subcase == EP_UNREGISTER)) {
		i = 0; 
		if (estatus == rpc_s_invalid_inquiry_context) {
			old_context = inq_context;
			inq_context = (rpc_ep_inq_handle_t)NULL;
		}	  
		while (true) 
		{
 
	   		 rpc_mgmt_ep_elt_inq_next(inq_context, &if_id, &binding, NULL, 
					&annotation, &tstatus); 
		    TSHMessage(ctx, MOINFO, "rpc_mgmt_ep_elt_inq_next status: %d\n", tstatus); 
          	/* There is no easy way to know when the test should reutrn rpc_s_ok or rpc_s_no_more_elements,
				therefore, it's handled after the loop, before break.  Only invalid_inquiry_context
                can be forced to generate the status. */
 			if (estatus_next == rpc_s_invalid_inquiry_context)
				TSHStatus(ctx, ttype_next, "rpc_mgmt_ep_elt_inq_next", tstatus, estatus_next);
                         
                                   
			if ((subcase == EP_UNREGISTER && tstatus == rpc_s_ok && (estatus == rpc_s_ok || estatus == rpc_s_no_interfaces)) ||  
                (tstatus == rpc_s_no_more_elements && estatus == rpc_s_invalid_binding) ) {
				/* delete endpoint address */ 
				if (estatus == rpc_s_no_interfaces)
					rpc_mgmt_ep_unregister(ep_binding, NULL, binding, (uuid_t *)NULL, &tstatus);
				else
					rpc_mgmt_ep_unregister(ep_binding, &if_id, binding, (uuid_t *)NULL, &tstatus);
		    	TSHMessage(ctx, MOINFO, "rpc_mgmt_ep_unregister: %d\n", tstatus); 
		    	if (TSHStatus(ctx, ttype_unregister, "rpc_mgmt_ep_unregister", tstatus, estatus))
                 	if (estatus == rpc_s_no_interfaces)
						break;
            }

                
			if (tstatus == rpc_s_ok) {
				i++;
				if (binding != NULL) {
					rpc_binding_free(&binding,  &tstatus);
			    	TSHMessage(ctx, MOINFO, "rpc_binding_free: %d\n", tstatus); 
    				TSHStatus(ctx, CLNUP, "rpc_binding_free", cstatus, rpc_s_ok);
				}
				rpc_string_free(&annotation, &tstatus);
			    TSHMessage(ctx, MOINFO, "rpc_string_free: %d\n", tstatus); 
			    TSHStatus(ctx, CLNUP, "rpc_string_free", cstatus, rpc_s_ok);
			}
			else  { 
				/* At this point, for inquiry_next test, the test status should be no more elements */
				if (estatus != rpc_s_invalid_inquiry_context && subcase == EP_ELT_INQ_NEXT)
					TSHStatus(ctx, TEST, "rpc_mgmt_ep_elt_inq_next", tstatus, rpc_s_no_more_elements);
				break;
			}
		} /* while */  
		if (estatus_next == rpc_s_invalid_inquiry_context) 
			inq_context = old_context;  /* restore context for inq_done */
	}
     
		

    if (subcase == EP_ELT_INQ_DONE) {
		if (estatus == rpc_s_invalid_inquiry_context) {
			old_context = inq_context;
			inq_context = (rpc_ep_inq_handle_t)NULL;
		}
    	rpc_mgmt_ep_elt_inq_done(&inq_context, &tstatus); 
        TSHMessage(ctx, MOINFO, "rpc_mgmt_ep_elt_inq_done status: %d\n", tstatus); 
        TSHStatus(ctx, ttype_done, "rpc_mgmt_ep_elt_inq_done", tstatus, estatus_done);
		if (estatus == rpc_s_invalid_inquiry_context) {
				inq_context = old_context;
		    	rpc_mgmt_ep_elt_inq_done(&inq_context, &cstatus);  
      	   		TSHMessage(ctx, MOINFO, "rpc_mgmt_ep_elt_inq_done status: %d\n", cstatus); 
            	TSHStatus(ctx, ttype_done, "rpc_mgmt_ep_elt_inq_done", cstatus, rpc_s_ok);
		}
 	}
	else {
    	rpc_mgmt_ep_elt_inq_done(&inq_context, &cstatus); 
        TSHMessage(ctx, MOINFO, "rpc_mgmt_ep_elt_inq_done status: %d\n", cstatus); 
        TSHStatus(ctx, ttype_done, "rpc_mgmt_ep_elt_inq_done", cstatus, rpc_s_ok);
	}    

	if (tstatus != estatus)
		return(estatus);
	if (cstatus != rpc_s_ok)
		return(cstatus);
	return(rpc_s_ok);

}

void fake(handle) 
handle_t 	handle;
{
}

void fakes(handle)
handle_t 	handle;
{
}

