/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: util_xmit.c,v $
 * Revision 1.1.4.2  1996/02/17  23:18:45  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:27  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:32:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:59  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:58:46  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:47:47  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:33:34  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <test_common.h>
#include <xmitas_nested.h>

void small_t_to_xmit
#ifdef NIDL_PROTOTYPES
(
    small_t *smp,
    short_t **shpp
)
#else
(smp, shpp)
    small_t *smp;
    short_t **shpp;
#endif
{
    *shpp = (short_t *)malloc(sizeof(short_t));
    /* Add 2 to the value so we'll know this translation took place. */
    (*shpp)->sh = smp->sm + 2;
}

void small_t_from_xmit
#ifdef NIDL_PROTOTYPES
(
    short_t *shp,
    small_t *smp
)
#else
(shp, smp)
    short_t *shp;
    small_t *smp;
#endif
{
    /* Add 3 to the value so we'll know this translation took place. */
    smp->sm = shp->sh + 3;
}

void small_t_free_xmit
#ifdef NIDL_PROTOTYPES
(
    short_t *shp
)
#else
(shp)
    short_t *shp;
#endif
{
    free(shp);
}

void small_t_free_inst
#ifdef NIDL_PROTOTYPES
(
    small_t *smp
)
#else
(smp)
    small_t *smp;
#endif
{
}

void short_t_to_xmit
#ifdef NIDL_PROTOTYPES
(
    short_t *shp,
    long_t  **lopp
)
#else
(shp, lopp)
    short_t *shp;
    long_t  **lopp;
#endif
{
    *lopp = (long_t  *)malloc(sizeof(long_t ));
    /* Add 7 to the value so we'll know this translation took place. */
    (*lopp)->lo = shp->sh + 7;
}

void short_t_from_xmit
#ifdef NIDL_PROTOTYPES
(
    long_t  *lop,
    short_t *shp
)
#else
(lop, shp)
    long_t  *lop;
    short_t *shp;
#endif
{
    /* Add 11 to the value so we'll know this translation took place. */
    shp->sh = lop->lo + 11;
}

void short_t_free_xmit
#ifdef NIDL_PROTOTYPES
(
    long_t  *lop
)
#else
(lop)
    long_t  *lop;
#endif
{
    free(lop);
}

void short_t_free_inst
#ifdef NIDL_PROTOTYPES
(
    short_t *shp
)
#else
(shp)
    short_t *shp;
#endif
{
}
