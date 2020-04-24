/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pgo_test_basic.c,v $
 * Revision 1.2.11.3  1996/02/17  23:25:36  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:45  marty]
 *
 * Revision 1.2.11.2  1995/12/11  21:49:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:38  root]
 * 
 * Revision 1.2.7.1  1994/10/25  17:18:01  sekhar
 * 	[ OT 12398 ]  Checking in Sean Mullan's changes for testing
 * 		      security replication.
 * 	[1994/10/24  17:00:53  sekhar]
 * 
 * Revision 1.2.4.1  1993/10/13  20:35:22  mccann
 * 	CR8651 64bit porting changes
 * 	[1993/10/13  20:35:01  mccann]
 * 
 * Revision 1.2.2.2  1992/12/31  17:31:05  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:56:28  htf]
 * 
 * Revision 1.2  1992/01/19  22:14:39  devrcs
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
   
#define main_code
#include <sec_rgy.h>
#include <sec_rgy_bind.h>

/* Global variables */
sec_rgy_handle_t        write_context;  /* from sec_rgy_site_open_update */
sec_rgy_handle_t        read_context;   /* from sec_rgy_site_open_query */
         
void do_get_by_name  
#ifdef __STDC__
(    
CtxBlock           *ctx,
ndr_long_int       subcase,
error_status_t     estatus,	
sec_rgy_domain_t   domain,
sec_rgy_name_t     pgo_name,
signed32	   unix_num,
error_status_t     *tstatus	
)
#else
(ctx, subcase, estatus, domain, pgo_name, unix_num, tstatus)
CtxBlock           *ctx;
ndr_long_int       subcase;
error_status_t     estatus;	
sec_rgy_domain_t   domain;
sec_rgy_name_t     pgo_name;
signed32	   unix_num;
error_status_t     *tstatus;	
#endif
{
    sec_rgy_pgo_item_t pgo, item, item_check;
    sec_rgy_name_t     item_name;
    sec_rgy_cursor_t   cursor;
    sec_rgy_name_t     scope;

    *scope = '\0';

    /* wait for propagation to any replicas */
    SEC_RGY_SITE_SYNCH(write_context);

    sec_rgy_cursor_reset(&cursor);
    CALL_RGY(sec_rgy_pgo_get_by_name)(read_context, domain, pgo_name, &cursor, &item, tstatus);
    TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_get_by_name(%s..%s), status: %d\n", 
                domain_text[domain], pgo_name, *tstatus);
    if (TSHStatus (ctx, (subcase == PGO_ADD)? SETUP : TEST ,
        	  "sec_rgy_pgo_get_by_name failed", *tstatus,
                  (subcase == PGO_ADD)? error_status_ok: estatus)) 
       	return;
    else
	*tstatus = error_status_ok;
    if (estatus != error_status_ok)
	return;  /* if expected is not ok, no sense to check the result */
    if (subcase == PGO_ADD ) {
	compare_pgo(ctx, "sec_rgy_pgo_add", pgo_name, &pgo, pgo_name, &item, tstatus); 
       	return;
    }
    sec_rgy_cursor_reset(&cursor);
    CALL_RGY(sec_rgy_pgo_get_by_unix_num)(read_context, domain, scope, unix_num, true,
                                          &cursor, &item_check, item_name, tstatus);
    if (TSHStatus (ctx, SETUP, "sec_rgy_pgo_get_by_unix_num failed", 
                   *tstatus, error_status_ok)) 
       	return;
    compare_pgo(ctx, "sec_rgy_pgo_get_by_name", pgo_name, &item, item_name, 
                &item_check, tstatus); 
}

  
void do_get_by_id
#ifdef __STDC__
(    
CtxBlock          *ctx,
error_status_t     estatus,	
sec_rgy_domain_t   domain,
sec_rgy_name_t     pgo_name,
boolean32          is_an_alias,
error_status_t     *tstatus	
)
#else
(ctx, estatus, domain, pgo_name, is_an_alias, tstatus)
CtxBlock           *ctx;
error_status_t     estatus;	
sec_rgy_domain_t   domain;
sec_rgy_name_t     pgo_name;
boolean32          is_an_alias; 
error_status_t     *tstatus;	
#endif
{
    sec_rgy_pgo_item_t pgo, item, item_check;
    sec_rgy_name_t     item_name;
    sec_rgy_cursor_t   cursor;
    sec_rgy_name_t     scope;

    /* wait for propagation to any replicas */
    SEC_RGY_SITE_SYNCH(write_context);

    *scope = '\0';
    if (estatus == sec_rgy_object_not_found){ /* To generate a new ID */
    CALL_RGY(uuid_create_nil)(&(item.id), tstatus);
    if (TSHStatus (ctx, SETUP, "uuid_create_nil failed", 
                   *tstatus,  error_status_ok))
        return;      
    } else { 
        sec_rgy_cursor_reset(&cursor); 
    	/* To get the id of the name */
        CALL_RGY(sec_rgy_pgo_get_by_name)(read_context, domain, pgo_name,
                                          &cursor, &item, tstatus);
        if (TSHStatus (ctx, SETUP, "sec_rgy_pgo_get_by_name failed", 
                       *tstatus, error_status_ok)) 
  	    return;
    }
    sec_rgy_cursor_reset(&cursor);
    CALL_RGY(sec_rgy_pgo_get_by_id)(read_context, domain, scope, &item.id,
                                    is_an_alias, &cursor, &item_check,
		                    item_name, tstatus);
    TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_get_by_id(%s), status: %d\n", 
                domain_text[domain], *tstatus);
    if (TSHStatus (ctx, TEST, "sec_rgy_pgo_get_by_id failed", *tstatus, estatus)) 
       	return;
    else
	*tstatus = error_status_ok;
    if (estatus != error_status_ok)
	return;  /* if expected is not ok, no sense to check the result */

    compare_pgo(ctx, "sec_rgy_pgo_get_by_id", pgo_name, &item, 
                item_name, &item_check, tstatus); 
}

