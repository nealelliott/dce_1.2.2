/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pgo_key_transfer.c,v $
 * Revision 1.2.4.2  1996/02/17  23:25:35  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:44  marty]
 *
 * Revision 1.2.4.1  1995/12/11  21:49:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:37  root]
 * 
 * Revision 1.2.2.2  1992/12/31  17:30:59  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:56:23  htf]
 * 
 * Revision 1.2  1992/01/19  22:14:37  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
*/
/*
 *  OSF DCE Version 1.0 
*/
/*
 * 
 * Copyright 1989 by Apollo Computer, Inc.
 * 
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of Apollo Computer, Inc not
 * be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 * Apollo Computer, Inc. makes no representations about the
 * suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 * 
 */
 
/* pgo_key_transfer.c */
#define main_code

#include "sec_rgy.h"

do_pgo_name_to_id
#ifdef __STDC__
(    
CtxBlock          *ctx,
error_status_t     estatus,	
sec_rgy_domain_t   domain,
sec_rgy_name_t     pgo_name,
error_status_t     *tstatus	
)
#else
(ctx, estatus, domain, pgo_name, tstatus)
CtxBlock           *ctx;
error_status_t     estatus;	
sec_rgy_domain_t   domain;
sec_rgy_name_t     pgo_name;
error_status_t     *tstatus;	
#endif
{
    sec_rgy_cursor_t    cursor; 
    sec_rgy_pgo_item_t  pgo;
	uuid_t				id;
   
	get_pgo(ctx, estatus, domain, pgo_name, &pgo, tstatus);
	if (*tstatus != error_status_ok || estatus != error_status_ok)
		return;

	CALL_RGY(sec_rgy_pgo_name_to_id)(CONTEXT, domain, pgo_name, &id, tstatus);
    TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_name_to_id, status: %d\n", *tstatus);
    if (TSHStatus (ctx, TEST,
	        	  "sec_rgy_pgo_name_to_id failed", *tstatus, estatus))
		return;
	else { 
		if (estatus != error_status_ok) {
			*tstatus = error_status_ok;
			return; 
		}
	}  
	  
	if (!uuid_equal(&id, &pgo.id, tstatus)) {
	    if (TSHStatus (ctx, SETUP,
	        	  "uuid_equal failed: bad uuid", *tstatus, rpc_s_ok))
		return; 

	    TSHStatus (ctx, TEST,
	        	  "sec_rgy_pgo_name_to_id failed, uuid not matched", true, false);
		*tstatus = TSH_DATA_MISSMATCH;
	}

}  

do_pgo_id_to_name
#ifdef __STDC__
(    
CtxBlock          *ctx,
error_status_t     estatus,	
sec_rgy_domain_t   domain,
sec_rgy_name_t     pgo_name,
error_status_t     *tstatus	
)
#else
(ctx, estatus, domain, pgo_name, tstatus)
CtxBlock           *ctx;
error_status_t     estatus;	
sec_rgy_domain_t   domain;
sec_rgy_name_t     pgo_name;
error_status_t     *tstatus;	
#endif
{
    sec_rgy_cursor_t    cursor; 
    sec_rgy_pgo_item_t  pgo;
	sec_rgy_name_t		name;
   
       
	get_pgo(ctx, estatus, domain, pgo_name, &pgo, tstatus);
	if (*tstatus != error_status_ok || estatus != error_status_ok)
		return;

	CALL_RGY(sec_rgy_pgo_id_to_name)(CONTEXT, domain, &pgo.id, name, tstatus);
    TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_id_to_name, name: %s, status: %d\n", name, *tstatus);
    if (TSHStatus (ctx, TEST,
	        	  "sec_rgy_pgo_id_to_name", *tstatus, estatus))
		return;
	else { 
		if (estatus != error_status_ok) {
			*tstatus = error_status_ok;
			return; 
		}
	}  
	  
	if (strcmp(pgo_name, name)) {
	    TSHStatus (ctx, TEST,
	        	  "sec_rgy_pgo_id_to_name failed, name not matched", true, false);
		*tstatus = TSH_DATA_MISSMATCH;
	}

}

