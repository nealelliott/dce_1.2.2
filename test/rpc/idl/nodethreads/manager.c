/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: manager.c,v $
 * Revision 1.1.4.2  1996/02/17  23:15:14  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:12  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:25:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:13:57  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:31:23  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:35:35  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:25:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#define STUBBASE_H
#include <nodethreads.h>
#include <dce/cma.h>
#include <test_common.h>

typedef struct thread_params_t {
    rpc_ss_thread_handle_t rpc_handle;
    list_element *p_list;
} thread_params_t;

void fill_in_1(p_thread_params)
thread_params_t *p_thread_params;
{
    list_element *cursor;
    long *p_long;
                                 
    rpc_ss_set_thread_handle(p_thread_params->rpc_handle);
    for (cursor=p_thread_params->p_list; cursor!=NULL; cursor = cursor->link)
    {
        p_long = (long *)rpc_ss_allocate(sizeof(long));
        *p_long = cursor->value;
        cursor->p_value_1 = p_long;
    }
}

void fill_in_2(p_thread_params)
thread_params_t *p_thread_params;
{
    list_element *cursor;
    long *p_long;
                                 
    rpc_ss_set_thread_handle(p_thread_params->rpc_handle);
    for (cursor=p_thread_params->p_list; cursor!=NULL; cursor = cursor->link)
    {
        p_long = (long *)rpc_ss_allocate(sizeof(long));
        *p_long = cursor->value;
        cursor->p_value_2 = p_long;
    }
}

void fill_in_3(p_list)
list_element *p_list;
{
    list_element *cursor;
    long *p_long;
    
    for (cursor=p_list; cursor!=NULL; cursor = cursor->link)
    {
        p_long = (long *)rpc_ss_allocate(sizeof(long));
        *p_long = cursor->value;
        cursor->p_value_3 = p_long;
    }
}

static void his_oper(h, p_list)
    /* [in] */handle_t h;
    /* [in,out] */ list_element *p_list;
{ 
    cma_t_thread thread_handle;
    cma_t_exit_status thread_status;
    cma_t_address thread_result;
    thread_params_t thread_params;

    thread_params.rpc_handle = rpc_ss_get_thread_handle();
    thread_params.p_list = p_list;
    cma_thread_create(&thread_handle,&cma_c_null,
               (cma_t_start_routine)fill_in_1,(cma_t_address)&thread_params);
    cma_thread_join(&thread_handle,&thread_status,&thread_result);
    cma_thread_create(&thread_handle,&cma_c_null,
               (cma_t_start_routine)fill_in_2,(cma_t_address)&thread_params);
    cma_thread_join(&thread_handle,&thread_status,&thread_result);
    fill_in_3(p_list);
}

globaldef nodethreads_v0_0_epv_t nodethreads_v0_0_m_epv = {his_oper};