void do_get_by_unix_num
#ifdef __STDC__
(    
CtxBlock          *ctx,
error_status_t     estatus,	
sec_rgy_domain_t   domain,
sec_rgy_name_t     pgo_name,
signed32	   unix_num,
boolean32          is_an_alias, 
error_status_t     *tstatus	
)
#else
(ctx, estatus, domain, pgo_name, unix_num, is_an_alias, tstatus)
CtxBlock           *ctx;
error_status_t     estatus;	
sec_rgy_domain_t   domain;
sec_rgy_name_t     pgo_name;
signed32           unix_num;
boolean32          is_an_alias; 
error_status_t     *tstatus;	
#endif
{
    sec_rgy_pgo_item_t pgo, item, item_check;
    sec_rgy_name_t     item_name;
    sec_rgy_cursor_t   cursor;
    sec_rgy_name_t     scope;

    /* wait for propagation to any replicas */
    SEC_RGY_SITE_SYNCH(write_context);

    *scope = '\0';

    sec_rgy_cursor_reset(&cursor);
    CALL_RGY(sec_rgy_pgo_get_by_unix_num) (read_context, domain, scope, 
                                           unix_num, is_an_alias, &cursor, 
               			           &item_check, item_name, tstatus);
    TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_get_by_unix_num (%s..%d), status: %d\n", 
                domain_text[domain], unix_num,* tstatus);
    if (TSHStatus (ctx, TEST, "sec_rgy_pgo_get_by_unix_num failed",
                   *tstatus, estatus)) 
       	return;
    else
	*tstatus = error_status_ok;
    if (estatus != error_status_ok)
	return;  /* if expected is not ok, no sense to check the result */

    sec_rgy_cursor_reset(&cursor);
    CALL_RGY(sec_rgy_pgo_get_by_name)(read_context, domain, pgo_name,
                                      &cursor, &item, tstatus);
    if (TSHStatus (ctx, SETUP, "sec_rgy_pgo_get_by_name failed",
                   *tstatus, error_status_ok)) 
       	return;
    compare_pgo(ctx, "sec_rgy_pgo_get_by_unix_num", pgo_name,
                &item, item_name, &item_check, tstatus); 
} 


