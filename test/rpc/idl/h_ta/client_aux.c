/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: client_aux.c,v $
 * Revision 1.1.4.2  1996/02/17  23:14:20  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:47  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:06:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:31:47  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:57:58  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:34:22  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:31:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*%a%copyright(Copyright)
    70 1.1 5/8/91
*/

#include <stdio.h>
#include <h_ta.h>

#ifdef DEBUG
ndr_boolean verbose = ndr_true;
#else
ndr_boolean verbose = ndr_false;
#endif

static handle_t real_handle;

record_t op1_param = {
    4,  /*  index   */
    {   /*  called  */
        BIND_CALLED,
        TO_XMIT_CALLED,
        FREE_XMIT_CALLED,
        UNBIND_CALLED
    },
};

record_t op1_result = {
    4,  /*  index   */
    {   /*  called  */
        BIND_CALLED,
        TO_XMIT_CALLED,
        FROM_XMIT_CALLED,
        IN_MANAGER
    },
};

record_t op2_param = {
    7,  /*  index   */
    {   /*  called  */
        BIND_CALLED,
        TO_XMIT_CALLED,
        FROM_XMIT_CALLED,
        IN_MANAGER,
        TO_XMIT_CALLED,
        FROM_XMIT_CALLED,
        UNBIND_CALLED
    },
};

record_t op3_outparam = {
    6,  /*  index   */
    {   /*  called  */
        BIND_CALLED,
        TO_XMIT_CALLED,
        FROM_XMIT_CALLED,
        IN_MANAGER,
        TO_XMIT_CALLED,
        FROM_XMIT_CALLED
    },
};

#define op3_result op1_result
#define op3_inparam op1_param

static void
show_order(record)
record_t *record;
{
    int i;

    for (i = 0; i < record->index; ++i) {
        char *call;
        switch (record->called[i]) {
        case BIND_CALLED:
            call = "bind";
            break;
        case UNBIND_CALLED:
            call = "unbind";
            break;
        case TO_XMIT_CALLED:
            call = "to_xmit";
            break;
        case FROM_XMIT_CALLED:
            call = "from_xmit";
            break;
        case FREE_XMIT_CALLED:
            call = "free_xmit";
            break;
        case FREE_INST_CALLED:
            call = "free_inst";
            break;
        case IN_MANAGER:
            call = "manager";
            break;
        default:
            call = "UNKNOWN";
            break;
        }
        printf("\t%d.\t%s\n", i, call);
    }
}

static void
clear_record(record)
record_t *record;
{
    int i;

    record->index = 0;
    for (i = 0; i < CALLED_SIZE; ++i)
        record->called[i] = 0;
}

static int
compare_record(expected,actual)
    record_t *expected;
    record_t *actual;
{
    int i;

    if (expected->index != actual->index)
        return 1;

    for (i = 0; i < expected->index; ++i)
        if (expected->called[i] != actual->called[i])
            return 1;

    return 0;
}

int client_aux(h,passes)
    handle_t h;
    int passes;
{
    long        failures = 0;
    record_t    client_record, server_record, result;
    pres_t      pstruct;

    if (verbose)
        printf("In h_ta_test().\n");

    pstruct = &client_record;
    real_handle = h;

    clear_record(&client_record);
    result = client_record;
    result = h_ta_op1(&client_record);
    failures += compare_record(&op1_param, &client_record);
    failures += compare_record(&op1_result, &result);
    if (verbose) {
        printf("\nOrder of support calls in client:\n");
        show_order(&client_record);
        printf("\nOrder of support calls from server:\n");
        show_order(&result);
        printf("\n");
    }

    clear_record(&client_record);
    result = client_record;
    result = h_ta_op2(&pstruct);
    failures += compare_record(&op2_param, pstruct);
    if (verbose) {
        printf("\nOrder of support calls:\n");
        show_order(pstruct);
        printf("\n");
    }

    clear_record(&client_record);
    result = server_record = client_record;
    pstruct = &server_record;
    result = h_ta_op3(&client_record, &pstruct);
    failures += compare_record(&op3_inparam, &client_record);
    failures += compare_record(&op3_outparam, pstruct);
    failures += compare_record(&op3_result, &result);
    if (verbose) {
        printf("\nOrder of support calls in [in] param:\n");
        show_order(&client_record);
        printf("\nOrder of support calls in [out] param:\n");
        show_order(pstruct);
        printf("\nOrder of support calls in return structure:\n");
        show_order(&result);
        printf("\n");
    }

    return failures;
}

handle_t pres_t_bind
#ifdef NIDL_PROTOTYPES
(
    pres_t h
)
#else
(h)
    pres_t h;
#endif
{
    if (verbose)
        printf("In pres_t_bind().\n");

    h->called[h->index++] = BIND_CALLED;

    return real_handle;
}

void pres_t_unbind
#ifdef NIDL_PROTOTYPES
(
    pres_t uh,
    handle_t h
)
#else
(uh, h)
    pres_t uh;
    handle_t h;
#endif
{
    if (verbose)
        printf("In pres_t_unbind().\n");

    uh->called[uh->index++] = UNBIND_CALLED;
}
