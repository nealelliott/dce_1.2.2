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
 * Revision 1.1.4.2  1996/02/17  23:14:22  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:48  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:06:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:31:52  root]
 * 
 * Revision 1.1.2.3  1993/02/04  22:26:34  htf
 * 		h_ta_op3() needs to allocate a record_t to be pointed to by *hout,
 * 		the stub will invoke pres_t_free_inst() to free the space.
 * 	[1993/02/04  21:18:08  htf]
 * 
 * Revision 1.1.2.2  1993/01/07  19:58:14  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:34:33  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:32:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*%a%copyright(Copyright)
    73 1.1 5/8/91
*/

#include <stdio.h>
#include <h_ta.h>

ndr_boolean verbose;

static record_t
mgr_op1
#ifdef NIDL_PROTOTYPES
(
    /* in */    pres_t h
)
#else
(h)
pres_t h;
#endif
{
    if (verbose)
        printf("In h_ta_mgr_op1().\n");

    h->called[h->index++] = IN_MANAGER;

    return *h;
}

static record_t
mgr_op2
#ifdef NIDL_PROTOTYPES
(
    /* in, out */   pres_t *h
)
#else
(h)
pres_t *h;
#endif
{
    if (verbose)
        printf("In h_ta_mgr_op2().\n");

    (*h)->called[(*h)->index++] = IN_MANAGER;

    return **h;
}

static record_t
mgr_op3
#ifdef NIDL_PROTOTYPES
(
    /* in */    pres_t hin,
    /* out */   pres_t *hout
)
#else
(hin, hout)
pres_t hin;
pres_t *hout;
#endif
{
    int i = 0;

    if (verbose)
        printf("In h_ta_mgr_op3().\n");

/* 
 * Note that we rely on the stub to allocate and manage a pres_t object and
 * to initialize hout to point to it.  We then allocate space and point the
 *  pres_t object to it.  The space will be freed when the stub invokes
 * pres_t_free_inst()
 */
    if (NULL == (*hout = (pres_t)malloc(sizeof(record_t))))
    {
        fprintf(stderr, "In manager: Cannot allocate op3() hout object.\n");
        exit(1);
    }


    (hin)->called[(hin)->index++] = IN_MANAGER;

/*
 * copy the underlying structure. we can't just " *hout = hin; " as
 * originally written.
 */
    (*hout)->index = hin->index;
    for (i = 0; i < CALLED_SIZE; ++i)
        (*hout)->called[i] = hin->called[i];

    return **hout;
}


globaldef h_ta_v0_0_epv_t h_ta_v0_0_m_epv = {
    mgr_op1,
    mgr_op2,
    mgr_op3
};