do_pgo_name_to_unix_num
#ifdef __STDC__
(    
CtxBlock          *ctx,
error_status_t     estatus,	
sec_rgy_domain_t   domain,
sec_rgy_name_t     pgo_name,
error_status_t     *tstatus	
)
#else
(ctx, estatus, domain, pgo_name, tstatus)
CtxBlock           *ctx;
error_status_t     estatus;	
sec_rgy_domain_t   domain;
sec_rgy_name_t     pgo_name;
error_status_t     *tstatus;	
#endif
{
    sec_rgy_cursor_t    cursor; 
    sec_rgy_pgo_item_t  pgo;
	long				unix_num;
         

	get_pgo(ctx, estatus, domain, pgo_name, &pgo, tstatus);
	if (*tstatus != error_status_ok || estatus != error_status_ok)
		return;

	CALL_RGY(sec_rgy_pgo_name_to_unix_num)(CONTEXT, domain, pgo_name, &unix_num, tstatus);
    TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_name_to_unix_num, unix_num: %d, status: %d\n", unix_num, *tstatus);
    if (TSHStatus (ctx, TEST,
	        	  "sec_rgy_pgo_name_to_unix_num failed", *tstatus, estatus))
		return;
	else { 
		if (estatus != error_status_ok) {
			*tstatus = error_status_ok;
			return; 
		}
	}  
	  

	if (TSHStatus (ctx, TEST,
	        	  "sec_rgy_pgo_name_to_unix_num failed, unix_num not matched", pgo.unix_num, unix_num))
		*tstatus = TSH_DATA_MISSMATCH;


}


do_pgo_unix_num_to_name
#ifdef __STDC__
(    
CtxBlock          *ctx,
error_status_t     estatus,	
sec_rgy_domain_t   domain,
sec_rgy_name_t     pgo_name,
error_status_t     *tstatus	
)
#else
(ctx, estatus, domain, pgo_name, tstatus)
CtxBlock           *ctx;
error_status_t     estatus;	
sec_rgy_domain_t   domain;
sec_rgy_name_t     pgo_name;
error_status_t     *tstatus;	
#endif
{
    sec_rgy_cursor_t    cursor; 
    sec_rgy_pgo_item_t  pgo;
	sec_rgy_name_t		name;
   
	get_pgo(ctx, estatus, domain, pgo_name, &pgo, tstatus);
	if (*tstatus != error_status_ok || estatus != error_status_ok)
		return;

	CALL_RGY(sec_rgy_pgo_unix_num_to_name)(CONTEXT, domain, pgo.unix_num, name, tstatus);
    TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_unix_num_to_name, name: %s, status: %d\n", name, *tstatus);
    if (TSHStatus (ctx, TEST,
	        	  "sec_rgy_pgo_unix_num_to_name failed", *tstatus, estatus))
		return;
	else { 
		if (estatus != error_status_ok) {
			*tstatus = error_status_ok;
			return; 
		}
	}  
	  
	if (strcmp(pgo_name, name)) {
	    TSHStatus (ctx, TEST,
	        	  "sec_rgy_pgo_unix_num_to_name failed, name not matched", true, false);
		*tstatus = TSH_DATA_MISSMATCH;
	}


} 


do_pgo_unix_num_to_id
#ifdef __STDC__
(    
CtxBlock          *ctx,
error_status_t     estatus,	
sec_rgy_domain_t   domain,
sec_rgy_name_t     pgo_name,
error_status_t     *tstatus	
)
#else
(ctx, estatus, domain, pgo_name, tstatus)
CtxBlock           *ctx;
error_status_t     estatus;	
sec_rgy_domain_t   domain;
sec_rgy_name_t     pgo_name;
error_status_t     *tstatus;	
#endif
{
    sec_rgy_cursor_t    cursor; 
    sec_rgy_pgo_item_t  pgo;
	uuid_t				id;
   
            

	get_pgo(ctx, estatus, domain, pgo_name, &pgo, tstatus);
	if (*tstatus != error_status_ok || estatus != error_status_ok)
		return;

	CALL_RGY(sec_rgy_pgo_unix_num_to_id)(CONTEXT, domain, pgo.unix_num, &id, tstatus);
    TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_unix_num_to_id, status: %d\n", *tstatus);
    if (TSHStatus (ctx, TEST,
	        	  "sec_rgy_pgo_unix_num_to_id failed", *tstatus, estatus))
		return;
	else { 
		if (estatus != error_status_ok) {
			*tstatus = error_status_ok;
			return; 
		}
	}  
	  
	if (!uuid_equal(&id, &pgo.id, tstatus)) {
	    if (TSHStatus (ctx, SETUP,
	        	  "uuid_equal failed: bad uuid", *tstatus, rpc_s_ok))
		return; 

	    TSHStatus (ctx, TEST,
	        	  "sec_rgy_pgo_unix_num_to_id failed, uuid not matched", true, false);
		*tstatus = TSH_DATA_MISSMATCH;
	}

}



