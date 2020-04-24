/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* 
 * HISTORY 
 * $Log: do_import_binding.c,v $
 * Revision 1.1.4.2  1996/02/17  23:34:45  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:55  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:09:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:24:18  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:51:58  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:48:47  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/21  22:05:30  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  19:49:38  cbrooks]
 * 
 * $EndLog$
 */

/* 
 * do_import_binding( entryName, interfaceSpec, validationfunction, 
 		bindingHandle, status)
 *
 * Get binding from name service database. 
 * N.B. only returns a bad status code providing that we cannot retrieve 
 * a binding. Failures to end import, free bindings, etc., are announced,
 * but the program continues.
 */

#include <dtscp.h>
#include <stddef.h>

void dts_dtscp_do_import_binding ( 
        char 	entry_name[], 
	rpc_if_handle_t interface_spec,	
	boolean (*validateFunc)(rpc_binding_handle_t _1),
	rpc_binding_handle_t  *binding_h,
	unsigned32 * statusp
	)
{
    rpc_ns_handle_t import_context; /* required to import (rpcbase.h) */
    unsigned32 localstatus = rpc_s_ok ;
    unsigned32 returnstatus= rpc_s_ok ;

    /* begin */

    *binding_h = 0 ;

    rpc_ns_binding_import_begin( /* set context to import binding handles */
	rpc_c_ns_syntax_default, /* use default syntax                  */
	(unsigned_char_t *)entry_name, /* begin search with this name   */
	interface_spec,		/* interface specification (inventory.h)*/
	NULL,			/* no optional object UUID required     */
	&import_context,	/* import context obtained              */
	&localstatus 
	);

    dts_dtscp_check_status(localstatus, "Can't begin import:", 
			   dts_e_dtscp_resume);
    if (localstatus != rpc_s_ok) 
    {
	returnstatus = localstatus ;
	goto LastExit ;
    }

    for(;;)
    {   
	/* import a binding handle */

	rpc_ns_binding_import_next( 
	   import_context,	/* context from rpc_ns_binding_import_begin */
	   binding_h,		/* a binding handle is obtained             */
           &localstatus 				   
	   );

	if( localstatus != rpc_s_ok) 
	{
	    dts_dtscp_check_status(localstatus,
				   "Can't import a binding handle:", 
				   dts_e_dtscp_resume);
	    returnstatus = localstatus ;
	    break;
	}

#ifdef VALIDATE_ENDPOINTS
	if (validateFunc( *binding_h )) &&
	  ( rpc_ep_resolve_binding( binding_h, interface_spec, &localstatus), localstatus == rpc_s_ok))
		break;
#else
	if ( validateFunc( *binding_h ) ) /* we'll take this one */
	    break;
#endif
	else 
	{
	    /* free binding information not selected */

	    rpc_binding_free( binding_h, &localstatus );
	    dts_dtscp_check_status(localstatus,
				   "free binding information fails:", 
				   dts_e_dtscp_resume);
	}
    } /*end while */

    rpc_ns_binding_import_done( &import_context, &localstatus );
    dts_dtscp_check_status(localstatus, "end import fails:", dts_e_dtscp_resume);

 LastExit:
    ;
    *statusp = returnstatus ;
    return;    
}

