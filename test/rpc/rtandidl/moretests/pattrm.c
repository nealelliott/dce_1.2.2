/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY 
 * $Log: pattrm.c,v $
 * Revision 1.1.7.2  1996/02/17  23:21:39  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:50  marty]
 *
 * Revision 1.1.7.1  1995/12/11  20:08:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:23  root]
 * 
 * Revision 1.1.4.1  1994/04/07  22:56:34  tom
 * 	Bug 9966 - explicitly set exception to unknown, don't
 * 	just depend on stack garbage.
 * 	[1994/04/07  22:56:01  tom]
 * 
 * Revision 1.1.2.4  1993/02/03  18:21:55  cuti
 * 	Add routines to handle context handle object
 * 	[1993/02/01  22:39:39  cuti]
 * 
 * Revision 1.1.2.3  1993/01/25  18:06:43  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:42:28  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  23:38:17  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:11:21  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:39:01  devrcs
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

#include <nidltest.h>
#include <pattr.h>
#include <stdio.h>
#include <pthread.h>
#include <dce/rpc.h>
#include <dce/rpcexc.h>

/* extern char *rpc_ss_allocate(); */
  
get_exception(st_in, my_exception)
int st_in;
EXCEPTION *my_exception;
{
           switch (st_in) {
		case 0 : *my_exception = rpc_x_unknown_remote_fault;
			 break;
                case 1 : *my_exception = rpc_x_comm_failure;
                         break;
                case 2 : *my_exception = rpc_x_invalid_tag;
                         break;
                case 3 : *my_exception = rpc_x_invalid_bound;
                         break;
                case 4 : *my_exception = rpc_x_unknown_remote_fault;
                         break;
                case 5 : *my_exception = rpc_x_unknown_error;
                         break;
                case 6 : *my_exception = rpc_x_no_memory;
                         break;
            }  

}





/*
 * p a t t r t e s t _ o p 1
 */
void pattrtest_op1(h, st_out)
handle_t h;
error_status_t *st_out;
{    
	EXCEPTION my_exception;

	get_exception(*st_out, &my_exception);
    RAISE(my_exception);
}
    

/*
 * p a t t r t e s t _ o p 2
 */
void pattrtest_op2(h, st_in, st_out)
handle_t h;
error_status_t st_in;
error_status_t *st_out;
{
	EXCEPTION my_exception;

	get_exception(st_in, &my_exception);
    RAISE(my_exception);
}
    

/*
 * p a t t r t e s t _ o p 3
 *
 */
void pattrtest_op3(h, i, o)
handle_t h;
ndr_long_int *i;
ndr_long_int *o;
{
    /* i is an [in] passed by reference */
    *o = *i + 1;
}

/*
 * p a t t r t e s t _ o p 4
 *
 */
void pattrtest_op4(h, someptr, o)
handle_t h;
ptrtest *someptr;
ndr_long_int *o;
{
    (*someptr)->a = 1;
    (*someptr)->b = 2;
    (*someptr)->c = 3;
}

/*
 * p a t t r t e s t _ o p 5
 *
 */
void pattrtest_op5(h, someptr, o)
handle_t h;
ptrtest *someptr;
ndr_long_int *o;
{
    *someptr = (ptrtest)rpc_ss_allocate(sizeof(realstruct));
    (*someptr)->a = 1;
    (*someptr)->b = 2;
    (*someptr)->c = 3;
}

/*
 * p a t t r t e s t _ o p 6
 *
 */
void pattrtest_op6(h, someptr, o)
handle_t h;
ptrtest *someptr;
ndr_long_int *o;
{
    *someptr = NULL;
}                    



void ch_op1 (h, this_object)
handle_t h;
my_context * this_object;
{

	int i=10; 
         

	*this_object = &i;

}   

void ch_op2(h, this_object, status) 
handle_t	h;
my_context  this_object;
error_status_t	*status;
{
printf("In op2 \n");

      fprintf(stderr, "idl server: exiting...\n");
      rpc_mgmt_stop_server_listening(NULL, status);
      fprintf(stderr, "rpc_server: rpc_mgmt_stop_server_listening status: %d\n", *status);

                                                  
}   

void my_context_rundown(this_object)
my_context  this_object;
{
	/* no op */

}

