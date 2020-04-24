/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: util_xmit.c,v $
 * Revision 1.1.5.2  1996/02/17  23:14:24  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:49  marty]
 *
 * Revision 1.1.5.1  1995/12/11  20:06:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:31:54  root]
 * 
 * Revision 1.1.3.3  1993/01/07  19:58:16  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:34:36  bbelch]
 * 
 * Revision 1.1.3.2  1992/09/29  21:21:43  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:46:53  weisman]
 * 
 * Revision 1.1  1992/01/19  03:32:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*%a%copyright(Copyright)
    74 1.1 5/8/91
*/

#include <h_ta.h>
#include <stdio.h>

extern ndr_boolean verbose;

static pres_t pstruct;
static xmit_t *parray;

void pres_t_to_xmit
#ifdef NIDL_PROTOTYPES
(
    pres_t *object,
    xmit_t **xmit_object
)
#else
(object, xmit_object)
pres_t *object;
xmit_t **xmit_object;
#endif
{
    int i = 0;

    if (verbose)
        printf("In pres_t_to_xmit().\n");

    (*object)->called[(*object)->index++] = TO_XMIT_CALLED;

    if (NULL == (*xmit_object = (xmit_t *)malloc(sizeof(xmit_t)))) {
        fprintf(stderr, "In pres_t_to_xmit: Cannot allocate xmit_object.\n");
        exit(1);
    }

    (**xmit_object)[i++] = (*object)->index;
    for (; i < XMIT_T_SIZE; ++i)
        (**xmit_object)[i] = (*object)->called[i - (XMIT_T_SIZE - CALLED_SIZE)];

    /*  In case we need them elswhere.
    */
    pstruct = *object;
    parray = *xmit_object;
}

void pres_t_from_xmit
#ifdef NIDL_PROTOTYPES
(
    xmit_t *xmit_object,
    pres_t *object
)
#else
(xmit_object, object)
xmit_t *xmit_object;
pres_t *object;
#endif
{
    int i = 0;

    if (verbose)
        printf("In pres_t_from_xmit().\n");

    if (NULL == (*object = (pres_t)malloc(sizeof(record_t))))
    {
        fprintf(stderr, "In pres_t_from_xmit: Cannot allocate presented object.\n");
        exit(1);
    }

    (*object)->index = **xmit_object;
    for (i = 0; i < CALLED_SIZE; ++i)
        (*object)->called[i] = (*xmit_object)[i + (XMIT_T_SIZE - CALLED_SIZE)];

    (*object)->called[(*object)->index++] = FROM_XMIT_CALLED;

    /*  In case we need them elswhere.
    */
    pstruct = *object;
    parray = xmit_object;
}

void pres_t_free_xmit
#ifdef NIDL_PROTOTYPES
(
    xmit_t *xmit_object
)
#else
(xmit_object)
xmit_t *xmit_object;
#endif
{
    if (verbose)
        printf("In pres_t_free_xmit().\n");

    /*  Just for form's sake, since we'll free it before returning.
    */
    (*xmit_object)[(XMIT_T_SIZE - CALLED_SIZE) + (*xmit_object)[0]++] = FREE_XMIT_CALLED;

    /*  An attempt to leave a persistent record.
    */
    pstruct->called[pstruct->index++] = FREE_XMIT_CALLED;

    free(xmit_object);
}

void pres_t_free_inst
#ifdef NIDL_PROTOTYPES
(
    pres_t *object
)
#else
(object)
pres_t *object;
#endif
{
    if (verbose)
        printf("In pres_t_free_inst().\n");

    /*  Just for form's sake, since it's about to be discarded.
    */
    (*object)->called[(*object)->index++] = FREE_INST_CALLED;

    /*  An attempt to leave a persistent record.
    */
/* SNI_SVR4 BUGFIX
 * Can't do this -- the xmit_object struct has already been freed in
 *"_free_xmit"
 *
 *  (*parray)[(XMIT_T_SIZE - CALLED_SIZE) + (*parray)[0]++] = FREE_INST_CALLED;
 */

    free(*object);
}