void do_replace 
#ifdef __STDC__
(    
CtxBlock          *ctx,
error_status_t     estatus,	
sec_rgy_domain_t   domain,
sec_rgy_name_t     pgo_name,
signed32	   unix_num,
boolean32          is_an_alias, 
error_status_t     *tstatus	
)
#else
(ctx, estatus, domain, pgo_name, unix_num, is_an_alias, tstatus)
CtxBlock           *ctx;
error_status_t     estatus;	
sec_rgy_domain_t   domain;
sec_rgy_name_t     pgo_name;
signed32           unix_num;
boolean32          is_an_alias; 
error_status_t     *tstatus;	
#endif
{
    sec_rgy_pgo_item_t pgo, item, item_check;
    sec_rgy_name_t     item_name;
    sec_rgy_cursor_t   cursor;
    sec_rgy_name_t     scope;

    /* wait for propagation to any replicas */
    SEC_RGY_SITE_SYNCH(write_context);

    *scope = '\0';

    if (estatus == error_status_ok || estatus == sec_rgy_not_implemented) {
	/* Expecting to find the object, so go get the uuid */
        sec_rgy_cursor_reset(&cursor); 
   	/* since uuid field can't be changed, so get the old one */
        CALL_RGY(sec_rgy_pgo_get_by_name)(read_context, domain, pgo_name,
                                          &cursor, &item, tstatus);
        TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_get_by_name(%s..%s), status: %d\n",
                    domain_text[domain], pgo_name, *tstatus);
        if (TSHStatus (ctx,  SETUP, "sec_rgy_pgo_get_by_name failed", 
                       *tstatus, error_status_ok)) 
	    return;
                                      
        sec_rgy_cursor_reset(&cursor);

    	/* form the pgo item */   
    	pgo.id = item.id;
    	pgo.flags = (is_an_alias)? sec_rgy_pgo_is_an_alias: sec_rgy_pgo_flags_none;
    	if (unix_num != 0)
      	    pgo.unix_num = unix_num;
    	else
    	    pgo.unix_num = item.unix_num;
        sprintf((char *) pgo.fullname, "%s %05.05d", pgo_name, pgo.unix_num); 
    } 

    CALL_RGY(sec_rgy_pgo_replace)(write_context, domain, pgo_name, &pgo, tstatus);
    TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_replace (%s..%s), status: %d\n", 
                domain_text[domain], pgo_name, *tstatus);
    if (TSHStatus (ctx, TEST, "sec_rgy_pgo_replace failed", *tstatus, estatus)) 
       	return;
    else
	*tstatus = error_status_ok; 
}  



void do_is_member
#ifdef __STDC__
(    
CtxBlock          *ctx,
ndr_long_int       subcase,
error_status_t     estatus,	
sec_rgy_domain_t   domain,
sec_rgy_name_t     go_name,
sec_rgy_name_t     p_name,
boolean32          member,
error_status_t     *tstatus	
)
#else
(ctx, subcase, estatus, domain, go_name, p_name, member, tstatus)
CtxBlock           *ctx;
ndr_long_int       subcase;
error_status_t     estatus;	
sec_rgy_domain_t   domain;
sec_rgy_name_t     go_name;
sec_rgy_name_t     p_name;
boolean32	   member;
error_status_t     *tstatus;	
#endif
{    

    boolean32 is_member=false;

    /* wait for propagation to any replicas */
    SEC_RGY_SITE_SYNCH(write_context);

    is_member = sec_rgy_pgo_is_member(read_context, domain, go_name, p_name, tstatus);
    TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_is_member, result: %d, status: %d\n",
                is_member, *tstatus);
    if (TSHStatus (ctx, (subcase == PGO_ADD_MEMBER)? SETUP: TEST,
        	  "sec_rgy_pgo_is_member failed, status", *tstatus,  
                  (subcase == PGO_ADD_MEMBER)? error_status_ok: estatus))
	return; 
    else
	if (estatus != error_status_ok) {
 	    *tstatus = error_status_ok;
	    return;	
	}


    if (TSHStatus (ctx, (subcase == PGO_ADD_MEMBER)? SETUP: TEST,
		   "sec_rgy_pgo_is_member failed, result", is_member,  
                   (subcase == PGO_ADD_MEMBER)?true: member))
	*tstatus = TSH_DATA_MISSMATCH;
}

  

#define MAX_NUM			2
/* #define MAX_PERSON_NUM	6  */

sec_rgy_member_t    person_name[] = {
	"apple", "pear", "kiwi", "banana", "orange", "grape" };



