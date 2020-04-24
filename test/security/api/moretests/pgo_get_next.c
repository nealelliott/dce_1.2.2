/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pgo_get_next.c,v $
 * Revision 1.2.4.2  1996/02/17  23:25:34  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:43  marty]
 *
 * Revision 1.2.4.1  1995/12/11  21:49:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:36  root]
 * 
 * Revision 1.2.2.2  1992/12/31  17:30:54  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:56:19  htf]
 * 
 * Revision 1.2  1992/01/19  22:14:36  devrcs
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

/* pgo_get_next.c */
 
#define main_code

#include "sec_rgy.h"

sec_rgy_handle_t  rgy_context;
char	*domain_text[sec_rgy_domain_last] = { "person", "group", "org" };

        
#define DEFAULT_PERSON_NUM	12
#define DEFAULT_GROUP_NUM	13
#define DEFAULT_ORG_NUM      4
  
  

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
    short             existing_cnt;		/* argv[1], expected return from test */
    error_status_t    tstatus;		/* actual value returned from rpc */
    error_status_t    estatus;		/* actual value returned from rpc */
    error_status_t    cstatus;		/* actual value returned from rpc clenaup*/
    unsigned32	      args_required;     /* depends on who called this function */

	sec_rgy_pgo_item_t pgo, item, item_check;
    sec_rgy_name_t     pgo_name, item_name, new_name;
    sec_rgy_domain_t   domain;
    sec_rgy_pname_t    node_name, site_name;/* site_name = ip:node_name for 3/15/91
                                               and rgyd should be on the same node as test node */
	long			   unix_num;

    sec_rgy_cursor_t   cursor;
    sec_rgy_name_t     scope;
	short				cnt;
	short				max_cnt;

 	

    args_required = 3;

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
    existing_cnt = atol(argv[1]); 
	domain  = atoi(argv[2]); 

	switch (domain) {
		case sec_rgy_domain_person: max_cnt = DEFAULT_PERSON_NUM + existing_cnt;
			break;
        case sec_rgy_domain_group:  max_cnt = DEFAULT_GROUP_NUM + existing_cnt;
			break;
		case sec_rgy_domain_org:    max_cnt = DEFAULT_ORG_NUM + existing_cnt;
			break;
	}
                                              
    
	tstatus = error_status_ok; /* initial */
	estatus = error_status_ok; /* initial */
        
    do_site_open(ctx, sec_rgy_status_ok, &CONTEXT, &tstatus); 
  
    sec_rgy_cursor_reset(&cursor); 
	*scope = '\0';
	cnt = 0;

	do {
		CALL_RGY(sec_rgy_pgo_get_next)(CONTEXT, domain, scope, &cursor, &item, item_name, &tstatus);
		cnt++; 
       /*
		if (++cnt <= max_cnt)
			estatus = error_status_ok;
		else
			estatus = sec_rgy_no_more_entries;
        */

        TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_get_next: %d, status: %d\n", cnt, tstatus);

        if (TSHStatus (ctx, TEST,
 			  "sec_rgy_pgo_get_next failed", 1, 1))
	     return(tstatus); 

		if (tstatus == sec_rgy_no_more_entries) {
			tstatus = error_status_ok;  /* reset the status */
			break;
		}
  
   		get_pgo(ctx, estatus, domain, item_name, &pgo, &tstatus);
	    compare_pgo(ctx, "sec_rgy_pgo_get_next", item_name, &item, item_name, &pgo, &tstatus);
	    TSHMessage (ctx, MOTRACE, "compare result, status: %d\n", tstatus);
		if (tstatus != error_status_ok)
			return(tstatus);
	} while (true);
/*
    CALL_RGY(sec_rgy_site_close)(&(CONTEXT), &tstatus); 
    TSHMessage (ctx, MOTRACE, "sec_rgy_site_open for site: %s, status: %d\n", site_name, cstatus);

    if (TSHStatus (ctx, CLNUP,
		  "sec_rgy_site_close failed", cstatus, error_status_ok))
     return(cstatus);
*/
	
    return(tstatus);

}

