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
 * Revision 1.1.10.2  1996/02/17  23:13:20  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:13  marty]
 *
 * Revision 1.1.10.1  1995/12/11  20:00:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:29:37  root]
 * 
 * Revision 1.1.8.1  1994/06/23  18:25:28  tom
 * 	From DEC: add opaque context handle support.
 * 	[1994/06/23  18:24:12  tom]
 * 
 * Revision 1.1.6.2  1993/06/15  21:05:27  sommerfeld
 * 	In done(), free *ch rather than ch.
 * 	[1993/06/14  21:17:02  sommerfeld]
 * 
 * Revision 1.1.2.2  1993/01/07  19:49:54  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:30:24  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:24:16  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <context2.h>
#include <test_common.h>

static int null_return_count = 0;

typedef struct entry_t
{
    struct entry_t *next;
} entry_t;

entry_t Table = {NULL};
entry_t Table2 = {NULL};

static void new
(
    /* [in] */ handle_t h,
    /* [out] */ ch_t *ch
)
{
    entry_t *n;

    null_return_count++;
    if (null_return_count%10)
    {
        if (n = (entry_t *)malloc(sizeof(entry_t)))
        {
            n->next = Table.next;
            Table.next = n;
        }
        else printf("new: Out of memory, returning NULL context handle\n");

        *ch = n;
    }
    else *ch = NULL;
}

static void new_2
(
    /* [in] */ handle_t h,
    /* [out] */ ch_t2 *ch
)
{
    entry_t *n;

    if (null_return_count%10)
    {
        if (n = (entry_t *)malloc(sizeof(entry_t)))
        {
            n->next = Table2.next;
            Table2.next = n;
        }
        else printf("new: Out of memory, returning NULL context handle\n");

        *ch = n;
    }
    else *ch = NULL;
}

/* Function returning a context handle */
static idl_void_p_t new_return(h)
    /* [in] */ handle_t h;
{
    ch_t ch;
    new(h,&ch);
    return (idl_void_p_t) ch;
}

static idl_void_p_t new_return_2(h)
    /* [in] */ handle_t h;
{
    ch_t2 ch;
    new_2(h,&ch);
    return (idl_void_p_t) ch;
}

static idl_ulong_int inq
(
    /* [in] */ ch_t ch
)
{
    return (idl_ulong_int)ch;
}

static idl_ulong_int inq_2
(
    /* [in] */ ch_t2 ch
)
{
    return (idl_ulong_int)ch;
}

static idl_small_int done
(
    /* [in, out] */ ch_t *ch
)
{
    entry_t *tmp = &Table;
    boolean32 found = false;

    while (tmp && !found)
    {
        if (tmp->next == *ch) 
        {
            found = true;
	    tmp->next = tmp->next->next;
            free(*ch);
        }
        tmp = tmp->next;
    }

    *ch = NULL;

    if (Table.next == NULL) 
    {
       fprintf(stdout,"%% Server Done: Table empty %%\n");
       fflush(stdout);
       fflush(stderr);
    }

    if (!found) 
    {
        printf("done: entry not found!\n");
        fflush(stdin);
        return 1;
    }
    else return 0;
}

static idl_small_int done_2
(
    /* [in, out] */ ch_t2 *ch
)
{
    entry_t *tmp = &Table2;
    boolean32 found = false;

    while (tmp && !found)
    {
        if (tmp->next == *ch)
        {
            found = true;
            tmp->next = tmp->next->next;
            free(*ch);
        }
        tmp = tmp->next;
    }

    *ch = NULL;

    if (Table2.next == NULL) 
    {
       fprintf(stdout,"%% Server Done: Table empty %%\n");
       fflush(stdout);
       fflush(stderr);
    }

    if (!found) 
    {
        printf("done_2: entry not found!\n");
        fflush(stdin);
        return 1;
    }
    else return 0;
}

void ch_t_rundown
#ifdef NIDL_PROTOTYPES
(
    rpc_ss_context_t context_handle
)
#else
(context_handle)
    rpc_ss_context_t context_handle;
#endif
{
#ifdef DEBUG
    fprintf(stdout,"%% Server Rundown: called with %lx %%\n", context_handle);
#else
    fprintf(stdout,"\n%% Server Rundown called %%\n");
#endif
    fflush(stdout);
    fflush(stderr);
    done((ch_t *)&context_handle);
}

void ch_t2_rundown
#ifdef NIDL_PROTOTYPES
(
    rpc_ss_context_t context_handle
)
#else
(context_handle)
    rpc_ss_context_t context_handle;
#endif
{
#ifdef DEBUG
    fprintf(stdout,"%% Server Rundown: called with %lx %%\n", context_handle);
#else
    fprintf(stdout,"\n%% Server Rundown called %%\n");
#endif
    fflush(stdout);
    fflush(stderr);
    done_2((ch_t2 *)&context_handle);
}


/*globaldef*/ context2_v0_0_epv_t context2_v0_0_m_epv = {new, inq, done,
                                                              new_return,
                                                         new_2, inq_2, done_2,
                                                              new_return_2};