void get_members_num
#ifdef __STDC__
(    
CtxBlock          *ctx,
sec_rgy_domain_t   domain,
sec_rgy_name_t     go_name,
signed32           *num_members,
error_status_t     *tstatus	
)
#else
(ctx, domain, go_name, num_members, tstatus)
CtxBlock           *ctx;
sec_rgy_domain_t   domain;
sec_rgy_name_t     go_name;
signed32           *num_members;
error_status_t     *tstatus;	
#endif
{
    sec_rgy_cursor_t    cursor; 
    sec_rgy_member_t	member_list[MAX_NUM]; 
    signed32		num_supplied;

    /* wait for propagation to any replicas */
    SEC_RGY_SITE_SYNCH(write_context);

    sec_rgy_cursor_reset(&cursor); 
                  
    CALL_RGY(sec_rgy_pgo_get_members)(read_context, domain, go_name, &cursor, MAX_NUM, 
                                      member_list, &num_supplied, num_members, tstatus);
    TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_get_members, status: %d, num: %d\n",
                *tstatus, *num_members);
    if (TSHStatus (ctx, SETUP, "sec_rgy_pgo_get_member failed", *tstatus,  error_status_ok)) 
	return; 
}
    

void do_get_members
#ifdef __STDC__
(    
CtxBlock          *ctx,
error_status_t     estatus,	
sec_rgy_domain_t   domain,
sec_rgy_name_t     go_name,
error_status_t     *tstatus	
)
#else
(ctx, estatus, domain, go_name, tstatus)
CtxBlock           *ctx;
error_status_t     estatus;	
sec_rgy_domain_t   domain;
sec_rgy_name_t     go_name;
error_status_t     *tstatus;	
#endif
{
    sec_rgy_cursor_t    cursor; 
    sec_rgy_member_t	member_list[MAX_NUM]; 
    signed32		num_supplied;
    signed32		num_members;
    long		cnt=0; 
    long		i, j; 
    boolean32		match;

    /* wait for propagation to any replicas */
    SEC_RGY_SITE_SYNCH(write_context);

    sec_rgy_cursor_reset(&cursor); 
                  
    while (true) {
	CALL_RGY(sec_rgy_pgo_get_members)(read_context, domain, go_name, &cursor, MAX_NUM, 
                                          member_list, &num_supplied, &num_members, tstatus);
	TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_get_members, status: %d\n", *tstatus);
   	if (TSHStatus (ctx, SETUP, "sec_rgy_pgo_get_member failed", *tstatus,  estatus)) 
 	    return;
    	else  /* reset tstatus for estatus != error_status_ok */
	    *tstatus = error_status_ok; 
	if (estatus != error_status_ok)
	    return;
	for (i=0; i<num_supplied; i++) {
	    TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_get_members, name: %s\n", member_list[i]);
	    match = false;
	    for (j=0;; j++) {
		if (person_name[j] == NULL)
		    break;
		if (!strcmp(member_list[i], person_name[j])) {
		    match = true;
		    break;  
		}
	    }  
	    if (!match) {
		*tstatus = TSH_DATA_MISSMATCH; 
		return;
	    }
	} 
	if ((cnt=cnt+num_supplied) >= num_members)
	    break;
    }
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
    sec_rgy_name_t     pgo_name, item_name, other_name;
    sec_rgy_domain_t   domain;
    signed32	       unix_num;
    boolean32          is_an_alias;
    signed32           num_members;

    args_required = 7;

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
    unix_num = UNIX_BASE_NUM + atoi(argv[4]);
    is_an_alias = atoi(argv[5]);
    strcpy(other_name, argv[6]);
	

    tstatus = error_status_ok; /* initial */

    SEC_RGY_SITE_OPEN_WRITE(&write_context);
    SEC_RGY_SITE_OPEN_READ(&read_context);
    CONTEXT = write_context;
                  
    switch (subcase) {
	case PGO_ADD: 
       	    do_add_pgo(ctx, subcase, estatus, domain, pgo_name, unix_num, is_an_alias, &tstatus);
 	    break;

	case PGO_GET_BY_NAME: 
	    do_get_by_name(ctx, subcase, estatus, domain, pgo_name, unix_num, &tstatus);
	    break; 

	case PGO_GET_BY_ID: 
	    do_get_by_id(ctx, estatus, domain, pgo_name, is_an_alias, &tstatus);
	    break;

	case PGO_GET_BY_UNIX_NUM:
	    do_get_by_unix_num(ctx, estatus, domain, pgo_name, unix_num, is_an_alias, &tstatus);
	    break;

	case PGO_DELETE:
	    do_delete_pgo(ctx, subcase, estatus, domain, pgo_name, &tstatus); 
	    break;

	case PGO_RENAME:
	    CALL_RGY(sec_rgy_pgo_rename)(write_context, domain, pgo_name, other_name, &tstatus);
            TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_rename (%s..%s) to %s, status: %d\n", 
                        domain_text[domain], pgo_name, other_name, tstatus);
            if (TSHStatus (ctx, TEST,
	        	  "sec_rgy_pgo_rename failed", tstatus, estatus))
		break;
	    else   /* reset tstatus for estatus != error_status_ok */
		tstatus = error_status_ok; 

	    break;

	case PGO_REPLACE: 
	    do_replace(ctx, estatus, domain, pgo_name, unix_num, is_an_alias, &tstatus);
	    break;

	case PGO_ADD_MEMBER:  
	    /* setup */
	    if (estatus == error_status_ok) {
		/* add person */
  	 	do_add_pgo(ctx, subcase, error_status_ok, domain, pgo_name, unix_num, false, &tstatus);
		if (tstatus != error_status_ok)
		    break;
		do_add_pgo(ctx, subcase, error_status_ok, sec_rgy_domain_person, other_name, unix_num, false, &tstatus);
		if (tstatus != error_status_ok)
		    break;
	    }
	    do_add_member(ctx, subcase, estatus, domain, pgo_name, other_name, &tstatus);
               
            /*  Since is_member is buggy, no checking for now 3/20/91
	    if (estatus != error_status_ok)
			*/ 

	    break;

	case PGO_IS_MEMBER:  /* is_an_alias used to indicate is member or not */
	    if (is_an_alias == false && estatus == error_status_ok)  {
                do_add_pgo (ctx, subcase, error_status_ok, sec_rgy_domain_person, other_name, unix_num, false, &tstatus);
	        if (tstatus != error_status_ok)
		    break;  
	    }
	    do_is_member(ctx, subcase, estatus, domain, pgo_name, other_name, is_an_alias, &tstatus);
	    if (is_an_alias == false && estatus == error_status_ok)  {
          	do_delete_pgo (ctx, subcase, error_status_ok, sec_rgy_domain_person, other_name, &tstatus);
	        if (tstatus != error_status_ok)
		    break;  
	    }
             
	    break; 

	case PGO_DELETE_MEMBER:
	case PGO_DELETE_NO_MEMBER:
	    if (subcase == PGO_DELETE_NO_MEMBER || estatus == sec_rgy_bad_domain) {
		/* add person , but not add to a member*/
  	 	do_add_pgo(ctx, subcase, error_status_ok, sec_rgy_domain_person, other_name, unix_num, false, &tstatus);
		if (tstatus != error_status_ok)
		    break;
	    }
	    do_delete_member(ctx, subcase, estatus, domain, pgo_name, other_name, &tstatus);

	    /* cleanup */
	    if (estatus == error_status_ok)  {
		reset_tstatus(&tstatus); 
		do_delete_pgo(ctx, subcase, error_status_ok, sec_rgy_domain_person, other_name, &tstatus); /* cleanup */ 
		get_members_num(ctx, domain, pgo_name, &num_members, &tstatus);
                if (tstatus != error_status_ok)
		    break;
                if (num_members == 0) 
                    /* Because of the test design, only when no more member exists, the member owner name can be deleted */
		    do_delete_pgo(ctx, subcase, error_status_ok, domain, pgo_name, &tstatus); /* cleanup */
	    } 

	    if (estatus == sec_rgy_bad_domain)
		do_delete_pgo(ctx, subcase, error_status_ok, sec_rgy_domain_person, other_name, &tstatus); /* cleanup */ 
	    break; 

	case PGO_GET_MEMBERS: 
	    do_get_members(ctx, estatus, domain, pgo_name, &tstatus);

	    break;
 
	}
    
    CALL_RGY(sec_rgy_site_close)(write_context, &cstatus); 
    TSHMessage (ctx, MOTRACE, "sec_rgy_site_close status: %d\n", cstatus);

    if (TSHStatus (ctx, CLNUP,
		  "sec_rgy_site_close failed", cstatus, error_status_ok))
        tstatus = cstatus;

    CALL_RGY(sec_rgy_site_close)(read_context, &cstatus);
    TSHMessage (ctx, MOTRACE, "sec_rgy_site_close status: %d\n", cstatus);

    if (TSHStatus (ctx, CLNUP,
                  "sec_rgy_site_close failed", cstatus, error_status_ok))
        tstatus = cstatus;

    return (tstatus);

}