do_pgo_id_to_unix_num
#ifdef __STDC__
(    
CtxBlock          *ctx,
error_status_t     estatus,	
sec_rgy_domain_t   domain,
sec_rgy_name_t     pgo_name,
error_status_t     *tstatus	
)
#else
(ctx, estatus, domain, pgo_name, tstatus)
CtxBlock           *ctx;
error_status_t     estatus;	
sec_rgy_domain_t   domain;
sec_rgy_name_t     pgo_name;
error_status_t     *tstatus;	
#endif
{
    sec_rgy_cursor_t    cursor; 
    sec_rgy_pgo_item_t  pgo;
	long    			unix_num;
   
               
	get_pgo(ctx, estatus, domain, pgo_name, &pgo, tstatus);
	if (*tstatus != error_status_ok || estatus != error_status_ok)
		return;

	CALL_RGY(sec_rgy_pgo_id_to_unix_num)(CONTEXT, domain, &pgo.id, &unix_num, tstatus);
    TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_id_to_unix_num,unix_num: %d, status: %d\n", unix_num, *tstatus);
    if (TSHStatus (ctx, TEST,
	        	  "sec_rgy_pgo_id_to_unix_num failed", *tstatus, estatus))
		return;
	else { 
		if (estatus != error_status_ok) {
			*tstatus = error_status_ok;
			return; 
		}
	}  
	  

   if (TSHStatus (ctx, TEST,
	        	  "sec_rgy_pgo_id_to_unix_num failed, uuid not matched", pgo.unix_num, unix_num))
		*tstatus = TSH_DATA_MISSMATCH;


}

ndr_long_int TestFunction
/* Formal parameters */
#ifdef __STDC__
(
CtxBlock          *ctx,
ndr_long_int      argc,
unsigned_char_p_t argv[]
)
#else
(ctx, argc, argv) 
CtxBlock          *ctx;
ndr_long_int      argc;
unsigned_char_p_t argv[];
#endif
{

/* Local variables */
    ndr_long_int      subcase;		/* argv[0], subcase to run */
    error_status_t    estatus;		/* argv[1], expected return from test */
    error_status_t    tstatus;		/* actual value returned from rpc */
    error_status_t    cstatus;		/* actual value returned from rpc clenaup*/
    unsigned32	       args_required;     /* depends on who called this function */

	sec_rgy_pgo_item_t pgo, item, item_check;
    sec_rgy_name_t     pgo_name, item_name, new_name;
    sec_rgy_domain_t   domain;
    sec_rgy_pname_t    node_name, site_name;/* site_name = ip:node_name for 3/15/91
                                               and rgyd should be on the same node as test node */
	long			   unix_num = 1000;

    sec_rgy_cursor_t   cursor;
    sec_rgy_name_t     scope;
	

    args_required = 4;

/* Check the number of arguments passed in.  They should be exactly the   
   required number when you come into this routine. */

    if (argc != args_required)
     {
       TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
					args_required, argc);
       return(TSH_BAD_ARG_COUNT);
     }

/* Obtain the minimum input arguments from the .tsh command line. */

    subcase = atol(argv[0]);
    estatus = atol(argv[1]); 
	domain  = atoi(argv[2]);
	strcpy(pgo_name, argv[3]);
/*	strcpy(site_name, argv[4]);  */

    
	tstatus = error_status_ok; /* initial */
   
    do_site_open(ctx, sec_rgy_status_ok, &CONTEXT, &tstatus); 
	      
	/*  Setup to add PGO */
	if (estatus == error_status_ok) {
		do_add_pgo(ctx, subcase, estatus, domain, pgo_name, unix_num, false, &tstatus);
		if (tstatus != error_status_ok)
			return(tstatus);
	}		

	switch (subcase) {
		case PGO_NAME_TO_ID:
 			do_pgo_name_to_id(ctx, estatus, domain, pgo_name, &tstatus);
			break;

		case PGO_ID_TO_NAME:
            do_pgo_id_to_name(ctx, estatus, domain, pgo_name, &tstatus);
			break;

		case PGO_NAME_TO_UNIX_NUM:
 			do_pgo_name_to_unix_num(ctx, estatus, domain, pgo_name, &tstatus);
			break;

		case PGO_UNIX_NUM_TO_NAME:
            do_pgo_unix_num_to_name(ctx, estatus, domain, pgo_name, &tstatus);
			break;

		case PGO_ID_TO_UNIX_NUM:
            do_pgo_id_to_unix_num(ctx, estatus, domain, pgo_name, &tstatus);
			break;

		case PGO_UNIX_NUM_TO_ID:
            do_pgo_unix_num_to_id(ctx, estatus, domain, pgo_name, &tstatus);
			break;

	} 
     
	/* cleanup */ 
	if (estatus == error_status_ok)
		do_delete_pgo(ctx, subcase, error_status_ok, domain, pgo_name, &tstatus); /* cleanup */
/*
    CALL_RGY(sec_rgy_site_close)(&(CONTEXT), &tstatus); 
    TSHMessage (ctx, MOTRACE, "sec_rgy_site_close for site: %s, status: %d\n", site_name, cstatus);

    if (TSHStatus (ctx, CLNUP,
		  "sec_rgy_site_close failed", cstatus, error_status_ok))
     return(cstatus);

*/
	
	return(tstatus);

}
