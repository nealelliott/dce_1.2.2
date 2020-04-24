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
 * Revision 1.1.8.2  1996/02/17  23:13:39  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:24  marty]
 *
 * Revision 1.1.8.1  1995/12/11  20:02:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:30:14  root]
 * 
 * Revision 1.1.6.1  1994/06/30  17:58:29  tom
 * 	Get rid of FREE macro (OT 11148).
 * 	[1994/06/30  17:55:43  tom]
 * 
 * Revision 1.1.4.3  1993/02/03  16:40:44  htf
 * 	ool_xmitas_t_free_inst() shouldn't free space, since
 * 		ool_xmitas_t_from_xmit() didn't allocate any
 * 	[1993/02/02  00:03:59  htf]
 * 
 * Revision 1.1.4.2  1993/01/07  19:53:43  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:32:11  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/02  19:02:24  rsalz
 * 	Add ool_xmitas_t_free_inst.
 * 	[1992/06/02  18:59:40  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:32:22  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <cover_ool.h>
#include <util_macros.h>

void ool_xmitas_t_to_xmit
#ifdef NIDL_PROTOTYPES
(
    ool_xmitas_t    *pres_p,
    float_t         **xmit_p_p
)
#else
(pres_p, xmit_p_p)
    ool_xmitas_t    *pres_p;
    float_t         **xmit_p_p;
#endif
{
    MALLOC((float_t *), *xmit_p_p, sizeof(float_t));
    **xmit_p_p = pres_p->l;     /* Convert long to float */
}

void ool_xmitas_t_from_xmit
#ifdef NIDL_PROTOTYPES
(
    float_t         *xmit_p,
    ool_xmitas_t    *pres_p
)
#else
(xmit_p, pres_p)
    float_t         *xmit_p;
    ool_xmitas_t    *pres_p;
#endif
{
    pres_p->l = *xmit_p;        /* Convert float to long */
}

void ool_xmitas_t_free_inst
#ifdef NIDL_PROTOTYPES
(
    ool_xmitas_t	*object
)
#else
(object)
    ool_xmitas_t	*object;
#endif
{
    /* nothing allocated, nothing freed */
}

void ool_xmitas_t_free_xmit
#ifdef NIDL_PROTOTYPES
(
    float_t         *xmit_p
)
#else
(xmit_p)
    float_t         *xmit_p;
#endif
{
    free(xmit_p);
}

void struct_xmitas_t_to_xmit
#ifdef NIDL_PROTOTYPES
(
    struct_xmitas_t     *pres_p,
    combined_struct_t   **xmit_p_p
)
#else
(pres_p, xmit_p_p)
    struct_xmitas_t     *pres_p;
    combined_struct_t   **xmit_p_p;
#endif
{
    MALLOC((combined_struct_t *), *xmit_p_p, sizeof(combined_struct_t));
    (*xmit_p_p)->sh = pres_p->sh;
    (*xmit_p_p)->l  = pres_p->struc.l;
    (*xmit_p_p)->f  = pres_p->struc.f;
    (*xmit_p_p)->d  = pres_p->d;
}

void struct_xmitas_t_from_xmit
#ifdef NIDL_PROTOTYPES
(
    combined_struct_t   *xmit_p,
    struct_xmitas_t     *pres_p
)
#else
(xmit_p, pres_p)
    combined_struct_t   *xmit_p;
    struct_xmitas_t     *pres_p;
#endif
{
    pres_p->sh      = xmit_p->sh;
    pres_p->struc.l = xmit_p->l;
    pres_p->struc.f = xmit_p->f;
    pres_p->d       = xmit_p->d;
}

void struct_xmitas_t_free_xmit
#ifdef NIDL_PROTOTYPES
(
    combined_struct_t   *xmit_p
)
#else
(xmit_p)
    combined_struct_t   *xmit_p;
#endif
{
    free(xmit_p);
}

void struct_xmitas_t_free_inst
#ifdef NIDL_PROTOTYPES
(
    struct_xmitas_t     *pres_p
)
#else
(pres_p)
    struct_xmitas_t     *pres_p;
#endif
{
/*
 * The top-level presented type is a stack surrogate - we would only have to
 * free any underlying storage.  Since there is none, we have nothing to free.
 */
}

void sub_xmitas_t_to_xmit
#ifdef NIDL_PROTOTYPES
(
    sub_xmitas_t    *pres_p,
    ptr_xmit_t      **xmit_p_p
)
#else
(pres_p, xmit_p_p)
    sub_xmitas_t    *pres_p;
    ptr_xmit_t      **xmit_p_p;
#endif
{
    MALLOC((ptr_xmit_t *), *xmit_p_p, sizeof(ptr_xmit_t));
    MALLOC((ndr_long_int *), (*xmit_p_p)->lp, sizeof(ndr_long_int));
    MALLOC((ndr_short_float *), (*xmit_p_p)->fp, sizeof(ndr_short_float));
    *((*xmit_p_p)->lp) = pres_p->l;
    *((*xmit_p_p)->fp) = pres_p->f;
}

void sub_xmitas_t_from_xmit
#ifdef NIDL_PROTOTYPES
(
    ptr_xmit_t      *xmit_p,
    sub_xmitas_t    *pres_p
)
#else
(xmit_p, pres_p)
    ptr_xmit_t      *xmit_p;
    sub_xmitas_t    *pres_p;
#endif
{
    pres_p->l = *xmit_p->lp;
    pres_p->f = *xmit_p->fp;
}

void sub_xmitas_t_free_xmit
#ifdef NIDL_PROTOTYPES
(
    ptr_xmit_t   *xmit_p
)
#else
(xmit_p)
    ptr_xmit_t   *xmit_p;
#endif
{
    free(xmit_p->lp);
    free(xmit_p->fp);
    free(xmit_p);
}

void sub_xmitas_t_free_inst
#ifdef NIDL_PROTOTYPES
(
    sub_xmitas_t    *pres_p
)
#else
(pres_p)
    sub_xmitas_t    *pres_p;
#endif
{
/*
 * The top-level presented type is a stack surrogate - we would only have to
 * free any underlying storage.  Since there is none, we have nothing to free.
 */
}
